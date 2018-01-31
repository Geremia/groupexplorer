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


#include "glsceneview.h"


#if QT_VERSION >= 0x040000
#define MEM_ARRAY_TYPE Q3MemArray
#include <QMouseEvent>
#include <Q3MemArray>
#else
#define MEM_ARRAY_TYPE QMemArray
#endif


GLSceneView::GLSceneView ( QWidget *parent, const char *name )
    : QGLWidget ( parent, name ),
	pressedOnX( 0 ), pressedOnY( 0 ),
	lastDragXDist( 0 ), lastDragYDist( 0 ),
	lastDragXLoc( 0 ), lastDragYLoc( 0 ),
	useLighting( TRUE ), useFog( FALSE ), lineWidth( -1.0 ), pointSize( -1.0 )
{
	scene = new GLSceneNode();
	scene->name = "sceneview";
    scene->addChild( fixed = new GLSceneNode() );
    fixed->name = "fixedcontainer";
    scene->addChild( moving = new GLSceneNode() );
    moving->name = "mutablecontainer";
	clearColor[0] = clearColor[1] = clearColor[2] = clearColor[3] = 0.0;
	ambientColor[0] = ambientColor[1] = ambientColor[2]
	                = ambientColor[3] = 0.0;
}

GLSceneView::~GLSceneView ()
{
	delete scene;
}

void GLSceneView::addChild ( GLSceneNode* ch )
{
	moving->addChild( ch );
}

void GLSceneView::addFixedChild ( GLSceneNode* ch )
{
	fixed->addChild( ch );
}

void GLSceneView::setClearColor ( GLdouble r, GLdouble g, GLdouble b, GLdouble a )
{
	clearColor[0] = r; clearColor[1] = g; clearColor[2] = b; clearColor[3] = a;
}

void GLSceneView::setUseLighting ( bool use )
{
	useLighting = use;
}

void GLSceneView::setAmbientColor ( GLfloat r, GLfloat g, GLfloat b,
                                    GLfloat a )
{
	ambientColor[0] = r; ambientColor[1] = g;
	ambientColor[2] = b; ambientColor[3] = a;
}

void GLSceneView::setFogValues ( GLfloat r, GLfloat g, GLfloat b,
                                 GLfloat start, GLfloat end )
{
    fogColor[0] = r;
    fogColor[1] = g;
    fogColor[2] = b;
    fogStart = start;
    fogEnd = end;
}

void GLSceneView::setUseFog ( bool use )
{
    useFog = use;
}

void GLSceneView::setLineWidth ( GLdouble w )
{
    lineWidth = w;
}

void GLSceneView::setPointSize ( GLdouble s )
{
    pointSize = s;
}

void GLSceneView::initializeGL ()
{
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	// surely, we could create more private variables and member functions
	// for setting them, and have all these things be customizable, but there's
	// no need to go crazy with such stuff until it's needed
}

void GLSceneView::setupProjectionMatrix ()
{
	// this is a nice default, but descendants should feel free to override;
    double aspectRatio = double( width() ) / double( height() );
    double nearZ = 0.01;
    double farZ = 100.0;
    double fovy = 60.0;
    gluPerspective( ( aspectRatio > 1 ) ? fovy : fovy / aspectRatio,
                    aspectRatio, nearZ, farZ );
    // NOTE: do NOT move matrix mode and load identity up here, because it screws
    // up picking ROYALLY
}

void GLSceneView::setupModelviewMatrix ()
{
	// this is a reasonable default, but descendants should feel free to override
	gluLookAt( 5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
}

void GLSceneView::resizeGL ( int w, int h )
{
    glViewport( 0, 0, (GLint)w, (GLint)h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
    setupProjectionMatrix();
}

void GLSceneView::paintGL ()
{
	glClearColor( clearColor[0], clearColor[1], clearColor[2], clearColor[3] );
	glRenderMode( GL_RENDER );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	setupProjectionMatrix();

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	setupModelviewMatrix();

	if ( useLighting ) {
        glEnable( GL_LIGHTING );
		// the next two lines just ensure defaults:
		glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE );
		glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );

		glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambientColor );
		glShadeModel( GL_SMOOTH );
	} else {
        glDisable( GL_LIGHTING );
    }

    if ( useFog ) {
        glEnable( GL_FOG );
        glFogi( GL_FOG_MODE, GL_LINEAR );
        glFogfv( GL_FOG_COLOR, fogColor );
        glFogf( GL_FOG_START, fogStart );
        glFogf( GL_FOG_END, fogEnd );
    } else {
        glDisable( GL_FOG );
    }

    if ( lineWidth >= 0 ) glLineWidth( lineWidth );
    if ( pointSize >= 0 ) glPointSize( pointSize );

	if ( scene ) {
		scene->render( GLSceneNode::RenderLights, GL_RENDER, this );
		scene->render( GLSceneNode::RenderOpaque, GL_RENDER, this );
		scene->render( GLSceneNode::RenderTransparent, GL_RENDER, this );
	}
}

QPoint GLSceneView::pressedOn ()
{
	return QPoint( pressedOnX, pressedOnY );
}

QPoint GLSceneView::lastDrag ()
{
	return QPoint( lastDragXDist, lastDragYDist );
}

void GLSceneView::mousePressEvent ( QMouseEvent* e )
{
	lastDragXLoc = pressedOnX = e->x();
	lastDragYLoc = pressedOnY = e->y();
	lastDragXDist = lastDragYDist = 0;
	// then descendants can reimplement this to do:
	//   GLSceneView::mousePressEvent( e );
	// then their processing after, using pressedOn() function
}

void GLSceneView::mouseReleaseEvent ( QMouseEvent* e )
{
	pressedOnX = pressedOnY = -1; // mouse not down
	// descendants can reimplement, like with mousePressEvent()
	emit mouseRelease( e->x(), e->y() );
	// but because of above line, please call this method too!
}

void GLSceneView::mouseDoubleClickEvent ( QMouseEvent* e )
{
	pressedOnX = pressedOnY = -1; // mouse not down
	// descendants can reimplement, like with mousePressEvent()
	emit doubleClick( e->x(), e->y() );
	// but because of above line, please call this method too!
}

void GLSceneView::mouseMoveEvent ( QMouseEvent* e )
{
	// descendants can reimplement this to do:
	//   GLSceneView::mouseMoveEvent( e );
	// then their processing after, using lastDrag() function
	lastDragXDist = e->x() - lastDragXLoc;
	lastDragYDist = e->y() - lastDragYLoc;
	lastDragXLoc = e->x();
	lastDragYLoc = e->y();
}

void GLSceneView::wheelEvent ( QWheelEvent* e )
{
    emit wheelSpun( e->delta(), e->orientation() );
}

void GLSceneView::contextMenuEvent ( QContextMenuEvent* e )
{
	emit contextMenu( e->x(), e->y() );
}


void GLSceneView::resetScene ()
{
	moving->reset();
}

void GLSceneView::rotateScene ( GLdouble degsX, GLdouble degsY, GLdouble degsZ )
{
	TSRMatrix tmp;
	tmp.rotate( degsX, degsY, degsZ );
	moving->leftMultiplyBy( tmp );
}

void GLSceneView::rotateScene ( GLdouble degs,
                                GLdouble axX, GLdouble axY, GLdouble axZ )
{
	TSRMatrix tmp;
	tmp.rotate( degs, axX, axY, axZ );
	moving->leftMultiplyBy( tmp );
}

void GLSceneView::translateScene ( GLdouble x, GLdouble y, GLdouble z )
{
	TSRMatrix tmp;
	tmp.translate( x, y, z );
	moving->leftMultiplyBy( tmp );
}

void GLSceneView::scaleScene ( GLdouble x, GLdouble y, GLdouble z )
{
	TSRMatrix tmp;
	tmp.scale( x, y, z );
	moving->leftMultiplyBy( tmp );
}

void GLSceneView::fetchModelviewMatrix ( GLdouble putItHere[] )
{
    makeCurrent();
    GLint prevMatrixMode;
    glGetIntegerv( GL_MATRIX_MODE, &prevMatrixMode );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
    glLoadIdentity();
	setupModelviewMatrix();
	glGetDoublev( GL_MODELVIEW_MATRIX, putItHere );
    glPopMatrix();

	glMatrixMode( prevMatrixMode );
}

void GLSceneView::fetchProjectionMatrix ( GLdouble putItHere[] )
{
    makeCurrent();
    GLint prevMatrixMode;
    glGetIntegerv( GL_MATRIX_MODE, &prevMatrixMode );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
    glLoadIdentity();
	setupProjectionMatrix();
	glGetDoublev( GL_PROJECTION_MATRIX, putItHere );
    glPopMatrix();

	glMatrixMode( prevMatrixMode );
}

QRect GLSceneView::viewportRectangle ()
{
    makeCurrent();
	GLint viewport[4];
	glGetIntegerv( GL_VIEWPORT, viewport );
    return QRect( viewport[0], viewport[1], viewport[2], viewport[3] );
}

void GLSceneView::worldToScreen ( GLdouble wx, GLdouble wy, GLdouble wz,
                                  GLdouble& sx, GLdouble& sy, GLdouble& sz )
{
    makeCurrent();
	GLint viewport[4];
	GLdouble modelviewmatrix[16];
	GLdouble projectionmatrix[16];

	glGetIntegerv( GL_VIEWPORT, viewport );
	fetchModelviewMatrix( modelviewmatrix );
	fetchProjectionMatrix( projectionmatrix );

	sx = sy = sz = (GLdouble)0.0;  // in case gluProject goes wrong
	gluProject( wx, wy, wz, modelviewmatrix, projectionmatrix, viewport,
	            &sx, &sy, &sz );
}

void GLSceneView::screenToWorld ( GLdouble sx, GLdouble sy, GLdouble sz,
                                  GLdouble& wx, GLdouble& wy, GLdouble& wz )
{
    makeCurrent();
	GLint viewport[4];
	GLdouble modelviewmatrix[16];
	GLdouble projectionmatrix[16];

	glGetIntegerv( GL_VIEWPORT, viewport );
	fetchModelviewMatrix( modelviewmatrix );
	fetchProjectionMatrix( projectionmatrix );

	wx = wy = wz = 0.0; // in case something goes wrong with gluUnProject
	gluUnProject( sx, sy, sz, modelviewmatrix, projectionmatrix, viewport,
	              &wx, &wy, &wz );
}

void GLSceneView::nodeToScreen ( GLSceneNode* n,
                                 GLdouble& sx, GLdouble& sy, GLdouble& sz )
{
	GLdouble wx, wy, wz;
	n->relativeCoordinates( 0.0, 0.0, 0.0, wx, wy, wz );
	worldToScreen( wx, wy, wz, sx, sy, sz );
}

#define BUFSIZE 512
void GLSceneView::pickClickToBuf ( GLdouble wx, GLdouble wy,
                                   GLuint*& buf, GLint& size )
{
    makeCurrent();
	GLuint selectBuffer[BUFSIZE];
	GLint viewport[4];

	glSelectBuffer( BUFSIZE, selectBuffer );
	glRenderMode( GL_SELECT );
	glInitNames();

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	viewport[0] = viewport[1] = 0;
	viewport[2] = width();  viewport[3] = height();
	gluPickMatrix( wx, (GLdouble)viewport[3] - wy, 5.0, 5.0, viewport );
	setupProjectionMatrix();

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	setupModelviewMatrix();

	if ( scene ) scene->render( GLSceneNode::RenderOpaque |
	                            GLSceneNode::RenderTransparent,
	                            GL_SELECT, this );

	glFlush();

	// The following code comes from pages 536-543 of the OpenGL Red Book.
	// I have condensed their code to do only the stuff I need it to.
	// Thank you, Mason Woo, et al.
    // Then, 07.29.05, I expanded it to sort by depth (nearest first).
	size = glRenderMode( GL_RENDER );
	buf = 0;
	if ( size < 0 ) return;
	buf = new GLuint[size];

    // first go through the whole selection buffer and pull out the near z values
    // for each record, so that we can sort things by them.
	GLuint* iterator = selectBuffer;
    GLuint* nearZValues = new GLuint[size];
	for ( int i = 0 ; ( i < size ) &&
	                  ( iterator < selectBuffer + BUFSIZE ) ; i++ ) {
        // grab near z value for this record
        if ( iterator + 1 < selectBuffer + BUFSIZE ) nearZValues[i] = *(iterator+1);
		iterator += 3 + *iterator; // jump to next record:
	}

    // now find the permutation that would (bubble) sort those z values into ascending
    // order; invert it so that we can use its inverse below to extract the records into
    // sorted order.
    MEM_ARRAY_TYPE<uint> perm;
    perm.resize( size );
    for ( int i = 0 ; i < size ; i++ ) perm[i] = (uint)i;
    for ( int i = 1 ; i < size ; i++ ) {
        for ( int j = i ; j > 0 ; j-- ) {
            if ( nearZValues[perm[j-1]] > nearZValues[perm[j]] ) {
                uint tmp = perm[j];
                perm[j] = perm[j-1];
                perm[j-1] = tmp;
            }
        }
    }
    delete nearZValues;
    MEM_ARRAY_TYPE<uint> perminv;
    perminv.resize( size );
    for ( int i = 0 ; i < size ; i++ ) perminv[perm[i]] = (uint)i;

    // loop back through the buffer, this time pulling out the innermost tag of each hit
    // record (the most specific one, the one at the leaf of the scene tree), using the
    // inverted permutation to place them in the result buffer in distance-sorted order.
	iterator = selectBuffer;
	for ( int i = 0 ; ( i < size ) &&
	                  ( iterator < selectBuffer + BUFSIZE ) ; i++ ) {
		// jump to last name (the most specific):
		iterator += 2 + *iterator;
		// record it, provided we didn't jump out of the buffer:
		if ( iterator < selectBuffer + BUFSIZE ) buf[perminv[i]] = *(iterator++);
	}
}
// you use this routine if you make your own calls to gl{Push,Pop}Name()
// in your rendering routines, and want to interpret the names yourself
LIST_TYPE<GLuint> GLSceneView::pickClickTags ( GLdouble wx, GLdouble wy,
                                                bool* overflow )
{
	LIST_TYPE<GLuint> result;
	if ( scene ) {
		GLuint* hitlist;
		GLint hits;
		pickClickToBuf( wx, wy, hitlist, hits );
		for ( int i = 0 ; i < hits ; i++ ) result << hitlist[i];
		if ( hitlist ) delete hitlist;
		if ( overflow ) *overflow = ( hits < 0 );
	}
	return result;
}
// you use this routine if you do NOT make ANY calls to gl{Push,Pop}Name()
// in your rendering, and therefore can trust the pushing and popping that
// GLSceneNode naturally does to be the only thing on the name stack, and
// thus the unique name assignments and interpretation done below to be
// accurate and sensible (i.e. you won't get any 0s in the result of this fn)
LIST_TYPE<GLSceneNode*> GLSceneView::pickClick ( GLdouble wx, GLdouble wy,
                                                  bool* overflow )
{
	LIST_TYPE<GLSceneNode*> result;
	if ( scene ) {
		scene->createUniqueGLNames();
		GLuint* hitlist;
		GLint hits;
		pickClickToBuf( wx, wy, hitlist, hits );
		for ( int i = 0 ; i < hits ; i++ ) result << 0;
		scene->decodeUniqueGLNames( hitlist, hits, result );
		if ( hitlist ) delete hitlist;
		if ( overflow ) *overflow = ( hits < 0 );
	}
	return result;
}

QString GLSceneView::debugScene ()
{
	return scene->debugString();
}

