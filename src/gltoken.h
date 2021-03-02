#ifndef GLTOKEN_H
#define GLTOKEN_H

#include "glbody.h"

/**
 * @brief GLToken stellt den Grundspielstein dar.
 * @authors Luca Graef, Julian Witte
 */
class GLToken: public GLBody
{

public:
    /**
     * @brief GLToken Konstruktor.
     * @param name
     * @param radius
     * @param textureFile
     * @param color
     */
    GLToken(const QString & name, float radius = 1.0f, const QString textureFile = "", const GLColorRgba & color = GLColorRgba::clBlue);

    /**
     * @brief makeSurface
     * @param pointContainer
     * @param indexContainer
     */
    virtual void makeSurface(QVector<GLPoint> * pointContainer, QVector<IndexType> *indexContainer) Q_DECL_OVERRIDE;

     /**
      *  @brief draw the token.
      * @param renderer
      * @param useBuffers
      */
    virtual void draw(GLESRenderer * renderer, bool useBuffers = false) Q_DECL_OVERRIDE;

};

#endif // GLTOKEN_H
