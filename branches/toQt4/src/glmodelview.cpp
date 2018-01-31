/*
 * 
 * 
 * This file is part of Group Explorer.
 * Copyright 2005 Nathan Carter
 * 
 * Group Explorer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * Group Explorer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Group Explorer; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * Linking Group Explorer statically or dynamically with other modules is
 * making a combined work based on Group Explorer.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole combination.
 * 
 * In addition, as a special exception, the copyright holders of Group
 * Explorer give you permission to combine Group Explorer program with free
 * software programs or libraries that are released under the GNU LGPL and
 * with code included in the standard release of the Qt Undo/Redo Framework
 * under its standard license (or modified versions of such code, with
 * unchanged license). You may copy and distribute such a system following
 * the terms of the GNU GPL for Group Explorer and the licenses of the other
 * code concerned, provided that you include the source code of that other
 * code when and as the GNU GPL requires distribution of source code.
 * 
 * Note that people who make modified versions of Group Explorer are not
 * obligated to grant this special exception for their modified versions; it
 * is their choice whether to do so. The GNU General Public License gives
 * permission to release a modified version without this exception; this
 * exception also makes it possible to release a modified version which
 * carries forward this exception.
 * 
 * 
 * 
 */


#include "glmodelview.h"
#include <qapplication.h>
#include <math.h>
#if QT_VERSION >= 0x040000
#include <QMouseEvent>
#endif

GLModelView::GLModelView ( QWidget* parent, const char* name )
	: GLSceneView( parent, name ),
      allowRotate( TRUE ), rotateButtonState( Qt::LeftButton ),
      allowDrag( TRUE ),
      dragButtonState( /*Qt::ButtonState( Qt::LeftButton | Qt::ShiftButton )*/
                       (int)Qt::LeftButton | (int)Qt::ShiftModifier ),
      dragStart( -1, -1 ),
      allowOther( TRUE ),
      otherButtonState( /*Qt::ButtonState( Qt::LeftButton | Qt::AltButton )*/
                       (int)Qt::LeftButton | (int)Qt::AltModifier ),
      otherStart( -1, -1 ),
      minzoomdist( 0.0 ), maxzoomdist( 100.0 )
{
	setCameraPosition( 0.0, 0.0, 1.0 );
}

void GLModelView::setRotateAllowed ( bool allowed )
{
	allowRotate = allowed;
}

void GLModelView::setRotateButtonState ( Qt::ButtonState state )
{
	rotateButtonState = state;
}

void GLModelView::setDragAllowed ( bool allowed )
{
	allowDrag = allowed;
}

void GLModelView::setDragButtonState ( Qt::ButtonState state )
{
	dragButtonState = state;
}

void GLModelView::setOtherAllowed ( bool allowed )
{
	allowOther = allowed;
}

void GLModelView::setOtherButtonState ( Qt::ButtonState state )
{
	otherButtonState = state;
}

void GLModelView::setZoomLimits ( GLdouble mindistance, GLdouble maxdistance )
{
    minzoomdist = mindistance;
    maxzoomdist = maxdistance;
    zoomIn( 0.0 ); // just caps it but doesn't change it
}

void GLModelView::zoomIn ( GLdouble distance )
{
    GLdouble x, y, z, l;
    getCameraPosition( x, y, z );
    l = unitize( x, y, z ) - distance;
    if ( l < minzoomdist ) l = minzoomdist;
    if ( l > maxzoomdist ) l = maxzoomdist;
    setCameraPosition( x*l, y*l, z*l );
}

void GLModelView::zoomOut ( GLdouble distance )
{
    zoomIn( -distance );
}

void GLModelView::zoomInByFactor ( GLdouble factor )
{
    GLdouble x, y, z;
    getCameraPosition( x, y, z );
    zoomIn( length( x, y, z ) * factor );
}

void GLModelView::zoomOutByFactor ( GLdouble factor )
{
    zoomInByFactor( -factor );
}

void GLModelView::mousePressEvent ( QMouseEvent* e )
{
    GLSceneView::mousePressEvent( e );

    if ( allowDrag && ( ( e->button() | e->state() ) == dragButtonState ) ) {
        dragStart = e->pos();
        emit startDrag( dragStart );
    } else if ( allowOther && ( ( e->button() | e->state() ) == otherButtonState ) ) {
        otherStart = otherLast = e->pos();
        emit startOther( otherStart );
    }
}

void GLModelView::mouseMoveEvent ( QMouseEvent* e )
{
	GLSceneView::mouseMoveEvent( e );

	if ( allowRotate && ( e->state() == rotateButtonState ) ) {

		GLdouble rightx, righty, rightz, upx, upy, upz;
		// we must assume the camera is not on the y-axis
        // if it is; just give some values that might work sometimes
		if ( ( cameraPosition[0] == 0.0 ) && ( cameraPosition[2] == 0.0 ) ) {
            rightx = 1.0;
            upz = -1.0;
            righty = rightz = upx = upy = 0.0;
            /*
			qWarning( QString(
			        "GLModelView: camera position invalid: %1,%2,%3" )
			          .arg( cameraPosition[0] )
					  .arg( cameraPosition[1] )
					  .arg( cameraPosition[2] ) );
            */
		} else {
		    crossproduct( 0.0, 1.0, 0.0,
		                  cameraPosition[0], cameraPosition[1], cameraPosition[2],
		                  rightx, righty, rightz );
		    crossproduct( cameraPosition[0], cameraPosition[1], cameraPosition[2],
		                  rightx, righty, rightz, upx, upy, upz );
        }

		QPoint tmp = lastDrag();
		rotateScene( tmp.x(), upx, upy, upz );
		rotateScene( tmp.y(), rightx, righty, rightz );
		updateGL();
    }

    if ( otherStart.x() > -1 ) {
        emit duringOther( otherLast, e->pos() );
        otherLast = e->pos();
    }
}

void GLModelView::mouseReleaseEvent ( QMouseEvent* e )
{
    GLSceneView::mouseReleaseEvent( e );

    if ( dragStart.x() > -1 ) {
        emit finishDrag( dragStart, e->pos() );
        dragStart = QPoint( -1, -1 );
    } else if ( otherStart.x() > -1 ) {
        emit finishOther( otherStart, e->pos() );
        otherStart = QPoint( -1, -1 );
    }
}

void GLModelView::setCameraPosition ( GLdouble x, GLdouble y, GLdouble z )
{
	cameraPosition[0] = x; cameraPosition[1] = y; cameraPosition[2] = z;
}

void GLModelView::getCameraPosition ( GLdouble& x, GLdouble& y, GLdouble& z )
{
	x = cameraPosition[0]; y = cameraPosition[1]; z = cameraPosition[2];
}

GLdouble GLModelView::distanceFromEye ( GLdouble wx, GLdouble wy, GLdouble wz )
{
    GLdouble tmpx = wx - cameraPosition[0];
    GLdouble tmpy = wy - cameraPosition[1];
    GLdouble tmpz = wz - cameraPosition[2];
    return sqrt( tmpx * tmpx + tmpy * tmpy + tmpz * tmpz );
}

void GLModelView::setupModelviewMatrix ()
{
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	// we must assume the camera is not on the y-axis
    // if it is on the y-axis, we guess at what would be a good up-vector
	if ( ( cameraPosition[0] == 0.0 ) && ( cameraPosition[2] == 0.0 ) ) {
        /*
		qWarning( QString( "GLModelView: camera position invalid: %1,%2,%3" )
		          .arg( cameraPosition[0] )
				  .arg( cameraPosition[1] )
				  .arg( cameraPosition[2] ) );
        */
        gluLookAt( cameraPosition[0], cameraPosition[1], cameraPosition[2],
                   0.0, 0.0, 0.0, 0.0, 0.0, -1.0 );
	} else {
        gluLookAt( cameraPosition[0], cameraPosition[1], cameraPosition[2],
	               0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
    }
}


