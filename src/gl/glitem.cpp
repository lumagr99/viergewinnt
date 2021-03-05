#include "glitem.h"
#include "glcolorrgba.h"
#include "gldefines.h"
#include "glesrenderer.h"

#include <QDebug>
#include <QQuickWindow>
#include <QSGSimpleRectNode>
#include <QString>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLShaderProgram>
#include <QtQuick/QQuickItem>

#include <math.h>

#ifdef Q_OS_ANDROID
#ifndef GLES
#define GLES //Android uses GLES 2.0
#endif
#endif

GLItem::GLItem(QQuickItem* parent,
    const QString& vertexShaderFilename,
    const QString& fragmentShaderFilename)
    : QQuickItem(parent)
{
    m_renderer = nullptr;
    m_fragmentShaderFilename = fragmentShaderFilename;
    m_vertexShaderFilename = vertexShaderFilename;
    setFlag(ItemHasContents);
    m_geometryIsValid = false;
    m_colorArrayEnabled = false;
    m_texturesEnabled = false;
    m_lightingEnabled = true;
    m_activatePaintBeforeQml = true;
    m_activatePaintAfterQml = true;
    m_movementEnabled = false;
    m_loopMovement = false;
    m_rotationIncrement = 0.2f; //degree/frame

    m_lightDirection = v_XYZ;
    m_backgroundColor = GLColorRgba::clBlack;

    m_eye = 10.0 * v_XYZ;
    m_center = v_Zero;
    m_up = v_Y;
    //pespective
    m_fovy = 45.0;
    m_aspect = 1.0;
    m_near = 1.0;
    m_far = 100.0;
    m_orthoMode = false;
    m_drawAxes = false;
    //viewport
    m_viewportX = 0;
    m_viewportY = 0;
    m_viewportW = 0;
    m_viewportH = 0;
    //The windowChanged signal is emitted by QQuickItem when it is added to the scenegraph.
    //This is the first time when a valid window exists.
    connect(this, SIGNAL(windowChanged(QQuickWindow*)),
        this, SLOT(handleWindowChanged(QQuickWindow*)));
    m_geometryIsValid = false; //invalidate geometry, we may need to set it up for the new window
    m_timer = new QTimer(this);
    m_timer->setInterval(20);
    connect(m_timer, SIGNAL(timeout()),
        this, SLOT(onTimerTimeout()), Qt::DirectConnection);
    m_guiThreadRotation = 0.0;
    m_renderThreadRotation = 0.0;
    setFlags(flags() | QQuickItem::ItemHasContents);
    m_firstAxesPoint = 0;
    m_lastAxesPoint = 0;
    m_guiThreadShutdownRequest = false; //flag for triggering shutdown, copied in synchronizeThreads
    m_renderThreadShutdownRequest = false; //triggers cleanupOpenGl()
    m_show3dImage = true;
}

GLItem::~GLItem()
{
    // deleteRenderer();
}

/**
 * @brief GlItem::updatePaintNode
 * @param node Returns the root of the subtree to be rendered ON TOP of scene rendered
 * in paint().
 * @return
 */

QSGNode* GLItem::updatePaintNode(QSGNode* node, UpdatePaintNodeData*)
{
    //   qDebug() << "GlItem::updatePaintNode called";
    Q_UNUSED(node);
    return nullptr; //remove this line and uncomment the rest of this function to draw a blue rectangle
    //Example code for a blue rectangle
    /*    QSGSimpleRectNode *rect = static_cast<QSGSimpleRectNode *>(node);
    if (!rect) {
        rect = new QSGSimpleRectNode();
    }
    double w = 100;
    if(window())
        w = window()->width();
    rect->setRect(0, 0, w, 100);
    rect->setColor(Qt::blue);
    return rect;
    */
}

bool GLItem::movementEnabled()
{
    return (m_timer && m_timer->isActive());
}

void GLItem::paintBefore()
{
    //   qDebug() << "GlItem::paintBefore() called";

    if (!m_show3dImage)
        return;

    if (m_renderThreadShutdownRequest) {
        cleanUpOpenGl();
        deleteRenderer();
        return;
    }
    if (!m_renderer)
        initializeRenderer();
    if (!m_geometryIsValid)
        setupGeometry();
    if (!isVisible())
        return;
    setupView(true);
    m_renderer->bind();
    paintUnderQmlScene();
    m_renderer->release();
    window()->resetOpenGLState();
}

void GLItem::paintAfter()
{
    //  qDebug() << "GlItem::paintAfter() called";
    if (!m_show3dImage)
        return;

    if (m_renderThreadShutdownRequest) {
        cleanUpOpenGl();
        deleteRenderer();
        return;
    }
    if (!m_renderer)
        initializeRenderer();
    if (!m_geometryIsValid)
        setupGeometry();
    if (!isVisible())
        return;
    setupView(false); // enables scissor test
    m_renderer->bind();
    paintOnTopOfQmlScene();
    if (m_drawAxes)
        drawAxes();
    m_renderer->release();
    glDisable(GL_SCISSOR_TEST);
    window()->resetOpenGLState();
}

void GLItem::initializeRenderer()
{
    qDebug() << "GlItem::initializeRenderer called.";
    //    if(window()->openglContext() && !m_renderer){
    //        connect(window()->openglContext(), &QOpenGLContext::aboutToBeDestroyed,
    //                this, &GLItem::destroyTextureObjects, Qt::DirectConnection);
    //        connect(window()->openglContext(), &QOpenGLContext::aboutToBeDestroyed, //cleanup context before destroying window
    //                this, &GLItem::deleteRenderer, Qt::DirectConnection);
    //    }
    //Renderer lives in Render Thread, GLItem lives in GUI Thread,
    // therefore GLItem can not be parent of Renderer and Renderer MUST be created here without a parent
    if (!m_renderer)
        m_renderer = new GLESRenderer(nullptr, m_vertexShaderFilename, m_fragmentShaderFilename);
}

void GLItem::deleteRenderer()
{
    qDebug() << "GlItem::deleteRenderer() called.";
    if (m_renderer) {
        delete m_renderer;
        m_renderer = nullptr;
    }
}

void GLItem::cleanUpOpenGl()
{
    qDebug() << "GLItem::cleanUpOpenGl()";
    destroyTextureObjects();
}

void GLItem::doSynchronizeThreads()
{
    m_renderThreadRotation = m_guiThreadRotation;
}

void GLItem::synchronizeThreads()
{
    m_renderThreadShutdownRequest = m_guiThreadShutdownRequest;
    doSynchronizeThreads();
}

void GLItem::toggleMove()
{
    qDebug() << "GlItem::togglemMove() called";
    if (m_timer->isActive())
        m_timer->stop();
    else
        m_timer->start();
    emit movementEnabledChanged(m_movementEnabled);
}

void GLItem::mousePressed(int x, int y)
{
    qDebug() << "GlItem::mousePressed at x:" << x << " y: " << y;
}

void GLItem::mousePositionChanged(int x, int y)
{
    qDebug() << "GlItem::mouse position changed to  x:" << x << " y: " << y;
}

void GLItem::mouseReleased(int x, int y)
{
    qDebug() << "GlItem::mouse released at  x:" << x << " y: " << y;
}

void GLItem::setViewportX(int arg)
{
    if (m_viewportX == arg)
        return;

    m_viewportX = arg;
    emit viewportXChanged(arg);
}

void GLItem::setViewportY(int arg)
{
    qDebug() << "setViewPortY: " << arg;
    if (m_viewportY == arg)
        return;
    m_viewportY = arg;
    emit viewportYChanged(arg);
}

void GLItem::setupView(bool clearBuffers)
{
    if (!setupRenderer())
        return;
    glViewport(m_viewportX, m_viewportY, m_viewportW, m_viewportH);
    //qDebug() << "viewport x: " << vx << " y: " << vy << " w: " << vw << " h: " << vh;
    glEnable(GL_SCISSOR_TEST);
    glScissor(m_viewportX, m_viewportY, m_viewportW, m_viewportH);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(true);
    if (clearBuffers) {
#ifndef GLES
        glClearDepth(1.0);
#endif
        glClearColor(m_backgroundColor.red(), m_backgroundColor.green(),
            m_backgroundColor.blue(), m_backgroundColor.alpha());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //   glPolygonMode(GL_FRONT, GL_FILL);
    //   glPolygonMode(GL_BACK, GL_LINES);
}

bool GLItem::setupRenderer()
{
    if (!m_renderer || !window())
        return false;

    qreal ratio = window()->devicePixelRatio();
    m_viewportW = int(ratio * width());
    m_viewportH = int(ratio * height());
    m_renderer->setViewport(m_viewportX, m_viewportY, m_viewportW, m_viewportH);

    m_aspect = static_cast<float>(m_viewportW) / static_cast<float>(m_viewportH);
    if (!m_orthoMode) {
        m_renderer->setPerspective(m_fovy,
            m_aspect,
            m_near,
            m_far);
    } else {
        m_renderer->setOrtho(-m_orthoRange * m_aspect, m_orthoRange * m_aspect,
            -m_orthoRange, m_orthoRange, 0.1f, 100.0f); // for debugging
    }
    m_renderer->setLookAt(m_eye, //eye
        m_center, //center
        m_up); //up
    //setup light before turning
    m_renderer->setLightDirection(m_lightDirection);
    m_renderer->setLightingEnabled(true);
    //now turn
    m_renderer->transform(m_cameraTransform);
    m_renderer->rotate(m_renderThreadRotation, QVector3D(0.0, 1.0, 0.0));
    return true;
}

/**
 * @brief GlItem::handleWindowChanged
 *
 * Connect to the window's signals. This can not be done in the constructor, because at that time
 * there is no valid window yet.
 * @param win The window in which this QQuickItem will be painted.
 */
void GLItem::handleWindowChanged(QQuickWindow* win)
{
    qDebug() << "GlItem::handleWindowChanged() called.";
    if (win) {
        // Connect the beforeRendering signal to our paint function.
        // Since this call is executed on the rendering thread it must be
        // a Qt::DirectConnection

        if (m_activatePaintBeforeQml)
            connect(win, SIGNAL(beforeRendering()),
                this, SLOT(paintBefore()), Qt::DirectConnection);
        if (m_activatePaintAfterQml)
            connect(win, SIGNAL(afterRendering()),
                this, SLOT(paintAfter()), Qt::DirectConnection);
        connect(win, SIGNAL(beforeSynchronizing()),
            this, SLOT(synchronizeThreads()), Qt::DirectConnection);
        //        connect(win, &QQuickWindow::sceneGraphInvalidated,
        //                this, &GLItem::destroyTextureObjects, Qt::DirectConnection);
        //        if(window()->openglContext()){
        //            connect(window()->openglContext(), &QOpenGLContext::aboutToBeDestroyed,
        //                    this, &GLItem::destroyTextureObjects, Qt::DirectConnection);
        //            connect(window()->openglContext(), &QOpenGLContext::aboutToBeDestroyed, //cleanup context before destroying window
        //                    this, &GLItem::deleteRenderer, Qt::DirectConnection);
        //        }
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
    }
}

void GLItem::destroyTextureObjects()
{
    if (!window())
        return;
    if (!window()->openglContext())
        return;
    window()->openglContext()->makeCurrent(window());
    doDestroyTextureObjects();
    window()->openglContext()->doneCurrent();
}

void GLItem::onTimerTimeout()
{
    //qDebug() << "GlItem::onTimerTimeout() called";
    doTimerTimeout();
    if (window())
        window()->update();
    update();
}

void GLItem::doTimerTimeout()
{
    if (m_movementEnabled) {
        m_guiThreadRotation += m_rotationIncrement;
        if (m_guiThreadRotation > 360.0f) {
            m_guiThreadRotation = 0.0f;
            if (!m_loopMovement)
                setRotationEnabled(false);
        }
    }
}

void GLItem::doDestroyTextureObjects()
{
    qDebug() << "GLItem::doDestroyTextureObjects called";
}

void GLItem::setupGeometry()
{
    m_geometryIsValid = true;
    if (m_drawAxes)
        createAxes(5.0);
}

void GLItem::paintUnderQmlScene()
{
    qDebug() << "GlItem::paintUnderQmlScene() was called. Overwrite this function to paint your scene!";
}

void GLItem::paintOnTopOfQmlScene()
{
    qDebug() << "GlItem::paintOnTopOfQmlScene() was called. Overwrite this function to paint your scene!";
}

void GLItem::drawAxes()
{
    if (m_lastAxesPoint - m_firstAxesPoint == 0) //We have no axes
    {
        qDebug() << "GLItem::drawAxes(): No axes data available.";
        return;
    }
    GLfloat lineWidth[4]; //4 floats, just for safety
    glGetFloatv(GL_LINE_WIDTH, lineWidth);
    glLineWidth(3.0);
    m_renderer->setColorArrayEnabled(true);

    m_renderer->setLightingEnabled(false);
    int stride = sizeof(GLPoint);
    m_renderer->activateAttributeArray(GLESRenderer::VERTEX_LOCATION,
        m_points[0].vertexPointer(), stride);
    m_renderer->activateAttributeArray(GLESRenderer::COLOR_LOCATION,
        m_points[0].colorPointer(), stride);
    glDrawArrays(GL_LINES, m_firstAxesPoint, m_lastAxesPoint - m_firstAxesPoint + 1); //Coordinate Axes
    m_renderer->disableAttributeArrays();
    //restore old settings
    m_renderer->setColorArrayEnabled(m_colorArrayEnabled);
    m_renderer->setLightingEnabled(m_lightingEnabled);
    glLineWidth(lineWidth[0]);
}

void GLItem::createAxis(float length, const QVector3D& origin, const QVector3D& axis,
    const QVector3D& normal, const QVector3D& texCoord,
    const GLColorRgba& color)
{
    int ticks = static_cast<int>(floor(static_cast<double>(length)));
    float tickLength = 0.2f;
    m_points.append(GLPoint(origin - axis * length, normal, texCoord, color));
    m_points.append(GLPoint(origin + axis * length, normal, texCoord, color));
    for (int tick = -ticks; tick <= ticks; tick++) {
        m_points.append(GLPoint(origin + axis * tick, normal, texCoord, color));
        if (tick % 5 == 0)
            m_points.append(GLPoint(origin + axis * tick + normal * tickLength * 2, normal, texCoord, color));
        else
            m_points.append(GLPoint(origin + axis * tick + normal * tickLength, normal, texCoord, color));
    }
    QVector3D normal2 = QVector3D::crossProduct(axis, normal).normalized();
    for (int tick = -ticks; tick <= ticks; tick++) {
        m_points.append(GLPoint(origin + axis * tick, normal2, texCoord, color));
        if (tick % 5 == 0)
            m_points.append(GLPoint(origin + axis * tick + normal2 * tickLength * 2, normal2, texCoord, color));
        else
            m_points.append(GLPoint(origin + axis * tick + normal2 * tickLength, normal2, texCoord, color));
    }
}

void GLItem::createAxes(float length)
{

    m_firstAxesPoint = m_points.size();

    //X-axis
    QVector3D origin = v_Zero;
    QVector3D axis = v_X;
    QVector3D normal = v_Y;
    QVector3D texCoord = v_Zero;
    GLColorRgba color = cl_Red;

    createAxis(length, origin, axis, normal, texCoord, color);
    //Y-Axis
    axis = v_Y;
    normal = v_Z;
    color = cl_Green;
    createAxis(length, origin, axis, normal, texCoord, color);

    //Z-Axis
    axis = v_Z;
    normal = v_X;
    color = cl_Blue;
    createAxis(length, origin, axis, normal, texCoord, color);

    m_lastAxesPoint = m_points.size() - 1;
}
