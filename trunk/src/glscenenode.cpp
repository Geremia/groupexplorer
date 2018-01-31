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


#include "glscenenode.h"
#include "glmodelview.h"
#include <math.h>


GLSceneNode::RenderFlags GLSceneNode::RenderTransparent = 1;
GLSceneNode::RenderFlags GLSceneNode::RenderOpaque = 2;
GLSceneNode::RenderFlags GLSceneNode::RenderLights = 4;
GLSceneNode::RenderFlags GLSceneNode::RenderHidden = 8;


GLSceneNode::GLSceneNode ( GLSceneNode* parent )
	: TSRMatrix(), transparent( FALSE ), hidden( FALSE ), glnametag( 0 ),
	  name( QString::null ), parent( parent ),
	  renderingOn( 0 ), lastRenderedOn( 0 ),
	  displayList( 0 ), neverDisplayList( FALSE ), renderMode( GL_RENDER ),
	  useColor( FALSE ), useMaterial( FALSE )
{
	children.setAutoDelete( TRUE );
	color[0] = color[1] = color[2] = color[3] = 1.0;
	diffuse[0] = diffuse[1] = diffuse[2] = diffuse[3] = 1.0;
	specular[0] = specular[1] = specular[2] = specular[3] = 1.0;
	ambient[0] = ambient[1] = ambient[2] = ambient[3] = 0.0;
	emissive[0] = emissive[1] = emissive[2] = emissive[3] = 0.0;
	shininess = 50.0;
    if ( parent ) parent->addChild( this );
}

GLSceneNode::~GLSceneNode ()
{
    if ( QObjectWatcher::isValid( lastRenderedOn ) ) {
        lastRenderedOn->makeCurrent();
	    if ( displayList ) glDeleteLists( displayList, 1 );
    }
}

void GLSceneNode::setColor ( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
	color[0] = r; color[1] = g; color[2] = b; color[3] = a;
	useColor = TRUE;
	useMaterial = FALSE;
	if ( a < 1.0 ) transparent = TRUE;
}

/*
GLfloat* GLSceneNode::getColor ()
{
    return color;
}

GLfloat* GLSceneNode::getDiffuse ()
{
    return diffuse;
}

GLfloat* GLSceneNode::getSpecular ()
{
    return specular;
}

GLfloat* GLSceneNode::getAmbient ()
{
    return ambient;
}

GLfloat* GLSceneNode::getEmissive ()
{
    return emissive;
}

GLfloat GLSceneNode::getShininess ()
{
    return shininess;
}

*/
void GLSceneNode::setDiffuse ( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
	diffuse[0] = r; diffuse[1] = g; diffuse[2] = b; diffuse[3] = a;
	useMaterial = TRUE;
	useColor = FALSE;
	if ( a < 1.0 ) transparent = TRUE;
}

void GLSceneNode::setSpecular ( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
	specular[0] = r; specular[1] = g; specular[2] = b; specular[3] = a;
	useMaterial = TRUE;
	useColor = FALSE;
}

void GLSceneNode::setAmbient ( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
	ambient[0] = r; ambient[1] = g; ambient[2] = b; ambient[3] = a;
	useMaterial = TRUE;
	useColor = FALSE;
}

void GLSceneNode::setEmissive ( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
	emissive[0] = r; emissive[1] = g; emissive[2] = b; emissive[3] = a;
	useMaterial = TRUE;
	useColor = FALSE;
}

void GLSceneNode::setShininess ( GLfloat s )
{
	shininess = s;
	useMaterial = TRUE;
	useColor = FALSE;
}

void GLSceneNode::addChild ( GLSceneNode* child )
{
	children.append( child );
	child->parent = this;
}

void GLSceneNode::turnOffDisplayList ()
{
	neverDisplayList = TRUE;
}

void GLSceneNode::markDisplayListDirty ()
{
    if ( QObjectWatcher::isValid( lastRenderedOn ) ) {
        lastRenderedOn->makeCurrent();
	    if ( displayList ) {
		    glDeleteLists( displayList, 1 );
		    displayList = 0;
        }
	}
}

GLuint GLSceneNode::createUniqueGLNames ( GLuint nextName )
{
	glnametag = nextName++;
	for ( uint i = 0 ; i < children.count() ; i++ )
		nextName = children.at( i )->createUniqueGLNames( nextName );
	return nextName;
}

void GLSceneNode::decodeUniqueGLNames ( GLuint* names, uint length,
	                                    LIST_TYPE<GLSceneNode*>& decoded )
{
	for ( uint i = 0 ; i < length ; i++ )
		if ( names[i] == glnametag )
			decoded[i] = this;
	for ( uint i = 0 ; i < children.count() ; i++ )
		children.at( i )->decodeUniqueGLNames( names, length, decoded );
}

void GLSceneNode::absoluteCameraPosition ( GLdouble& x, GLdouble& y, GLdouble& z )
{
    // it gives 0,0,0 if it can't find the camera
    if ( renderingOn && renderingOn->inherits( "GLModelView" ) ) {
        GLModelView* mv = ( GLModelView* )renderingOn;
        mv->getCameraPosition( x, y, z );
    } else {
        x = y = z = 0.0;
    }
}

void GLSceneNode::cameraPosition ( GLdouble& x, GLdouble& y, GLdouble& z )
{
    // get the camera position in the local coordinate system of this node
    // it (still) gives 0,0,0 if it can't find the camera
    absoluteCameraPosition( x, y, z );
    if ( x || y || z ) inverseRelativeCoordinates( x, y, z, x, y, z );
}

void GLSceneNode::absoluteScreenXYZ ( GLdouble centerX, GLdouble centerY, GLdouble centerZ,
                                      GLdouble& Xx, GLdouble& Xy, GLdouble& Xz,
                                      GLdouble& Yx, GLdouble& Yy, GLdouble& Yz,
                                      GLdouble& Zx, GLdouble& Zy, GLdouble& Zz )
{
    if ( renderingOn && renderingOn->inherits( "GLSceneView" ) ) {
        GLdouble tmpx, tmpy, tmpz;
        GLSceneView* sv = ( GLSceneView* )renderingOn;
        sv->worldToScreen( centerX, centerY, centerZ, tmpx, tmpy, tmpz );
        sv->screenToWorld( tmpx + 10, tmpy, tmpz, Xx, Xy, Xz );
        sv->screenToWorld( tmpx, tmpy - 10, tmpz, Yx, Yy, Yz );
        sv->screenToWorld( tmpx, tmpy, 1.0, Zx, Zy, Zz );
        Xx -= centerX;  Xy -= centerY;  Xz -= centerZ;
        Yx -= centerX;  Yy -= centerY;  Yz -= centerZ;
        Zx -= centerX;  Zy -= centerY;  Zz -= centerZ;
        unitize( Xx, Xy, Xz );
        unitize( Yx, Yy, Yz );
        unitize( Zx, Zy, Zz );
    } else {
        Xx = Xy = Xz = Yx = Yy = Yz = Zx = Zy = Zz = 0.0;
    }
}

void GLSceneNode::screenXYZ ( GLdouble centerX, GLdouble centerY, GLdouble centerZ,
                              GLdouble& Xx, GLdouble& Xy, GLdouble& Xz,
                              GLdouble& Yx, GLdouble& Yy, GLdouble& Yz,
                              GLdouble& Zx, GLdouble& Zy, GLdouble& Zz )
{
    if ( renderingOn && renderingOn->inherits( "GLSceneView" ) ) {
        GLdouble tmpx, tmpy, tmpz;
        GLSceneView* sv = ( GLSceneView* )renderingOn;
        relativeCoordinates( centerX, centerY, centerZ, tmpx, tmpy, tmpz );
        sv->worldToScreen( tmpx, tmpy, tmpz, tmpx, tmpy, tmpz );
        sv->screenToWorld( tmpx + 10, tmpy, tmpz, Xx, Xy, Xz );
        sv->screenToWorld( tmpx, tmpy - 10, tmpz, Yx, Yy, Yz );
        sv->screenToWorld( tmpx, tmpy, 1.0, Zx, Zy, Zz );
        inverseRelativeCoordinates( Xx, Xy, Xz, Xx, Xy, Xz );
        inverseRelativeCoordinates( Yx, Yy, Yz, Yx, Yy, Yz );
        inverseRelativeCoordinates( Zx, Zy, Zz, Zx, Zy, Zz );
        Xx -= centerX;  Xy -= centerY;  Xz -= centerZ;
        Yx -= centerX;  Yy -= centerY;  Yz -= centerZ;
        Zx -= centerX;  Zy -= centerY;  Zz -= centerZ;
        unitize( Xx, Xy, Xz );
        unitize( Yx, Yy, Yz );
        unitize( Zx, Zy, Zz );
    } else {
        Xx = Xy = Xz = Yx = Yy = Yz = Zx = Zy = Zz = 0.0;
    }
}

void GLSceneNode::preRender ()
{
	if ( useColor ) {
		glDisable( GL_LIGHTING );
		glColor4fv( color );
	}
	if ( useMaterial ) {
		glEnable( GL_LIGHTING );
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specular );
		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, ambient );
		glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, emissive );
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, shininess );
	}
}

void GLSceneNode::postRender ()
{
    // feel free to reimplement; I don't need it for anything (yet)
}

void GLSceneNode::renderMe ()
// To make a non-optimized descendant, reimplement this to render the object.
// If you want to make an object that uses display lists for optimization,
// then don't reimplement this method; instead, reimplement useDisplayList()
// to return TRUE, and reimplement displayListRender() to render the object.
{
	if ( useDisplayList() ) {
		glGetIntegerv( GL_RENDER_MODE, &renderMode );
		if ( displayList && !neverDisplayList &&
		     ( renderMode != GL_SELECT ) &&
		     ( renderMode != GL_FEEDBACK ) &&
		     ( glIsList( displayList ) == GL_TRUE ) ) {
			// if already optimized (and supposed to be) call fast version:
			glCallList( displayList );
		} else {
			// try to optimize by grabbing a display list:
			if ( !neverDisplayList && !displayList &&
			     ( renderMode != GL_SELECT ) &&
			     ( renderMode != GL_FEEDBACK ) &&
			     ( displayList = glGenLists( 1 ) ) ) {
				// remember this render for later:
				glNewList( displayList, GL_COMPILE_AND_EXECUTE );
				displayListRender();
				glEndList();
			} else {
				// either no more display lists available,
				// or someone turned on neverDisplayList, so do slow render:
				displayListRender();
			}
		}
	}
	// else this function does nothing, so subclasses need to reimplement it
}

void GLSceneNode::render ( RenderFlags flags, GLenum rendermode,
                           QGLWidget* target )
// target == the widget on which I'm being rendered, or 0 if not on a QGLWidget
//           (although I can't see that happening right now...?)
// This assumes the matrix mode is modelview.
{
	if ( !this->hidden || ( flags & RenderHidden ) ) {
		lastRenderedOn = renderingOn = target;
        QObjectWatcher::watch( lastRenderedOn );

		glPushMatrix();
		multToGLStack();

		if ( rendermode == GL_SELECT ) glPushName( glnametag );

		preRender();
		if ( ( this->transparent && ( flags & RenderTransparent ) )
		  || ( !this->transparent && ( flags & RenderOpaque ) ) ) renderMe();
		for ( uint i = 0 ; i < children.count() ; i++ )
			children.at( i )->render( flags, rendermode, target );
		postRender();

		if ( rendermode == GL_SELECT ) glPopName();
		glPopMatrix();

		renderingOn = 0;
	}
}

TSRMatrix* GLSceneNode::composeMatrixStack ()
{
	TSRMatrix* result;
	if ( parent ) {
		result = parent->composeMatrixStack();
	} else {
		result = new TSRMatrix();
	}
	result->rightMultiplyBy( *this );
	return result;
}

void GLSceneNode::relativeCoordinates (
		GLdouble relX, GLdouble relY, GLdouble relZ,
		GLdouble& absX, GLdouble& absY, GLdouble& absZ,
		GLSceneNode* ancestor )
{
	// base of recursion:
	if ( ancestor == this ) {
		absX = relX;  absY = relY;  absZ = relZ;
		return;
	}

	// first transform relXYZ as OpenGL would do with this node's matrix
	// then ask for the location of that abs point relative to rel parent
	transformVector( relX, relY, relZ, absX, absY, absZ );
	if ( parent )
		parent->relativeCoordinates( absX, absY, absZ, absX, absY, absZ, ancestor );
}

void GLSceneNode::inverseRelativeCoordinates (
		GLdouble absX, GLdouble absY, GLdouble absZ,
	    GLdouble& relX, GLdouble& relY, GLdouble& relZ,
		GLSceneNode* ancestor )
{
	// base of recursion:
	if ( ancestor == this ) {
		relX = absX;  relY = absY;  relZ = absZ;
		return;
	}

	// this behaves like relativeCoordinates, but in backwards order:
	if ( parent )
		parent->inverseRelativeCoordinates( absX, absY, absZ, absX, absY, absZ, ancestor );
	TSRMatrix* tmp = createInverse();
	tmp->transformVector( absX, absY, absZ, relX, relY, relZ );
	delete tmp;
}

void GLSceneNode::normalizeScaling ()
{
    // Sometimes one needs to be able to draw undistorted objects, even if the stack of
    // matrices above the object about to be drawn would otherwise distort it.
    // In such a case, you call this routine right before rendering the object (e.g., in
    // preRender()).  This routine calculates appropriate parameters for a call to
    // scale(...) and makes the call.  So you can think of this routine just like a
    // special case of a scale(...) call, because that's what it is.  Hence, for instance,
    // you can conjugate it by a translation to scale about a different origin.
    // Note that this only works if the axes are perpendicular.  That is, if the angle
    // in absolute coordinates between local x and y (or between local y and z, or x and
    // z) is not pi/2, this won't work right.  If you find yourself in that situation,
    // maybe normalizing higher up the scene hierarchy will help you.  See that there are
    // checks built into this routine, below, to issue console warnings in such a case.

    double origx, origy, origz, xx, xy, xz, yx, yy, yz, zx, zy, zz;

    relativeCoordinates( 0, 0, 0, origx, origy, origz ); // find origin

    relativeCoordinates( 1, 0, 0, xx, xy, xz ); // find how far away 1,0,0 is
    xx -= origx;  xy -= origy;  xz -= origz;
    double xscale = sqrt( xx*xx + xy*xy + xz*xz ); // use that to compute x scaling

    relativeCoordinates( 0, 1, 0, yx, yy, yz ); // find how far away 0,1,0 is
    yx -= origx;  yy -= origy;  yz -= origz;
    double yscale = sqrt( yx*yx + yy*yy + yz*yz ); // use that to compute y scaling

    if ( QABS( xx * yx + xy * yy + xz * yz ) > 0.01 )
        qWarning( QString( "GLSceneNode(%7)::normalizeScaling(): "
                           "local standard vectors not orthogonal: "
                           "x vector=<%1,%2,%3> dot y vector=<%4,%5,%6> = %8" )
                  .arg( xx ).arg( xy ).arg( xz )
                  .arg( yx ).arg( yy ).arg( yz ).arg( name )
                  .arg( xx * yx + xy * yy + xz * yz ) );

    relativeCoordinates( 0, 0, 1, zx, zy, zz ); // find how far away 0,0,1 is
    zx -= origx;  zy -= origy;  zz -= origz;
    double zscale = sqrt( zx*zx + zy*zy + zz*zz ); // use that to compute z scaling

    if ( QABS( xx * zx + xy * zy + xz * zz ) > 0.01 )
        qWarning( QString( "GLSceneNode(%7)::normalizeScaling(): "
                           "local standard vectors not orthogonal: "
                           "x vector=<%1,%2,%3> dot z vector=<%4,%5,%6> = %8" )
                  .arg( xx ).arg( xy ).arg( xz )
                  .arg( zx ).arg( zy ).arg( zz ).arg( name )
                  .arg( xx * zx + xy * zy + zx * zz ) );

    if ( QABS( zx * yx + zy * yy + zz * yz ) > 0.01 )
        qWarning( QString( "GLSceneNode(%7)::normalizeScaling(): "
                           "local standard vectors not orthogonal: "
                           "z vector=<%1,%2,%3> dot y vector=<%4,%5,%6> = %8" )
                  .arg( zx ).arg( zy ).arg( zz )
                  .arg( yx ).arg( yy ).arg( yz ).arg( name )
                  .arg( zx * yx + zy * yy + zz * yz ) );

    double min = QMIN( QMIN( xscale, yscale ), zscale );
    scale( min / xscale, min / yscale, min / zscale );
}

void GLSceneNode::setTransparent ( bool transp, bool recur )
{
	transparent = transp;
	if ( recur )
		for ( uint i = 0 ; i < children.count() ; i++ )
			children.at( i )->setTransparent( transp, recur );
}

void GLSceneNode::setHidden ( bool hid, bool recur )
{
	hidden = hid;
	if ( recur )
		for ( uint i = 0 ; i < children.count() ; i++ )
			children.at( i )->setHidden( hid, recur );
}

bool GLSceneNode::useDisplayList ()
{
	return FALSE;
}

void GLSceneNode::displayListRender ()
{
	// virtual; reimplement me if needed
}

QString GLSceneNode::debugString ( uint indent )
{
	QString indentStr;
	indentStr.fill( ' ', indent );
	QString result = QString( "%1%2 @ %4\n" )
	                 .arg( indentStr ).arg( name, 15 )
					 .arg( TSRMatrix::debugString() );
	for ( uint i = 0 ; i < children.count() ; i++ )
		result += children.at( i )->debugString( indent + 4 );
	return result;
}

void GLSceneNode::debugGLMatrix ()
{
	GLfloat stackmatrix[16];
	glGetFloatv( GL_MODELVIEW_MATRIX, stackmatrix );
	QString tmp;
	for ( uint i = 0 ; i < 4 ; i++ ) for ( uint j = 0 ; j < 4 ; j++ )
		tmp += QString( "%1" ).arg( stackmatrix[j*4+i], 7, 'f', 2 );
	TSRMatrix* tmp2 = composeMatrixStack();
	qDebug( ">>>>>>> compare these matrices...1) actual, 2) expected" );
	qDebug( tmp + "\n" + tmp2->debugString() );
}


QObjectWatcher* QObjectWatcher::instance ()
{
    static QObjectWatcher* i = new QObjectWatcher();
    return i;
}

void QObjectWatcher::watch ( QObject* w )
{
    if ( !instance()->watchees.contains( w ) ) {
        instance()->watchees.append( w );
        connect( w, SIGNAL(destroyed(QObject*)),
                 instance(), SLOT(watcheeDestroyed(QObject*)) );
    }
}

bool QObjectWatcher::isValid ( QObject* w )
{
    return ( bool )( instance()->watchees.contains( w ) );
}

void QObjectWatcher::watcheeDestroyed ( QObject* qglwidget )
{
    if ( watchees.contains( qglwidget ) ) watchees.remove( qglwidget );
}


