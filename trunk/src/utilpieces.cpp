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


#include "utilpieces.h"
#include <qpainter.h>
//Added by qt3to4:
#include <QPixmap>
#include <QMouseEvent>
#include <QLabel>
#include <QKeyEvent>


ScrollableLabel::ScrollableLabel ( QWidget* parent, const char* name )
    : QLabel( parent, name ), baseMagnitude( 5 )
{
    setFocusPolicy( Qt::StrongFocus );
}

void ScrollableLabel::keyPressEvent ( QKeyEvent* e )
{
    int magnitude = baseMagnitude;
    if ( e->state() & Qt::ShiftButton ) magnitude *= 5;
    switch ( e->key() ) {
        case Qt::Key_Up:        emit scroll( 0, -magnitude );     break;
        case Qt::Key_Down:      emit scroll( 0,  magnitude );     break;
        case Qt::Key_PageUp:    emit scroll( 0, -10*magnitude );  break;
        case Qt::Key_PageDown:  emit scroll( 0,  10*magnitude );  break;
        case Qt::Key_Left:      emit scroll( -magnitude, 0 );     break;
        case Qt::Key_Right:     emit scroll(  magnitude, 0 );     break;
        default:                e->ignore();
    }
}

void ScrollableLabel::setBaseMagnitude ( uint m )
{
    baseMagnitude = m;
}

void ScrollableLabel::mousePressEvent ( QMouseEvent* e )
{
    dragStart = e->pos();
    emit startDrag( dragStart );
}

void ScrollableLabel::mouseReleaseEvent ( QMouseEvent* e )
{
    emit finishDrag( dragStart, e->pos() );
}


RichListBoxItem::RichListBoxItem ( QString text, Q3ListBox* listbox, bool selectable )
    : Q3ListBoxItem( listbox )
{
    setText( text );
    setSelectable( selectable );
    srt = new Q3SimpleRichText( text, QFont() ); // default app font
    //srt->adjustSize();
};

RichListBoxItem::RichListBoxItem ( 
        QString text, Q3ListBox* listbox, Q3ListBoxItem* after, bool selectable )
    : Q3ListBoxItem( listbox, after )
{
    setText( text );
    setSelectable( selectable );
    srt = new Q3SimpleRichText( text, QFont() ); // default app font
    //srt->adjustSize();
};

RichListBoxItem::~RichListBoxItem ()
{
    delete srt;
}

int RichListBoxItem::RTTI = 9876500;
int RichListBoxItem::rtti () const
{
    return RichListBoxItem::RTTI;
}

void RichListBoxItem::setWhatsThisHTML ( QString html ) {
    whatsThis = html;
}

QString RichListBoxItem::getWhatsThisHTML () {
    return whatsThis;
}

int RichListBoxItem::height ( const Q3ListBox* ) const
{
    return srt->height();
}

int RichListBoxItem::width ( const Q3ListBox* ) const
{
    return srt->width/*Used*/();
}

void RichListBoxItem::setTag ( uint t )
{
    tag = t;
}

uint RichListBoxItem::getTag ()
{
    return tag;
}

QPixmap RichListBoxItem::getPixmap ()
{
    QPixmap result( srt->width/*Used*/(), srt->height() );
    QPainter p( &result );
    p.fillRect( QRect( 0, 0, result.width(), result.height() ),
                QBrush( listBox()->colorGroup().color( QColorGroup::Background ),
                        Qt::SolidPattern ) );
    srt->draw( &p, 0, 0, QRect( 0, 0, result.width(), result.height() ),
               listBox()->colorGroup() );
    return result;
}

void RichListBoxItem::paint ( QPainter* p )
{
    srt->draw( p, 0, 0, QRect( 0, 0, width( 0 ), height( 0 ) ),
               listBox()->colorGroup() );
}


/*
RichCustomMenuItem::RichCustomMenuItem ( QString richtext )
    : QCustomMenuItem()
{
    srt = new Q3SimpleRichText( richtext, QFont() ); // default app font
    srt->adjustSize();
};

RichCustomMenuItem::~RichCustomMenuItem ()
{
    delete srt;
}

bool RichCustomMenuItem::fullSpan () {
    return TRUE;
}

void RichCustomMenuItem::paint(
        QPainter* p, const QColorGroup& cg, bool, bool, // activated, enabled, resp.
        int x, int y, int w, int h )
{
    srt->draw( p, x, y, QRect( x, y, w, h ), cg );
}

QSize RichCustomMenuItem::sizeHint()
{
    return QSize( srt->width(), srt->height() );
}
*/


