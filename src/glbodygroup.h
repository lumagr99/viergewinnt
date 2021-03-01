/***************************************************************************
 *   Copyright (C) 2015 by Walter Roth                                     *
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
#ifndef GLBODYGROUP_H
#define GLBODYGROUP_H

#include "glbody.h"

#include <QString>
#include <QVector>

/**
 * @brief The GLBodyGroup class A GLBody which contains an additional group of various GLBodies e.g. a house with doors and windows
 * For body groups containg geometrically identical bodies use class GLMultipleBody e.g.  four wheels of a car, or 26 keys of a typewriter
 */

class GLBodyGroup: public GLBody
{
public:
    GLBodyGroup(const QString & name);
    ~GLBodyGroup()Q_DECL_OVERRIDE;
    /**
     * @brief deleteTextureObjects delete all texture objects from all objects in m_Objects
     */
    void destroyTextureObjects()Q_DECL_OVERRIDE;
    /**
     * @brief draw Draw all objects using renderer.
     * @param renderer
     */
    virtual void draw(GLESRenderer *renderer, bool useBuffers = false)Q_DECL_OVERRIDE;
    /**
     * @brief readModelsAndTextures Read all model files and set all textures.
     * To be overwritten by subclasses.
     */
    virtual void readModelsAndTextures(QVector<GLPoint> * pointContainer, QVector<IndexType> *indexContainer);

    /**
     * @brief updateAnimatedProperties Call updateAnimatedProperties on all objects.
     */
    void updateAnimatedProperties(float animationState)Q_DECL_OVERRIDE;
    /**
     * @brief startAnimation Calls startAnimation on m_animatedObjects.
     */
    void startAnimation(float animationState = 0.0)Q_DECL_OVERRIDE;
    /**
     * @brief finishAnimation Calls finishAnimation on m_animatedObjects.
     */
    void finishAnimation()Q_DECL_OVERRIDE;

protected:
    QVector<GLBody*> m_Objects;
    QVector<GLBody*> m_animatedObjects;
};

#endif // GLBODYGROUP_H
