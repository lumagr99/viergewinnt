#ifndef GLMOUSERAY_H
#define GLMOUSERAY_H

#include "glbody.h"

/**
 * @brief MouseRay für das Debuggen von Mausklicks.
 */
class GLMouseRay : public GLBody {

public:
    /**
     * @brief Konstruktor. Erstellt und Initialisiert ein neues GLMouseRay-Objekt.
     * @param name Name des Objekts.
     * @param color Farbe des Objekts.
     */
    explicit GLMouseRay(const QString& name, const GLColorRgba& color);

    /**
     * @brief Erzeugt die Geometriedaten des Objekts.
     * @param pointContainer QVector mit den Punkten des Objekts.
     * @param indexContainer QVector mit den Punktindizes des Objekts.
     */
    virtual void makeSurface(QVector<GLPoint>* pointContainer, QVector<IndexType>* indexContainer) Q_DECL_OVERRIDE;

    /**
     * @brief Zeichnet das GLMouseRay-Objekt im aktuellen Kontext.
     * @param renderer Renderer mit dem das MouseRay-Objekt gezeichnet werden soll.
     * @param useBuffers Verwendung von Buffern.
     */
    virtual void draw(GLESRenderer* renderer, bool useBuffers = false) Q_DECL_OVERRIDE;

    /**
     * @brief Setzt die Punkte des MouseRays.
     * @param nearPoint Nahpunkt.
     * @param farPoint Fernpunk.
     */
    void setPoints(const QVector3D& nearPoint, const QVector3D& farPoint);

private:
    /**
     * @brief Nahpunkt.
     */
    QVector3D m_nearPoint;

    /**
     * @brief Fernpunkt.
     */
    QVector3D m_farPoint;
};

#endif // GLMOUSERAY_H
