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

/*
 *  Nathan Carter made this class, stealing the general idea and a bunch of the code
 *  from Qt's QSplashScreen class.  The idea is that it's not a window with a pixmap
 *  and some words, but rather just a window with nothing in it.  You should not make
 *  one of these alone and use it for a splash screen!  Rather, you should toss some
 *  stuff inside.  The utility of this is that if you need a more flexible splash
 *  screen than QSplashScreen, this is your starting point, and you can toss a layout
 *  manager inside and pile a few widgets together in whatever way fits your needs.
 *  Like, maybe a QPixmap, a QProgressBar, and a QLabel, vertically stacked.
 */


#ifndef BLANKSPLASHSCREEN_H
#define BLANKSPLASHSCREEN_H


#include <Qt3Support/Q3VBox>


class BlankSplashScreen : public Q3VBox {
    
public:
    BlankSplashScreen( Qt::WindowFlags f = 0 );

    void finish( QWidget *w );
    void repaint();
    
    void centerOnDesktop ();

protected:
    void mousePressEvent( QMouseEvent * );
    
};


#endif // BLANKSPLASHSCREEN_H
