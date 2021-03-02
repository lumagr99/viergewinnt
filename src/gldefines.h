/********************************************************
 * Convenience objects for building OpenGL surfaces.
 *
 *  Created on: 24.04.2012
 *      Author: Walter Roth
 *  Copyright by Walter Roth 2012, Licence: GPL
 *
 *******************************************************/

/** \file
  * \brief Convenience objects for building OpenGL surfaces.
  *
  * Predefined 3D-vectors and colors that are frequently used.
  */

#ifndef GLDEFINES_H
#define GLDEFINES_H

#include <QVector3D>
#include "glcolorrgba.h"
#include "GL/gl.h"

#define IndexType GLushort

#define USE_QOPENGL_FUNCTIONS

#define M_PIf 3.141592654f

//Predefined vectors

const static QVector3D v_Zero = QVector3D(0.0, 0.0, 0.0);
const static QVector3D v_X = QVector3D(1.0, 0.0, 0.0);
const static QVector3D v_Y = QVector3D(0.0, 1.0, 0.0);
const static QVector3D v_Z = QVector3D(0.0, 0.0, 1.0);
const static QVector3D v_XY = QVector3D(1.0, 1.0, 0.0);
const static QVector3D v_XZ = QVector3D(1.0, 0.0, 1.0);
const static QVector3D v_YZ = QVector3D(0.0, 1.0, 1.0);
const static QVector3D v_XYZ = QVector3D(1.0, 1.0, 1.0);


/** Predefined global colors for convenience.
  */
const static GLColorRgba cl_White      = GLColorRgba(1.0,1.0,1.0);
const static GLColorRgba cl_Black      = GLColorRgba(0.0,0.0,0.0);
const static GLColorRgba cl_Red        = GLColorRgba(1.0,0.0,0.0);
const static GLColorRgba cl_Green      = GLColorRgba(0.0,1.0,0.0);
const static GLColorRgba cl_Blue       = GLColorRgba(0.0,0.0,1.0);
const static GLColorRgba cl_LightBlue  = GLColorRgba(0.5,0.5,1.0);
const static GLColorRgba cl_Gray       = GLColorRgba(0.5,0.5,0.5);
const static GLColorRgba cl_Yellow     = GLColorRgba(1.0,1.0,0.0);
const static GLColorRgba cl_Magenta    = GLColorRgba(1.0,0.0,1.0);
const static GLColorRgba cl_Cyan       = GLColorRgba(0.0,1.0,1.0);



#endif // GLDEFINES_H
