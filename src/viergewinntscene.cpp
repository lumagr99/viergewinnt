#include <QDebug>
#include <QThread>
#include <math.h>

#include "viergewinntscene.h"

VierGewinntScene::VierGewinntScene()
{
    m_backgroundColor = GLColorRgba::clBlack;
    m_timer->start(16);
    m_loopMovement = true;
    m_eye = 12.0f * v_Y + 25.0f * v_Z;
}

void VierGewinntScene::paintUnderQmlScene()
{
    m_vierGewinnt->draw(m_renderer);
}

void VierGewinntScene::paintOnTopOfQmlScene()
{
    //Do Nothing
}

void VierGewinntScene::setupGeometry()
{
    GLItem::setupGeometry();
    m_vierGewinnt = new VierGewinnt(this);
    connect(m_vierGewinnt, &VierGewinnt::gameOver, this, &VierGewinntScene::gameOver);
}

void VierGewinntScene::doSynchronizeThreads()
{
    GLItem::doSynchronizeThreads();

    if (!m_renderer) {
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

    if(m_timer->isActive()) {
        if (m_vierGewinnt->animateDescent()) {
            m_vierGewinnt->descentAnimation();
        }else if(m_vierGewinnt->animateJumpUp()) {
            m_vierGewinnt->jumpUpAnimation();
        } else if(m_vierGewinnt->animateJumpDown()) {
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

}

void VierGewinntScene::startGame()
{

}

void VierGewinntScene::stopGame()
{
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
