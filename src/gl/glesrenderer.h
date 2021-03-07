/***************************************************************************
 *   Copyright (C) 2012 by Walter Roth                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef GLESRENDERER_H
#define GLESRENDERER_H

#include <QList>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QStack>

#include "glcolorrgba.h"
#include "gldefines.h"

/** \brief A render program that can be used as a simple replacement for the OpenGL fixed
  * function pipeline.
  * \author Walter Roth, 2014
  *
  * GLESRenderer supports vertex, colors, normals, texCoords, textures and matrix manipulation.
  * It owns a modelview and a projection matrix which can be used like in the old days of OpenGL 1.
  * The vertex shader program needs to stick to the following variable names:
  * attribute highp vec4 a_Vertex;
  * attribute highp vec4 a_Color;
  * attribute highp vec3 a_Normal;
  * attribute highp vec2 a_TexCoord;
  *
  * uniform bool u_ColorArrayEnabled
  * uniform bool u_LightingEnabled;
  * uniform highp mat4 u_MvpMatrix;
  * uniform highp mat3 u_NormalMatrix;
  * uniform highp vec3 u_LightDirection;
  * uniform highp vec4 u_AmbientAndDiffuseColor;
  * uniform highp vec4 u_SpecularColor;
  * uniform highp float u_Shininess;
  * uniform mediump float int u_PointSize;
  *
  * The fragment shader texture variable must have the following name:
  * uniform sampler2d s_Texture;
  *
  * These names are used to determine the locations of the varables in the shaders.
  * If you need more variables, subclass GLESRenderer and add new variable(s) to
  * location_XXX variables. Extend virtual intialize(), bind() and disableAttributeArrays() functions
  * to support the new variable(s).
  *
  * The shader source should be in a separate file (e.g. added to the resources).
  * Example shaders vshader1 and fshader1 emulate a simple fixed-function lighted OpenGL 1 scene.
  *
  * Call initialize, after a valid OpenGL context has been made current.
  * Setup viewing with setViewport, setLookAt and setPerspective.
  * Viewport coordinates must be given as
  * GL-viewport coordinates, where lower left is (0,0).
  * Call bind to bind the program and set the uniform values you need.
  * After this, call activateAttributeLocation to enable the attribute arrays.
  * To change attribute arrays, call disableAttributeArrays and then again activateAttributeLocations.
  * Finally call release.
  *
  * GLESRenderer contains functions for projecting points from modelspace to screen and unprojecting points
  * from screen to model space. These functions use the current matrix settings. Therefore, you have to
  * (re)set the matrices to the values that are used to paint the model, before you call the project and
  * unproject functions.
  *
  * For convenient mouse interaction, GLESRenderer supplies the calculateMousePoints(), distanceToMouseClick()
  * and mouseIntersection() functions. These functions use the unproject functions and therefore do also need
  * valid model painting matrix settings.
  */

#define USE_QOPENGL_FUNCTIONS

class GLESRenderer : public QObject
        #ifdef USE_QOPENGL_FUNCTIONS
        ,
        public QOpenGLFunctions
        #endif
{
    Q_OBJECT
public:
    typedef enum {
        INVALID_LOCATION,
        VERTEX_LOCATION,
        NORMAL_LOCATION,
        COLOR_LOCATION,
        TEXCOORD_LOCATION
    } AttributeLocation;

    explicit GLESRenderer(QObject* parent,
                          const QString& vShaderFilename = ":/src/vshader.vsh",
                          const QString& fShaderFilename = ":/src/fshader.fsh");

    virtual ~GLESRenderer();

    //=========================================================================
    /**
      * \name Initialization, binding and releasing
      */
    ///@{
    /**
      * Compile shaders, get attribute and uniform locations of shaders.
      * This function needs an active OpenGL context.
      * Returns false, if initialization failed.
      */
    virtual bool initialize();

    /**
      * Bind program and transfer attribute and uniform data to the shaders.
      * Calls initialize, if not alraedy initialized.
      * Returns false, if binding failed.
      */
    virtual bool bind();

    /**
      * Releases program. To be called, when all rendering is finished.
      * Calls disableAttributeArrays and releases program.
      */
    void release();
    ///@}

    //=================================================================
    /**
      * \name Unproject, project and mouse interaction functions
      */
    ///@{
    /**
      * Uses viewport and matrix settings to reverse the vertex transformation chain.
      * vWin x,y and z are the coordinates in window coordinates. Lower left of viewport
      * is winX = 0, winY = 0. Use winZ = 0.0 for a point on near clipping plane
      * and winZ = 1.0 for a point on far clipping plane.
      * This function assumes, that the vertex shader computes gl_positon as v * mvpMatrix.
      * Use a custom shader reverse function, if this is not true.
      * Returns coordinate in object space.
      */
    QVector3D unProjectViewportPoint(const QVector3D& vWin);

    /**
      * Performs inverse viewport transform.
      */
    QVector3D viewportToClip(const QVector3D& v);
    /**
      * Returns the points on near and far clipping plane, that correspond to the
      * mouseX and mouseY coordinates of a mouse click.
      * mouseX and mouseY are coordinates as delivered by QMouseEvent or QDeclarativeMouseEvent.
      */
    void calculateMousePoints(QVector3D* nearPoint, QVector3D* farPoint, const QPoint& mousePos);

    /**
      * Calculate the distance of the "mouse ray line" to point p in model space.
      * mouseX, mouseY are the coordinates of the mouse click as delivered by QMouseEvent.
      * Returns distance.
      */
    float distanceToMouseClick(QVector3D p, const QPoint& mousePos);

    /**
      * Calculates intersection of "mouse ray line" with the plane defined by normal and d.
      * mouseX, mouseY are the coordinates of the mouse click as delivered by QMouseEvent.
      * Returns true, if intersection is in the visible frustum, else returns false.
      * If return value is true, *intersection is the intersection of the mouse ray line with
      * the plane normal*(x,y,z) + d = 0.
      * If return value is false, *intersection is not modified.
      */
    bool mouseIntersection(QVector3D* intersection, QVector3D normal, float d, const QPoint& mousePos);
    ///@}

    //======================================================================================
    ///@{

    /** Getters and setters for uniforms
       */
    /**
      * Set modelview matrix. Updates mvpMatrix and normalMatrix too.
      * Call setPMatrix or setPerspective first.
      */
    void setMvMatrix(const QMatrix4x4 newVal);
    /**
      * Getter for debugging.
      */
    const QMatrix4x4& getMvMatrix() { return m_mvMatrix; }

    /**
      * Getter for debugging.
      */
    const QMatrix4x4 getViewMatrix() { return m_viewMatrix; }
    /**
       * Getter for debugging.
       */
    const QMatrix3x3& getNormalMatrix() { return m_normalMatrix; }

    /**
      * Sets mvMatrix to a lookAt transformation.
      * Call setPMatrix or setPerspective first.
      */
    void setLookAt(const QVector3D& eye, const QVector3D& center, const QVector3D& up);

    /**
      * Set projection matrix. Call setMvMatrix after this.
      */
    void setPMatrix(const QMatrix4x4 newVal);
    /**
      * Getter for debugging.
      */
    const QMatrix4x4& getPMatrix() { return m_pMatrix; }
    /**
      * Setup projection matrix. Call setMvMatrix after this.
      */
    void setPerspective(GLfloat fovy, GLfloat aspect, GLfloat nearClip, GLfloat farClip);
    /**
     * @brief setOrtho Set pMatrix to an orthogonal projection.
     * @param left
     * @param right
     * @param bottom
     * @param top
     * @param nearPlane
     * @param farPlane
     */
    void setOrtho(float left, float right, float bottom, float top, float nearPlane, float farPlane);
    /**
      * Set viewport
      * Viewport coordinates must be given as
      * GL-viewport coordinates, where lower left is (0,0).
      */
    void setViewport(int x, int y, int w, int h);
    /**
      * Get the present viewport settings from GL engine
      */
    void readGLViewportSettings();

    /**
      * Set the color for ambient and diffuse lighting (or no lighting).
      * Alternatively use a color array and color attribute.
      * Defaults to white.
      */
    void setAmbientAndDiffuseColor(const GLColorRgba newVal);
    /**
      * Set the dimming factor for ambient light.
      * Defaults to 0.2.
      */
    void setAmbientLightBrightness(float newVal);

    /**
      * Set the color for specular lighting.
      * Defaults to white.
      */
    void setSpecularColor(const GLColorRgba newVal);
    /**
      * Set the shininess (exponent) for specular lighting.
      * Defaults to 200.
      */
    void setShininess(float newVal);

    /**
     * @brief setSourceAlpha Set the source alpha value used for blending.
     * glEnable(GL_BLEND) and glBlendfunc calls are required!
     * @param newVal
     */
    void setSourceAlpha(float newVal);
    /**
     * @brief sourceAlpha The present source alpha value for blending
     * @return
     */
    float sourceAlpha() { return m_sourceAlpha; }
    /**
      * Enable / disable lighting.
      * Defaults to false.
      */
    void setLightingEnabled(bool newVal);
    /**
      * Returns lighting flag.
      */
    bool isLightingEnabled() { return m_lightingEnabled; }
    /**
      * Enable / disable color array.
      * Defaults to false.
      */
    void setColorArrayEnabled(bool newVal);
    /**
      * Returns lighting flag.
      */
    bool isColorArrayEnabled() { return m_colorArrayEnabled; }
    /**
      * Set the texture flag.
      * Defaults to false.
      */
    void setTextureEnabled(bool newVal);
    /**
      * Returns texture flag;
      */
    bool isTextureEnabled() { return m_textureEnabled; }
    /**
     * @brief setMaskActive Set the fragment shader mask flag
     * @param newVal
     */
    void setMaskActive(bool newVal);
    /**
     * @brief isMaskActive
     * @return Returns mask flag.
     */
    bool isMaskActive() { return m_maskActive; }
    /**
      * Set light direction.
      * Defaults to (1.0, 1.0, 1.0)
      */
    void setLightDirection(const QVector3D& newVal);
    /**
      * Getter.
      */
    const QVector3D lightDirection() { return m_lightDirection; }

    /**
      * Set size of points drawn with GL_POINTS.
      * Defaults to 2.0.
      */
    void setPointSize(float newVal);
    /**
     * @brief setMaskCenter Set center of fragment shader mask
     * @param newVal New center in viewport coordinates
     */
    void setMaskCenter(const QPoint& newVal);

    /**
     * @brief setMaskDiameter Set diameter of fragment shader mask
     * @param newVal New diameter in pixels.
     */
    void setMaskDiameter(float newVal);
    /**
      * Returns pointSize;
      */
    float pointSize() { return m_pointSize; }

    /**
      * Pushes mvMatrix.
      */
    void pushMvMatrix() { m_mvMatrixStack.push(m_mvMatrix); }

    /**
     * @brief matrixStackSize for debugging
     * @return
     */
    int matrixStackSize() { return m_mvMatrixStack.size(); }
    /**
      * Pops mvMatrix from stack and updates normalMatrix and mvpMatrix.
      */
    void popMvMatrix();

    /**
      * Translates mvMatrix by v
      */
    void translate(const QVector3D& v);
    void translate(float x, float y, float z);
    /**
      * Rotates mvMatrix by angle around axis
      */
    void rotate(GLfloat angle, const QVector3D& axis);
    /**
      * Rotates mvMatrix by angle around axis (x,y,z)
      */
    void rotate(float angle, float x, float y, float z);
    void zoom(float zoomFactor);

    /**
     * @brief transform Multiplies mvMatrix with transformation
     * @param transformation
     */
    void transform(const QMatrix4x4& transformation);

    /**
     * @brief addTransformation
     * Multiplies additionalTransformation to mvMatrix.
     * @param additionalTransformation
     */
    void addTransformation(const QMatrix4x4 additionalTransformation);
    /**
      * Scales mvMatrix.
      */
    void scale(const QVector3D& v);

    ///@}

    //======================================================================================

    ///@{
    /**
      * \name Attribute setters
      */

    /**
      * Enables 2D Vertex, normal, color or texCoord arrays and sets start adresses of arrays
      * Type may be: VERTEX_LOCATION, NORMAL_LOCATION, COLOR_LOCATION, TEXCOORD_LOCATION
      */
    bool activateAttributeArray(AttributeLocation location, const QVector2D* values, int stride = 0);
    /**
      * Enables 3D Vertex, normal, color or texCoord arrays and sets start adresses of arrays
      * Type may be: VERTEX_LOCATION, NORMAL_LOCATION, COLOR_LOCATION, TEXCOORD_LOCATION
      */
    bool activateAttributeArray(AttributeLocation location, const QVector3D* values, int stride = 0);
    /**
      * Enables 4D Vertex or color arrays and sets start adresses of arrays
      * Type may be: VERTEX_LOCATION, COLOR_LOCATION
      */
    bool activateAttributeArray(AttributeLocation location, const GLColorRgba* values, int stride = 0);
    /**
      * To be used with arrays (e.g. QVector<GLPoint>) of GLPoint objects.
      * Enables 3D Vertex, normal, color or texCoord buffer
      * location may be: VERTEX_LOCATION, NORMAL_LOCATION, COLOR_LOCATION, TEXCOORD_LOCATION
      * tupleSize is set to 3 for VERTEX_LOCATION, NORMAL_LOCATION, TEXCOORD_LOCATION, to 4 for COLOR_LOCATION
      */
    void activateAttributeBuffer(AttributeLocation location);
    /**
      * Disables all enabled attribute arrays.
      * To be called after rendering an object.
      */
    virtual void disableAttributeArrays();
    ///@}

    //==============================================================
    ///@{

    /**
      * \name Debugging
      */

    /**
      * Multiplies current mvp matrix with v. Mainly for debugging.
      */
    QVector3D modelToClip(const QVector3D& v);
    /**
      * Performs viewport transform. Mainly for debugging.
      */
    QVector3D clipToViewport(const QVector3D& v);
    /**
     * @brief modelToViewport transform vector from model space to viewport
     * @param v
     * @return vieport coordinates
     */
    QVector3D modelToViewport(const QVector3D& v);
    /**
     * @brief modelToMouse transform vector from model space to mouse coordinates
     * @param v
     * @return
     */
    QVector3D modelToMouse(const QVector3D& v);

protected:
    /**
     * @brief activateAttributeArray
     * Enables  Vertex, normal, color or texCoord arrays and sets start adresses of arrays
     * location may be: VERTEX_LOCATION, NORMAL_LOCATION, COLOR_LOCATION, TEXCOORD_LOCATION
     * @param location
     * @param values Address of first number
     * @param tupleSize Numbers per vertex
     * @param stride Distance to next tuple
     * @return
     */
    bool activateAttributeArray(AttributeLocation location, const float* values, int tupleSize, int stride = 0);

    bool m_initialized;
    bool m_bound;

    //Uniforms to be passed to the shader
    //flags
    bool m_lightingEnabled; // flag for lighting
    bool m_colorArrayEnabled; //flag for a valid color array
    bool m_textureEnabled; //flag for active texturing
    bool m_invertedMvpMatrixValid; //flag for a valid inverted ModelViewProjection matrix
    bool m_maskActive; //flag for an active fragment shader mask

    //matrices
    QMatrix4x4 m_mvpMatrix; //combined modelview and projection matrix
    //The inverted mvp matrix for unproject purposes
    //Use the invertedMvpMatrixValid flag for calculating it when required.
    QMatrix4x4 m_invertedMvpMatrix;
    QStack<QMatrix4x4> m_mvMatrixStack;
    QMatrix4x4 m_mvMatrix; //modelview matrix
    QMatrix4x4 m_viewMatrix; //viewMatrix part of mvMatrix for debugging
    QMatrix4x4 m_pMatrix; //projection matrix
    float m_fovy;
    float m_aspect;
    float m_nearClip; //near clipping plane
    float m_farClip; //far clipping plane
    bool m_OrthoMode;
    QMatrix3x3 m_normalMatrix; //matrix for normal transformation

    //lighting
    QVector3D m_lightDirection; //direction of directional light
    QVector3D m_halfPlaneVector; //half plane vector for specular light calculation
    GLColorRgba m_ambientAndDiffuseColor;
    float m_ambientLightBrightness;
    GLColorRgba m_specularColor;
    float m_shininess;
    float m_sourceAlpha;

    //texturing
    float m_pointSize;
    //masking
    float m_maskDiameter;
    QPoint m_maskCenter;

    //Shaders and program
    QString m_vShaderFileName; //vertex shader
    QString m_fShaderFileName; //fragment shader
    QOpenGLShader* m_vShader; //vertex shader
    QOpenGLShader* m_fShader; // fragment shader
    QOpenGLShaderProgram* m_renderProgram;

    // Locations of shader variables
    int m_location_aVertex;
    int m_location_aColor;
    int m_location_aNormal;
    int m_location_aTexCoord;
    int m_location_uColorArrayEnabled;
    int m_location_uLightingEnabled;
    int m_location_uNormalMatrix;
    int m_location_uMvpMatrix;
    int m_location_uAmbientAndDiffuseColor;
    int m_location_uAmbientLightBrightness;
    int m_location_uSpecularColor;
    int m_location_uShininess;
    int m_location_uSourceAlpha;
    int m_location_uLightDirection;
    int m_location_uHalfPlaneVector;
    int m_location_uPointSize;
    int m_location_uTextureSampler;
    int m_location_uTextureEnabled;
    int m_location_uMaskDiameter;
    int m_location_uMaskCenter;
    int m_location_uMaskActive;

    //The list of enabled attribute locations
    QList<int> m_activeAttributeLocations;
    //viewport
    int m_viewportX;
    int m_viewportY;
    int m_viewportW;
    int m_viewportH;
};

#endif // GLESRENDERER_H
