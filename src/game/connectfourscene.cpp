#include <QDebug>
#include <math.h>

#include "connectfourscene.h"

ConnectFourScene::ConnectFourScene()
    : m_vierGewinnt(nullptr)
    , m_mouseRay(nullptr)
    , m_sounds(nullptr)
    , m_mousePressReceived(false)
    , m_mouseReleaseReceived(false)
    , m_mousePositionChangedReceived(false)
    , m_drawMouseRays(false)
    , m_newGame(false)
    , m_player(Player::RedPlayer)
{
    m_backgroundColor = GLColorRgba::clBlack;
    m_timer->start(16);
    m_loopMovement = true;
    m_eye = 12.0f * v_Y + 25.0f * v_Z;
    m_sounds = new SoundEngine(this);
    m_sounds->setEnabled(true);

    createAxes(5.0f);
}

void ConnectFourScene::paintUnderQmlScene()
{
    m_vierGewinnt->draw(m_renderer);
    if (m_drawMouseRays) {
        m_renderer->setLightingEnabled(false);
        m_mouseRay->draw(m_renderer);
        m_renderer->setLightingEnabled(true);
    }
}

void ConnectFourScene::paintOnTopOfQmlScene()
{
    //Tue nichts.
}

void ConnectFourScene::setupGeometry()
{
    GLItem::setupGeometry();

    m_vierGewinnt = new ConnectFour(this, m_player);
    m_mouseRay = new GLMouseRay("MouseRay", GLColorRgba::clGreen);

    connect(m_vierGewinnt, &ConnectFour::gameOver, this, &ConnectFourScene::gameOver);
    connect(m_vierGewinnt, &ConnectFour::soundReqeuest, m_sounds, &SoundEngine::playSound);
}

void ConnectFourScene::doSynchronizeThreads()
{
    GLItem::doSynchronizeThreads();

    if (!m_renderer) {
        return;
    }

    if (m_newGame) {
        m_newGame = false;
        stopRotation();

        if (m_player == Player::RedPlayer) {
            m_player = Player::GreenPlayer;
        } else {
            m_player = Player::RedPlayer;
        }
        m_guiThreadRotation = 0.0f;

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
        m_renderer->calculateMousePoints(&m_mouseNear, &m_mouseFar, m_mousePressPosition);
        m_mouseRay->setPoints(m_mouseNear, m_mouseFar);

        m_renderer->pushMvMatrix();
        m_renderer->setMvMatrix(m_vierGewinnt->getMvMatrix());
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
            if (!m_vierGewinnt->animateDescent()) {
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

void ConnectFourScene::newGame()
{
    stopGame();
    m_newGame = true;
    startGame();
}

void ConnectFourScene::startGame()
{
    m_timer->start();
}

void ConnectFourScene::stopGame()
{
    m_timer->stop();
}

void ConnectFourScene::startRotation(float increment)
{
    m_movementEnabled = true;
    m_rotationIncrement = fabs(increment);
}

void ConnectFourScene::stopRotation()
{
    m_movementEnabled = false;
}

void ConnectFourScene::mousePressed(int x, int y)
{
    m_mousePressPosition = QPoint(x, y);
    m_renderer->calculateMousePoints(&m_mouseNear, &m_mouseFar, m_mousePressPosition);
    m_mousePressReceived = true;
}

void ConnectFourScene::mousePositionChanged(int x, int y)
{
    m_mousePositionChangedTo = QPoint(x, y);
    m_mousePositionChangedReceived = true;
}

void ConnectFourScene::mouseReleased(int x, int y)
{
    m_mouseReleasePosition = QPoint(x, y);
    m_renderer->calculateMousePoints(&m_mouseNear, &m_mouseFar, m_mouseReleasePosition);
    m_mouseReleaseReceived = true;
}

void ConnectFourScene::handleWheelEvent(int angleDelta)
{
    if (angleDelta < 0) {
        m_eye *= 1.05f;
    } else {
        m_eye /= 1.05f;
    }
}
