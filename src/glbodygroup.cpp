#include "glbodygroup.h"

GLBodyGroup::GLBodyGroup(const QString &name)
    :GLBody(name)
{
}

GLBodyGroup::~GLBodyGroup()
{
    GLBody * object;
    foreach(object, m_Objects)
        delete object;
    m_Objects.clear();
}

void GLBodyGroup::destroyTextureObjects()
{
    GLBody::destroyTextureObjects();
    for(GLBody* object: m_Objects)
        object->destroyTextureObjects();
}

void GLBodyGroup::draw(GLESRenderer *renderer, bool useBuffers)
{
    GLBody * object;
    renderer->pushMvMatrix();
    renderer->addTransformation(m_transformationMatrix);
    foreach(object, m_Objects)
        object->draw(renderer, useBuffers);
    renderer->popMvMatrix();
}

void GLBodyGroup::readModelsAndTextures(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer)
{
    m_points = pointContainer;
    m_indices= indexContainer;
}

void GLBodyGroup::updateAnimatedProperties(float animationState)
{
    GLBody * object;
    foreach(object, m_animatedObjects)
        object->updateAnimatedProperties(animationState);
}

void GLBodyGroup::startAnimation(float animationState)
{
    GLBody::startAnimation(animationState);
    GLBody * object;
    foreach(object, m_animatedObjects)
        object->startAnimation(animationState);
}

void GLBodyGroup::finishAnimation()
{
    for(GLBody * object : m_animatedObjects)
        object->finishAnimation();
    m_animatedObjects.clear();
}

