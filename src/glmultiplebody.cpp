#include "glmultiplebody.h"
#include "glmultiplebody.h"

GLMultipleBody::GLMultipleBody(const QString &name)
    :GLBody (name)
{
   //create the copies in subclassed constructors
}

GLMultipleBody::~GLMultipleBody()
{
    for(GLBody * copy : m_copies)
        delete copy;
}

void GLMultipleBody::destroyTextureObjects()
{
    GLBody::destroyTextureObjects();
    for(GLBody * body : m_copies)
        body->destroyTextureObjects();
}

void GLMultipleBody::draw(GLESRenderer *renderer, bool useBuffers)
{
    if(!m_surfaceIsValid)
        makeSurface(nullptr, nullptr);
    if(m_copies.size() == 0) //check for existing points
    {
        qDebug()<< m_name + " GLMultipleBody::draw  ERROR: No copies! name: " << m_name;
        return;
    }
    if(m_copies[0]->pointsSize() == 0)
    {
        qDebug()<< m_name + " GLMultipleBody::draw  ERROR: No points! name: " << m_copies[0]->name();
        return;
    }

    //get state
    m_textureEnabled = renderer->isTextureEnabled();
    bool lightingEnabled = renderer->isLightingEnabled();
    bool colorArrayEnabled = renderer->isColorArrayEnabled();
    //set transformations
    calculateDrawMatrix();
    renderer->pushMvMatrix();
    renderer->addTransformation(m_drawMatrix);

    if(useBuffers){
        activateAttributeBuffers(renderer);
    }
    else{
        activateAttributeArrays(renderer);
    }

    for(int index = 0; index < m_copies.size(); index++ )
    {
       // int stackSize = renderer->matrixStackSize();
        beforeDrawGeometries(renderer, index);
        m_copies[index]->drawGeometries(renderer, useBuffers);
        afterDrawGeometries(renderer, index);
//        if(renderer->matrixStackSize() != stackSize){
//            qDebug() << " GLMultipleBody::draw: Matrix stack size error detected. Stack size is "
//                     << renderer->matrixStackSize() << " should be " << stackSize;
//            exit (1);
//        }
    }

    //cleanup
    renderer->setLightingEnabled(lightingEnabled);
    renderer->setColorArrayEnabled(colorArrayEnabled);
    renderer->setTextureEnabled(m_textureEnabled);
    renderer->disableAttributeArrays();
    renderer->popMvMatrix();
    glFinish();
}

void GLMultipleBody::beforeDrawGeometries(GLESRenderer * renderer,int index)
{
    Q_UNUSED (renderer);
    Q_UNUSED(index);
}

void GLMultipleBody::afterDrawGeometries(GLESRenderer * renderer,int index)
{
    Q_UNUSED (renderer);
    Q_UNUSED(index);
}


