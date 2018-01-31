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


#include <qwidget.h>
#include <qimage.h>
#include <qstring.h>
#include <qprinter.h>
#include <q3simplerichtext.h>
#include <q3canvas.h>


class Printing {

public:
	Printing ( QWidget* parent = 0 );

    // in these four functions, the printer has been set up and is being given to us:
	void printImage ( const QImage& image, QPrinter& printer );
	void printRichText ( const QString& text, QPrinter& printer );
    void printCanvas ( const Q3Canvas& canvas, QPrinter& printer );
	static void dispatchPrintImage ( const QImage& image, QPrinter& printer );
	static void dispatchPrintRichText ( const QString& text, QPrinter& printer );
    static void dispatchPrintCanvas ( const Q3Canvas& canvas, QPrinter& printer );

    // in these four functions, we must set up our private printer and use it:
	void printImage ( const QImage& image );
	void printRichText ( const QString& text );
    void printCanvas ( const Q3Canvas& canvas );
	static void dispatchPrintImage ( const QImage& image );
	static void dispatchPrintRichText ( const QString& text );
    static void dispatchPrintCanvas ( const Q3Canvas& canvas );

private:
	QPrinter printer;
	QWidget* parent;
	int oneInch;

	static Printing* firstCreated;
    static Printing* getFirstPrinting ();

	void recomputeOneInch ();
	void printPage ( QPainter *painter, const Q3SimpleRichText &richText,
	                 int pageHeight, int index );

};


