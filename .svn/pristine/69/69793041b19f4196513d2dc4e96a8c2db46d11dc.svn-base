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


#ifndef GLSCENENODE_H
#define GLSCENENODE_H


#include <qstring.h> // pulls in QT_VERSION in version-independent way,
                     // as long as QT += qt3support in .pro file

#if QT_VERSION >= 0x040000
#define LIST_TYPE QList
#define PTR_LIST_TYPE Q3PtrList
#include <QList>
#include <Q3PtrList>
#else
#define LIST_TYPE QValueList
#define PTR_LIST_TYPE QPtrList
#include <qvaluelist.h>
#include <qptrlist.h>
#endif


#include <qgl.h>
#include <qobject.h>
#include "tsrmatrix.h"


class GLSceneNode : public TSRMatrix
{

public:
	bool transparent;
	bool hidden;
	GLuint glnametag;
	QString name;
	PTR_LIST_TYPE<GLSceneNode> children;

	typedef uint RenderFlags;
	static RenderFlags RenderTransparent;
	static RenderFlags RenderOpaque;
	static RenderFlags RenderLights;
	static RenderFlags RenderHidden;

	GLSceneNode* parent; // it's public, but don't modify it if you can help it

	GLSceneNode ( GLSceneNode* parent = 0 );
	virtual ~GLSceneNode ();

	virtual void setColor ( GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0 );
	virtual void setDiffuse ( GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0 );
	virtual void setSpecular ( GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0 );
	virtual void setAmbient ( GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0 );
	virtual void setEmissive ( GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0 );
	virtual void setShininess ( GLfloat s );
    /*
	virtual GLfloat* getColor ();
	virtual GLfloat* getDiffuse ();
	virtual GLfloat* getSpecular ();
	virtual GLfloat* getAmbient ();
	virtual GLfloat* getEmissive ();
	virtual GLfloat getShininess ();
    */

	virtual void addChild ( GLSceneNode* child );

	virtual void setTransparent ( bool transp = TRUE, bool recur = TRUE );
	virtual void setHidden ( bool hid = TRUE, bool recur = TRUE );

	void turnOffDisplayList ();
	void markDisplayListDirty ();

	GLuint createUniqueGLNames ( GLuint nextName = 0 ); // returns next avail
	void decodeUniqueGLNames ( GLuint* names, uint length,
	                           LIST_TYPE<GLSceneNode*>& decoded );

	virtual void preRender ();
    virtual void postRender ();
	virtual void renderMe ();
	virtual void render ( RenderFlags flags = RenderOpaque,
	                      GLenum rendermode = GL_RENDER,
	                      QGLWidget* target = 0 );

    void absoluteCameraPosition ( GLdouble& x, GLdouble& y, GLdouble& z );
    void cameraPosition ( GLdouble& x, GLdouble& y, GLdouble& z ); // in local coords
    // get me unit vectors representing screen x (right), screen y (up) and screen z (in),
    // given the center point in absolute world coordinates centerX,centerY,centerZ:
    void absoluteScreenXYZ ( GLdouble centerX, GLdouble centerY, GLdouble centerZ,
                             GLdouble& Xx, GLdouble& Xy, GLdouble& Xz,
                             GLdouble& Yx, GLdouble& Yy, GLdouble& Yz,
                             GLdouble& Zx, GLdouble& Zy, GLdouble& Zz );
    void screenXYZ ( GLdouble centerX, GLdouble centerY, GLdouble centerZ,
                     GLdouble& Xx, GLdouble& Xy, GLdouble& Xz,
                     GLdouble& Yx, GLdouble& Yy, GLdouble& Yz,
                     GLdouble& Zx, GLdouble& Zy, GLdouble& Zz ); // in local coords

	TSRMatrix* composeMatrixStack ();
	void relativeCoordinates ( GLdouble relX, GLdouble relY, GLdouble relZ,
	                           GLdouble& absX, GLdouble& absY, GLdouble& absZ,
							   GLSceneNode* ancestor = 0 );
	void inverseRelativeCoordinates (
		GLdouble absX, GLdouble absY, GLdouble absZ,
	    GLdouble& relX, GLdouble& relY, GLdouble& relZ,
		GLSceneNode* ancestor = 0 );

    void normalizeScaling ();
    // see the .cpp file for documentation on this bad boy

	QString debugString ( uint indent = 0 );
	void debugGLMatrix (); // check matrix on GL stack...is it as expected?

protected:
	QGLWidget* renderingOn;
	QGLWidget* lastRenderedOn;
	GLuint displayList;
	bool neverDisplayList;
	GLint renderMode;

	GLfloat color[4];
	bool useColor;
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat ambient[4];
	GLfloat emissive[4];
	GLfloat shininess;
	bool useMaterial;

	virtual bool useDisplayList ();
	virtual void displayListRender ();

};


class QObjectWatcher : public QObject
{
    Q_OBJECT

public:
    static void watch ( QObject* w );
    static bool isValid ( QObject* w );

public slots:
    void watcheeDestroyed ( QObject* qglwidget );

private:
    PTR_LIST_TYPE<QObject> watchees;
    static QObjectWatcher* instance ();

};


#endif // GLSCENENODE_H


