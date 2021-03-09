#ifndef GLMOUSERAY_H
#define GLMOUSERAY_H

#include "glbody.h"

/**
 * @brief The GLMouseRay class
 *
 * Objekte der Klasse MouseRay werden zum Debuggen von Mausklicks verwendet.
 */
class GLMouseRay : public GLBody {

public:
    /**
     * @brief GLMouseRay
     *
     * Erstellt und Initialisiert ein neues GLMouseRay-Objekt.
     *
     * @param name Name des Objekts.
     * @param color Farbe des Objekts.
     */
    explicit GLMouseRay(const QString& name, const GLColorRgba& color);

    /**
     * @brief makeSurface
     *
     * Erzeugt die Geometriedaten des Objekts.
     *
     * @param pointContainer QVector mit den Punkten des Objekts.
     * @param indexContainer QVector mit den Punktindizes des Objekts.
     */
    virtual void makeSurface(QVector<GLPoint>* pointContainer, QVector<IndexType>* indexContainer) Q_DECL_OVERRIDE;

    /**
     * @brief draw
     *
     * Zeichnet das GLMouseRay-Objekt im aktuellen Kontext.
     *
     * @param renderer Renderer mit dem das MouseRay-Objekt gezeichnet werden soll.
     * @param useBuffers Verwendung von Buffern.
     */
    virtual void draw(GLESRenderer* renderer, bool useBuffers = false) Q_DECL_OVERRIDE;

    /**
     * @brief setPoints
     *
     * Setzt die Punkte des MouseRays.
     *
     * @param nearPoint Nahpunkt.
     * @param farPoint Fernpunk.
     */
    void setPoints(const QVector3D& nearPoint, const QVector3D& farPoint);

private:
    /**
     * @brief m_nearPoint
     *
     * Nahpunkt.
     */
    QVector3D m_nearPoint;

    /**
     * @brief m_farPoint
     *
     * Fernpunkt.
     */
    QVector3D m_farPoint;
};

#endif // GLMOUSERAY_H
