#ifndef GLTABLEPLATE_H
#define GLTABLEPLATE_H

#include "glbody.h"


/**
 * @brief GLTablePlate stellt die Tischplatte dar.
 * @authors Luca Graef, Julian Witte
 */
class GLTablePlate: public GLBody
{

public:
    /**
     * @brief GLToken Konstruktor.
     * @param name
     * @param radius
     * @param textureFile
     * @param color
     */
    GLTablePlate(const QString & name, float radius = 1.0f, const QString textureFile = ":/textures/table.JPG", const GLColorRgba & color = GLColorRgba::clGray);

    /**
     * @brief makeSurface
     * @param pointContainer
     * @param indexContainer
     */
    void makeSurface(QVector<GLPoint> * pointContainer, QVector<IndexType> *indexContainer)Q_DECL_OVERRIDE;

     /**
      *  @brief draw the token.
      * @param renderer
      * @param useBuffers
      */
    void draw(GLESRenderer * renderer, bool useBuffers = false) Q_DECL_OVERRIDE;

};

#endif // GLTABLEPLATE_H
