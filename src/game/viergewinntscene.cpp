#include <QDebug>
#include <math.h>

#include "viergewinntscene.h"

VierGewinntScene::VierGewinntScene()
    : m_vierGewinnt(nullptr)
    , m_mouseRay(nullptr)
    , m_sounds(nullptr)
    , m_mousePressReceived(false)
    , m_mouseReleaseReceived(false)
    , m_mousePositionChangedReceived(false)
    , m_drawMouseRays(false)
    , m_newGame(false)
{
    m_backgroundColor = GLColorRgba::clBlack;
    m_timer->start(16);
    m_loopMovement = true;
    m_eye = 12.0f * v_Y + 25.0f * v_Z;
    m_sounds = new SoundEngine(this);
    m_sounds->setEnabled(true);

    createAxes(5.0f);
}

void VierGewinntScene::paintUnderQmlScene()
{
    m_vierGewinnt->draw(m_renderer);
    if (m_drawMouseRays) {
        m_renderer->setLightingEnabled(false);
        m_mouseRay->draw(m_renderer);
        m_renderer->setLightingEnabled(true);
    }
}

void VierGewinntScene::paintOnTopOfQmlScene()
{
    //Do Nothing
}

void VierGewinntScene::setupGeometry()
{
    GLItem::setupGeometry();

    m_vierGewinnt = new VierGewinnt(this);
    m_mouseRay = new GLMouseRay("MouseRay", GLColorRgba::clGreen);

    connect(m_vierGewinnt, &VierGewinnt::gameOver, this, &VierGewinntScene::gameOver);
    connect(m_vierGewinnt, &VierGewinnt::soundReqeuest, m_sounds, &SoundEngine::playSound);

    setupBuffers();
}

void VierGewinntScene::setupBuffers()
{
    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if(!m_vertexBuffer->create()){
        qDebug() << "VierGewinntScene::setupBuffers(): Vertex Buffer create failed";
        exit(1);
    }
    m_vertexBuffer->bind();
    m_vertexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw); // StaticDraw is default. However we set it for clearity.
    m_vertexBuffer->allocate(m_points.data(), m_points.size() * static_cast<int>(sizeof(GLPoint)) );
    m_vertexBuffer->release();

    m_indexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    if(!m_indexBuffer->create()){
        qDebug() << "VierGewinntScene::setupBuffers(): Index Buffer create failed";
        exit(1);
    }
    m_indexBuffer->bind();
    m_indexBuffer->allocate(m_indices.data(), m_indices.size() * static_cast<int>(sizeof(IndexType)));
    m_indexBuffer->release();
}

void VierGewinntScene::doSynchronizeThreads()
{
    GLItem::doSynchronizeThreads();

    if (!m_renderer) {
        return;
    }

    if(m_newGame) {
        m_newGame = false;
        stopRotation();
        m_renderThreadRotation = 0.0f;
        m_guiThreadRotation = 0.0f;
        m_eye = 12.0f * v_Y + 25.0f * v_Z;
        QMatrix4x4 mvMatrix;
        mvMatrix.setToIdentity();
        m_renderer->setMvMatrix(mvMatrix);
        delete m_vierGewinnt;
        delete m_mouseRay;
        setupGeometry();
        return;
    }

    if (m_mouseReleaseReceived) {
        m_renderer->pushMvMatrix();
        m_renderer->setMvMatrix(m_vierGewinnt->getMvMatrix());
        m_renderer->calculateMousePoints(&m_mouseNear, &m_mouseFar, m_mouseReleasePosition);
        m_renderer->popMvMatrix();
        m_vierGewinnt->deselectToken();
    } else if (m_mousePressReceived) {
        m_renderer->pushMvMatrix();
        m_renderer->setMvMatrix(m_vierGewinnt->getMvMatrix());
        m_mouseRay->setPoints(m_mouseFar, m_mouseFar);
        m_renderer->calculateMousePoints(&m_mouseNear, &m_mouseFar, m_mousePressPosition);
        m_renderer->mouseIntersection(&m_lastIntersection, v_Y, 0.0f, m_mousePressPosition);
        m_renderer->popMvMatrix();
        m_vierGewinnt->selectToken(m_mouseNear, m_mouseFar, m_eye);
    } else if (m_mousePositionChangedReceived) {
        m_renderer->pushMvMatrix();
        m_renderer->setMvMatrix(m_vierGewinnt->getMvMatrix());
        QVector3D oldIntersection = m_lastIntersection;
        m_renderer->mouseIntersection(&m_lastIntersection, v_Y, 0.0f, m_mousePositionChangedTo);
        m_renderer->popMvMatrix();

        m_moveVector += m_lastIntersection - oldIntersection;
        m_vierGewinnt->moveToken(m_moveVector);
        m_moveVector = v_Zero;
    }

    if (m_timer->isActive()) {
        if (m_vierGewinnt->animateDescent()) {
            m_vierGewinnt->descentAnimation();
            //Muss hier abgespielt werden, da das emittieren eines Signals in der Animation zu Framedrops führt
            if(m_vierGewinnt->animateDescent()) {
                m_sounds->playSound(":/sounds/TokenInserted.wav");
            }
        } else if (m_vierGewinnt->animateJumpUp()) {
            m_vierGewinnt->jumpUpAnimation();
        } else if (m_vierGewinnt->animateJumpDown()) {
            m_vierGewinnt->jumpDownAnimation();
        }
        m_vierGewinnt->cameraRotationAnimation();
    }

    m_mousePressReceived = false;
    m_mouseReleaseReceived = false;
    m_mousePositionChangedReceived = false;

}

void VierGewinntScene::newGame()
{
    stopGame();
    m_newGame = true;
    startGame();
}

void VierGewinntScene::startGame()
{
    m_timer->start();
}

void VierGewinntScene::stopGame()
{
    m_timer->stop();
}

void VierGewinntScene::startRotation(float increment)
{
    m_movementEnabled = true;
    m_rotationIncrement = fabs(increment);
}

void VierGewinntScene::stopRotation()
{
    m_movementEnabled = false;
}

void VierGewinntScene::mousePressed(int x, int y)
{
    m_mousePressPosition = QPoint(x, y);
    m_renderer->calculateMousePoints(&m_mouseNear, &m_mouseFar, m_mousePressPosition);
    m_mousePressReceived = true;
}

void VierGewinntScene::mousePositionChanged(int x, int y)
{
    m_mousePositionChangedTo = QPoint(x, y);
    m_mousePositionChangedReceived = true;
}

void VierGewinntScene::mouseReleased(int x, int y)
{
    m_mouseReleasePosition = QPoint(x, y);
    m_renderer->calculateMousePoints(&m_mouseNear, &m_mouseFar, m_mouseReleasePosition);
    m_mouseReleaseReceived = true;
}

void VierGewinntScene::handleWheelEvent(int angleDelta)
{
    if (angleDelta < 0) {
        m_eye *= 1.05f;
    } else {
        m_eye /= 1.05f;
    }
}

void VierGewinntScene::toggleMouseRays()
{
    m_drawMouseRays = !m_drawMouseRays;
}

void VierGewinntScene::toggleAxes()
{
    m_drawAxes = !m_drawAxes;
}
