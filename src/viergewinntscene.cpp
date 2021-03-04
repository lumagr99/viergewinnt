#include <math.h>
#include <QDebug>

#include "viergewinntscene.h"


VierGewinntScene::VierGewinntScene()
{
    m_backgroundColor = GLColorRgba::clBlack;
    m_timer->start(16);
    m_drawAxes = true;
    m_loopMovement = true;
    m_eye = 12.0f * v_Y + 20.0f * v_Z;

    m_fovy = 45.0f;
    m_aspect = 0.0f;
    m_orthoMode = false;
    m_orthoRange = 1.0f;
}


void VierGewinntScene::paintUnderQmlScene()
{
    //Do nothing
}


void VierGewinntScene::paintOnTopOfQmlScene()
{
    m_vierGewinnt->draw(m_renderer);
    m_renderer->setLightingEnabled(false);
    m_mouseRay->draw(m_renderer);
    m_renderer->setLightingEnabled(true);
}

void VierGewinntScene::setupGeometry()
{
    //qDebug() << "VierGewinntScene::setupGeometry() called.";
    GLItem::setupGeometry();
    m_mouseRay = new GLMouseRay("MouseRay", GLColorRgba::clGreen);
    m_vierGewinnt = new VierGewinnt(this);
}

void VierGewinntScene::doSynchronizeThreads()
{
    GLItem::doSynchronizeThreads();
    if(!m_renderer) {
        return;
    }

    if(m_mouseReleaseReceived)
    {
        m_renderer->calculateMousePoints(&m_mouseNear, &m_mouseFar, m_mouseReleasePosition);
        m_vierGewinnt->deselectToken();
    }
    else if(m_mousePressReceived)
    {
        m_mouseRay->setPoints(m_mouseNear, m_mouseFar);
        m_renderer->calculateMousePoints(&m_mouseNear, &m_mouseFar,  m_mousePressPosition);
        m_renderer->mouseIntersection(&m_lastIntersection, v_Y, 0.0f, m_mousePressPosition);
        m_vierGewinnt->selectToken(m_mouseNear, m_mouseFar, m_eye);
    }
    else if (m_mousePositionChangedReceived)
    {
        QVector3D oldIntersection = m_lastIntersection;
        m_renderer->mouseIntersection(&m_lastIntersection, v_Y, 0.0f, m_mousePositionChangedTo);
        m_moveVector += m_lastIntersection - oldIntersection;
        m_vierGewinnt->moveToken(m_moveVector);
        m_moveVector = v_Zero;
    }

    m_mousePressReceived = false;
    m_mouseReleaseReceived = false;
    m_mousePositionChangedReceived = false;
}

void VierGewinntScene::rotateLeft(float increment)
{
    //qDebug() << "VierGewinntScene::rotateLeft() with increment" << increment << "called.";
    m_movementEnabled = true;
    m_rotationIncrement = -fabs(increment);
}

void VierGewinntScene::stopRotation()
{
    //qDebug() << "VierGewinntScene::stopRotation() called.";
    m_movementEnabled = false;
}

void VierGewinntScene::rotateRight(float increment)
{
    //qDebug() << "VierGewinntScene::rotateRight() with increment" << increment << "called.";
    m_movementEnabled = true;
    m_rotationIncrement = fabs(increment);
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
    if(angleDelta < 0) {
        m_eye *= 1.05f;
    }
    else {
        m_eye /= 1.05f;
    }
}

void VierGewinntScene::handleKeyEvent(int key)
{
    if(key >= Qt::Key_1 && key <= Qt::Key_7) {
        //m_vierGewinnt->insertToken(key - KEY_OFFSET);
    }
}
