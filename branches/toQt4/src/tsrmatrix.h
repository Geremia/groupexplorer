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


#ifndef TSRMATRIX_H
#define TSRMATRIX_H


#include <qgl.h>


class TSRMatrix {

public:
	TSRMatrix ();
	TSRMatrix ( const TSRMatrix& m );
	TSRMatrix ( const GLdouble m[] );

	void reset ();

	void translate ( GLdouble x, GLdouble y, GLdouble z );
	void scale ( GLdouble x, GLdouble y, GLdouble z );
	void rotate ( GLdouble degsX, GLdouble degsY, GLdouble degsZ );
	void rotate ( GLdouble degs, GLdouble axX, GLdouble axY, GLdouble axZ );
	void rotate ( GLdouble thisX, GLdouble thisY, GLdouble thisZ,
	              GLdouble toX, GLdouble toY, GLdouble toZ );

	void loadToGLStack ();
	void multToGLStack ();

	void readFrom ( const TSRMatrix& other );
	void readFrom ( const GLdouble other[] );

	void rightMultiplyBy ( const TSRMatrix& other );
	void rightMultiplyBy ( const GLdouble other[] );
	void leftMultiplyBy ( const TSRMatrix& other );
	void leftMultiplyBy ( const GLdouble other[] );
	void transformVector ( GLdouble inX, GLdouble inY, GLdouble inZ,
	                       GLdouble& outX, GLdouble& outY, GLdouble& outZ );

	TSRMatrix* createInverse ();

	QString debugString ( bool shortversion = TRUE );

private:
	GLdouble matrix[16];
	
};


void crossproduct ( GLdouble u1, GLdouble u2, GLdouble u3,
                    GLdouble v1, GLdouble v2, GLdouble v3,
					GLdouble& ans1, GLdouble& ans2, GLdouble& ans3 );
GLdouble length ( GLdouble x, GLdouble y, GLdouble z );
GLdouble unitize ( GLdouble& x, GLdouble& y, GLdouble& z ); // returns previous length
void perpendicular ( GLdouble inx, GLdouble iny, GLdouble inz, // find any perpendicular
                     GLdouble& outx, GLdouble& outy, GLdouble& outz ); // of same length


#endif // TSRMATRIX_H

