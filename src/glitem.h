#ifndef GLITEM_H
#define GLITEM_H

#include <QQuickItem>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector>
#include <QTimer>


#include "glpoint.h"
#include "glesrenderer.h"

/**
 * @brief The GlItem class is a 3D-scene item designed for use in QML SceneGraphs.
 * It should be subclassed  according to your requirements, registered with qmlRegisterType
 * and used as a component in QML files.
 * The default constructor will use src/vshader.vsh and src/fshader.fsh as shaders.
 * Fell free to use shaders of your own. Refer to the GLESRenderer class for shader requirements.
 * Geometry data should be put into m_points and m_vertices containers for maximum performance.
 * Use paintUnderQml or paintOnTopOfQml to paint a 3D scene under or on top of the QML elements of the QML SceneGraph.
 * For debugging geometries, call toggleMove to let the scene rotate around m_rotationAxis.
 * Warning:
 * Updating geometry data must be carried out in synchronizeThreads, because drawing will be performed
 * on the rendering thread. The rendering thread is waiting when synchronizeThreads is called.
 *
 * Shutdown Sequence:
 * Call requestShutdown on GUI thread to free OpenGl resources for thisGlItem. Will be copied to render thread
 * on next call to synchronizeThreads.
 * OpenGl resources will be freed on next call to paintBefore or paintAfter.
 * Call isShuttingDown from top window (ApplicationWindow) to check for freed resources. If true is returned,
 * call Qt.quit() in QML or call QWindow::close() from QMainWindow.
 */
class GLItem : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QString vertexShaderFilename READ vertexShaderFilename WRITE setVertexShaderFilename NOTIFY vertexShaderFilenameChanged)
    Q_PROPERTY(QString fragmentShaderFilename READ fragmentShaderFilename WRITE setFragmentShaderFilename NOTIFY fragmentShaderFilenameChanged)
    Q_PROPERTY(bool movementEnabled READ movementEnabled  WRITE setRotationEnabled NOTIFY movementEnabledChanged)
    Q_PROPERTY(bool loopMovement READ loopMovement WRITE setLoopMovement NOTIFY loopMovementChanged)
    Q_PROPERTY(int viewportX READ viewportX WRITE setViewportX NOTIFY viewportXChanged)
    Q_PROPERTY(int viewportY READ viewportY WRITE setViewportY NOTIFY viewportYChanged)
    Q_PROPERTY(bool show3dImage READ show3dImage WRITE setShow3dImage NOTIFY show3dImageChanged)

    typedef enum{
        VERTEX_LOCATION,
        NORMAL_LOCATION,
        COLOR_LOCATION,
        TEXCOORD_LOCATION
    }AttributeLocation;

    bool m_show3dImage;

public:
    explicit GLItem(QQuickItem *parent = nullptr,
                    const QString &vertexShaderFilename = ":/shaders/vshader.vsh",
                    const QString &fragmentShaderFilename = ":/shaders/fshader.fsh");
    virtual ~GLItem();


    /**
     * @brief updatePaintNode Overwrite this function, if you want to add items to the scenegraph.
     *        Do not call this function. It will be called automatically.
     * @param node
     * @return
     */
    QSGNode * updatePaintNode(QSGNode *node, UpdatePaintNodeData *);

    //simple getters
    QString fragmentShaderFilename() const{return m_fragmentShaderFilename;}
    QString vertexShaderFilename() const{return m_vertexShaderFilename;}

    bool movementEnabled();

    void setEye (const QVector3D & newVal)
    {
        m_eye = newVal;
    }

    int viewportX() const
    {
        return m_viewportX;
    }

    int viewportY() const
    {
        return m_viewportY;
    }



    bool loopMovement() const
    {
        return m_loopMovement;
    }

    bool show3dImage() const
    {
        return m_show3dImage;
    }

signals:
    
    void vertexShaderFilenameChanged(QString arg);
    void fragmentShaderFilenameChanged(QString arg);
    void movementEnabledChanged(bool arg);

    void viewportXChanged(int arg);

    void viewportYChanged(int arg);

    void loopMovementChanged(bool loopMovement);


    void show3dImageChanged(bool show3dImage);

public slots:
    /**
     * @brief paintBefore
     * Activates renderer, clear color and depth buffers and calls paintUnderQmlScene.
     * This function should not be overwritten
     */
    void paintBefore();
    /**
     * @brief paintAfter
     * Activates renderer, clear color and depth buffers and calls paintOnTopOfQmlScene.
     * This function should not be overwritten
     */
    void paintAfter();

    void toggleMove();

    /** Mouse event handler to be called from QML
     **/
   void mousePressed(int x, int y);
   void mousePositionChanged(int x, int y);
   void mouseReleased(int x, int y);

    // Simple setters
    void setVertexShaderFilename(QString arg)
    {
        if (m_vertexShaderFilename != arg) {
            m_vertexShaderFilename = arg;
            emit vertexShaderFilenameChanged(arg);
        }
    }
    void setFragmentShaderFilename(QString arg)
    {
        if (m_fragmentShaderFilename != arg) {
            m_fragmentShaderFilename = arg;
            emit fragmentShaderFilenameChanged(arg);
        }
    }


    void setViewportX(int arg);

    void setViewportY(int arg);

    void setLoopMovement(bool loopMovement)
    {
        if (m_loopMovement == loopMovement)
            return;

        m_loopMovement = loopMovement;
        emit loopMovementChanged(m_loopMovement);
    }

    void setRotationEnabled(bool movementEnabled)
    {
        if (m_movementEnabled == movementEnabled)
            return;

        m_movementEnabled = movementEnabled;
        emit movementEnabledChanged(m_movementEnabled);
    }
    /**
     * @brief requestShutdown set theshutdown flag in GUI thread.
     * Will be copied to m_renderThreadShutdownRequest in synchronizeThreads.
     * m_renderThreadShutdownRequest triggers cleanup and shutdown in paintBefore
     */
    void requestShutdown()
    {
        m_guiThreadShutdownRequest = true;
    }
    /**
     * @brief isShuttingDown
     * @return true, if shutdown request has been processed
     */
    bool isShuttingDown(){
        return m_renderThreadShutdownRequest && m_renderer == nullptr;
    }

    void setShow3dImage(bool show3dImage)
    {
        if (m_show3dImage == show3dImage)
            return;

        m_show3dImage = show3dImage;
        emit show3dImageChanged(m_show3dImage);
    }

protected slots:
    /**
     * @brief handleWindowChanged
     * @param win This function is called when the parent Window changes.
     * This is also the case, when a parent window is set for the first time.
     */
    void handleWindowChanged(QQuickWindow *win);
    /**
     * @brief destroyTextureObjects connected to QOpenGLContext::aboutToBeDestroyed
     */
    void destroyTextureObjects();

     /**
     * @brief onTimerTimeout Overwrite for moving the scene.
     */
    void onTimerTimeout();
    /**
     * @brief synchronizeThreads
     * Render thread is sleeping when this function is called.
     * Copy geometry modifications from GuiThread owned variables here.
     */
    void synchronizeThreads();
    /**
     * @brief deleteRenderer
     * Delete renderer unloads shader program and deletes renderer.
     */
    void deleteRenderer();

protected:
    /**
     * @brief cleanUpOpenGl delete OpenGl Resources e.g. buffers and textures
     * MUST be called on the rendering thread with a current OpenGl context.
     */
    virtual void cleanUpOpenGl();
    /**
     * @brief doSynchronizeThreads Copy data from GUI-thread to render-thread and vice versa.
     * Virtual function to be overridden by subclasses
     */
    virtual void doSynchronizeThreads();

    virtual void doTimerTimeout();

    /**
     * @brief doDestroyTextureObjects destroy all texture objects when GL-context is about to be destroyed.
     * To be overridden
     */
    virtual void doDestroyTextureObjects();

    /**
     * @brief paintUnderQmlScene
     * Virtual function for painting under a QML scene. This function is called by paintBefore after
     * calling createGeometries and initializing and binding the renderer.
     * Overwrite in subclasses for painting geometries in m_points with the renderer.
     */
    virtual void paintUnderQmlScene();
    /**
     * @brief paintUnderQmlScene
     * Virtual function for painting on top of a QML scene. This function is called by paintAfter after
     * calling createGeometries and initializing and binding the renderer.
     * Overwrite in subclasses for painting geometries in m_points with the renderer.
     */
    virtual void paintOnTopOfQmlScene();
    virtual void drawAxes();
    virtual void createAxes(float length);

    /**
     * @brief setupGeometry Put the geometric data into the points array and set m_geometryIsValid flag.
     * MUST be overridden in subclasses. GlItem::setupGeometry() does nothing.
     */
    virtual void setupGeometry();
    /**
     * @brief setupView Setup matrices, lighting and basic GL rendering settings
     * GlItem::setupView sets up a basic view with (0,0,0) in the center of the screen.
     * You may override this function to fit your requirements.
     */
    virtual void setupView(bool clearBuffers);
    /**
     * @brief setupRenderer Setup matrices, lighting. Does not call glXXX functions
     */
    virtual bool setupRenderer();

    /**
     * @brief createAxis creates a coordinate axis
     * @param length Total length of axis, starting from origin
     * @param origin Start coordinate of axis
     * @param axis Direction of axis
     * @param normal Direction of ticks and dummy normal.
     * @param texCoord Dummy texture coordinate
     * @param color Color of axis
     */
    void createAxis(float length, const QVector3D &origin, const QVector3D &axis,
                    const QVector3D &normal, const QVector3D &texCoord,
                    const GLColorRgba &color);

    QVector<GLPoint> * points(){return &m_points;}
    QVector<IndexType> * indices(){return &m_indices;}
    GLESRenderer * renderer() {return m_renderer;}

    //Geometry arrays for coordinate axes
    QVector<QVector3D> m_vertices;
    QVector<QVector3D> m_normals;
    QVector<QVector3D> m_texCoords;
    QVector<QVector4D> m_colors;
    //flags
    bool m_geometryIsValid;
    bool m_colorArrayEnabled;
    bool m_texturesEnabled;
    bool m_lightingEnabled;
    bool m_activatePaintBeforeQml;
    bool m_activatePaintAfterQml;
    bool m_orthoMode;//orthogonal projection for debugging
    float m_orthoRange; //range to be displayed
    bool m_drawAxes;

    //lighting and colors
    QVector3D m_lightDirection;
    GLfloat m_ambientLightBrightness;
    GLColorRgba m_backgroundColor;

    //vectors for lookAt
    QVector3D m_eye;
    QVector3D m_center;
    QVector3D m_up;

    //pespective
    float m_fovy;
    float m_aspect;
    float m_near;
    float m_far;

    //viewport
    int m_viewportX;
    int m_viewportY;
    int m_viewportW;
    int m_viewportH;

    //rotation
    QTimer * m_timer;
    float m_guiThreadRotation;
    float m_renderThreadRotation;
    bool m_loopMovement; //single or permanent rotation
    bool m_movementEnabled;
    float m_rotationIncrement;


    //shaders
    QString m_vertexShaderFilename;
    QString m_fragmentShaderFilename;

    //Camera transformation matrix
    QMatrix4x4 m_cameraTransform;

    /**
     * @brief createShaderProgram
     * creates and links the shader program using vshader.
     */
    virtual void initializeRenderer();
    //containers for geometry
    QVector <GLPoint> m_points;
    QVector <IndexType> m_indices;
    //renderer
    GLESRenderer * m_renderer;
    int m_firstAxesPoint;
    int m_lastAxesPoint;

    bool m_guiThreadShutdownRequest; //flag for triggering shutdown, copied in synchronizeThreads
    bool m_renderThreadShutdownRequest; //triggers cleanupOpenGl()
};

#endif // GLITEM_H
