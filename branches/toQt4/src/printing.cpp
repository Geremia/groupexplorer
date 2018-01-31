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
 * Much of the code in here I took from the excellent Blanchette & Summerfield
 * book "C++ GUI Programming with Qt 3."  Thanks, Trolls.
 */


#include "printing.h"
#include <math.h>
#include <qrect.h>
#include <qsize.h>
#include <qpainter.h>
#include <q3paintdevicemetrics.h>
#include <qapplication.h>


Printing* Printing::firstCreated = 0;


Printing::Printing ( QWidget* parent )
    : parent( parent )
{
    if ( !parent && qApp ) parent = qApp->mainWidget();
    if ( !firstCreated ) firstCreated = this;
}

Printing* Printing::getFirstPrinting ()
{
    if ( !firstCreated ) firstCreated = new Printing();
    return firstCreated;
}

void Printing::recomputeOneInch ()
{
    Q3PaintDeviceMetrics metrics( &printer );
    oneInch = metrics.logicalDpiY();
}

void Printing::printImage ( const QImage& image, QPrinter& printer )
{
    recomputeOneInch();

    QPainter painter( &printer );
    QRect rect = painter.viewport();
    QSize size = image.size();
    size.scale( rect.size(), Qt::KeepAspectRatio );

    painter.setViewport( rect.x(), rect.y(),
                         size.width(), size.height() );
    painter.setWindow( image.rect() );

    for ( int i = 0 ; i < (int)printer.numCopies() ; i++ ) {
        if ( i ) printer.newPage();
        painter.drawImage( oneInch, oneInch, image );
    }
}

void Printing::printImage ( const QImage& image )
{
    if ( printer.setup( parent ) ) printImage( image, printer );
}

void Printing::printRichText ( const QString& text, QPrinter& printer )
{
    recomputeOneInch();

    QPainter painter( &printer );
    int pageHeight = painter.window().height() - 2 * oneInch;
    int pageWidth = painter.window().width() - 2 * oneInch;

    Q3SimpleRichText richText( text, QFont( "Helvetica", 14 ),
                              "", 0, 0, pageHeight );
    richText.setWidth( &painter, pageWidth );
    int numPages = (int)ceil( (double)richText.height() / pageHeight );

    for ( int i = 0 ; i < (int)printer.numCopies() ; i++ ) {
        for ( int j = 0 ; j < numPages ; j++ ) {
            if ( i || j ) printer.newPage();
            printPage( &painter, richText, pageHeight,
                       ( printer.pageOrder() == QPrinter::LastPageFirst ) ?
                       numPages - j - 1 : j );
        }
    }
}

void Printing::printRichText ( const QString& text )
{
    if ( printer.setup( parent ) ) printRichText( text, printer );
}

void Printing::printCanvas ( const Q3Canvas& canvas, QPrinter& printer )
{
    recomputeOneInch();

    QPainter painter( &printer );
    QRect rect = painter.viewport();
    QSize size = canvas.size();
    size.scale( rect.size(), Qt::KeepAspectRatio );

    painter.setViewport( rect.x(), rect.y(), size.width(), size.height() );
    painter.setWindow( canvas.rect() );
    //painter.drawRect( painter.window() );
    painter.setClipRect( painter.viewport() );

    // get visible items listed from closest to farthest away
    Q3CanvasItemList items = canvas.collisions( canvas.rect() );

    for ( int i = 0 ; i < (int)printer.numCopies() ; i++ ) {
        if ( i ) printer.newPage();
        // loop from farthest to closest, so occlusion becomes our friend:
        Q3CanvasItemList::const_iterator it = items.end();
        while ( it != items.begin() ) {
            --it;
            (*it)->draw( painter );
        }
    }
}

void Printing::printCanvas ( const Q3Canvas& canvas )
{
    if ( printer.setup( parent ) ) printCanvas( canvas, printer );
}

void Printing::printPage ( QPainter *painter, const Q3SimpleRichText &richText,
                           int pageHeight, int index )
{
    QRect rect( 0, index * pageHeight + oneInch,
                richText.width(), pageHeight );

    // page content
    painter->save();//WorldMatrix();
    painter->translate( 0, -rect.y() );
    richText.draw( painter, oneInch, oneInch, rect,
                   parent ? parent->colorGroup() : QColorGroup() );
    painter->restore();//WorldMatrix();
    // page number
    painter->drawText( painter->window(), Qt::AlignHCenter | Qt::AlignBottom,
                       QString::number( index + 1 ) );
}

void Printing::dispatchPrintImage ( const QImage& image )
{
    getFirstPrinting()->printImage( image );
}

void Printing::dispatchPrintRichText ( const QString& text )
{
    getFirstPrinting()->printRichText( text );
}

void Printing::dispatchPrintCanvas ( const Q3Canvas& canvas )
{
    getFirstPrinting()->printCanvas( canvas );
}

void Printing::dispatchPrintImage ( const QImage& image, QPrinter& printer )
{
    getFirstPrinting()->printImage( image, printer );
}

void Printing::dispatchPrintRichText ( const QString& text, QPrinter& printer )
{
    getFirstPrinting()->printRichText( text, printer );
}

void Printing::dispatchPrintCanvas ( const Q3Canvas& canvas, QPrinter& printer )
{
    getFirstPrinting()->printCanvas( canvas, printer );
}


