/***************************************************************************
 *   Copyright (C) 2008, 2012 by Walter Roth                               *
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
#include "glesrenderer.h"

#include <math.h>

//#define DEBUG_GLESRENDERER

//#ifdef DEBUG_GLESRENDERER
#include "glpoint.h"
#include "shaderdebugger.h"
//#endif

#ifdef Q_OS_ANDROID
#ifndef GLES
#define GLES //Android uses GLES 2.0
#endif
#endif

GLESRenderer::GLESRenderer(QObject* parent,
    const QString& vShaderFilename,
    const QString& fShaderFilename)
    : QObject(parent)
{
    this->m_vShaderFileName = vShaderFilename;
    this->m_fShaderFileName = fShaderFilename;

    //flags
    m_initialized = false;
    m_bound = false;
    m_lightingEnabled = false; // flag for lighting
    m_colorArrayEnabled = false;
    m_invertedMvpMatrixValid = false; //delay matrix inversion until it is really neccessary
    m_textureEnabled = false;
    m_maskActive = false;

    m_lightDirection = QVector3D(1.0, 1.0, 1.0); //position of directional light
    m_pointSize = 4.0f;
    m_shininess = 200.0;
    m_sourceAlpha = 1.0f;
    m_ambientAndDiffuseColor = cl_White;
    m_ambientLightBrightness = 0.2f;
    m_specularColor = cl_White;

    m_vShader = nullptr; //vertex shader, render thread
    m_fShader = nullptr; // fragment shader, render thread
    m_renderProgram = nullptr; // GUI-Thread

    // Locations of shader variables
    //attributes
    m_location_aVertex = -1;
    m_location_aColor = -1;
    m_location_aNormal = -1;
    m_location_aTexCoord = -1;
    //uniforms
    //flags
    m_location_uColorArrayEnabled = -1;
    m_location_uTextureEnabled = -1;
    m_location_uLightingEnabled = -1;
    m_location_uMaskActive = -1;
    //matrices
    m_location_uNormalMatrix = -1;
    m_location_uMvpMatrix = -1;
    //lighting
    m_location_uAmbientAndDiffuseColor = -1;
    m_location_uAmbientLightBrightness = -1;
    m_location_uSpecularColor = -1;
    m_location_uShininess = -1;
    m_location_uSourceAlpha = -1;
    m_location_uLightDirection = -1;
    m_location_uHalfPlaneVector = -1;

    //texture
    m_location_uTextureSampler = -1;
    //mask
    m_location_uMaskCenter = -1;
    m_location_uMaskDiameter = -1;
    //viewport
    m_viewportX = 0;
    m_viewportY = 0;
    m_viewportW = 100;
    m_viewportH = 100;

    //clippings
    m_fovy = 45.0;
    m_aspect = 1.0;
    m_nearClip = 1.0;
    m_farClip = 10.0;
}
/**
 * @brief GLESRenderer::~GLESRenderer
 * We need to delete the shaders here. They can not be added to the child list, because they live in render thread.
 */
GLESRenderer::~GLESRenderer()
{
    if (m_vShader)
        delete m_vShader;
    if (m_fShader)
        delete m_fShader;
}

/**
  * Set modelview matrix. Updates mvpMatrix and normalMatrix too.
  * Call setPMatrix first.
  */
void GLESRenderer::setMvMatrix(const QMatrix4x4 newVal)
{
    m_mvMatrix = newVal;
    m_normalMatrix = m_mvMatrix.normalMatrix(); //invert and transpose mvMatrix
    m_invertedMvpMatrixValid = false; //delay matrix inversion until it is really neccessary

    if (m_bound && (m_location_uNormalMatrix != -1))
        m_renderProgram->setUniformValue(m_location_uNormalMatrix, m_normalMatrix);

    m_mvpMatrix = m_pMatrix * m_mvMatrix;
    if (m_bound && (m_location_uMvpMatrix != -1))
        m_renderProgram->setUniformValue(m_location_uMvpMatrix, m_mvpMatrix);
#ifdef DEBUG_GLESRENDERER
    ShaderDebugger::debugMatrix4x4(m_mvMatrix, "GLESRenderer uses modelview matrix:");
    ShaderDebugger::debugMatrix3x3(m_normalMatrix, "GLESRenderer uses normal matrix:");
    ShaderDebugger::debugMatrix4x4(m_mvpMatrix, "GLESRenderer uses  MVP matrix:");
#endif
}

/**
  * Sets mvMatrix to a lookAt transformation.
  * Call setPMatrix or setPerspective first.
  */
void GLESRenderer::setLookAt(const QVector3D& eye, const QVector3D& center, const QVector3D& up)
{
    QMatrix4x4 m;
    m.setToIdentity();
    m.lookAt(eye, center, up);
    m_viewMatrix = m;
    setMvMatrix(m);
}

/**
  * Set projection matrix. Call setMvMatrix after this.
  */
void GLESRenderer::setPMatrix(const QMatrix4x4 newVal)
{
    m_pMatrix = newVal;
    m_mvpMatrix = m_pMatrix * m_mvMatrix;
    if (m_bound && (m_location_uMvpMatrix != -1))
        m_renderProgram->setUniformValue(m_location_uMvpMatrix, m_mvpMatrix);
}

/**
  * Setup projection matrix. Call setMvMatrix after this.
  */
void GLESRenderer::setPerspective(GLfloat fovy, GLfloat aspect, GLfloat nearClip, GLfloat farClip)
{
    m_fovy = fovy;
    m_aspect = aspect;
    m_nearClip = nearClip;
    m_farClip = farClip;
    m_pMatrix.setToIdentity();
    m_pMatrix.perspective(m_fovy, m_aspect, m_nearClip, m_farClip);
    m_mvpMatrix = m_pMatrix * m_mvMatrix;
    if (m_bound && (m_location_uMvpMatrix != -1))
        m_renderProgram->setUniformValue(m_location_uMvpMatrix, m_mvpMatrix);
#ifdef DEBUG_GLESRENDERER
    ShaderDebugger::debugMatrix4x4(m_pMatrix, "GLESRenderer uses projection matrix:");
#endif
}

void GLESRenderer::setOrtho(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
    m_fovy = 0.0;
    m_aspect = (left - right) / (top - bottom);
    m_nearClip = nearPlane;
    m_farClip = farPlane;
    m_pMatrix.setToIdentity();
    m_pMatrix.ortho(left, right, bottom, top, nearPlane, farPlane);
    m_mvpMatrix = m_pMatrix * m_mvMatrix;
    if (m_bound && (m_location_uMvpMatrix != -1))
        m_renderProgram->setUniformValue(m_location_uMvpMatrix, m_mvpMatrix);
#ifdef DEBUG_GLESRENDERER
    ShaderDebugger::debugMatrix4x4(m_pMatrix, "GLESRenderer uses projection matrix:");
#endif
}

/**
  * Set viewport
  */
void GLESRenderer::setViewport(int x, int y, int w, int h)
{
    m_viewportX = x;
    m_viewportY = y;
    m_viewportW = w;
    m_viewportH = h;
}

/**
  * Uses viewport and matrix settings to reverse the vertex transformation chain.
  * WinX, winY, winZ are the coordinates in window coordinates. Lower left of viewport
  * is winX = 0, winY = 0. Use winZ = 0.0 for a point on near clipping plane
  * and winZ = 1.0 for a point on far clipping plane.
  * This function assumes, that the vertex shader computes gl_positon as v * mvpMatrix.
  * Use a custom shader reverse function, if this is not true.
  * Returns coordinate in object space.
  */
QVector3D GLESRenderer::unProjectViewportPoint(const QVector3D& vWin)
{
    QVector3D vClip = viewportToClip(vWin);
    //    QVector3D vWinBack = clipToViewport(vClip);
    ShaderDebugger::debugVector3D(vClip, "Vector in clip space:");
    if (!m_invertedMvpMatrixValid) {
        m_invertedMvpMatrix = m_mvpMatrix.inverted();
        m_invertedMvpMatrixValid = true;
    }
    ShaderDebugger::debugMatrix4x4(m_mvpMatrix, "MVP Matrix:");
    ShaderDebugger::debugMatrix4x4(m_invertedMvpMatrix, "Inverted MVP Matrix:");
    QVector3D result = m_invertedMvpMatrix * vClip;
    ShaderDebugger::debugVector3D(result, "Vector in model space:");
    return result;
}

/**
  * Performs inverse viewport transform.
  */
QVector3D GLESRenderer::viewportToClip(const QVector3D& vWin)
{
    //reverse viewport transformation, for original code see below
    float xClip = (vWin.x() - static_cast<float>(m_viewportX)) / static_cast<float>(m_viewportW) * 2.0f - 1.0f;
    float yClip = (vWin.y() - static_cast<float>(m_viewportY)) / static_cast<float>(m_viewportH) * 2.0f - 1.0f;
    float zClip = 2.0f * vWin.z() - 1.0f;

    // original code from gluUnproject
    //Transformation of normalized coordinates between -1 and 1
    // in[0]=(winx-(float)viewport[0])/(float)viewport[2]*2.0-1.0;
    // in[1]=(winy-(float)viewport[1])/(float)viewport[3]*2.0-1.0;
    // in[2]=2.0*winz-1.0;
    // in[3]=1.0;

    QVector3D result = QVector3D(xClip, yClip, zClip);
    ShaderDebugger::debugVector3D(result, "Vector in clip space:");
    return result;
}

/**
  * Returns the points on near and far clipping plane, that correspond to the
  * mouseX and mouseY coordinates of a mouse click.
  * mouseX and mouseY are coordinates as delivered by QMouseEvent or QDeclarativeMouseEvent.
  */
void GLESRenderer::calculateMousePoints(QVector3D* nearPoint, QVector3D* farPoint, const QPoint& mousePos)
{
    float winX = m_viewportX + mousePos.x();
    float winY = m_viewportY + (m_viewportH - mousePos.y());
    *nearPoint = unProjectViewportPoint(QVector3D(winX, winY, 0.0));
    *farPoint = unProjectViewportPoint(QVector3D(winX, winY, 1.0));
    //qDebug() << "nearPoint: " << *nearPoint << "  farPoint: " << *farPoint;
}

/**
  * Calculate the distance of the "mouse ray line" to point p in model space.
  * mouseX, mouseY are the coordinates of the mouse click as delivered by QMouseEvent.
  * Returns distance.
  */
float GLESRenderer::distanceToMouseClick(QVector3D p, const QPoint& mousePos)
{
    QVector3D nearPoint, farPoint;
    calculateMousePoints(&nearPoint, &farPoint, mousePos);

    QVector3D lineVector = nearPoint - farPoint;
    float area = QVector3D::crossProduct(p - nearPoint, lineVector).length();
    float distance = area / lineVector.length();

    return distance;
}

/**
  * Calculates intersection of "mouse ray line" with the plane defined by normal and d.
  * mouseX, mouseY are the coordinates of the mouse click as delivered by QMouseEvent.
  * Returns true, if intersection is in the visible frustum, else returns false.
  * If return value is true, *intersection is the intersection of the mouse ray line with
  * the plane normal*(x,y,z) + d = 0.
  * If return value is false, *intersection is not modified.
  */
bool GLESRenderer::mouseIntersection(QVector3D* intersection, QVector3D normal, float d, const QPoint& mousePos)
{
    float m0, m15;
    QVector3D pNear, pFar; //mouse intersections on near and far clipping plane
    calculateMousePoints(&pNear, &pFar, mousePos);
    QVector3D v = pFar - pNear; //vector from near to far clipping plane
    m15 = d + QVector3D::dotProduct(normal, pNear);
    m0 = QVector3D::dotProduct(normal, v);
    if (m0 == 0.0f)
        return false;
    float lambda = -m15 / m0;
    if (lambda > 1.0f || lambda < 0.0f) //we have no intersection in frustum
        return false;
    *intersection = pNear + lambda * v;
    return true;
}

/**
  * Set the color for ambient and diffuse lighting (or no lighting).
  * Alternatively use a color array and color attribute.
  */
void GLESRenderer::setAmbientAndDiffuseColor(const GLColorRgba newVal)
{
    m_ambientAndDiffuseColor = newVal;
    if (m_bound && m_location_uAmbientAndDiffuseColor != -1)
        m_renderProgram->setUniformValue(m_location_uAmbientAndDiffuseColor, m_ambientAndDiffuseColor);
}

/**
  * Set the dimming factor for ambient light.
  * Defaults to 0.2.
  */
void GLESRenderer::setAmbientLightBrightness(float newVal)
{
    m_ambientLightBrightness = newVal;
    if (m_bound && m_location_uAmbientLightBrightness != -1)
        m_renderProgram->setUniformValue(m_location_uAmbientLightBrightness, m_ambientLightBrightness);
}

/**
  * Set the color for specular lighting.
  */
void GLESRenderer::setSpecularColor(const GLColorRgba newVal)
{
    if (newVal == m_specularColor)
        return;
    m_specularColor = newVal;
    if (m_bound && (m_location_uSpecularColor != -1))
        m_renderProgram->setUniformValue(m_location_uSpecularColor,
            m_specularColor.red(), m_specularColor.green(),
            m_specularColor.blue(), m_specularColor.alpha());
}

/**
  * Set the shininess for specular lighting.
  */
void GLESRenderer::setShininess(float newVal)
{
    m_shininess = newVal;
    if (m_bound && (m_location_uShininess != -1))
        m_renderProgram->setUniformValue(m_location_uShininess, m_shininess);
}

void GLESRenderer::setSourceAlpha(float newVal)
{
    m_sourceAlpha = newVal;
    if (m_bound && (m_location_uSourceAlpha != -1))
        m_renderProgram->setUniformValue(m_location_uSourceAlpha, m_sourceAlpha);
}

/**
  * Enable / disable lighting.
  */
void GLESRenderer::setLightingEnabled(bool newVal)
{
    if (newVal == m_lightingEnabled)
        return;
    m_lightingEnabled = newVal;
    if (m_bound && (m_location_uLightingEnabled != -1))
        m_renderProgram->setUniformValue(m_location_uLightingEnabled, m_lightingEnabled);
}

/**
  * Enable / disable color array.
  */
void GLESRenderer::setColorArrayEnabled(bool newVal)
{
    if (newVal == m_colorArrayEnabled)
        return;
    m_colorArrayEnabled = newVal;
    if (m_bound && (m_location_uColorArrayEnabled != -1))
        m_renderProgram->setUniformValue(m_location_uColorArrayEnabled, m_colorArrayEnabled);
}

/**
  * Set the texture flag.
  */
void GLESRenderer::setTextureEnabled(bool newVal)
{
    if (newVal == m_textureEnabled)
        return;
    m_textureEnabled = newVal;
    if (m_bound && (m_location_uTextureEnabled != -1))
        m_renderProgram->setUniformValue(m_location_uTextureEnabled, m_textureEnabled);
}

void GLESRenderer::setMaskActive(bool newVal)
{
    if (newVal == m_maskActive)
        return;
    m_maskActive = newVal;
    if (m_bound && (m_location_uMaskActive != -1))
        m_renderProgram->setUniformValue(m_location_uMaskActive, m_maskActive);
}

/**
  * Set light direction.
  */
void GLESRenderer::setLightDirection(const QVector3D& newVal)
{
    m_lightDirection = newVal;
#ifdef DEBUG_GLESRENDERER
    ShaderDebugger::debugVector3D(m_lightDirection, "GLESRenderer uses lightDirection in object space:");
#endif
    QMatrix4x4 nMatrix = QMatrix4x4(m_normalMatrix);
    m_lightDirection = (nMatrix * m_lightDirection).normalized(); //transform to eye space
    m_halfPlaneVector = (m_lightDirection + QVector3D(0.0, 0.0, 1.0)).normalized(); //eye direction is 0,0,1 in eye space
#ifdef DEBUG_GLESRENDERER
    ShaderDebugger::debugVector3D(m_lightDirection, "GLESRenderer uses lightDirection in eye space:");
    ShaderDebugger::debugVector3D(m_lightDirection, "GLESRenderer uses halfplane vector in eye space:");
#endif
    if (m_bound && (m_location_uLightDirection != -1))
        m_renderProgram->setUniformValue(m_location_uLightDirection, m_lightDirection);
    if (m_bound && (m_location_uHalfPlaneVector != -1))
        m_renderProgram->setUniformValue(m_location_uHalfPlaneVector, m_halfPlaneVector);
}

/**
  * Set size of points drawn with GL_POINTS.
  */
void GLESRenderer::setPointSize(float newVal)
{
    m_pointSize = newVal;
    if (m_bound && (m_location_uPointSize != -1))
        m_renderProgram->setUniformValue(m_location_uPointSize, m_pointSize);
#ifndef USE_QOPENGL_FUNCTIONS
    glPointSize(m_pointSize); //set point size independent of vertex shader
#endif
}

void GLESRenderer::setMaskCenter(const QPoint& newVal)
{
    m_maskCenter = newVal;
    if (m_bound && m_location_uMaskCenter != -1)
        m_renderProgram->setUniformValue(m_location_uMaskCenter, m_maskCenter);
}

void GLESRenderer::setMaskDiameter(float newVal)
{
    m_maskDiameter = newVal;
    if (m_bound && m_location_uMaskDiameter != -1)
        m_renderProgram->setUniformValue(m_location_uMaskDiameter, m_maskDiameter * m_maskDiameter);
}

/**
  * Pops mvMatrix from stack and updates normalMatrix and mvpMatrix.
  */
void GLESRenderer::popMvMatrix()
{
    setMvMatrix(m_mvMatrixStack.pop());
}

/**
  * Translates mvMatrix by v
  */
void GLESRenderer::translate(const QVector3D& v)
{
    m_mvMatrix.translate(v);
    setMvMatrix(m_mvMatrix); //update normalMatrix and mvpMatrix and copy to shader
}

/**
  * Rotates mvMatrix by angle around axis
  */
void GLESRenderer::rotate(GLfloat angle, const QVector3D& axis)
{
    m_mvMatrix.rotate(angle, axis);
    setMvMatrix(m_mvMatrix); //update normalMatrix and mvpMatrix and copy to shader
}

void GLESRenderer::addTransformation(const QMatrix4x4 additionalTransformation)
{
    setMvMatrix(m_mvMatrix * additionalTransformation); //update normalMatrix and mvpMatrix and copy to shader
}

/**
  * Scales mvMatrix.
  */
void GLESRenderer::scale(const QVector3D& v)
{
    m_mvMatrix.scale(v);
    setMvMatrix(m_mvMatrix); //update normalMatrix and mvpMatrix and copy to shader
}

void GLESRenderer::rotate(float angle, float x, float y, float z)
{
    m_mvMatrix.rotate(angle, x, y, z);
    setMvMatrix(m_mvMatrix);
} /* ----- end of method mvRotate ----- */

void GLESRenderer::zoom(float zoomFactor)
{
    // Es muss vor dem Zoom die Einheitsmatrix generiert werden. Ansonsten wird das vorherige Zoomen dazu addiert
    m_pMatrix.setToIdentity();
    m_pMatrix.perspective(m_fovy * zoomFactor, m_aspect, m_nearClip, m_farClip);
    // Auch hier muss die ProjectionModelView Matrix  neu berechnet werden
    m_mvpMatrix = m_pMatrix * m_mvMatrix;
}

void GLESRenderer::transform(const QMatrix4x4& transformation)
{
    setMvMatrix(m_mvMatrix * transformation);
} /* ----- end of method zoomCamera ----- */

void GLESRenderer::translate(float x, float y, float z)
{
    m_mvMatrix.translate(x, y, z);
    setMvMatrix(m_mvMatrix);
} /* ----- end of method translate ----- */

/**
  * Compile shaders, get attribute and uniform locations.
  * This function needs an active OpenGL context.
  */
bool GLESRenderer::initialize()
{
    if (m_initialized)
        return true;
#ifdef USE_QOPENGL_FUNCTIONS
    QOpenGLFunctions::initializeOpenGLFunctions();
#endif
    //Setup shaders and program
    m_vShader = new QOpenGLShader(QOpenGLShader::Vertex, this); //vertex shader
    m_vShader->compileSourceFile(m_vShaderFileName);
    if (!m_vShader->isCompiled()) {
        qDebug("GLESRenderer::initialize: Compiling vertex shader failed. Log follows:\n%s",
            qPrintable(m_vShader->log()));
        return false;
    }
    m_fShader = new QOpenGLShader(QOpenGLShader::Fragment, this); // fragment shader
    m_fShader->compileSourceFile(m_fShaderFileName);
    if (!m_fShader->isCompiled()) {
        qDebug("GLESRenderer::initialize: Compiling fragment shader failed. Log follows:\n%s",
            qPrintable(m_fShader->log()));
        return false;
    }

    m_renderProgram = new QOpenGLShaderProgram(this);
    m_renderProgram->addShader(m_vShader);
    m_renderProgram->addShader(m_fShader);
    m_renderProgram->link();
    if (!m_renderProgram->isLinked()) {
        qDebug("GLESRenderer::initialize: Linking program failed. Log follows:\n%s",
            qPrintable(m_renderProgram->log()));
        return false;
    }

    // Get all locations of shader variables
    //Get locations of attributes and uniforms
    //Non existing attributes and uniforms will return -1
    //Attributes
    m_location_aVertex = m_renderProgram->attributeLocation("a_Vertex");
    m_location_aColor = m_renderProgram->attributeLocation("a_Color");
    m_location_aNormal = m_renderProgram->attributeLocation("a_Normal");
    m_location_aTexCoord = m_renderProgram->attributeLocation("a_TexCoord");
    //Uniforms
    //flags
    m_location_uLightingEnabled = m_renderProgram->uniformLocation("u_LightingEnabled");
    m_location_uColorArrayEnabled = m_renderProgram->uniformLocation("u_ColorArrayEnabled");
    m_location_uTextureEnabled = m_renderProgram->uniformLocation("u_TextureEnabled");
    m_location_uMaskActive = m_renderProgram->uniformLocation("u_MaskActive");
    //matrices
    m_location_uNormalMatrix = m_renderProgram->uniformLocation("u_NormalMatrix");
    m_location_uMvpMatrix = m_renderProgram->uniformLocation("u_MvpMatrix");
    //lighting
    m_location_uAmbientAndDiffuseColor = m_renderProgram->uniformLocation("u_AmbientAndDiffuseColor");
    m_location_uAmbientLightBrightness = m_renderProgram->uniformLocation("u_AmbientLightBrightness");
    m_location_uLightDirection = m_renderProgram->uniformLocation("u_LightDirection");
    m_location_uSpecularColor = m_renderProgram->uniformLocation("u_SpecularColor");
    m_location_uShininess = m_renderProgram->uniformLocation("u_Shininess");
    m_location_uSourceAlpha = m_renderProgram->uniformLocation("u_SourceAlpha");
    m_location_uHalfPlaneVector = m_renderProgram->uniformLocation("u_HalfPlaneVector");
    //texture
    m_location_uTextureSampler = m_renderProgram->uniformLocation("s_Texture");
    //point size
    m_location_uPointSize = m_renderProgram->uniformLocation("u_PointSize");
    //mask
    m_location_uMaskDiameter = m_renderProgram->uniformLocation("u_MaskDiameterSquare");
    m_location_uMaskCenter = m_renderProgram->uniformLocation("u_MaskCenter");

#ifdef DEBUG_GLESRENDERER
    ShaderDebugger::setEnabled(true);
    ShaderDebugger::debugUniforms(m_renderProgram->programId());
    ShaderDebugger::setEnabled(false);
#endif

    //get present viewport settings
    readGLViewportSettings();
    m_initialized = true;
    return true;
}

/**
  * Bind program and transfer attribute and uniform data to the shaders.
  * Calls initialize, if not already initialized.
  */
bool GLESRenderer::bind()
{
    bool ok = true;
    if (!m_initialized)
        ok = initialize();
    if (!ok)
        return false;
    m_renderProgram->bind();
    //Activate uniforms
    //flags
    if (m_location_uColorArrayEnabled != -1)
        m_renderProgram->setUniformValue(m_location_uColorArrayEnabled, m_colorArrayEnabled);
    if (m_location_uLightingEnabled != -1)
        m_renderProgram->setUniformValue(m_location_uLightingEnabled, m_lightingEnabled);
    if (m_location_uTextureEnabled != -1)
        m_renderProgram->setUniformValue(m_location_uTextureEnabled, m_textureEnabled);
    if (m_location_uMaskActive != -1)
        m_renderProgram->setUniformValue(m_location_uMaskActive, m_maskActive);
    //matrices
    if (m_location_uNormalMatrix != -1)
        m_renderProgram->setUniformValue(m_location_uNormalMatrix, m_normalMatrix);
    if (m_location_uMvpMatrix != -1)
        m_renderProgram->setUniformValue(m_location_uMvpMatrix, m_mvpMatrix);
    //lighting
    if (m_location_uAmbientAndDiffuseColor != -1)
        m_renderProgram->setUniformValue(m_location_uAmbientAndDiffuseColor,
            m_ambientAndDiffuseColor.red(), m_ambientAndDiffuseColor.green(),
            m_ambientAndDiffuseColor.blue(), m_ambientAndDiffuseColor.alpha());
    if (m_location_uAmbientLightBrightness != -1)
        m_renderProgram->setUniformValue(m_location_uAmbientLightBrightness, m_ambientLightBrightness);
    if (m_location_uLightDirection != -1)
        m_renderProgram->setUniformValue(m_location_uLightDirection, m_lightDirection);
    if (m_location_uSpecularColor != -1)
        m_renderProgram->setUniformValue(m_location_uSpecularColor,
            m_specularColor.red(), m_specularColor.green(),
            m_specularColor.blue(), m_specularColor.alpha());
    if (m_location_uShininess != -1)
        m_renderProgram->setUniformValue(m_location_uShininess, m_shininess);
    if (m_location_uSourceAlpha != -1)
        m_renderProgram->setUniformValue(m_location_uSourceAlpha, m_sourceAlpha);
    if (m_location_uHalfPlaneVector != -1)
        m_renderProgram->setUniformValue(m_location_uHalfPlaneVector, m_halfPlaneVector);
    //texture
    if (m_location_uTextureSampler != -1)
        m_renderProgram->setUniformValue(m_location_uTextureSampler, 0); //set sampler to use texture unit 0
    //point size
    if (m_location_uPointSize != -1)
        m_renderProgram->setUniformValue(m_location_uPointSize, m_pointSize);
#ifndef GLES
        //glPointSize(m_pointSize); //set point size independent of vertex shader
#endif
    if (m_location_uMaskDiameter != -1)
        m_renderProgram->setUniformValue(m_location_uMaskDiameter, m_maskDiameter * m_maskDiameter);
    if (m_location_uMaskCenter != -1)
        m_renderProgram->setUniformValue(m_location_uMaskCenter, QVector2D(m_viewportW / 2, m_viewportH / 2));

    m_bound = true;
    return m_bound;
}

/**
  * Enables Vertex, normal, color or texCoord arrays and sets start adresses of arrays
  * arrayLocation may be: VERTEX_LOCATION, NORMAL_LOCATION, COLOR_LOCATION, TEXCOORD_LOCATION
  */
bool GLESRenderer::activateAttributeArray(AttributeLocation arrayLocation, const QVector2D* values, int stride)
{
    return activateAttributeArray(arrayLocation, reinterpret_cast<const float*>(values), 2, stride);
}
/**
  * Enables Vertex, normal, color or texCoord arrays and sets start adresses of arrays
  * arrayLocation may be: VERTEX_LOCATION, NORMAL_LOCATION, COLOR_LOCATION, TEXCOORD_LOCATION
  */
bool GLESRenderer::activateAttributeArray(AttributeLocation arrayLocation, const QVector3D* values, int stride)
{
    return activateAttributeArray(arrayLocation, reinterpret_cast<const float*>(values), 3, stride);
}
/**
  * Enables Vertex, normal, color or texCoord arrays and sets start adresses of arrays
  * arrayLocation may be: VERTEX_LOCATION, NORMAL_LOCATION, COLOR_LOCATION, TEXCOORD_LOCATION
  */
bool GLESRenderer::activateAttributeArray(AttributeLocation arrayLocation, const GLColorRgba* values, int stride)
{
    return activateAttributeArray(arrayLocation, reinterpret_cast<const float*>(values), 4, stride);
}

/**
  * Enables Vertex, normal, color or texCoord arrays and sets start adresses of arrays
  * arrayLocation may be: VERTEX_LOCATION, NORMAL_LOCATION, COLOR_LOCATION, TEXCOORD_LOCATION
  */
bool GLESRenderer::activateAttributeArray(AttributeLocation arrayLocation, const float* values, int tupleSize, int stride)
{
    int location = -1;
    switch (arrayLocation) {
    case VERTEX_LOCATION:
        location = m_location_aVertex;
        break;
    case NORMAL_LOCATION:
        location = m_location_aNormal;
        break;
    case COLOR_LOCATION:
        location = m_location_aColor;
        break;
    case TEXCOORD_LOCATION:
        location = m_location_aTexCoord;
        break;
    default:
        return false;
    }

    if (values && (location != -1)) {
        m_renderProgram->enableAttributeArray(location);
        m_renderProgram->setAttributeArray(location, values, tupleSize, stride);
        m_activeAttributeLocations.append(location);
        return true;
    } else
        return false;
}

void GLESRenderer::activateAttributeBuffer(GLESRenderer::AttributeLocation bufferLocation)
{
    int location = -1;
    int tupleSize = 3;
    int offset = 0;
    switch (bufferLocation) {
    case VERTEX_LOCATION:
        location = m_location_aVertex;
        offset = 0;
        break;
    case NORMAL_LOCATION:
        location = m_location_aNormal;
        offset = GLPoint::normalOffset();
        break;
    case COLOR_LOCATION: {
        location = m_location_aColor;
        offset = GLPoint::colorOffset();
        tupleSize = 4; //RGBA colors
    } break;
    case TEXCOORD_LOCATION:
        location = m_location_aTexCoord;
        offset = GLPoint::texCoordOffset();
        break;
    default:
        qDebug() << "GLESRenderer::activateAttributeBuffer: Invalid location constant!";
    }
    m_renderProgram->enableAttributeArray(location);
    m_renderProgram->setAttributeBuffer(location, GL_FLOAT, offset, tupleSize, sizeof(GLPoint));
}

/**
  * Disables all enabled attribute arrays.
  */
void GLESRenderer::disableAttributeArrays()
{
    for (int i = 0; i < m_activeAttributeLocations.size(); i++)
        m_renderProgram->disableAttributeArray(m_activeAttributeLocations[i]);
    m_activeAttributeLocations.clear();
}

/**
  * Releases program. To be called, when all rendering is finished.
  */
void GLESRenderer::release()
{
    disableAttributeArrays();
    if (m_renderProgram)
        m_renderProgram->release();
    else
        qDebug() << "GLESRenderer::release() called without valid render program.";
    m_bound = false;
}

/**
  * get the present settings from GL engine
  */
void GLESRenderer::readGLViewportSettings()
{
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    m_viewportX = vp[0];
    m_viewportY = vp[1];
    m_viewportW = vp[2];
    m_viewportH = vp[3];
}

/**
  * Multiplies current mvp matrix with v. Mainly for debugging.
  */
QVector3D GLESRenderer::modelToClip(const QVector3D& v)
{
    ShaderDebugger::debugVector3D(v, "Vector in model space:");
    QVector3D result = m_mvpMatrix * v;
    ShaderDebugger::debugVector3D(result, "Vector in clip space:");
    return result;
}

/**
  * Performs viewport transform. Mainly for debugging.
  */
QVector3D GLESRenderer::clipToViewport(const QVector3D& v)
{
    float ox = m_viewportX + m_viewportW / 2.0f;
    float oy = m_viewportY + m_viewportH / 2.0f;
    ShaderDebugger::debugVector3D(v, "Vector in clip space:");
    float xw = (m_viewportW / 2.0f) * v.x() + ox;
    float yw = (m_viewportH / 2.0f) * v.y() + oy;
    float zw = ((m_farClip - m_nearClip) / 2.0f) * v.z() + (m_nearClip + m_farClip) / 2.0f;
    QVector3D result = QVector3D(xw, yw, zw);
    ShaderDebugger::debugVector3D(result, "Vector in viewport space:");
    return result;
}

QVector3D GLESRenderer::modelToViewport(const QVector3D& v)
{
    qDebug() << "GLESRenderer::modelToViewport vector in model space: " << v;
    QVector3D clipVector = modelToClip(v);
    QVector3D result = clipToViewport(clipVector);
    qDebug() << "GLESRenderer::modelToViewport vector in viewport: " << result;
    return result;
}

QVector3D GLESRenderer::modelToMouse(const QVector3D& v)
{
    QVector3D vViewPort = modelToViewport(v);
    QVector3D result = QVector3D(vViewPort.x() - m_viewportX, -vViewPort.y() + m_viewportH + m_viewportY, 0.0);
    return result;
}
