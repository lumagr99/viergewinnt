#include "myglitem.h"
#include <math.h>
#include <QDebug>
#include "QOpenGLBuffer"

MyGLItem::MyGLItem()
{
    m_backgroundColor = GLColorRgba::clBlack;
    m_drawAxes = true;
    m_timer->start(16);
    //m_movementEnabled = true;
    //m_sphere = new GLSphere("MySphere", 1.0, cl_Blue, ":/textures/black_disc.jpg");
    m_mouseray = new GLMouseRay();
}

void MyGLItem::paintUnderQmlScene()
{

}


void MyGLItem::paintOnTopOfQmlScene()
{
    renderer()->setTextureEnabled(true);

    m_court->draw(renderer(), false);
    m_tableplate->draw(renderer(), false);

    m_token->draw(renderer(), false);

    renderer()->setLightingEnabled(false);
    m_mouseray->draw(renderer());
    renderer()->setLightingEnabled(m_lightingEnabled);
    //activateBuffers();
}

void MyGLItem::setupGeometry()
{
    qDebug() << "MyGLItem::setupGeometry() called.";
    GLItem::setupGeometry();


    //setup vertexbuffer
     m_vertexBuffer= new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
     if(!m_vertexBuffer->create()){
        qDebug() <<"Vertex Buffer create failed";
        exit(1);
     }

    m_mouseray->makeSurface(nullptr, nullptr);

    m_renderer->setColorArrayEnabled(true);
    m_renderer->setLightingEnabled(true);
    m_renderer->setTextureEnabled(true);

    m_court = new GLCourt("court");
    m_tableplate = new GLTablePlate("tableplate");

    m_token = new GLTokenGreen("token");

    m_vertexBuffer->bind();
    m_vertexBuffer->allocate(m_points.data(), m_points.size() *
    static_cast<int>(sizeof(GLPoint)) );
    m_vertexBuffer->release();
}

void MyGLItem::rotateLeft(float increment)
{
    qDebug() << "MyGLItem::rotateLeft() called.";
    m_movementEnabled = true;
    m_rotationIncrement = -fabs(increment);
}

void MyGLItem::stopRotation()
{
    qDebug() << "MyGLItem::stopRotation() called.";
    m_movementEnabled = false;
}

void MyGLItem::rotateRight(float increment)
{
    qDebug() << "MyGLItem::rotateRight() called.";
    m_movementEnabled = true;
    m_rotationIncrement = fabs(increment);
}

void MyGLItem::activateBuffers(){
    m_vertexBuffer->bind();
    renderer()->activateAttributeBuffer(GLESRenderer::VERTEX_LOCATION);
    renderer()->activateAttributeBuffer(GLESRenderer::NORMAL_LOCATION);
    renderer()->activateAttributeBuffer(GLESRenderer::COLOR_LOCATION);
}

void MyGLItem::wheelEvent(QWheelEvent *e){
    qDebug() << "MyGLItem::wheelEvent called.";
    QVector3D vec = QVector3D(m_eye.x() - e->angleDelta().y(), m_eye.y() - e->angleDelta().y(), m_eye.z() - e->angleDelta().y());
    setEye(vec);
}

void MyGLItem::doSynchronizeThreads()
{
    GLItem::doSynchronizeThreads();
    if(m_mousePressed){
        qDebug() << "MyGLItem doSynchronize" << m_mousePosition;

        QVector3D nearPoint;
        QVector3D farPoint;
        renderer()->calculateMousePoints(&nearPoint, &farPoint, m_mousePosition);
        m_mouseray->setPoints(nearPoint, farPoint);

        m_mousePressed = false;
    }
}

void MyGLItem::mousePressed(int x, int y)
{
    qDebug() << "MyGLItem X: " << x << "Y: " << y;
    m_mousePosition = QPoint(x, y);
    m_mousePressed = true;
}

void MyGLItem::mousePositionChanged(int x, int y)
{
    qDebug() << "moved";
    m_mouse_x = x;
    m_mouse_y = y;
}

void MyGLItem::mouseReleased(int x, int y)
{
    qDebug() << "MyGLItem mouse released";
}
