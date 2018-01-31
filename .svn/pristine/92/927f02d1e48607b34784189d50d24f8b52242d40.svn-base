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


#include "tsrmatrix.h"
#include <math.h>


//#define PI 3.1415926535897932384626433832795028841971


void crossproduct ( GLdouble u1, GLdouble u2, GLdouble u3,
                    GLdouble v1, GLdouble v2, GLdouble v3,
					GLdouble& ans1, GLdouble& ans2, GLdouble& ans3 )
{
	ans1 = u2*v3-v2*u3;
	ans2 = v1*u3-u1*v3;
	ans3 = u1*v2-v1*u2;
}

GLdouble length ( GLdouble x, GLdouble y, GLdouble z )
{
    GLdouble result = x * x + y * y + z * z;
    return result ? sqrt( result ) : 0.0;
}

GLdouble unitize ( GLdouble& x, GLdouble& y, GLdouble& z )
{
    GLdouble len = length( x, y, z );
    if ( len != 0.0 ) {
        x /= len;
        y /= len;
        z /= len;
    }
    return len;
}

void perpendicular ( GLdouble inx, GLdouble iny, GLdouble inz,
                     GLdouble& outx, GLdouble& outy, GLdouble& outz )
{
    if ( iny || inz ) {
        crossproduct( inx, iny, inz, 1, 0, 0, outx, outy, outz );
    } else {
        crossproduct( inx, iny, inz, 0, 1, 0, outx, outy, outz );
    }
    unitize( outx, outy, outz );
    GLdouble length = sqrt( inx * inx + iny * iny + inz * inz );
    outx *= length;  outy *= length;  outz *= length;
}

void multiply ( GLdouble x[], const GLdouble y[] )
// multiplies matrix x on the right by matrix y, in place in x
{
	GLdouble copy[16];
	memcpy( copy, x, sizeof(GLdouble)*16 );
	for ( uint i = 0 ; i < 4 ; i++ ) {
		for ( uint j = 0 ; j < 4 ; j++ ) {
			uint idx = i * 4 + j;
			x[idx] = 0.0;
			for ( uint k = 0 ; k < 4 ; k++ ) x[idx] += copy[i*4+k]*y[k*4+j];
		}
	}
}


TSRMatrix::TSRMatrix ()
{
	reset();
}

TSRMatrix::TSRMatrix ( const TSRMatrix& m )
{
	readFrom( m );
}

TSRMatrix::TSRMatrix ( const GLdouble m[] )
{
	readFrom( m );
}

void TSRMatrix::reset ()
{
	for ( uint i = 0 ; i < 16 ; i++ ) matrix[i] = !( i % 5 );
}

void TSRMatrix::readFrom ( const TSRMatrix& m )
{
	readFrom( m.matrix );
}

void TSRMatrix::readFrom( const GLdouble m[] )
{
	memcpy( matrix, m, sizeof(GLdouble)*16 );
}

void TSRMatrix::loadToGLStack ()
{
	GLfloat tmp[16];
	for ( uint i = 0 ; i < 4 ; i++ )
		for ( uint j = 0 ; j < 4 ; j++ )
			tmp[i*4+j] = matrix[j*4+i];  // transpose (OpenGL is funny)
	glLoadMatrixf( tmp );
}

void TSRMatrix::multToGLStack ()
{
	GLfloat tmp[16];
	for ( uint i = 0 ; i < 4 ; i++ )
		for ( uint j = 0 ; j < 4 ; j++ )
			tmp[i*4+j] = matrix[j*4+i];  // transpose (OpenGL is funny)
	glMultMatrixf( tmp );
}

void TSRMatrix::rightMultiplyBy ( const TSRMatrix& other )
{
	rightMultiplyBy( other.matrix );
}

void TSRMatrix::rightMultiplyBy ( const GLdouble other[] )
{
	multiply( matrix, other );
}

void TSRMatrix::leftMultiplyBy ( const TSRMatrix& other )
{
	leftMultiplyBy( other.matrix );
}

void TSRMatrix::leftMultiplyBy ( const GLdouble other[] )
{
	GLdouble tmp[16];
	memcpy( tmp, other, sizeof(GLdouble)*16 );
	multiply( tmp, matrix );
	memcpy( matrix, tmp, sizeof(GLdouble)*16 );
}

void TSRMatrix::transformVector ( GLdouble inX, GLdouble inY, GLdouble inZ,
                                  GLdouble& outX, GLdouble& outY, GLdouble& outZ )
{
	outX = matrix[0]*inX + matrix[1]*inY + matrix[2]*inZ + matrix[3];
	outY = matrix[4]*inX + matrix[5]*inY + matrix[6]*inZ + matrix[7];
	outZ = matrix[8]*inX + matrix[9]*inY + matrix[10]*inZ + matrix[11];
}

void TSRMatrix::translate ( GLdouble x, GLdouble y, GLdouble z )
{
	GLdouble tr[16] = { 1.0, 0.0, 0.0,   x,
	                   0.0, 1.0, 0.0,   y,
					   0.0, 0.0, 1.0,   z,
					   0.0, 0.0, 0.0, 1.0 };
	rightMultiplyBy( tr );
}

void TSRMatrix::scale ( GLdouble x, GLdouble y, GLdouble z )
{
	GLdouble tr[16] = {   x, 0.0, 0.0, 0.0,
	                   0.0,   y, 0.0, 0.0,
					   0.0, 0.0,   z, 0.0,
					   0.0, 0.0, 0.0, 1.0 };
	rightMultiplyBy( tr );
}

void TSRMatrix::rotate ( GLdouble degsX, GLdouble degsY, GLdouble degsZ )
{
	GLdouble sz = sin( degsZ*M_PI/180 ), cz = cos( degsZ*M_PI/180 );
	GLdouble rotz[16] = {  cz, -sz, 0.0, 0.0,
	                      sz,  cz, 0.0, 0.0,
		                 0.0, 0.0, 1.0, 0.0,
		                 0.0, 0.0, 0.0, 1.0 };
	rightMultiplyBy( rotz );
	GLdouble sy = sin( degsY*M_PI/180 ), cy = cos( degsY*M_PI/180 );
	GLdouble roty[16] = {  cy, 0.0,  sy, 0.0,
	                     0.0, 1.0, 0.0, 0.0,
		                 -sy, 0.0,  cy, 0.0,
		                 0.0, 0.0, 0.0, 1.0 };
	rightMultiplyBy( roty );
	GLdouble sx = sin( degsX*M_PI/180 ), cx = cos( degsX*M_PI/180 );
	GLdouble rotx[16] = { 1.0, 0.0, 0.0, 0.0,
	                     0.0,  cx, -sx, 0.0,
		                 0.0,  sx,  cx, 0.0,
		                 0.0, 0.0, 0.0, 1.0 };
	rightMultiplyBy( rotx );
}

void TSRMatrix::rotate ( GLdouble degs, GLdouble axX, GLdouble axY, GLdouble axZ )
{
	GLdouble c = cos(degs*M_PI/180), s = sin(degs*M_PI/180);
	GLdouble xp, yp, zp, length;
	length = sqrt( axX*axX + axY*axY + axZ*axZ );
	xp = length ? axX/length : axX;
	yp = length ? axY/length : axY;
	zp = length ? axZ/length : axZ;
	// Thank you, OpenGL red book (Mason Woo, et al.) for this matrix:
	GLdouble rot[16] =
		{ xp*xp+(1-xp*xp)*c, xp*yp*(1-c)-zp*s,  xp*zp*(1-c)+yp*s,  0.0,
		  yp*xp*(1-c)+zp*s,  yp*yp+(1-yp*yp)*c, yp*zp*(1-c)-xp*s,  0.0,
		  zp*xp*(1-c)-yp*s,  zp*yp*(1-c)+xp*s,  zp*zp+(1-zp*zp)*c, 0.0,
		  0.0,               0.0,               0.0,               1.0  };
	rightMultiplyBy( rot );
}

void TSRMatrix::rotate ( GLdouble thisX, GLdouble thisY, GLdouble thisZ,
                         GLdouble toX, GLdouble toY, GLdouble toZ )
{
	GLdouble orthX, orthY, orthZ;
	crossproduct( thisX, thisY, thisZ, toX, toY, toZ, orthX, orthY, orthZ );
	GLdouble lengthThis, lengthTo, theta;
	lengthThis = sqrt( thisX*thisX + thisY*thisY + thisZ*thisZ );
	lengthTo = sqrt( toX*toX + toY*toY + toZ*toZ );
	theta = acos( ( thisX*toX + thisY*toY + thisZ*toZ )
	            / ( lengthThis * lengthTo ) ) * 180 / M_PI;
	rotate( theta, orthX, orthY, orthZ );
}

uint left ( uint missing, uint index ) {
	uint k[] = { 1, 2, 3, 0, 2, 3, 0, 1, 3, 0, 1, 2 };
	return k[3*missing+index];
}
GLdouble cofactor ( GLdouble m[], uint r1, uint r2, uint c1, uint c2 )
{
	return m[4*r1+c1]*m[4*r2+c2] - m[4*r2+c1]*m[4*r1+c2];
}
GLdouble cofactor ( GLdouble m[], uint r1, uint r2, uint r3,
                                  uint c1, uint c2, uint c3 )
{
	return m[4*r1+c1]*cofactor(m,r2,r3,c2,c3)
	     - m[4*r2+c1]*cofactor(m,r1,r3,c2,c3)
		 + m[4*r3+c1]*cofactor(m,r1,r2,c2,c3);
}
TSRMatrix* TSRMatrix::createInverse ()
{
	TSRMatrix* result = new TSRMatrix();
	for ( uint i = 0 ; i < 4 ; i++ )
		for ( uint j = 0 ; j < 4 ; j++ )
			result->matrix[4 * j + i] =
			  cofactor( matrix, left(i,0), left(i,1), left(i,2),
			                    left(j,0), left(j,1), left(j,2) );
	GLdouble determinant = matrix[0] * result->matrix[0]
                         - matrix[4] * result->matrix[1]
                         + matrix[8] * result->matrix[2]
                         - matrix[12] * result->matrix[3];
	for ( uint i = 0 ; i < 4 ; i++ )
		for ( uint j = 0 ; j < 4 ; j++ )
			if ( ( i + j ) % 2 ) result->matrix[4 * j + i] *= -1;
	if ( determinant )
		for ( uint i = 0 ; i < 16 ; i++ ) result->matrix[i] /= determinant;
	else
		for ( uint i = 0 ; i < 16 ; i++ ) result->matrix[i] = 0;
    /*
    TSRMatrix* test = new TSRMatrix( *this );
    test->rightMultiplyBy( *result );
    for ( uint i = 0 ; i < 16 ; i++ ) {
        double diff = test->matrix[i] - ( ( i % 5 ) ? 0.0 : 1.0 );
        if ( ( diff > 0.1 ) || ( diff < -0.1 ) ) {
            qDebug( QString( "PROBLEM: MATRIX INVERSE FAILED:\n%1\n%2\n%3" )
                    .arg( debugString( FALSE ) )
                    .arg( result->debugString( FALSE ) )
                    .arg( test->debugString( FALSE ) ) );
            break;
        }
    }
    delete test;
    */
	return result;
}

QString TSRMatrix::debugString ( bool shortversion )
{
	QString result;
	for ( uint i = 0 ; i < 16 ; i++ ) {
		result += QString( "%1" ).arg( matrix[i], 7, 'f', 2 );
		if ( !shortversion && ( ( i == 3 ) || ( i == 7 ) || ( i == 11 ) ) )
			result += "\n";
	}
	return result;
}

