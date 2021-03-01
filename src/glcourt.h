#ifndef GLCOURT_H
#define GLCOURT_H

#include "glbody.h"

/**
 * @brief GLCourt stellt das Spielfeld dar.
 * @authors Luca Graef, Julian Witte
 */

class GLCourt : public GLBody
{

public:
    /**
     * @brief GLCourt Konstruktor.
     * @param name
     * @param radius
     * @param textureFile
     * @param color
     */
    GLCourt(const QString & name, float radius = 1.0f, const QString textureFile = "", const GLColorRgba & color = GLColorRgba::clBlue);

    /**
     * @brief makeSurface
     * @param pointContainer
     * @param indexContainer
     */
    void makeSurface(QVector<GLPoint> * pointContainer, QVector<IndexType> *indexContainer)Q_DECL_OVERRIDE;

     /**
      * @brief draw the court.
      * @param renderer
      * @param useBuffers
      */
    void draw(GLESRenderer * renderer, bool useBuffers = false)Q_DECL_OVERRIDE;

};

#endif // GLCOURT_H
