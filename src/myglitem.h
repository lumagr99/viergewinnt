#ifndef MYGLITEM_H
#define MYGLITEM_H

#include "glitem.h"
#include "QVector3D"
#include "QOpenGLBuffer"
#include "glmouseray.h"
#include "glcourt.h"
#include "gltokengreen.h"
#include "gltokenred.h"
#include "gltableplate.h"

class MyGLItem : public GLItem
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor.
     */
    MyGLItem();

protected:

    /**
     * @brief paintUnderQmlScene
     * Virtual function for painting under a QML scene. This function is called by paintBefore after
     * calling createGeometries and initializing and binding the renderer.
     * Overwrite in subclasses for painting geometries in m_points with the renderer.
     */
    virtual void paintUnderQmlScene() Q_DECL_OVERRIDE;

    /**
     * @brief paintUnderQmlScene
     * Virtual function for painting on top of a QML scene. This function is called by paintAfter after
     * calling createGeometries and initializing and binding the renderer.
     * Overwrite in subclasses for painting geometries in m_points with the renderer.
     */
    virtual void paintOnTopOfQmlScene() Q_DECL_OVERRIDE;

    /**
     * @brief Erstellt die Punkte für die Axen und das Dreieck.
     */
    void setupGeometry() Q_DECL_OVERRIDE;

private:
     QOpenGLBuffer *m_vertexBuffer;

     /**
      * @brief Aktiviert buffer.
      */
     void activateBuffers();




protected:
     GLCourt *m_court;
     GLTokenGreen *m_token;
     GLTablePlate *m_tableplate;

     /**
      * @brief Wird beim scrollen mit dem Mausrad aufgerufen.
      * @param e
      */
     void wheelEvent (QWheelEvent * e) Q_DECL_OVERRIDE;

     GLMouseRay *m_mouseray;
     QPoint m_mousePosition;
     bool m_mousePressed = false;
     int m_mouse_x = 0;
     int m_mouse_y = 0;

     /**
      * @brief doSynchronizeThreads Copy data from GUI-thread to render-thread and vice versa.
      * Virtual function to be overridden by subclasses
      */
     virtual void doSynchronizeThreads() Q_DECL_OVERRIDE;

public slots:

    /**
     * @brief Startet die Linksrotation.
     * @param increment. Rotationsgeschwindigkeit.
     */
    void rotateLeft(float increment);

    /**
     * @brief Stoppt die Rotation.
     */
    void stopRotation();

    /**
     * @brief Startet die Rechtsrotation.
     * @param increment. Rotationsgeschwindigkeit.
     */
    void rotateRight(float increment);

    /**
     * @brief Wird bei einem Mausklick aufgerufen
     * @param x Position der Maus x Achse
     * @param y Position der Maus y Achse
     */
    void mousePressed(int x, int y);

    /**
     * @brief Wird bei einer Mausbewegung aufgerufen
     * @param x Position der Maus x Achse
     * @param y Position der Maus y Achse
     */
    void mousePositionChanged(int x, int y);

    /**
     * @brief Wird bei dem loslassen eines Mausklicks aufgerufen
     * @param x Position der Maus x Achse
     * @param y Position der Maus y Achse
     */
    void mouseReleased(int x, int y);

};

#endif // MYGLITEM_H
