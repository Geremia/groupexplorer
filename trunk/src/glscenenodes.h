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


#ifndef GLSCENENODES_H
#define GLSCENENODES_H


#include "glscenenode.h"
#include <qgl.h>
#include <qmap.h>


class GLTriangle : public GLSceneNode {

public:
	GLTriangle ( GLdouble x1, GLdouble y1, GLdouble z1,
	             GLdouble x2, GLdouble y2, GLdouble z2,
				 GLdouble x3, GLdouble y3, GLdouble z3 );

	virtual void renderMe ();

private:
	GLdouble x1, y1, z1, x2, y2, z2, x3, y3, z3;

};


class GLSquare : public GLSceneNode {

public:
	GLSquare ();

	virtual void renderMe ();

};


class GLCube : public GLSceneNode {

public:
	GLCube ();

};


class GLWireCube : public GLSceneNode {

public:
	GLWireCube ();

    virtual void renderMe ();

};


class GLCircle : public GLSceneNode {

public:
	GLCircle ( uint segments = 20 );

	virtual void displayListRender ();
	virtual bool useDisplayList ();

private:
	uint segments;

};


class GLSphere : public GLSceneNode {

public:
	GLSphere ( uint wedges = 20, uint stacks = 8 );

	virtual void displayListRender ();
	virtual bool useDisplayList ();

private:
	uint wedges;
	uint stacks;

};


class GLLine : public GLSceneNode {

public:
	GLLine ( GLdouble fromX, GLdouble fromY, GLdouble fromZ,
	         GLdouble toX, GLdouble toY, GLdouble toZ );

	virtual void renderMe ();

private:
	GLdouble fromX, fromY, fromZ;
	GLdouble toX, toY, toZ;

};


class GLConnectingLine : public GLSceneNode {

public:
	GLConnectingLine ( GLSceneNode* fromNode, GLSceneNode* toNode );

	virtual void renderMe ();

private:
	GLSceneNode* fromNode;
	GLSceneNode* toNode;

};


class GLTextLabel : public GLSceneNode {

public:
	GLTextLabel ( QString text, QFont font );

	virtual void renderMe ();

private:
	QString text;
	QFont font;

};


class GLLight : public GLSceneNode {

public:
	GLLight ( GLfloat x = 0.0, GLfloat y = 0.0, GLfloat z = 1.0,
	          GLfloat w = 0.0, QGLContext* c = 0 );

	void aim ( GLdouble x, GLdouble y, GLdouble z );
	void aimAt ( GLdouble x, GLdouble y, GLdouble z );
	void aimAt ( GLSceneNode* node );
	void focus ( GLfloat halfAngle );
	void concentrate ( GLfloat exp );
	void setSpotlight ( bool isSpot = TRUE );
	void tweakAttenuations ( GLfloat constant = 1.0,
	                         GLfloat linear = 0.0,
							 GLfloat quadratic = 0.0 );

	bool isValid ();

	// things this will override because they don't make sense for lights:
	virtual void setShininess ( GLfloat s );
	virtual void addChild ( GLSceneNode* child );
	virtual void setTransparent ( bool transp, bool recur );

	// things this will override because lights behave specially:
	virtual void render ( RenderFlags flags = RenderOpaque,
	                      GLenum rendermode = GL_RENDER,
	                      QGLWidget* target = 0 );

private:
	GLfloat position[4];
	GLfloat direction[3];
	GLfloat constantAtten, linearAtten, quadraticAtten;
	bool spotlight;
	GLfloat cutoff;
	GLfloat exponent;
	GLenum whichLight;
	bool valid;

	static QMap<QGLContext*,GLenum> nextLight;
	static GLint numLights;

};


class GLDisplayListWrapper : public GLSceneNode {

public:
	GLDisplayListWrapper ( GLSceneNode* content );

	virtual void displayListRender ();
	virtual bool useDisplayList ();

private:
	GLSceneNode* content;

	void processContent ( GLSceneNode* content );

};


// aims the positive z axis toward the camera before rendering its children,
// iff the camera can be detected from the renderingOn object
class GLTowardCameraWrapper : public GLSceneNode {

public:
	GLTowardCameraWrapper ( GLSceneNode* content );

	virtual void preRender ();

private:
	GLSceneNode* aimer;

};


#endif // GLSCENENODES_H


