#include <math.h>
#include <QDebug>

#include "viergewinntscene.h"


VierGewinntScene::VierGewinntScene()
{
    m_backgroundColor = GLColorRgba::clBlack;
    m_timer->start(16);
    m_drawAxes = true;
}


void VierGewinntScene::paintUnderQmlScene()
{
    //Do nothing
}


void VierGewinntScene::paintOnTopOfQmlScene()
{
    //qDebug() << "VierGewinntScene::paintOnTopOfQmlScene() called.";
    m_vierGewinnt->draw(m_renderer);
    m_renderer->setLightingEnabled(false);
    m_mouseRay->draw(m_renderer);
    m_renderer->setLightingEnabled(true);
}

void VierGewinntScene::setupGeometry()
{
    qDebug() << "VierGewinntScene::setupGeometry() called.";
    GLItem::setupGeometry();
    m_vierGewinnt = new VierGewinnt(this);
    m_mouseRay = new GLMouseRay("MouseRay", GLColorRgba::clRed);
}

void VierGewinntScene::doSynchronizeThreads()
{
    GLItem::doSynchronizeThreads();
    if(!m_renderer) {
        return;
    }

    if(m_mousePressReceived) {
        m_mouseRay->setPoints(m_mouseNear, m_mouseFar);
    }

    m_mousePressReceived = false;
    m_mouseReleaseReceived = false;
    m_mousePositionChangedReceived = false;
}

void VierGewinntScene::rotateLeft(float increment)
{
    qDebug() << "VierGewinntScene::rotateLeft() with increment" << increment << "called.";
    m_movementEnabled = true;
    m_rotationIncrement = -fabs(increment);
}

void VierGewinntScene::stopRotation()
{
    qDebug() << "VierGewinntScene::stopRotation() called.";
    m_movementEnabled = false;
}

void VierGewinntScene::rotateRight(float increment)
{
    qDebug() << "VierGewinntScene::rotateRight() with increment" << increment << "called.";
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
        m_eye *= 1.05;
    }
    else {
        m_eye /= 1.05;
    }
}
