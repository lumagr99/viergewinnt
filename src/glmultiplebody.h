#ifndef GLMULTIPLEBODY_H
#define GLMULTIPLEBODY_H

#include "glbodygroup.h"

/**
 * @brief The GLMultipleBody is a GLBody object drawn with multiple transformations, colors or textures.
 * For drawing, buffers or containers are bound and kept bound while drawing the bodies.
 * Copies in m_copies may use different sets of points in the containers/buffers or use the same set of points with different transformation,
 * texture and color settings. However, points and indices are stored in a single set of containers, passed to the call of makeSurface.
 * Warning: DO NOT exceed 65535 for containersize! Indices are of type GLushort!
 */
class GLMultipleBody : public GLBody
{
public:    
    /**
     * @brief GLMultipleBody calls GLBody constructor.
     * Create m_theBody and its copies and transformations etc. in subclassed constructors
     * @param name The name for this GLBody. Copies should get names with index suffixes.
     */
    GLMultipleBody(const QString & name);

    /**
      * Deletes all copies.
      */
    ~GLMultipleBody()Q_DECL_OVERRIDE;
    /**
     * @brief destroyTextureObjects delete all texture objects from all copies
     */
    virtual void destroyTextureObjects()Q_DECL_OVERRIDE;

    /** Binds the buffers or containers and calls drawGeometry on every body int m_objects.
     * Calls makeSurface if m_surfaceIsValid is false.
     * Needs an active (made current) GL-Context.
     */
     void draw(GLESRenderer * renderer, bool useBuffers = false)Q_DECL_OVERRIDE;
protected:
     /**
      * @brief beforeDrawGeometries is called for every object before drawGeomeries.
      * Does nothing. To be overridden by subclasses.
      * @param index
      */
     virtual void beforeDrawGeometries(GLESRenderer * renderer, int index);
     /**
      * @brief afterDrawGeometries is called for every object after drawGeomeries.
      * Does nothing. To be overridden by subclasses.
      * @param index
      */
     virtual void afterDrawGeometries(GLESRenderer * renderer, int index);

     /**
     * @brief drawBody Draws the body at index in m_objects.
     * @param index
     */
//    virtual void drawGeometries(GLESRenderer *renderer, bool useBuffers);
//    virtual void drawDebugGeometries(GLESRenderer * renderer, bool useBuffers = false);
     QVector<GLBody *> m_copies;
};

#endif // GLMULTIPLEBODY_H
