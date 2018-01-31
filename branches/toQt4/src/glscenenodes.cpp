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


#include "glscenenodes.h"
#include "glmodelview.h"
#include <math.h>


#define PI 3.1415926535897932384626433832795028841971


GLTriangle::GLTriangle ( GLdouble x1, GLdouble y1, GLdouble z1,
                         GLdouble x2, GLdouble y2, GLdouble z2,
                         GLdouble x3, GLdouble y3, GLdouble z3 )
	: x1( x1 ), y1( y1 ), z1( z1 ),
	  x2( x2 ), y2( y2 ), z2( z2 ),
	  x3( x3 ), y3( y3 ), z3( z3 )
{
	name = "triangle";
}

void GLTriangle::renderMe ()
{
	glBegin( GL_POLYGON );
		glVertex3f( x1, y1, z1 );
		glVertex3f( x2, y2, z2 );
		glVertex3f( x3, y3, z3 );
	glEnd();
}



GLSquare::GLSquare ()
{
	name = "square";
}

void GLSquare::renderMe ()
{
	glBegin( GL_POLYGON );
        glNormal3f( 0.0, 0.0,-1.0 );
		glVertex3f( 0.0, 0.0, 0.0 );
		glVertex3f( 0.0, 1.0, 0.0 );
		glVertex3f( 1.0, 1.0, 0.0 );
		glVertex3f( 1.0, 0.0, 0.0 );
	glEnd();
}



GLCube::GLCube ()
{
	GLSquare* s;

	s = new GLSquare();
	s->translate( 0.0, 0.0, 1.0 );
	addChild( s );
	s = new GLSquare();
	s->translate( 1.0, 0.0, 0.0 );
	s->rotate( 0.0, 180.0, 0.0 );
	addChild( s );
	s = new GLSquare();
	s->rotate( 90.0, 0.0, 0.0 );
	addChild( s );
	s = new GLSquare();
	s->translate( 0.0, 1.0, 1.0 );
	s->rotate( -90.0, 0.0, 0.0 );
	addChild( s );
	s = new GLSquare();
	s->rotate( 0.0, -90.0, 0.0 );
	addChild( s );
	s = new GLSquare();
	s->translate( 1.0, 0.0, 1.0 );
	s->rotate( 0.0, 90.0, 0.0 );
	addChild( s );

	name = "cube";
}


GLWireCube::GLWireCube ()
{
    name = "wirecube";
}

void GLWireCube::renderMe ()
{
    glLineWidth( 1.0 );
    glBegin( GL_LINES );
        glVertex3f( 0,0,0 ); glVertex3f( 0,0,1 );
        glVertex3f( 0,1,0 ); glVertex3f( 0,1,1 );
        glVertex3f( 1,0,0 ); glVertex3f( 1,0,1 );
        glVertex3f( 1,1,0 ); glVertex3f( 1,1,1 );
        glVertex3f( 0,0,0 ); glVertex3f( 0,1,0 );
        glVertex3f( 0,0,1 ); glVertex3f( 0,1,1 );
        glVertex3f( 1,0,0 ); glVertex3f( 1,1,0 );
        glVertex3f( 1,0,1 ); glVertex3f( 1,1,1 );
        glVertex3f( 0,0,0 ); glVertex3f( 1,0,0 );
        glVertex3f( 0,0,1 ); glVertex3f( 1,0,1 );
        glVertex3f( 0,1,0 ); glVertex3f( 1,1,0 );
        glVertex3f( 0,1,1 ); glVertex3f( 1,1,1 );
    glEnd();
}


GLCircle::GLCircle ( uint segments )
	: segments( segments )
{
	name = "circle";
}

bool GLCircle::useDisplayList ()
{
	return TRUE;
}

void GLCircle::displayListRender ()
{
	glBegin( GL_LINE_LOOP );
	for ( uint segment = 0 ; segment < segments ; segment++ ) {
		GLdouble pct = 2 * PI * segment / segments;
		glVertex3f( cos( pct ), sin( pct ), 0.0 );
	}
	glEnd();
}


GLSphere::GLSphere ( uint wedges, uint stacks )
	: wedges( wedges ), stacks( stacks )
{
	name = "sphere";
}

bool GLSphere::useDisplayList ()
{
	return TRUE;
}

void GLSphere::displayListRender ()
{
	uint wedge, stack;
	GLdouble lastY, lastR;
	GLdouble thisY, thisR;
	GLdouble pct;

	glEnable( GL_NORMALIZE );
	lastY = -1;
	lastR = 0;
	for ( stack = 1 ; stack <= stacks ; stack++ ) {
		thisY = -cos( PI * stack / stacks );
		thisR = sqrt( 1 - thisY * thisY );
		glBegin( GL_QUAD_STRIP );
			for ( wedge = 0 ; wedge <= wedges ; wedge++ ) {
				pct = 2 * PI * wedge / wedges;
                GLdouble c = cos( pct ), s = sin( pct );
				glNormal3f( thisR * c, thisY, thisR * s );
				glVertex3f( thisR * c, thisY, thisR * s );
				glNormal3f( lastR * c, lastY, lastR * s );
				glVertex3f( lastR * c, lastY, lastR * s );
			}
		glEnd();
		lastY = thisY;
		lastR = thisR;
	}
	glDisable( GL_NORMALIZE );
}


GLLine::GLLine ( GLdouble fromX, GLdouble fromY, GLdouble fromZ,
                 GLdouble toX, GLdouble toY, GLdouble toZ )
	: fromX( fromX ), fromY( fromY ), fromZ( fromZ ),
	toX( toX ), toY( toY ), toZ( toZ )
{
	name = "line";
}

void GLLine::renderMe ()
{
	glBegin( GL_LINES );
		glVertex3f( fromX, fromY, fromZ );
		glVertex3f( toX, toY, toZ );
	glEnd();
}


GLConnectingLine::GLConnectingLine ( GLSceneNode* fromNode,
                                     GLSceneNode* toNode )
	: fromNode( fromNode ), toNode( toNode )
{
	name = QString( "line connecting %1 to %2" )
	       .arg( fromNode ? fromNode->name : QString( "NULL-NODE" ) )
		   .arg( toNode ? toNode->name : QString( "NULL-NODE" ) );
}

void GLConnectingLine::renderMe ()
{
	GLdouble x, y, z;
	glBegin( GL_LINES );
		fromNode->relativeCoordinates( 0.0, 0.0, 0.0, x, y, z, parent );
		glVertex3f( x, y, z );
		toNode->relativeCoordinates( 0.0, 0.0, 0.0, x, y, z, parent );
		glVertex3f( x, y, z );
	glEnd();
}


GLTextLabel::GLTextLabel ( QString text, QFont font )
	: text( text ), font( font )
{
	name = "text (" + text + ")";
}

void GLTextLabel::renderMe ()
{
	if ( renderingOn ) renderingOn->renderText( 0.0, 0.0, 0.0, text, font );
}


QMap<QGLContext*,GLenum> GLLight::nextLight;
GLint GLLight::numLights = 8;

GLLight::GLLight ( GLfloat x, GLfloat y, GLfloat z, GLfloat w, QGLContext* c )
	: constantAtten( 1.0 ), linearAtten( 0.0 ), quadraticAtten( 0.0 ),
	spotlight( FALSE ), cutoff( 45.0 ), exponent( 1.0 )
{
	// first we need to figure out which light we are.
	//if ( !numLights ) glGetIntegerv( GL_MAX_LIGHTS, &numLights );
	// the above line was giving numLights == 0 for unknown reason...bleh
    if ( !nextLight.contains( c ) ) nextLight[c] = GL_LIGHT0;
	valid = ( ((GLint)(nextLight[c] - GL_LIGHT0)) < numLights );
	if ( valid ) whichLight = nextLight[c]++;

	position[0] = x;
	position[1] = y;
	position[2] = z;
	position[3] = w;
	aim( 0.0, 0.0, -1.0 );

	name = "light";
}

bool GLLight::isValid () {
	return valid;
}

void GLLight::aim ( GLdouble x, GLdouble y, GLdouble z )
{
	direction[0] = x;
	direction[1] = y;
	direction[2] = z;
}

void GLLight::aimAt ( GLdouble x, GLdouble y, GLdouble z )
{
	if ( position[3] ) { // infinite lights don't get aimed this way
		GLdouble x2, y2, z2;
		inverseRelativeCoordinates( x, y, z, x2, y2, z2 );
		relativeCoordinates( x2, y2, z2, x, y, z );
		direction[0] = x2 - position[0]/position[3];
		direction[1] = y2 - position[1]/position[3];
		direction[2] = z2 - position[2]/position[3];
	}
}

void GLLight::aimAt ( GLSceneNode* node )
{
	GLdouble x, y, z;
	node->relativeCoordinates( 0.0, 0.0, 0.0, x, y, z );
	aimAt( x, y, z );
}

void GLLight::focus ( GLfloat halfAngle )
{
	cutoff = halfAngle;
	setSpotlight( TRUE );
}

void GLLight::concentrate ( GLfloat exp )
{
	exponent = exp;
	setSpotlight( FALSE );
}

void GLLight::setSpotlight ( bool isSpot )
{
	name = ( spotlight = isSpot ) ? "spotlight" : "light";
}

void GLLight::tweakAttenuations (
		GLfloat constant, GLfloat linear, GLfloat quadratic )
{
	constantAtten = constant;
	linearAtten = linear;
	quadraticAtten = quadratic;
	spotlight = TRUE;
}

void GLLight::setShininess ( GLfloat s )
{
	qWarning( QString( "GLLight::setShininess(%1) call inappropriate\n"
	                   "\t(lights only have diffuse/ambient/specular colors)" )
	          .arg( s ) );
}

void GLLight::addChild ( GLSceneNode* child )
{
	qWarning( QString( "GLLight::addChild( [%1] ) call inappropriate\n"
	                   "\t(instead, add lights as children to other nodes)" )
	          .arg( child->name ) );
}

void GLLight::setTransparent ( bool transp, bool recur )
{
	qWarning( QString( "GLLight::setTransparent(%1,%2) call inappropriate\n"
	                   "\t(lights get rendering priority anyway)" )
	          .arg( transp ? "TRUE" : "FALSE" )
			  .arg( recur ? "TRUE" : "FALSE" ) );
}

void GLLight::render ( RenderFlags flags, GLenum, QGLWidget* )
{
	if ( ( flags & RenderLights ) && valid ) {

		glPushMatrix();
		multToGLStack();

		if ( hidden ) {
			glDisable( whichLight );
		} else {
			glEnable( GL_LIGHTING );
			glEnable( whichLight );
			if ( useColor ) glLightfv( whichLight, GL_DIFFUSE, color );
			if ( useMaterial ) {
				glLightfv( whichLight, GL_DIFFUSE, diffuse );
				glLightfv( whichLight, GL_SPECULAR, specular );
				glLightfv( whichLight, GL_AMBIENT, ambient );
			}
			glLightfv( whichLight, GL_POSITION, position );
			if ( spotlight ) {
				glLightfv( whichLight, GL_SPOT_DIRECTION, direction );
				glLightf( whichLight, GL_SPOT_CUTOFF, cutoff );
				glLightf( whichLight, GL_SPOT_EXPONENT, exponent );
			}
			glLightf( whichLight, GL_CONSTANT_ATTENUATION, constantAtten );
			glLightf( whichLight, GL_LINEAR_ATTENUATION, linearAtten );
			glLightf( whichLight, GL_QUADRATIC_ATTENUATION, quadraticAtten );
		}
		
		glPopMatrix();
	}
}


/*
 * Note: don't use this guy to wrap transparent things with opaque things.
 * And if you wrap transparent stuff together, mark this guy as transparent.
 * ...although I haven't tested that...
 */
GLDisplayListWrapper::GLDisplayListWrapper ( GLSceneNode* content )
	: content( content )
{
	name = "display list wrapper";
        addChild( content ); // this will handle deletion later
	processContent( content );
}

bool GLDisplayListWrapper::useDisplayList ()
{
	return TRUE;
}

void GLDisplayListWrapper::displayListRender ()
{
	content->render(); // just defaults...maybe this will need to change 1 day?
}

void GLDisplayListWrapper::processContent ( GLSceneNode* content )
{
	// ensure this doesn't try to nest display lists (it doesn't work):
	content->turnOffDisplayList();
	// recur on its children:
	for ( uint i = 0 ; i < content->children.count() ; i++ )
		processContent( content->children.at( i ) );
}


GLTowardCameraWrapper::GLTowardCameraWrapper ( GLSceneNode* content )
{
	name = "aim-to-camera container";
	aimer = new GLSceneNode();
	aimer->name = "aim-to-camera node";
	addChild( aimer );
	aimer->addChild( content );
}

void GLTowardCameraWrapper::preRender ()
{
	GLSceneNode::preRender();
	// now if I can find out where the camera is, I do my thing:
	if ( renderingOn && renderingOn->inherits( "GLModelView" ) ) {
		GLModelView* mv = (GLModelView*)renderingOn;
		GLdouble x, y, z;
		mv->getCameraPosition( x, y, z );
		// so where is the camera from MY point of view?
		GLdouble relX, relY, relZ;
		inverseRelativeCoordinates( x, y, z, relX, relY, relZ );
		GLdouble tx,ty,tz;//tmp--debugging
		relativeCoordinates( 0.0, 0.0, 0.0, tx, ty, tz );
		relativeCoordinates( relX, relY, relZ, tx, ty, tz );
		// now let's aim the z axis that way...
		aimer->reset();
		aimer->rotate( 0.0, 0.0, 1.0, relX, relY, relZ );
	}
}

