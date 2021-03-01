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
#include "glbody.h"

#include <QImage>
#include <QtOpenGL/QGLWidget>
#include <QFile>
#include <math.h>

uint GLBody::BINARY_FORMAT_VERSION = 10;

/**Constructor does NOT create the surface.
*/
GLBody::GLBody(const QString &name, float radius, const GLColorRgba &color, const QString textureFile)
{
  init();
  m_name = name;
  m_radius = radius;
  m_color = color;
  if(!textureFile.isEmpty())
  {
    m_textureFilenames.append(textureFile); //DO NOT call setTextureFile here! It needs a current context!
  }
  m_animationStart = 0.0;
}

GLBody::GLBody(QString name, const QVector3D offset)
{
    init();
    m_name = name;
    m_modelOffset = offset;
}

void GLBody::init(){
    m_surfaceIsValid = false;
    m_color = cl_White;
    m_frameColor = cl_LightBlue;
    m_specularColor = cl_White;
    m_shininess = 50;
    m_radius = fOne;
    m_drawingMode = GL_TRIANGLES;
    m_activeTextureIndex = 0;
    m_ownTextures = true;
    m_selected = false;
    m_animationActive = false;
    m_animateForwards = true;
    m_animationStart = fZero;
    m_points = nullptr;
    m_ownPointsContainer = false;
    m_indices = nullptr;
    m_ownIndexContainer = false;
    m_firstPoint = 0;
    m_nextPoint = 0;
    m_firstIndex = 0;
    m_nextIndex = 0;

    //For debugging
    m_firstNormalPoint = 0;
    m_nextNormalPoint = 0;
    m_firstFramePoint = 0;
    m_nextFramePoint = 0;
   // m_firstHitPoint = 0;
   // m_nextHitPoint = 0;
    m_firstNormalIndex = 0;
    m_nextNormalIndex = 0;
    m_firstFrameIndex = 0;
    m_nextFrameIndex = 0;
    m_showNormals = false;
    m_showFrame = false;
    m_hasCustomFrame = false;
}

void GLBody::activateAttributeBuffers(GLESRenderer *renderer)
{
    //enable required arrays
    renderer->activateAttributeBuffer(GLESRenderer::VERTEX_LOCATION);
    if (renderer->isLightingEnabled())
        renderer->activateAttributeBuffer(GLESRenderer::NORMAL_LOCATION);
    renderer->activateAttributeBuffer(GLESRenderer::TEXCOORD_LOCATION);
    renderer->activateAttributeBuffer(GLESRenderer::COLOR_LOCATION);
}

void GLBody::activateAttributeArrays(GLESRenderer *renderer)
{
    int stride = sizeof(GLPoint);
    //enable required arrays
    renderer->activateAttributeArray(GLESRenderer::VERTEX_LOCATION,
                                     m_points->data()[0].vertexPointer(), stride);
    if (renderer->isLightingEnabled())
        renderer->activateAttributeArray(GLESRenderer::NORMAL_LOCATION,
                                        m_points->data()[0].normalPointer(), stride);
    renderer->activateAttributeArray(GLESRenderer::TEXCOORD_LOCATION,
                 m_points->data()[0].texCoordPointer(), stride);
    renderer->activateAttributeArray(GLESRenderer::COLOR_LOCATION,
                 m_points->data()[0].colorPointer(), stride);
}


/** Destructor
 */
GLBody::~GLBody()
{
    if((m_textures.size() > 0) && m_ownTextures)
        qDebug() << "GLBody::~GLBody() " << m_name << " Texture Objects were not destroyed.";
    if(m_ownPointsContainer)
      delete m_points;
    if(m_ownIndexContainer)
      delete m_indices;
}

/** Creates the surface. Should be called, when a GL engine is already running.
  * Is called automatically by draw, if required.
  */
void GLBody::makeSurface(QVector<GLPoint> *pointContainer, QVector<IndexType> * indexContainer)
{
#ifdef USE_QOPENGL_FUNCTIONS
    QOpenGLFunctions::initializeOpenGLFunctions();
#endif
    if(pointContainer == nullptr)
    {
        m_points = new QVector<GLPoint>();
        m_ownPointsContainer = true;
    }
    else m_points = pointContainer;
    if(indexContainer == nullptr)
    {
        m_indices = new QVector<IndexType>();
        m_ownIndexContainer = true;
    }
    else m_indices = indexContainer;
    if(m_textureFilenames.size() > 0 && m_ownTextures)
      createTextureObjects();
    m_surfaceIsValid = true;
}

/** Set texture objects from files in m_TextureFileNames. Returns true on success
 */
bool GLBody::createTextureObjects(GLenum minfilter, GLenum magfilter, GLenum wrapmode){
  if(m_textureFilenames.size() == 0)
    return false;

  QImage  image;
  destroyTextureObjects();

 // QString textureFileName;
  for(QString textureFileName: m_textureFilenames)
  {
     // qDebug() << m_name + " Loading Texture:" + textureFileName;
      if (image.load(textureFileName)){
          //we have got a valid image, give it to GL
        QOpenGLTexture * texture = new QOpenGLTexture(image.mirrored());
        texture->setWrapMode(static_cast <QOpenGLTexture::WrapMode>(wrapmode));
        texture->setMinMagFilters(static_cast<QOpenGLTexture::Filter>(minfilter), static_cast<QOpenGLTexture::Filter>(magfilter));
        if(texture->textureId() == 0) //GL does not work properly
        {
          delete texture;
          qDebug() << m_name + " Could not create texture object. GL-engine not yet active?";
          return false;
        }
        m_textures.append(texture);
      }
      else{ //loading failed
        qDebug() << m_name + " Texture loading failed";
        return false;
      }
  }
  if(m_activeTextureIndex >=  m_textures.size()){
      qDebug()<< "GLBody::createTextureObjects: active texture index out of range for object " << m_name;
      m_activeTextureIndex = 0;
  }
  m_ownTextures = true;
  return true;
}

void GLBody::destroyTextureObjects()
{
    if(m_ownTextures){
        for(QOpenGLTexture* texture: m_textures)
            delete texture;
    }
    m_textures.clear();
    m_activeTextureIndex = 0;
}

void GLBody::shareGeometry(const GLBody *other)
{
#ifdef USE_QOPENGL_FUNCTIONS
    QOpenGLFunctions::initializeOpenGLFunctions();
#endif

    //points
    m_ownPointsContainer = false;
    m_points = other->m_points;
    m_firstPoint = other->m_firstPoint;
    m_nextPoint = other->m_nextPoint;

    //indices
    m_ownIndexContainer = false;
    m_indices = other->m_indices;
    m_firstIndex = other->m_firstIndex;
    m_nextIndex = other->m_nextIndex;

    //normals
    m_showNormals = other->m_showNormals;
    m_firstNormalPoint = other->m_firstNormalPoint;
    m_nextNormalPoint = other->m_nextNormalPoint;
    m_firstNormalIndex = other->m_firstNormalIndex;
    m_nextNormalIndex = other->m_nextNormalIndex;

    //frame
    m_showFrame = other->m_showFrame;
    m_firstFramePoint = other->m_firstFramePoint;
    m_nextFramePoint = other->m_nextFramePoint;
    m_firstFrameIndex = other->m_firstFrameIndex;
    m_nextFrameIndex = other->m_nextFrameIndex;

    //min/max coordinates
    m_minCoordinate = other->m_minCoordinate;
    m_maxCoordinate = other->m_maxCoordinate;

    //textures
    if(m_textureFilenames.size() == 0){ //we have no texture files yet
        m_textureFilenames = other->m_textureFilenames;
        m_textures = other->m_textures;
        m_ownTextures =false;
    }

    //drawing mode
    m_drawingMode = other->m_drawingMode;

    //validity
    m_surfaceIsValid = other->m_surfaceIsValid;
}

void GLBody::shareTextureObjects(const GLBody *other)
{
    m_textureFilenames = other->m_textureFilenames;
    m_textures = other->m_textures;
    m_ownTextures = false;
}

bool GLBody::readBinaryModelFile(const QString &dataFileName, QVector<GLPoint> * pointContainer, QVector<IndexType> * indexContainer)
{
    makeSurface(pointContainer, indexContainer);
    QFile binaryFile( dataFileName );
    if( !binaryFile.open( QIODevice::ReadOnly )) {
        qDebug() << m_name + " GLBody::readBinaryFile: could not open file: " + dataFileName;
        return false;
    }
    // Die skip Methode braucht man hier nicht, da jedesmal wenn was gelesen wurde bleibt die Position bestehen
    QDataStream     dataStream( & binaryFile );
    unsigned int    formatVersion = 0;
    int    numberOfPoints = 0;
    int    numberOfIndices = 0;

    dataStream.readRawData(reinterpret_cast<char*>( & formatVersion), 4);
    if(formatVersion != 10 && formatVersion != 11)
    {
        qDebug() << m_name << "   GLBody::readBinaryModelFile: Error! Format version mismatch. Version in File: " << formatVersion;
        return false;
    }
    dataStream.readRawData(reinterpret_cast<char*>( & numberOfPoints), 4);
    int    pointBytes = numberOfPoints * static_cast<int>(sizeof(GLPoint));
    dataStream.readRawData(reinterpret_cast<char*>( & numberOfIndices), 4);
    int    indexBytes = 0; //numberOfIndices * sizeof(GLshort );

    m_firstPoint = pointsSize();
    m_points->resize(m_points->size() + numberOfPoints);
    dataStream.readRawData(reinterpret_cast<char*>((*m_points)[m_firstPoint].vertexPointer()), pointBytes);
    m_nextPoint = pointsSize();

    m_firstIndex = indicesSize();
    if(formatVersion == 10){
        indexBytes = numberOfIndices * static_cast<int>(sizeof(GLuint ));
        QVector<IndexType> indices;
        indices.resize(numberOfIndices);
        dataStream.readRawData(reinterpret_cast<char*>( &(indices[0]) ), indexBytes);
        for(int i = 0; i < numberOfIndices; i++) //convert to short and append
            m_indices->append(indices[i]);
    }
    else if(formatVersion == 11){
        indexBytes = numberOfIndices * static_cast<int>(sizeof(IndexType ));
        m_indices->resize(m_indices->size() + numberOfIndices);
        dataStream.readRawData(reinterpret_cast<char*>( &(*m_indices)[m_firstIndex]), indexBytes);
    }
    m_nextIndex = indicesSize();
    //now reposition index numbers
    if(m_firstPoint != 0)
    {
      for(int i = m_firstIndex; i < m_nextIndex; i++)
        m_indices->data()[i] = m_indices->at(i) + m_firstPoint;
    }
    //move model zero to coordinate origin
    if(m_modelOffset != v_Zero)
      move(-m_modelOffset); //will call findMinMaxCoordinates()
    else
       findMinMaxCoordinates();
    return true;
}

void GLBody::rotateModelPoints(const QVector3D &axisCenter, const QVector3D &axisDirection, float angle)
{
    if(!m_surfaceIsValid)
        return;
    QMatrix4x4 rotator;
    rotator.rotate(angle, axisDirection);
    rotator.translate(-axisCenter);

    QMatrix4x4 translator;
    translator.translate(axisCenter);
    QMatrix4x4 transformer = translator * rotator;
    QMatrix4x4 normalMatrix = QMatrix4x4 (transformer.normalMatrix());
    for(int i = m_firstPoint; i < m_nextPoint; i++)
        (*m_points)[i].transform(transformer, normalMatrix);
    findMinMaxCoordinates();
}

void GLBody::calculateDrawMatrix()
{
    m_drawMatrix = m_transformationMatrix;
}

/** Draws the surface and calls makeSurface if required.
 * Needs an active (made current) GL-Context.
 */
void GLBody::draw(GLESRenderer * renderer, bool useBuffers){
  if(!m_surfaceIsValid)
    makeSurface(nullptr, nullptr);
  if(!m_points || m_points->size() == 0) //check for existing points
  {
      qDebug()<< m_name + " GLBody::draw  ERROR: No points! name: " << m_name;
      return;
  }
  bool textureEnabled = renderer->isTextureEnabled();
  bool lightingEnabled = renderer->isLightingEnabled();
  bool colorArrayEnabled = renderer->isColorArrayEnabled();

  //set colors
  renderer->setAmbientAndDiffuseColor(m_color);
  //set transformations
  calculateDrawMatrix();
  renderer->pushMvMatrix();
  renderer->addTransformation(m_drawMatrix);

  bindTexture(renderer);

  void * indexAddress = nullptr;
  if(useBuffers){
      indexAddress = reinterpret_cast<void *>( m_firstIndex * sizeof(IndexType));
      activateAttributeBuffers(renderer);
  }
  else{
      activateAttributeArrays(renderer);
      indexAddress =  &m_indices->data()[m_firstIndex];
  }

  if(m_nextIndex - m_firstIndex > 0) //we have indexes defined
    glDrawElements(m_drawingMode, m_nextIndex - m_firstIndex,
                   GL_UNSIGNED_SHORT, indexAddress);
  else
    glDrawArrays(m_drawingMode, m_firstPoint, m_nextPoint - m_firstPoint);

//  if(lightingEnabled)
//      renderer->setLightingEnabled(false);
//  if(!colorArrayEnabled)
//      renderer->setColorArrayEnabled(true);
//  releaseTexture(renderer);
//  if(m_showNormals){
//      if(useBuffers)
//          indexAddress = reinterpret_cast<void *>( m_firstNormalIndex * sizeof(IndexType));
//      else indexAddress =  &m_indices->data()[m_firstNormalIndex];
//      glDrawElements(GL_LINES, m_nextNormalIndex -m_firstNormalIndex,
//                     GL_UNSIGNED_SHORT, indexAddress);
//  }

  if(m_showFrame || m_showNormals)
      drawDebugGeometries(renderer, useBuffers);

  //cleanup
  renderer->setLightingEnabled(lightingEnabled);
  renderer->setColorArrayEnabled(colorArrayEnabled);
  renderer->setTextureEnabled(textureEnabled);
  renderer->disableAttributeArrays();
  renderer->popMvMatrix();
  glFinish();
}


/** Set texture file.
 */
bool GLBody::setTextureFile(const QString & textureFile){
 if(m_textures.size() != 0) //we have an activated texture already. Replace it.
 {
     destroyTextureObjects();
     m_textureFilenames.clear();
     m_textureFilenames.append(textureFile);
     return createTextureObjects();
 }
 else m_textureFilenames.append(textureFile);
 return true; //texture is not yet active, leave it as it is
}


bool GLBody::setTextureFiles(const QStringList &textureFiles)
{
    destroyTextureObjects();
    m_textureFilenames = textureFiles;
    return true;
}

/**
  * Moves the body by adding vMove to all vertices.
  */
void GLBody::move(QVector3D vMove)
{
    for(int i = m_firstPoint; i < m_nextPoint; i++)
        (*m_points)[i].move(vMove);
    m_center = m_center + vMove;
    findMinMaxCoordinates();
}

void GLBody::transformPoints()
{
    QMatrix4x4 normalMatrix = QMatrix4x4(m_transformationMatrix.normalMatrix());
    for(int i = m_firstPoint; i < m_nextPoint; i++){
        (*m_points)[i].transform(m_transformationMatrix, normalMatrix);
    }
    m_transformationMatrix.setToIdentity();
}

void GLBody::startAnimation(float startValue)
{
    m_animationStart = startValue;
    m_animationActive = true;
}

void GLBody::finishAnimation()
{
    m_animationActive = false;
}

float GLBody::localAnimationState(float globalAnimationState)
{
    float animationDistance = fOne - m_animationStart;
    float magnification = fOne / animationDistance;
    float result =(globalAnimationState - m_animationStart) * magnification;
    return result;
}

void GLBody::animateForwards()
{
    m_animateForwards = true;
    startAnimation();
}

void GLBody::animateBackwards()
{
    m_animateForwards = false;
    startAnimation();
}

void GLBody::updateAnimatedProperties(float animationState)
{
    Q_UNUSED(animationState)
    //Do Nothing, this function needs to be overwritten
}

void GLBody::setActiveTextureIndex(int i)
{
    if(m_activeTextureIndex != i)
         m_activeTextureIndex = i;
}

/**
  * Returns true, when line through p1 and p2 intersects body shpere
  * True if line through p1 and p2 intersects cube defined by minCoordinates and maxCoordinates.
  */
bool GLBody::isHit(QVector3D p1, QVector3D p2)
{
   QVector3D lineVector = p2 - p1;
//   if(m_firstHitPoint != 0)
//   {
//       (*m_points)[m_firstHitPoint] = GLPoint(p1, v_X, v_Zero, GLColorRgba::clRed);
//       (*m_points)[m_nextHitPoint - 1] =  GLPoint(p2, v_X, v_Zero, GLColorRgba::clRed);
//   }
   float area = QVector3D::crossProduct((m_center - p1), lineVector).length();
   float distance = area / lineVector.length();
   //qDebug("Radius: %f", radius);
   if( distance < m_radius)
       return isCubeHit(p1, p2);
   else return false;
}

bool GLBody::isCubeHit(QVector3D mouseNear, QVector3D mouseFar)
{
    QVector3D p1;
    QVector3D p2;
    QVector3D p3;
    QVector3D lineDirection = mouseFar - mouseNear;
    QVector3D position;
    bool result = false;
    //we need to check the six enclosing rectangles
    //we start with the 3 rectangles containing m_minCoordinate
    //back rect
    QString name = "-Z";
    p1 = m_minCoordinate; //llb
    p2 = QVector3D(m_maxCoordinate.x(), m_minCoordinate.y(), m_minCoordinate.z()); //rbb
    p3 = QVector3D(m_minCoordinate.x(), m_maxCoordinate.y(), m_minCoordinate.z()); //ltb
    //back rect
    result = isParallelogramHit(p1, p2, p3, mouseNear, lineDirection, &position);
    if(!result)
    {
        //bottom rect
        name = "-Y";
        p2 = QVector3D(m_maxCoordinate.x(), m_minCoordinate.y(), m_minCoordinate.z());
        p3 = QVector3D(m_minCoordinate.x(), m_minCoordinate.y(), m_maxCoordinate.z());
        result = isParallelogramHit(p1, p2, p3, mouseNear, lineDirection);
    }
    if(!result)
    {
        //left rect
        name = "-X";
        p2 = QVector3D(m_minCoordinate.x(), m_maxCoordinate.y(), m_minCoordinate.z());
        p3 = QVector3D(m_minCoordinate.x(), m_minCoordinate.y(), m_maxCoordinate.z());
        result = isParallelogramHit(p1, p2, p3, mouseNear, lineDirection);
    }
    if(!result)
    {
        //Now we check 3 rectangles containing m_maxCoordinate
        // top rect
        name = "+Y";
        p1 = m_maxCoordinate;
        p2 = QVector3D(m_maxCoordinate.x(), m_maxCoordinate.y(), m_minCoordinate.z());
        p3 = QVector3D(m_minCoordinate.x(), m_maxCoordinate.y(), m_maxCoordinate.z());
        result = isParallelogramHit(p1, p2, p3, mouseNear, lineDirection);
    }
    if(!result)
    {
        // right rect
        name = "+X";
        p2 = QVector3D(m_maxCoordinate.x(), m_minCoordinate.y(), m_maxCoordinate.z());
        p3 = QVector3D(m_maxCoordinate.x(), m_maxCoordinate.y(), m_minCoordinate.z());
        result = isParallelogramHit(p1, p2, p3, mouseNear, lineDirection);
    }
    if(!result)
    {
        // front rect
        name = "+Z";
        p2 = QVector3D(m_minCoordinate.x(), m_maxCoordinate.y(), m_maxCoordinate.z());
        p3 = QVector3D(m_maxCoordinate.x(), m_minCoordinate.y(), m_maxCoordinate.z());
        result = isParallelogramHit(p1, p2, p3, mouseNear, lineDirection);
    }
    if(result)
      qDebug() << m_name << " isCubeHit hit detected at: " << name ;
    return result;
}

bool GLBody::isTriangleHit(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3,
                           const QVector3D &pLine, const QVector3D &lineDirection, QVector3D * intersection)
{
  float a = fZero;
  float b = fZero;
  if(calculateIntersection(p1, p2, p3,pLine, lineDirection, &a, &b, intersection))
    return (a > fZero) && (b > fZero) && (a + b < fOne);
  else return false;
}

bool GLBody::isParallelogramHit(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3, const QVector3D &pLine, const QVector3D &lineDirection, QVector3D *intersection)
{
    float a = fZero;
    float b = fZero;
    if(calculateIntersection(p1, p2, p3,pLine, lineDirection, &a, &b, intersection))
      return (a > fZero) && (b > fZero) && (a < fOne) && (b < fOne);
    else return false;
}

bool GLBody::calculateIntersection(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3, const QVector3D &pLine, const QVector3D &lineDirection, float *a, float *b, QVector3D *intersection)
{
    double denominator = 0.0;
    //We solve the linear equation a*p12 + b * p13 + lambda * lineDirection = pLine - P1
    //We only need a and b.
    //An intersection exists if denominator determinant is != 0.
    //The intersection is inside the triangle if a > 0 and b > 0 and a+b < 1
    QVector3D p12 = p2 - p1;
    QVector3D p13 = p3 - p1;
    QVector3D rightSide = pLine - p1;
    QMatrix4x4 m; //Determinant function is available for 4x4 matrix only.
    QMatrix4x4 denominatorMatrix;
    denominatorMatrix.setColumn(0, p12);
    denominatorMatrix.setColumn(1, p13);
    denominatorMatrix.setColumn(2, lineDirection);
    denominator = denominatorMatrix.determinant();
    if(fabs(denominator) < 0.00001) //too small for an intersection
        return false;
    m = denominatorMatrix;
    m.setColumn(0, rightSide); //replace the a-Column with right side of equation
    *a = static_cast<float>(m.determinant() / denominator);
    m = denominatorMatrix;
    m.setColumn(1,rightSide); //replace the b-Column with right side of equation
    * b = static_cast<float>( m.determinant() / denominator);
    if(intersection != nullptr)
        * intersection = p1 + (*a) * p12 + (*b) * p13;
    return true;
}

void GLBody::createNormals(float length)
{
    if(!m_surfaceIsValid)
        return;
    m_firstNormalPoint = pointsSize();
    m_firstNormalIndex =  indicesSize();

    for(int i = m_firstPoint; i < m_nextPoint; i++)
    {
        m_indices->append( pointsSize());
        m_points->append(GLPoint(m_points->at(i).vertex(), v_X, v_Zero, cl_White));
        m_indices->append( pointsSize());
        m_points->append(GLPoint(m_points->at(i).vertex() + length * m_points->at(i).normal(), v_X, v_Zero, cl_White));
     }
    m_nextNormalPoint =  pointsSize();
    m_nextNormalIndex =  indicesSize();
}

void GLBody::createFrame()
{
    if(!m_surfaceIsValid)
        return;
    QVector3D vertex;
    QVector3D normal = v_X;
    QVector3D texCoord = v_Zero;
    m_firstFramePoint =  pointsSize();
    m_firstFrameIndex =  indicesSize();

    //create points
    vertex = QVector3D( m_minCoordinate.x(), m_minCoordinate.y(), m_minCoordinate.z());
    IndexType iLbb = pointsSize();
    m_points->append(GLPoint(vertex,normal, texCoord,m_frameColor));

    vertex = QVector3D( m_maxCoordinate.x(), m_minCoordinate.y(), m_minCoordinate.z());
    IndexType iRbb = pointsSize();
    m_points->append(GLPoint(vertex,normal, texCoord,m_frameColor));

    vertex = QVector3D( m_minCoordinate.x(), m_minCoordinate.y(), m_maxCoordinate.z());
    IndexType iLbf = pointsSize();
    m_points->append(GLPoint(vertex,normal, texCoord,m_frameColor));

    vertex = QVector3D( m_maxCoordinate.x(), m_minCoordinate.y(), m_maxCoordinate.z());
    IndexType iRbf = pointsSize();
    m_points->append(GLPoint(vertex,normal, texCoord,m_frameColor));

    vertex = QVector3D( m_minCoordinate.x(), m_maxCoordinate.y(), m_minCoordinate.z());
    IndexType iLtb = pointsSize();
    m_points->append(GLPoint(vertex,normal, texCoord,m_frameColor));

    vertex = QVector3D( m_maxCoordinate.x(), m_maxCoordinate.y(), m_minCoordinate.z());
    IndexType iRtb = pointsSize();
    m_points->append(GLPoint(vertex,normal, texCoord,m_frameColor));

    vertex = QVector3D( m_minCoordinate.x(), m_maxCoordinate.y(), m_maxCoordinate.z());
    IndexType iLtf = pointsSize();
    m_points->append(GLPoint(vertex,normal, texCoord,m_frameColor));

    vertex = QVector3D( m_maxCoordinate.x(), m_maxCoordinate.y(), m_maxCoordinate.z());
    IndexType iRtf = pointsSize();
    m_points->append(GLPoint(vertex,normal, texCoord,m_frameColor));

    //create indices for bottom rect
    m_indices->append(iLbb);
    m_indices->append(iRbb);
    m_indices->append(iRbb);
    m_indices->append(iRbf);
    m_indices->append(iRbf);
    m_indices->append(iLbf);
    m_indices->append(iLbf);
    m_indices->append(iLbb);

    //create indices for top rect
    m_indices->append(iLtb);
    m_indices->append(iRtb);
    m_indices->append(iRtb);
    m_indices->append(iRtf);
    m_indices->append(iRtf);
    m_indices->append(iLtf);
    m_indices->append(iLtf);
    m_indices->append(iLtb);

    //create indices for vertical lines
    //create indices for bottom rect
    m_indices->append(iLbb);
    m_indices->append(iLtb);
    m_indices->append(iRbb);
    m_indices->append(iRtb);
    m_indices->append(iRbf);
    m_indices->append(iRtf);
    m_indices->append(iLbf);
    m_indices->append(iLtf);

    m_nextFramePoint =static_cast<IndexType>( m_points->size());
    m_nextFrameIndex = indicesSize();

    //Create the initial hit line as a point
//    m_firstHitPoint = pointsSize();
//    m_points->append(GLPoint(m_minCoordinate, v_X, v_Zero, GLColorRgba::clRed));
//    m_points->append(GLPoint(m_minCoordinate, v_X, v_Zero, GLColorRgba::clRed));
//    m_nextHitPoint = pointsSize();
}

bool GLBody::isHit(QPoint mousePos, GLESRenderer *renderer)
{
    QVector3D p1, p2;
    calculateDrawMatrix();
    renderer->pushMvMatrix();
    renderer->addTransformation(m_drawMatrix);
    renderer->calculateMousePoints( &p1, & p2, mousePos);
    renderer->popMvMatrix();
    return isHit(p1, p2);
}

/*
 * Returns true, if enclosing spheres touch or intersect
 */
bool GLBody::spheresAreColliding(const GLBody *other)
{
    bool result;
    if(this == other)
        result = false;
    else
    {
        float distance = (m_center - other->m_center).length();
        result = (m_radius + other->m_radius) >= distance;
    }
    if(result)
        return true; //hook for debugger
    else return false; //hook for debugger
}

/**
  * Set new center and invalidate surface.
  */
void GLBody::setCenter(const QVector3D & newVal)
{
    m_center = newVal;
    m_surfaceIsValid = false;
}

void GLBody::setMinMaxCoordinates(const QVector3D min, const QVector3D max)
{
    m_minCoordinate = min;
    m_maxCoordinate = max;
    m_hasCustomFrame = true;
}


void GLBody::findMinMaxCoordinates()
{
    if(m_hasCustomFrame){
        if(m_showFrame)
            createFrame();
        return;
    }
  float minX, maxX, minY, maxY, minZ, maxZ;
  minX = minY = minZ = 1.0e10;
  maxX = maxY = maxZ = -1.0e10;
  for (int i = m_firstPoint; i < m_nextPoint; i++)
  {
      if(m_points->at(i).vertex().x() > maxX)
          maxX = m_points->at(i).vertex().x();
      if(m_points->at(i).vertex().y() > maxY)
          maxY = m_points->at(i).vertex().y();
      if(m_points->at(i).vertex().z() > maxZ)
          maxZ = m_points->at(i).vertex().z();
      if(m_points->at(i).vertex().x() < minX)
          minX = m_points->at(i).vertex().x();
      if(m_points->at(i).vertex().y() < minY)
          minY = m_points->at(i).vertex().y();
      if(m_points->at(i).vertex().z() < minZ)
          minZ = m_points->at(i).vertex().z();
  }
  m_minCoordinate = QVector3D(minX, minY, minZ);
  m_maxCoordinate = QVector3D(maxX, maxY, maxZ);
  m_center = fHalf * (m_minCoordinate + m_maxCoordinate);
  m_radius = fHalf * (m_maxCoordinate - m_minCoordinate).length();
  if(m_showNormals)
      createNormals();
  if(m_showFrame)
      createFrame();
  if(m_points->size() > 0xFFFF)
      qDebug() << "GLBody::findMinMaxCoordinates: Error! More than 65335 points."
               << " Too much for index datatype (GLushort). Image will be faulty.";
#ifdef DEBUG_GLBODY
  qDebug() << "GLBody " + m_name + " has " + QString::number(m_points->size())
              + " points and " + QString::number(m_indices->size()) + " indices.";
#endif
}


void GLBody::drawGeometries(GLESRenderer *renderer, bool useBuffers, int index)
{
    Q_UNUSED(index); //index can be useful in subclassed functions e.g. GLMultipleBody
    //set colors
    renderer->setAmbientAndDiffuseColor(m_color);
    //set transformations
    calculateDrawMatrix();
    renderer->pushMvMatrix();
    renderer->addTransformation(m_drawMatrix);
    //texture
    bindTexture(renderer);

    void * indexAddress = nullptr;
    if(useBuffers){
        indexAddress = reinterpret_cast<void *>( m_firstIndex * sizeof(IndexType));
    }
    else{
        indexAddress =  &m_indices->data()[m_firstIndex];
    }
    if(m_nextIndex - m_firstIndex > 0) //we have indexes defined
        glDrawElements(m_drawingMode, m_nextIndex - m_firstIndex,
                       GL_UNSIGNED_SHORT, indexAddress);
    else
        glDrawArrays(m_drawingMode, m_firstPoint, m_nextPoint - m_firstPoint);

    releaseTexture(renderer);
    //draw debug geometries
    if(m_showNormals || m_showFrame || m_selected)
    {
        drawDebugGeometries(renderer, useBuffers);
    }
    renderer->popMvMatrix();
}

void GLBody::bindTexture(GLESRenderer *renderer)
{
    if(m_textures.size() > 0){
        renderer->setTextureEnabled(true);
        m_textures[m_activeTextureIndex]->bind(); //bind to texture unit 0
    }
    else renderer->setTextureEnabled(false);
}

void GLBody::releaseTexture(GLESRenderer *renderer)
{
    if(renderer->isTextureEnabled() && (m_activeTextureIndex < m_textures.size()))
         m_textures[m_activeTextureIndex]->release();
}

void  GLBody::drawDebugGeometries(GLESRenderer *renderer, bool useBuffers)
{
    void * indexAddress = nullptr;
    bool lightingEnabled = renderer->isLightingEnabled();
    renderer->setLightingEnabled(false);
    bool colorArrayEnabled = renderer->isColorArrayEnabled();
    renderer->setColorArrayEnabled(true);
    bool textureEnabled = renderer->isTextureEnabled();
    renderer->setTextureEnabled(false);

    if(m_selected)
    {
        GLColorRgba pointColor(m_color.inverted());
        renderer->setAmbientAndDiffuseColor(pointColor);
  #ifdef GLES
        glDrawArrays(GL_POINTS, m_firstPoint, m_nextPoint - m_firstPoint);
  #else
        float oldPointSize = renderer->pointSize();
        renderer->setPointSize(5);
        glDrawArrays(GL_POINTS, m_firstPoint, m_nextPoint - m_firstPoint);
        renderer->setPointSize(oldPointSize);
  #endif
    }
    if(m_showNormals){
        if(useBuffers)
            indexAddress = reinterpret_cast<void *>( m_firstNormalIndex * sizeof(IndexType));
        else indexAddress =  &m_indices->data()[m_firstNormalIndex];
        glDrawElements(GL_LINES, m_nextNormalIndex -m_firstNormalIndex,
                       GL_UNSIGNED_SHORT, indexAddress);
    }

    if(m_showFrame)
    {
        //draw the frame
        GLfloat lineWidth[4]; //4 floats, just for safety
        glGetFloatv(GL_LINE_WIDTH, lineWidth);
        glLineWidth(3.0);
        if(useBuffers)
            indexAddress = reinterpret_cast<void *>( m_firstFrameIndex * sizeof(IndexType));
        else indexAddress =  &m_indices->data()[m_firstFrameIndex];
        glDrawElements(GL_LINES, m_nextFrameIndex -m_firstFrameIndex,
                       GL_UNSIGNED_SHORT, indexAddress);
//        //Draw the hit line
//        if(m_firstHitPoint != 0)
//        {
//            glLineWidth(5.0);
//            glDrawArrays(GL_LINES, m_firstHitPoint, m_nextHitPoint - m_firstHitPoint);
//        }
        glLineWidth(lineWidth[0]);
    }
    //clean up
    renderer->setTextureEnabled(textureEnabled);
    renderer->setColorArrayEnabled(colorArrayEnabled);
    renderer->setLightingEnabled(lightingEnabled);
}
