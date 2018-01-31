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


#include <q3simplerichtext.h>
//Added by qt3to4:
#include <QPixmap>
#include "mathtableitem.h"


#define MAX_GROUP_ORDER_OF_MAGNITUDE 20 // no groups of order >= 10^20, ok?  sorry.


GETableItem::GETableItem ( Q3Table* table, uint origRow, EditType et )
    : Q3TableItem( table, et ), origRow( origRow ), link( QString::null ),
    myKey( QString::null ), srt( 0 )
{
    setReplaceable( FALSE );
}

GETableItem::GETableItem ( Q3Table* table, uint origRow, QString data, EditType et )
    : Q3TableItem( table, et ), origRow( origRow ), link( QString::null ),
    myKey( data ), srt( 0 )
{
    setReplaceable( FALSE );
    setText( data );
}

GETableItem::GETableItem ( Q3Table* table, uint origRow, QPixmap data ) // uneditable
    : Q3TableItem( table, Never ), origRow( origRow ), link( QString::null ),
    myKey( QString::null ), srt( 0 ), html( "[IMAGE]" )
{
    setReplaceable( FALSE );
    setPixmap( data );
}

GETableItem::GETableItem ( Q3Table* table, uint origRow, int data ) // uneditable
    : Q3TableItem( table, Never ), origRow( origRow ), link( QString::null ), srt( 0 )
{
    setReplaceable( FALSE );
    setText( QString::number( data ) );
    myKey = QString::number( data );
    // now we embed numerical ordering inside lexicographic ordering:
    QString tmp;
    tmp.fill( '0', MAX_GROUP_ORDER_OF_MAGNITUDE - myKey.length() );
    myKey = tmp + myKey;
}

GETableItem::GETableItem ( Q3Table* table, uint origRow, MathText data, uint size )//unedtbl
    : Q3TableItem( table, Never ), origRow( origRow ), link( QString::null )
{
    setReplaceable( FALSE );
    
    QFont f;
    if ( size ) f.setPointSize( size );
    srt = new Q3SimpleRichText( "<nobr>" + ( html = data.HTML ) + "</nobr>", f );
    /*
    // make a temporary mathml widget that we'll suck the pixmap from:
    QtMmlWidget* mmlw = new QtMmlWidget();
    mmlw->setEraseColor( table->eraseColor() );
    mmlw->setBackgroundMode( PaletteBase );    
    // now try to set the pixmap for this widget from the math ML given:
    QString errorMessage;
    int errorLine, errorColumn;
    if ( !mmlw->setContent( data.mathML,
                            &errorMessage, &errorLine, &errorColumn ) ) {
        setText( "[MathML error]" );
        qWarning( QString( "MathML Error \"%1\" at line %2, column %3 of:\n%4" )
                .arg( errorMessage ).arg( errorLine ).arg( errorColumn )
                .arg( data.mathML ) );
    } else {
        mmlw->resize( mmlw->sizeHint() + QSize( 6, 6 ) );
        setPixmap( QPixmap::grabWidget( mmlw ) );
    }
    // clean up memory since we're done:
    delete mmlw;
    */
    
    myKey = data.text;
}

GETableItem::~GETableItem ()
{
    if ( srt ) delete srt;
}

void GETableItem::paint ( QPainter * p, const QColorGroup & cg, const QRect & cr,
                          bool selected )
{
    Q3TableItem::paint( p, cg, cr, selected );
    if ( srt ) srt->draw( p, 0, 0, QRect( 0, 0, 2 * srt->width(), srt->height() ), cg );
}

QSize GETableItem::sizeHint () const
{
    return srt ? QSize( srt->width/*Used*/(), srt->height() ) : Q3TableItem::sizeHint();
}

uint GETableItem::originalRow ()
{
    return origRow;
}

void GETableItem::setLink ( QString ref )
{
    link = ref;
}

QString GETableItem::getLink ()
{
    return link;
}

QString GETableItem::key () const
{
    return myKey;
}

QString GETableItem::asHTML ()
{
    return html.isNull() ? text() : html;
}

int GETableItem::RTTI = 102460;
int GETableItem::rtti () const
{
    return RTTI;
}


