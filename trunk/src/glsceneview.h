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


#ifndef GLSCENEVIEW_H
#define GLSCENEVIEW_H


#include <qstring.h> // pulls in QT_VERSION in version-independent way,
                     // as long as QT += qt3support in .pro file

#if QT_VERSION >= 0x040000
#define LIST_TYPE QList
#include <QList>
#else
#define LIST_TYPE QValueList
#include <qvaluelist.h>
#endif

// http://freeglut.sourceforge.net/docs/api.php#Freeglut.h_Header
#ifdef FREEGLUT
#include <GL/freeglut.h>
#else
#include <GL/glut.h>
#endif

#include <qgl.h>
#include "glscenenodes.h"


class GLSceneView : public QGLWidget
{
    Q_OBJECT

public:
    GLSceneView ( QWidget *parent = 0, const char *name = 0 );
    ~GLSceneView ();

	void addChild ( GLSceneNode* ch );
    void addFixedChild ( GLSceneNode* ch );

	void setClearColor ( GLdouble r, GLdouble g, GLdouble b, GLdouble a = 1.0 );
	void setUseLighting ( bool use = TRUE );
	void setAmbientColor ( GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0 );
    void setFogValues ( GLfloat r, GLfloat g, GLfloat b, GLfloat start, GLfloat end );
    void setUseFog ( bool use = TRUE );
    void setLineWidth ( GLdouble w = -1.0 ); // negative numbers mean don't touch it
    void setPointSize ( GLdouble s = -1.0 ); // negative numbers mean don't touch it

	void fetchModelviewMatrix ( GLdouble putItHere[] );
	void fetchProjectionMatrix ( GLdouble putItHere[] );
    QRect viewportRectangle (); 

	void worldToScreen ( GLdouble wx, GLdouble wy, GLdouble wz,
	                     GLdouble& sx, GLdouble& sy, GLdouble& sz );
	void screenToWorld ( GLdouble sx, GLdouble sy, GLdouble sz,
	                     GLdouble& wx, GLdouble& wy, GLdouble& wz );
	void nodeToScreen ( GLSceneNode* n,
	                    GLdouble& sx, GLdouble& sy, GLdouble& sz );

    LIST_TYPE<GLuint> pickClickTags ( GLdouble wx, GLdouble wy,
                                      bool* overflow = 0 );
    LIST_TYPE<GLSceneNode*> pickClick ( GLdouble wx, GLdouble wy,
                                        bool* overflow = 0 );

	void resetScene ();
	void rotateScene ( GLdouble degsX, GLdouble degsY, GLdouble degsZ );
	void rotateScene ( GLdouble degs, GLdouble axX, GLdouble axY, GLdouble axZ );
	void translateScene ( GLdouble x, GLdouble y, GLdouble z );
	void scaleScene ( GLdouble x, GLdouble y, GLdouble z );

	QString debugScene ();

signals:
	void mouseRelease ( int x, int y );
	void doubleClick ( int x, int y );
	void contextMenu ( int x, int y );
    void wheelSpun ( int delta, Qt::Orientation axis );

protected:
    virtual void initializeGL ();
    virtual void resizeGL ( int w, int h );
    virtual void paintGL ();

	QPoint pressedOn ();
	QPoint lastDrag ();

	// descendants may want to reimplement these; right now they
	// just bookkeep {pressedOn,lastDrag}{X,Y}:
	virtual void mousePressEvent ( QMouseEvent* e );
	virtual void mouseReleaseEvent ( QMouseEvent* e );
	virtual void mouseDoubleClickEvent ( QMouseEvent* e );
	virtual void mouseMoveEvent ( QMouseEvent* e );
    virtual void wheelEvent ( QWheelEvent* e );
	// descendants may also want to reimplement
	//   virtual void keyPressEvent ( QKeyEvent* e );
	// and
	//   virtual void keyReleaseEvent ( QKeyEvent* e );
	// but those need do no bookkeeping here, so they aren't declared here

	virtual void setupProjectionMatrix ();
	virtual void setupModelviewMatrix ();

	virtual void contextMenuEvent ( QContextMenuEvent* e );

private:
	GLSceneNode* scene;
    GLSceneNode* fixed;
    GLSceneNode* moving;

	int pressedOnX, pressedOnY;
	int lastDragXDist, lastDragYDist;
	int lastDragXLoc, lastDragYLoc;

	// options for initializeGL():
	// (surely we could have more, but let's not until we need them)
	GLdouble clearColor[4];
	bool useLighting;
	GLfloat ambientColor[4];
    GLfloat fogColor[3];
    GLfloat fogStart;
    GLfloat fogEnd;
    bool useFog;
    GLdouble lineWidth;
    GLdouble pointSize;

	void pickClickToBuf ( GLdouble wx, GLdouble wy, GLuint*& buf, GLint& size );

};


#endif // GLSCENEVIEW_H

