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


#include "geml.h"
#include "textinputdialog.h"
#include "gesheet.h"
#include "gevisualizer.h"
#include "gevlarge.h"
#include <qmatrix.h>
#include <qpainter.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <q3dragobject.h>
#include <qcursor.h>
#include <qfile.h>
#include <qregexp.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <QContextMenuEvent>
#include <QPixmap>
#include <QMouseEvent>
#include <Q3TextStream>
#include <QKeyEvent>
#include <Q3PointArray>
#include <Q3PtrList>
#include <math.h>


void pinkMe ( Q3CanvasItem* me, QPainter& painter )
{
    // highlight me with a magenta outline iff I'm selected
    if ( me->isSelected() ) {
        painter.setPen( QPen( Qt::magenta, 1 ) );
        painter.setBrush( Qt::NoBrush );
        if ( me->rtti() == Q3CanvasItem::Rtti_Line ) {
            Q3PointArray apts = ( ( GECanvasLine* )me )->areaPoints();
            for ( int i = 1 ; i < apts.count() ; i++ )
                painter.drawLine( apts[i-1], apts[i] );
            painter.drawLine( apts[apts.count() - 1], apts[0] );
        } else if ( me->rtti() == GECanvasMorphism::RTTI ) {
            Q3PointArray apts = ( ( GECanvasMorphism* )me )->areaPoints();
            for ( int i = 1 ; i < apts.count() ; i++ )
                painter.drawLine( apts[i-1], apts[i] );
            painter.drawLine( apts[apts.count() - 1], apts[0] );
        } else {
            QRect rect = me->boundingRect();
            painter.drawRect( rect.left(), rect.top(), rect.width(), rect.height() );
        }
    }
}


CanvasLayoutGrid::CanvasLayoutGrid ()
    : Q3CanvasRectangle( 0 ), hpadding( 0 ), vpadding( 0 ), hmargin( 0 ), vmargin( 0 ),
      numrows( 0 ), numcols( 0 )
{
}

CanvasLayoutGrid::~CanvasLayoutGrid ()
{
    // safely just deletes the invisible structures, not the content;
    // thus it is save to call placeOnCanvas(), then destroy this guy.
    for ( uint i = 0 ; i < gridEntries.size() ; i++ )
        if ( gridEntries[i] && ( gridEntries[i]->rtti() == CanvasLayoutGrid::RTTI ) )
            delete gridEntries[i];
}

void CanvasLayoutGrid::resize ( int rows, int cols )
{
    int cursize = gridEntries.size();
    if ( rows * cols < cursize )
        for ( int i = rows * cols ; i < cursize ; i++ ) delete gridEntries[i];
    gridEntries.resize( rows * cols );
    for ( int i = cursize ; i < rows * cols ; i++ ) gridEntries[i] = 0;
    numrows = rows;
    numcols = cols;
}

void CanvasLayoutGrid::insert ( int row, int col, Q3CanvasItem* item )
{
    int index = row * numcols + col;
    if ( index < (int)(gridEntries.size()) ) {
        if ( gridEntries[index] ) delete gridEntries[index];
        gridEntries[index] = item;
    }
}

void CanvasLayoutGrid::erase ( int row, int col )
{
    int index = row * numcols + col;
    if ( index < (int)(gridEntries.size()) ) {
        if ( gridEntries[index] ) delete gridEntries[index];
        gridEntries[index] = 0;
    }
}

void CanvasLayoutGrid::setMargins ( int h, int v )
{
    hmargin = h;
    vmargin = v;
}

void CanvasLayoutGrid::setPadding ( int h, int v )
{
    hpadding = h;
    vpadding = v;
}

int CanvasLayoutGrid::RTTI = 147092;
int CanvasLayoutGrid::rtti () const
{
    return CanvasLayoutGrid::RTTI;
}

QSize CanvasLayoutGrid::size ()
{
    int* widths = new int[numcols];
    int* heights = new int[numrows];
    for ( int i = 0 ; i < numrows ; i++ ) {
        heights[i] = 0;
        for ( int j = 0 ; j < numcols ; j++ ) {
            if ( !i ) widths[j] = 0;
            int index = numcols * i + j;
            if ( gridEntries[index] ) {
                int w;
                int h;
                if ( gridEntries[index]->rtti() == CanvasLayoutGrid::RTTI ) {
                    QSize s = ( ( CanvasLayoutGrid* )gridEntries[index] )->size();
                    w = s.width();
                    h = s.height();
                } else {
                    QRect r = gridEntries[index]->boundingRect();
                    w = r.width();
                    h = r.height();
                }
                if ( w > widths[j] ) widths[j] = w;
                if ( h > heights[i] ) heights[i] = h;
            }
        }
    }
    for ( int i = 1 ; i < numcols ; i++ ) widths[0] += widths[i];
    for ( int i = 1 ; i < numrows ; i++ ) heights[0] += heights[i];
    QSize result = QSize( widths[0] + 2 * hmargin + ( numcols - 1 ) * hpadding,
                          heights[0] + 2 * vmargin + ( numrows - 1 ) * vpadding );
    delete widths;
    delete heights;
    return result;
}



void CanvasLayoutGrid::placeOnCanvas ( Q3Canvas* canvas, int x, int y, int z )
{
    int* widths = new int[numcols];
    int* heights = new int[numrows];
    for ( int i = 0 ; i < numrows ; i++ ) {
        heights[i] = 0;
        for ( int j = 0 ; j < numcols ; j++ ) {
            if ( !i ) widths[j] = 0;
            int index = numcols * i + j;
            if ( gridEntries[index] ) {
                int w;
                int h;
                if ( gridEntries[index]->rtti() == CanvasLayoutGrid::RTTI ) {
                    QSize s = ( ( CanvasLayoutGrid* )gridEntries[index] )->size();
                    w = s.width();
                    h = s.height();
                } else {
                    QRect r = gridEntries[index]->boundingRect();
                    w = r.width();
                    h = r.height();
                }
                if ( w > widths[j] ) widths[j] = w;
                if ( h > heights[i] ) heights[i] = h;
            }
        }
    }
    int top = y + vmargin;
    for ( int i = 0 ; i < numrows ; i++ ) {
        int left = x + hmargin;
        if ( i ) top += vpadding + heights[i - 1];
        for ( int j = 0 ; j < numcols ; j++ ) {
            if ( j ) left += hpadding + widths[j - 1];
            int index = numcols * i + j;
            if ( gridEntries[index] ) {
                int w;
                int h;
                if ( gridEntries[index]->rtti() == CanvasLayoutGrid::RTTI ) {
                    QSize s = ( ( CanvasLayoutGrid* )gridEntries[index] )->size();
                    w = s.width();
                    h = s.height();
                } else {
                    QRect r = gridEntries[index]->boundingRect();
                    w = r.width();
                    h = r.height();
                }
                if ( gridEntries[index]->rtti() == CanvasLayoutGrid::RTTI ) {
                    ( ( CanvasLayoutGrid* )gridEntries[index] )->placeOnCanvas( canvas,
                        left + ( widths[j] - w ) / 2, top + ( heights[i] - h ) / 2, z );
                } else {
                    gridEntries[index]->setCanvas( canvas );
                    gridEntries[index]->setX( left + ( widths[j] - w ) / 2 );
                    gridEntries[index]->setY( top + ( heights[i] - h ) / 2 );
                    gridEntries[index]->setZ( z );
                    gridEntries[index]->show();
                }
            }
        }
    }
    delete widths;
    delete heights;
}


GECanvasText::GECanvasText ( Q3Canvas* canvas )
    : Q3CanvasText( canvas )
{
    setSelected( FALSE );
}

void GECanvasText::draw ( QPainter& painter )
{
    Q3CanvasText::draw( painter );
    pinkMe( this, painter );
}

void GECanvasText::appendXMLTo ( QDomNode n )
{
    QDomElement e = n.ownerDocument().createElement( "text" );
    append_string_node_to( "content", text(), e );
    append_string_node_to( "x", QString::number( x() ), e );
    append_string_node_to( "y", QString::number( y() ), e );
    append_string_node_to( "z", QString::number( z() ), e );
    append_string_node_to( "font", QString::number( font().pointSize() ), e );
    n.appendChild( e );
}

void GECanvasText::makeFromXML ( QDomNode n )
{
    for ( QDomNode ch = n.firstChild() ; !ch.isNull() ; ch = ch.nextSibling() ) {
        QString tmp;
        if ( ch.nodeName() == "content" ) {
            if ( !try_get_string( tmp, ch, "content", ".+" ) ) tmp = "CONTENT-ERROR";
            setText( tmp );
        } else if ( ch.nodeName() == "x" ) {
            if ( !try_get_string( tmp, ch, "x", "\\d+" ) ) tmp = "0";
            setX( tmp.toInt() );
        } else if ( ch.nodeName() == "y" ) {
            if ( !try_get_string( tmp, ch, "y", "\\d+" ) ) tmp = "0";
            setY( tmp.toInt() );
        } else if ( ch.nodeName() == "z" ) {
            if ( !try_get_string( tmp, ch, "z", "\\d+" ) ) tmp = "0";
            setZ( tmp.toInt() );
        } else if ( ch.nodeName() == "font" ) {
            if ( !try_get_string( tmp, ch, "font", "\\d+" ) ) tmp = "0";
            QFont f = font();
            f.setPointSize( tmp.toInt() );
            setFont( f );
        }
    }
}


GECanvasRect::GECanvasRect ( Q3Canvas* canvas )
    : Q3CanvasRectangle( canvas )
{
    setSelected( FALSE );
    setPen( QColor( 100, 100, 100, QColor::Rgb ) );
    setBrush( QColor( 200, 200, 200, QColor::Rgb ) );
}

void GECanvasRect::draw ( QPainter& painter )
{
    Q3CanvasRectangle::draw( painter );
    pinkMe( this, painter );
}

void GECanvasRect::appendXMLTo ( QDomNode n )
{
    QDomElement e = n.ownerDocument().createElement( "rect" );
    append_string_node_to( "x", QString::number( x() ), e );
    append_string_node_to( "y", QString::number( y() ), e );
    append_string_node_to( "z", QString::number( z() ), e );
    append_string_node_to( "w", QString::number( width() ), e );
    append_string_node_to( "h", QString::number( height() ), e );
    n.appendChild( e );
}

void GECanvasRect::makeFromXML ( QDomNode n )
{
    int w = -1;
    int h = -1;
    for ( QDomNode ch = n.firstChild() ; !ch.isNull() ; ch = ch.nextSibling() ) {
        QString tmp;
        if ( ch.nodeName() == "x" ) {
            if ( !try_get_string( tmp, ch, "x", "\\d+" ) ) tmp = "0";
            setX( tmp.toInt() );
        } else if ( ch.nodeName() == "y" ) {
            if ( !try_get_string( tmp, ch, "y", "\\d+" ) ) tmp = "0";
            setY( tmp.toInt() );
        } else if ( ch.nodeName() == "z" ) {
            if ( !try_get_string( tmp, ch, "z", "\\d+" ) ) tmp = "0";
            setZ( tmp.toInt() );
        } else if ( ch.nodeName() == "w" ) {
            if ( !try_get_string( tmp, ch, "w", "\\d+" ) ) tmp = "0";
            w = tmp.toInt();
            if ( ( w > -1 ) && ( h > -1 ) ) setSize( w, h );
        } else if ( ch.nodeName() == "h" ) {
            if ( !try_get_string( tmp, ch, "h", "\\d+" ) ) tmp = "0";
            h = tmp.toInt();
            if ( ( w > -1 ) && ( h > -1 ) ) setSize( w, h );
        }
    }
}


GECanvasLine::GECanvasLine ( Q3Canvas* canvas )
    : Q3CanvasLine( canvas ), from( 0 ), to( 0 )
{
    setSelected( FALSE );
}

void GECanvasLine::setFromItem ( Q3CanvasItem* item )
{
    from = item;
    update();
}

void GECanvasLine::setToItem ( Q3CanvasItem* item )
{
    to = item;
    update();
}

Q3CanvasItem* GECanvasLine::getFromItem ()
{
    return from;
}

Q3CanvasItem* GECanvasLine::getToItem ()
{
    return to;
}

void GECanvasLine::appendXMLTo ( QDomNode n, Q3CanvasItemList everyone )
{
    QDomElement e = n.ownerDocument().createElement( "line" );
    int fromIdx = -1;
    int toIdx = -1;
    if ( from || to ) {
        fromIdx = everyone.findIndex( from );
        if ( fromIdx < 0 ) return; // cut/copied this guy without source endpoint
        toIdx = everyone.findIndex( to );
        if ( toIdx < 0 ) return; // cut/copied this guy without destination endpoint
    }
    //if ( fromIdx > -1 ) {
        append_string_node_to( "from", QString::number( fromIdx ), e );
    //} else {
    //    append_string_node_to( "x1", QString::number( startPoint().x() ), e );
    //    append_string_node_to( "y1", QString::number( startPoint().y() ), e );
    //}
    //if ( toIdx > -1 ) {
        append_string_node_to( "to", QString::number( toIdx ), e );
    //} else {
    //    append_string_node_to( "x2", QString::number( endPoint().x() ), e );
    //    append_string_node_to( "y2", QString::number( endPoint().y() ), e );
    //}
    if ( !from && !to ) append_string_node_to( "z", QString::number( z() ), e );
    n.appendChild( e );
}

void GECanvasLine::makeFromXML ( QDomNode n, Q3CanvasItemList allsofar )
{
    int x1 = -1;
    int x2 = -1;
    int y1 = -1;
    int y2 = -1;
    from = to = 0;
    for ( QDomNode ch = n.firstChild() ; !ch.isNull() ; ch = ch.nextSibling() ) {
        QString tmp;
        if ( ch.nodeName() == "x1" ) {
            if ( !try_get_string( tmp, ch, "x1", "\\d+" ) ) tmp = "0";
            x1 = tmp.toInt();
        } else if ( ch.nodeName() == "y1" ) {
            if ( !try_get_string( tmp, ch, "y1", "\\d+" ) ) tmp = "0";
            y1 = tmp.toInt();
        } else if ( ch.nodeName() == "x2" ) {
            if ( !try_get_string( tmp, ch, "x2", "\\d+" ) ) tmp = "0";
            x2 = tmp.toInt();
        } else if ( ch.nodeName() == "y2" ) {
            if ( !try_get_string( tmp, ch, "y2", "\\d+" ) ) tmp = "0";
            y2 = tmp.toInt();
        } else if ( ch.nodeName() == "z" ) {
            if ( !try_get_string( tmp, ch, "z", "\\d+" ) ) tmp = "0";
            setZ( tmp.toInt() );
        } else if ( ch.nodeName() == "from" ) {
            if ( !try_get_string( tmp, ch, "from", "\\d+" ) ) tmp = "0";
            if ( tmp.toInt() < allsofar.count() ) setFromItem( allsofar[tmp.toUInt()] );
        } else if ( ch.nodeName() == "to" ) {
            if ( !try_get_string( tmp, ch, "to", "\\d+" ) ) tmp = "0";
            if ( tmp.toInt() < allsofar.count() ) setToItem( allsofar[tmp.toUInt()] );
        }
    }
    setPoints( x1, y1, x2, y2 ); // gets fixed (along with z values) in draw(), below
}


const int GECanvasLine::margin = 10;
void GECanvasLine::recomputeLocalVectors ( QPoint& dir, QPoint& perp ) const
{
    dir = endPoint() - startPoint();
    if ( dir.x() || dir.y() ) {
        dir = ( dir * margin ) / int( sqrt( dir.x()*dir.x() + dir.y()*dir.y() ) );
    } else {
        dir.setX( 0 );  dir.setY( margin );
    }
    perp = QPoint( -dir.y(), dir.x() );
}
void gecanvasline_draw_test ( QPoint try1, QPoint try2, QPoint& cur1, QPoint& cur2 )
{
    QPoint d1 = try1 - try2;
    QPoint d2 = cur1 - cur2;
    if ( d1.x()*d1.x() + d1.y()*d1.y() < d2.x()*d2.x() + d2.y()*d2.y() ) {
        cur1 = try1;
        cur2 = try2;
    }
}
QPoint gecanvasline_draw_point ( QRect r, int i )
{
    switch ( i ) {
        case 0:  return QPoint( r.left(), ( r.top() + r.bottom() ) / 2 );
        case 1:  return QPoint( r.right(), ( r.top() + r.bottom() ) / 2 );
        case 2:  return QPoint( ( r.left() + r.right() ) / 2, r.top() );
        default: return QPoint( ( r.left() + r.right() ) / 2, r.bottom() );
    }
}
void GECanvasLine::prepareToDraw ()
{
    QPoint s = startPoint();
    QPoint e = endPoint();
    if ( from ) {
        QRect frect = from->boundingRect();
        s = gecanvasline_draw_point( frect, 0 );
        if ( z() <= from->z() ) setZ( from->z() + 0.1 );
        if ( to ) {
            QRect trect = to->boundingRect();
            e = gecanvasline_draw_point( trect, 0 );
            if ( z() <= to->z() ) setZ( to->z() + 0.1 );
            for ( int i = 0 ; i < 4 ; i++ ) for ( int j = 0 ; j < 4 ; j++ )
                gecanvasline_draw_test( gecanvasline_draw_point( frect, i ),
                                        gecanvasline_draw_point( trect, j ), s, e );
        }
        for ( int i = 0 ; i < 4 ; i++ )
            gecanvasline_draw_test( gecanvasline_draw_point( frect, i ), e, s, e );
    }
    if ( to ) {
        QRect trect = to->boundingRect();
        if ( z() < to->z() ) setZ( to->z() );
        for ( int j = 0 ; j < 4 ; j++ )
            gecanvasline_draw_test( s, gecanvasline_draw_point( trect, j ), s, e );
    }
    setPoints( s.x(), s.y(), e.x(), e.y() );
}
void GECanvasLine::draw ( QPainter& painter )
{
    prepareToDraw();
    Q3CanvasLine::draw( painter );
    pinkMe( this, painter );
}

Q3PointArray GECanvasLine::areaPoints () const
{
    QPoint dir;
    QPoint perp;
    recomputeLocalVectors( dir, perp );
    Q3PointArray result( 4 );
    result[0] = startPoint() + perp - dir;
    result[1] = endPoint() + perp + dir;
    result[2] = endPoint() - perp + dir;
    result[3] = startPoint() - perp - dir;
    return result;
}


GECanvasMorphism::GECanvasMorphism ( Q3Canvas* canvas )
    : GECanvasLine( canvas ),
      showName( TRUE ), showDomCod( FALSE ), showEpiMon( FALSE ), showGens( FALSE ),
      manyLines( FALSE ), label( 0 )
{
}

GECanvasMorphism::~GECanvasMorphism ()
{
    if ( label ) delete label;
}

void GECanvasMorphism::setFromSmall ( GEVSmall* smallv )
{
    setFromItem( smallv );
    morphism.setDomain( smallv->getGroupFile() );
}

void GECanvasMorphism::setToSmall ( GEVSmall* smallv )
{
    setToItem( smallv );
    morphism.setCodomain( smallv->getGroupFile() );
}

void GECanvasMorphism::push ( GroupSubset* S )
{
    if ( to ) {
        GEltList image = morphism.pushSubset( S->elements() );
        QString desc = QString( "The subset %1 of %2 was pushed through "
                                "the morphism %3 to yield a subset of %4.  "
                                "This is often written %5[%6]." )
                       .arg( S->getName() )
                       .arg( morphism.getDomainName() )
                       .arg( morphism.getName() )
                       .arg( morphism.getCodomainName() )
                       .arg( morphism.getName() )
                       .arg( S->getName() );
        toSmall()->invokeSuggest( image, desc );
    }
}

void GECanvasMorphism::pull ( GroupSubset* S )
{
    if ( from ) {
        GEltList invimage = morphism.pullSubset( S->elements() );
        QString desc = QString( "The subset %1 of %2 was pulled backwards through "
                                "the morphism %3 to yield a subset of %4.  "
                                "This is often written %5<sup>-1</sup>[%6]." )
                       .arg( S->getName() )
                       .arg( morphism.getCodomainName() )
                       .arg( morphism.getName() )
                       .arg( morphism.getDomainName() )
                       .arg( morphism.getName() )
                       .arg( S->getName() );
        fromSmall()->invokeSuggest( invimage, desc );
    }
}

int GECanvasMorphism::RTTI = 987987;
int GECanvasMorphism::rtti () const
{
    return GECanvasMorphism::RTTI;
}

GEltList GECanvasMorphism::tryEmbedding ( GroupFile* G1, GroupFile* G2, GEltList H2,
                                          GEltList domgens, GEltList somecodgens,
                                          GECanvasMorphism* m )
{
    // tries to generate a map domgens[i] |-> codgens[i], where somecodgens is an initial
    // segment of codgens, and where <domgens> < G1, <codgens> < G2, and where H2 < G2
    // and we want the whole codomain of the map to lie inside H2.
    if ( somecodgens.size() == domgens.size() ) {
        // potenial homomorphism-generating lists: test them
        return m->buildMap( domgens, somecodgens );
    } else {
        // only have a partial list...much search for valid extensions:
        GEltList img = G2->group.subgroupGenBy( somecodgens );
        uint ord = G1->group.orderOf( domgens[somecodgens.size()] );
        GEltList trial = somecodgens.copy();
        trial.resize( trial.size() + 1 );
        for ( uint i = 0 ; i < H2.size() ; i++ ) {
            // since we're looking for monomorphisms only, orders must be equal:
            if ( !img.contains( H2[i] ) && ( G2->group.orderOf( H2[i] ) == ord ) ) {
                trial[somecodgens.size()] = H2[i];
                GEltList maybeSuccess = tryEmbedding( G1, G2, H2, domgens, trial, m );
                if ( maybeSuccess.size() ) return maybeSuccess; // success
            }
        }
        return GEltList(); // failure
    }
}
GEltList GECanvasMorphism::tryEmbedding ( GroupFile* G1, GroupFile* G2, GEltList H2 )
{
    GECanvasMorphism* m = new GECanvasMorphism( 0 );
    m->getMorphism()->setDomain( G1 );
    m->getMorphism()->setCodomain( G2 );
    GEltList gens = G1->group.findGens();
    GEltList result = tryEmbedding( G1, G2, H2, gens, GEltList(), m );
    return result;
}
GEltList GECanvasMorphism::tryEmbedding ( GroupFile* G1, GroupFile* G2 )
{
    return tryEmbedding( G1, G2, iota( G2->group.order ) );
}

GroupMorphism* GECanvasMorphism::getMorphism ()
{
    return &morphism;
}

GEltList GECanvasMorphism::buildMap ( GEltList dgens, GEltList cgens )
{
    // don't bother with empty gens--makes trivial map
    if ( !dgens.size() ) {
        GEltList result( morphism.getDomain()->group.order );
        result.fill( 0 );
        return result;
    }
    // ok now that we have some generators, let's generate the homomorphism by generating
    // the domain and codomain in lock step:
    // (this is just like Group::subgroupGenBy(), with two additional lines in inner loop)
    GEltList gendomain = dgens.copy();
    GEltList gencodomain = cgens.copy();
    Group* dg = &( morphism.getDomain()->group );
    Group* cg = &( morphism.getCodomain()->group );
    for ( uint i = 0 ; i < gendomain.size() ; i++ ) {
        for ( uint j = 0 ; j < dgens.size() ; j++ ) {
            GElt tryme = dg->op( gendomain[i], gendomain[j] );
            if ( !gendomain.contains( tryme ) ) {
                gendomain.resize( gendomain.size() + 1 );
                gendomain[gendomain.size() - 1] = tryme;
                gencodomain.resize( gencodomain.size() + 1 );
                gencodomain[gencodomain.size() - 1] =
                    cg->op( gencodomain[i], gencodomain[j] );
            }
        }
    }
    // now make the resulting map (0's everywhere outside gendomain):
    GEltList result( dg->order );
    result.fill( 0 );
    for ( uint i = 0 ; i < gendomain.size() ; i++ ) result[gendomain[i]] = gencodomain[i];
    // now test it to be sure it's a homomorphism:
    for ( uint i = 0 ; i < gendomain.size() ; i++ )
        for ( uint j = 0 ; j < gendomain.size() ; j++ )
            if ( result[dg->op( gendomain[i], gendomain[j] )] !=
                 cg->op( result[gendomain[i]], result[gendomain[j]] ) ) {
                /*
                QString bleh;
                for ( uint k = 0 ; k < dgens.size() ; k++ )
                    bleh += QString( "<%1,%2> " )
                            .arg( morphism.getDomain()->getRepresentation( dgens[k] ).text )
                            .arg( morphism.getCodomain()->getRepresentation( cgens[k] ).text );
                qDebug( "{ " + bleh + "} " +
                        QString( "is not a homomorphism because "
                                 "f(%1 %2) = f(%3) = %4 != %5 = %6 %7 = f(%8)f(%9)" )
                        .arg( morphism.getDomain()->getRepresentation( gendomain[i] ).text )
                        .arg( morphism.getDomain()->getRepresentation( gendomain[j] ).text )
                        .arg( morphism.getDomain()->getRepresentation( dg->op( gendomain[i], gendomain[j] ) ).text )
                        .arg( morphism.getCodomain()->getRepresentation( result[dg->op( gendomain[i], gendomain[j] )] ).text )
                        .arg( morphism.getCodomain()->getRepresentation( cg->op( result[gendomain[i]], result[gendomain[j]] ) ).text )
                        .arg( morphism.getCodomain()->getRepresentation( result[gendomain[i]] ).text )
                        .arg( morphism.getCodomain()->getRepresentation( result[gendomain[j]] ).text )
                        .arg( morphism.getDomain()->getRepresentation( gendomain[i] ).text )
                        .arg( morphism.getDomain()->getRepresentation( gendomain[j] ).text ) );
                */
                return GEltList(); // empty list means no homomorphism could be generated
            }
    return result; // this is success--we built a valid homomorphism
}

void GECanvasMorphism::appendXMLTo ( QDomNode n, Q3CanvasItemList everyone )
{
    QDomElement e = n.ownerDocument().createElement( "morp" );
    append_string_node_to( "name", morphism.getName(), e );
    append_string_node_to( "domg", elt_list_to_code( domgens ), e );
    append_string_node_to( "codg", elt_list_to_code( codgens ), e );
    //append_string_node_to( "map", morphism.encodeMap(), e );
    int fromIdx = -1;
    int toIdx = -1;
    if ( from || to ) {
        fromIdx = everyone.findIndex( from );
        if ( fromIdx < 0 ) return; // cut/copied this guy without source endpoint
        toIdx = everyone.findIndex( to );
        if ( toIdx < 0 ) return; // cut/copied this guy without destination endpoint
    }
    //if ( fromIdx > -1 ) {
        append_string_node_to( "from", QString::number( fromIdx ), e );
    //} else {
    //    append_string_node_to( "x1", QString::number( startPoint().x() ), e );
    //    append_string_node_to( "y1", QString::number( startPoint().y() ), e );
    //}
    //if ( toIdx > -1 ) {
        append_string_node_to( "to", QString::number( toIdx ), e );
    //} else {
    //    append_string_node_to( "x2", QString::number( endPoint().x() ), e );
    //    append_string_node_to( "y2", QString::number( endPoint().y() ), e );
    //}
    if ( !from && !to ) append_string_node_to( "z", QString::number( z() ), e );
    append_string_node_to( "opts", QString( "%1%2%3%4%5" )
                                   .arg( showName ? '1' : '0' )
                                   .arg( showDomCod ? '1' : '0' )
                                   .arg( showEpiMon ? '1' : '0' )
                                   .arg( showGens ? '1' : '0' )
                                   .arg( manyLines ? '1' : '0' ), e );
    n.appendChild( e );
}

void GECanvasMorphism::makeFromXML ( QDomNode n, Q3CanvasItemList allsofar )
{
    int x1 = -1;
    int x2 = -1;
    int y1 = -1;
    int y2 = -1;
    from = to = 0;
    for ( QDomNode ch = n.firstChild() ; !ch.isNull() ; ch = ch.nextSibling() ) {
        QString tmp;
        if ( ch.nodeName() == "x1" ) {
            if ( !try_get_string( tmp, ch, "x1", "\\d+" ) ) tmp = "0";
            x1 = tmp.toInt();
        } else if ( ch.nodeName() == "y1" ) {
            if ( !try_get_string( tmp, ch, "y1", "\\d+" ) ) tmp = "0";
            y1 = tmp.toInt();
        } else if ( ch.nodeName() == "x2" ) {
            if ( !try_get_string( tmp, ch, "x2", "\\d+" ) ) tmp = "0";
            x2 = tmp.toInt();
        } else if ( ch.nodeName() == "y2" ) {
            if ( !try_get_string( tmp, ch, "y2", "\\d+" ) ) tmp = "0";
            y2 = tmp.toInt();
        } else if ( ch.nodeName() == "z" ) {
            if ( !try_get_string( tmp, ch, "z", "\\d+" ) ) tmp = "0";
            setZ( tmp.toInt() );
        } else if ( ch.nodeName() == "from" ) {
            if ( !try_get_string( tmp, ch, "from", "\\d+" ) ) tmp = "0";
            if ( tmp.toInt() < allsofar.count() )
                setFromSmall( ( GEVSmall* )( allsofar[tmp.toUInt()] ) );
        } else if ( ch.nodeName() == "to" ) {
            if ( !try_get_string( tmp, ch, "to", "\\d+" ) ) tmp = "0";
            if ( tmp.toInt() < allsofar.count() )
                setToSmall( ( GEVSmall* )( allsofar[tmp.toUInt()] ) );
        /*
        } else if ( ch.nodeName() == "map" ) {
            if ( !try_get_string( tmp, ch, "map", "(?:(?:\\d+-)*\\d+)?" ) ) tmp = "";
            morphism.decodeMap( tmp );
        */
        } else if ( ch.nodeName() == "domg" ) {
            if ( !try_get_string( tmp, ch, "domg", "(?:(?:\\d+-)*\\d+)?" ) ) tmp = "";
            domgens = code_to_elt_list( tmp );
        } else if ( ch.nodeName() == "codg" ) {
            if ( !try_get_string( tmp, ch, "codg", "(?:(?:\\d+-)*\\d+)?" ) ) tmp = "";
            codgens = code_to_elt_list( tmp );
        } else if ( ch.nodeName() == "name" ) {
            if ( !try_get_string( tmp, ch, "name", ".+" ) ) tmp = "UNNAMED";
            morphism.setName( tmp );
        } else if ( ch.nodeName() == "opts" ) {
            if ( !try_get_string( tmp, ch, "opts", "[01]{5}" ) ) tmp = "10000";
            showName = ( tmp[0] == '1' );
            showDomCod = ( tmp[1] == '1' );
            showEpiMon = ( tmp[2] == '1' );
            showGens = ( tmp[3] == '1' );
            manyLines = ( tmp[4] == '1' );
        }
    }
    setPoints( x1, y1, x2, y2 ); // gets fixed (along with z values) in draw(), below
    morphism.setMap( buildMap( domgens, codgens ) );
    updateLabel();
}

void GECanvasMorphism::updateLabel ()
{
    if ( label ) delete label;
    label = 0;
    if ( !from || !to ) return; // no label if missing an end

    QStringList list;
    if ( showName ) {
        QString tmp = morphism.getName();
        if ( showDomCod ) {
            tmp += " : " + morphism.getDomainName() + " -> " + morphism.getCodomainName();
        }
        list << tmp;
    }
    if ( showEpiMon ) {
        QString tmp;
        if ( morphism.kernel().size() == 1 ) tmp += "1-1";
        if ( morphism.image().size() == morphism.getCodomain()->group.order ) {
            if ( tmp.length() ) tmp += ", ";
            tmp += "onto";
        }
        if ( tmp.length() ) list << tmp;
    }
    if ( showGens ) {
        for ( uint i = 0 ; i < domgens.size() ; i++ ) {
            list << QString( "%1 ( %2 ) = %3 " )
                    .arg( morphism.getName() )
                    .arg( morphism.getDomain()->getRepresentation( domgens[i] ).HTML )
                    .arg( morphism.getCodomain()->getRepresentation( codgens[i] ).HTML );
        }
        if ( !domgens.size() )
            list << ( "all map to " +
                      morphism.getCodomain()->getRepresentation( 0 ).HTML );
    }

    if ( list.count() )
        label = new Q3SimpleRichText(
            "<center>\n<nobr>" + list.join( "</nobr><br>\n<nobr>" ) + "</nobr>\n</center>",
            QFont() );
}

void GECanvasMorphism::drawArrow ( QPainter& painter, QPoint from, QPoint to )
{
    painter.setPen( Qt::black );
    QPoint dir = to - from;
    double dirlen = 0;
    if ( dir.x() || dir.y() ) {
        dirlen = sqrt( dir.x() * dir.x() + dir.y() * dir.y() );
        dir = dir * margin / dirlen;
    }
    QPoint perp( -dir.y(), dir.x() );
    double shyness = QMIN( dirlen ? ( margin / dirlen ) : 0, 0.3 );
    QPoint shyfrom = ( 1 - shyness ) * from + shyness * to;
    QPoint shyto = shyness * from + ( 1 - shyness ) * to;
    painter.drawLine( shyfrom, shyto );
    painter.drawLine( shyto, shyto - dir + perp );
    painter.drawLine( shyto, shyto - dir - perp );
}
void GECanvasMorphism::draw ( QPainter& painter )
{
    // somehow this routine is getting called even when the morphism is hidden.
    // this makes no sense, but since I am having trouble trackng down WHO is calling the
    // darn thing in such cases, I'll instead just prevent it from harming anyone:
    if ( !isVisible() ) return;
    // now on to more serious matters:
    prepareToDraw();
    QRect labelClipRect;
    if ( manyLines ) {
        for ( uint i = 0 ; i < morphism.getDomain()->group.order ; i++ )
            drawArrow( painter,
                       fromSmall()->elementLocationOnCanvas( i ),
                       toSmall()->elementLocationOnCanvas( morphism.lookup( i ) ) );
    } else {
        drawArrow( painter, startPoint(), endPoint() );
    }
    if ( label ) {
        if ( manyLines ) {
            QPoint fromCenter( (int)( fromSmall()->x() + fromSmall()->width() / 2 ),
                               (int)( fromSmall()->y() + fromSmall()->height() / 2 ) );
            QPoint toCenter( (int)( toSmall()->x() + toSmall()->width() / 2 ),
                             (int)( toSmall()->y() + toSmall()->height() / 2 ) );
            labelClipRect = QRect(
                ( fromCenter.x() + toCenter.x() - label->width() ) / 2,
                ( fromCenter.y() + toCenter.y() - label->height() ) / 2,
                label->width(), label->height() );
        } else {
            labelClipRect = QRect(
                ( endPoint().x() + startPoint().x() - label->width() ) / 2,
                ( endPoint().y() + startPoint().y() - label->height() ) / 2,
                label->width(), label->height() );
        }
        painter.setBrush( QBrush( QColor( 225, 200, 200, QColor::Rgb ) ) );
        painter.drawRect( labelClipRect );
        label->draw( &painter, labelClipRect.left(), labelClipRect.top(),
                     labelClipRect, QColorGroup() );
    }
    pinkMe( this, painter );
}

Q3PointArray GECanvasMorphism::areaPoints () const
{
    // modeled after GECanvasLine::areaPoints(), except make space for the morphism label
    Q3PointArray result( 4 );
    QPoint start;
    QPoint end;
    int thickness = margin;
    if ( label ) {
        if ( thickness < label->width() ) thickness = label->width();
        if ( thickness < label->height() ) thickness = label->height();
    }
    if ( manyLines ) {
        if ( thickness < fromSmall()->width() ) thickness = fromSmall()->width();
        if ( thickness < fromSmall()->height() ) thickness = fromSmall()->height();
        if ( thickness < toSmall()->width() ) thickness = toSmall()->width();
        if ( thickness < toSmall()->height() ) thickness = toSmall()->height();
        start = QPoint( (int)( fromSmall()->x() + fromSmall()->width() / 2 ),
                        (int)( fromSmall()->y() + fromSmall()->height() / 2 ) );
        end = QPoint( (int)( toSmall()->x() + toSmall()->width() / 2 ),
                      (int)( toSmall()->y() + toSmall()->height() / 2 ) );
    } else {
        start = startPoint();
        end = endPoint();
    }
    QPoint dir = end - start;
    if ( dir.x() || dir.y() ) {
        dir = ( dir * margin ) / int( sqrt( dir.x()*dir.x() + dir.y()*dir.y() ) );
    } else {
        dir.setX( 0 );  dir.setY( margin );
    }
    QPoint perp( -dir.y(), dir.x() );
    perp *= thickness * 0.7071 / margin; // overestimate
    dir *= thickness * 0.7071 / margin; // overestimate
    result[0] = start + perp - dir;
    result[1] = end + perp + dir;
    result[2] = end - perp + dir;
    result[3] = start - perp - dir;
    return result;
}

GEVSmall* GECanvasMorphism::fromSmall () const
{
    return ( GEVSmall* )from;
}

GEVSmall* GECanvasMorphism::toSmall () const
{
    return ( GEVSmall* )to;
}


GECanvasCornerHighlight::GECanvasCornerHighlight ( Q3Canvas* canvas )
    : Q3CanvasRectangle( canvas ), last( 0 )
{
}

void GECanvasCornerHighlight::surroundThis ( Q3CanvasItem* item )
{
    if ( ( last = item ) ) {
        QRect r = last->boundingRect();
        setX( r.left() - 1 );
        setY( r.top() - 1 );
        setSize( r.width() + 2, r.height() + 2 );
        setZ( last->z() + 1 );
        show();
    } else {
        hide();
    }
}

void GECanvasCornerHighlight::draw ( QPainter& painter )
{
    surroundThis( last ); // just updates our position in case the thing moved
    painter.setPen( QPen( Qt::darkCyan, 1 ) );
    Q3PointArray apts;
    if ( last->rtti() == Q3CanvasItem::Rtti_Line ) {
        apts = ( ( GECanvasLine* )last )->areaPoints();
    } else if ( last->rtti() == GECanvasMorphism::RTTI ) {
        apts = ( ( GECanvasMorphism* )last )->areaPoints();
    } else {
        QRect rect = last->boundingRect();
        apts.resize( 4 );
        apts[0] = rect.topLeft();
        apts[1] = rect.topRight();
        apts[2] = rect.bottomRight();
        apts[3] = rect.bottomLeft();
    }
    if ( !apts.size() ) return;
    QPoint ctr;
    for ( int i = 0 ; i < apts.size() ; i++ ) ctr += apts[i];
    ctr /= (int)( apts.size() );
    for ( int i = 1 ; i < apts.size() ; i++ ) {
        QPoint p13 = ( apts[i-1] * 2 + apts[i] ) / 3;
        QPoint p23 = ( apts[i-1] + apts[i] * 2 ) / 3;
        painter.drawLine( apts[i-1], p13 );
        painter.drawLine( p23, apts[i] );
    }
    QPoint p13 = ( apts[apts.size()-1] * 2 + apts[0] ) / 3;
    QPoint p23 = ( apts[apts.size()-1] + apts[0] * 2 ) / 3;
    painter.drawLine( apts[apts.size()-1], p13 );
    painter.drawLine( p23, apts[0] );
    /*
    int x = int( this->x() );
    int y = int( this->y() );
    int w = size().width();
    int h = size().height();
    int L = QMAX( QMIN( w, h ) / 3, 3 );
    painter.drawLine( x,     y,     x + L,     y );
    painter.drawLine( x + w, y,     x + w - L, y );
    painter.drawLine( x,     y + h, x + L,     y + h );
    painter.drawLine( x + w, y + h, x + w - L, y + h );
    painter.drawLine( x,     y,     x,         y + L );
    painter.drawLine( x,     y + h, x,         y + h - L );
    painter.drawLine( x + w, y,     x + w,     y + L );
    painter.drawLine( x + w, y + h, x + w,     y + h - L );
    */
}

Q3CanvasItem* GECanvasCornerHighlight::surrounded ()
{
    return last;
}


GESheet::GESheet ( int w, int h )
    : Q3Canvas( w, h )
{
    bkgd = new Q3CanvasRectangle( 0, 0, width(), height(), this );
    bkgd->setSelected( FALSE );
    bkgd->setPen( QPen( QColor( 25, 15, 175, QColor::Hsv ), 2 ) );
    bkgd->setBrush( QColor( 25, 15, 255, QColor::Hsv ) );
    bkgd->setZ( -999999.0 );
    bkgd->show();

    corn = new GECanvasCornerHighlight( this );
    corn->setSelected( FALSE );
    corn->surroundThis( 0 );
}

void GESheet::resize ( int w, int h )
{
    Q3Canvas::resize( w, h );
    bkgd->setSize( w, h );
}

Q3CanvasItemList GESheet::selectedItems ()
{
    Q3CanvasItemList result;
    Q3CanvasItemList all = allItems();
    Q3CanvasItemList::Iterator it;
    // put non-lines first in selection
    for ( it = all.begin() ; it != all.end() ; ++it )
        if ( (*it)->isSelected() && !GESheet::isLineItem( *it ) )
            result << (*it);
    // so that lines can refer to them by index if we do a copy or cut
    for ( it = all.begin() ; it != all.end() ; ++it )
        if ( (*it)->isSelected() && GESheet::isLineItem( *it ) )
            result << (*it);
    return result;
}

void GESheet::selectAll ( bool on )
{
    Q3CanvasItemList all = allItems();
    Q3CanvasItemList::Iterator it;
    for ( it = all.begin() ; it != all.end() ; ++it ) (*it)->setSelected( on );
}

void GESheet::setSelection ( Q3CanvasItemList items )
{
    Q3CanvasItemList all = allItems();
    Q3CanvasItemList::Iterator it;
    for ( it = all.begin() ; it != all.end() ; ++it )
        (*it)->setSelected( items.contains( *it ) );
}

Q3CanvasItem* GESheet::firstItemUnder ( QPoint canvasPoint )
{
    Q3CanvasItemList list = collisions( canvasPoint );
    for ( Q3CanvasItemList::Iterator it = list.begin() ; it != list.end() ; ++it )
        if ( ( bkgd != *it ) && ( corn != *it ) ) {
            // if it's a morphism, allow user to click the stuff under it, if there's any:
            if ( (*it)->rtti() == GECanvasMorphism::RTTI ) {
                //GECanvasMorphism* cm = ( GECanvasMorphism* )(*it);
                Q3CanvasItemList::Iterator next = it;
                while ( ( next != list.end() )
                     && ( ( bkgd == *next ) || ( corn == *next )
                       || ( (*next)->rtti() == GECanvasMorphism::RTTI ) ) ) ++next;
                if ( next != list.end() ) return *next;
            }
            // if it wasn't a morphism, or if there was nothing under it, select it:
            return *it;
        }
    return 0;
}

void GESheet::mouseOver ( QPoint p )
{
    Q3CanvasItem* under = firstItemUnder( p );
    if ( under != corn->surrounded() ) {
        corn->surroundThis( under );
        update();
    }
}

int GESheet::searchItem ( Q3CanvasItem* item, QString searchText )
{
    if ( item->rtti() == Q3CanvasItem::Rtti_Text ) {
        return ( ( Q3CanvasText* )( item ) )->text().find( searchText );
    } else {
        return -1;
    }
}

bool GESheet::canResizeItem ( Q3CanvasItem* item, int& w, int& h )
{
    if ( ( item->rtti() == Q3CanvasItem::Rtti_Rectangle )
      || ( item->rtti() == GEVSmall::RTTI ) ) {
        Q3CanvasRectangle* r = ( Q3CanvasRectangle* )item;
        w = r->width();
        h = r->height();
        return TRUE;
    }
    // other types here via else clauses
    return FALSE;
}

void GESheet::resizeItem ( Q3CanvasItem* item, int w, int h )
{
    if ( ( item->rtti() == Q3CanvasItem::Rtti_Rectangle )
      || ( item->rtti() == GEVSmall::RTTI ) ) {
        ( ( Q3CanvasRectangle* )item )->setSize( w, h );
    }
    // other types here via else clauses
}

bool GESheet::isLineItem ( Q3CanvasItem* item )
{
    return ( item->rtti() == Q3CanvasItem::Rtti_Line )
        || ( item->rtti() == GECanvasMorphism::RTTI );
}

void GESheet::appendXMLTo ( QDomNode n, Q3CanvasItem* item, Q3CanvasItemList items )
{
    if ( item->rtti() == Q3CanvasItem::Rtti_Text ) {
        ( ( GECanvasText* )item )->appendXMLTo( n );
    } else if ( item->rtti() == Q3CanvasItem::Rtti_Rectangle ) {
        ( ( GECanvasRect* )item )->appendXMLTo( n );
    } else if ( item->rtti() == Q3CanvasItem::Rtti_Line ) {
        ( ( GECanvasLine* )item )->appendXMLTo( n, items );
    } else if ( item->rtti() == GECanvasMorphism::RTTI ) {
        ( ( GECanvasMorphism* )item )->appendXMLTo( n, items );
    } else if ( item->rtti() == GEVSmall::RTTI ) {
        ( ( GEVSmall* )item )->appendXMLTo( n );
    }
    // OTHERS HERE
}

QString GESheet::itemToXMLString ( Q3CanvasItem* item )
{
    Q3CanvasItemList tmp;
    tmp << item;
    return itemsToXMLString( tmp );
}

Q3CanvasItem* GESheet::XMLToItem ( QDomNode n, Q3CanvasItemList items )
{
    if ( n.nodeName() == "text" ) {
        GECanvasText* result = new GECanvasText( 0 );
        result->makeFromXML( n );
        return result;
    } else if ( n.nodeName() == "rect" ) {
        GECanvasRect* result = new GECanvasRect( 0 );
        result->makeFromXML( n );
        return result;
    } else if ( n.nodeName() == "line" ) {
        GECanvasLine* result = new GECanvasLine( 0 );
        result->makeFromXML( n, items );
        return result;
    } else if ( n.nodeName() == "visualizer" ) {
        GEVisualizer* tmp = GEVSmall::makeFromXML( n );
        return tmp->getSmall();
    } else if ( n.nodeName() == "morp" ) {
        GECanvasMorphism* result = new GECanvasMorphism( 0 );
        result->makeFromXML( n, items );
        return result;
    }
    // and so on with other cases
    return 0;
}

Q3CanvasItem* GESheet::XMLStringToItem ( QString xml/*, QCanvasItemList items*/ )
{
    Q3CanvasItemList tmp = XMLStringToItems( xml/*, items*/ );
    return tmp.count() ? tmp[0] : 0;
}

Q3CanvasItemList GESheet::XMLStringToItems ( QString xml/*, QCanvasItemList items*/ )
{
    QString fullDocument = "<!DOCTYPE groupexplorerdummy>\n"
                           "<dummy>\n"
                           + xml +
                           "</dummy>\n";
    QDomDocument tmp;
    if ( !tmp.setContent( fullDocument ) ) return Q3CanvasItemList();
    Q3CanvasItemList result;
    for ( QDomNode ch = tmp.documentElement().firstChild() ;
          !ch.isNull() ; ch = ch.nextSibling() ) {
        Q3CanvasItem* item = XMLToItem( ch, result );
        if ( item ) result << item;
    }
    return result;
}

QString GESheet::itemsToXMLString ( Q3CanvasItemList items, int* missing )
{
    if ( missing ) *missing = 0;
    QDomDocument tmp( "groupexplorerdummy" );
    QDomElement root = tmp.createElement( "dummy" );
    uint count = 0;
    for ( Q3CanvasItemList::Iterator it = items.begin() ; it != items.end() ; ++it ) {
        appendXMLTo( root, *it, items );
        if ( missing ) {
            uint newcount = root.childNodes().count();
            if ( newcount == count ) (*missing)++; else count = newcount;
        }
    }
    return node_children_to_string( root );
}

QString GESheet::save ( QString filename )
{
    QFile file( filename );
    if ( !file.open( QIODevice::WriteOnly ) ) 
        return QString( "Cannot open file %1 for writing" ).arg( filename );
    Q3TextStream( &file ) << saveToString();
    file.close();
    return QString::null; // success
}

QString GESheet::saveToString ()
{
    QDomDocument doc( "groupexplorersheet" );
    QDomElement root = doc.createElement( "sheet" );
    root.setAttribute( "width", width() );
    root.setAttribute( "height", height() );
    doc.appendChild( root );
    Q3CanvasItemList list = allItems();
    Q3CanvasItemList lines; // keep all the connecting lines (and morphisms) separate from:
    Q3CanvasItemList nonlines; // the things they connect
    for ( Q3CanvasItemList::Iterator it = list.begin() ; it != list.end() ; it++ ) {
        if ( ( bkgd != *it ) && ( corn != *it ) && (*it)->isVisible() ) {
            if ( GESheet::isLineItem( *it ) ) {
                lines << *it;
            } else {
                nonlines << *it;
            }
        }
    }
    // before saving any objects to the file, first mention any groups up front that are
    // used in the file, so that we can later find them quickly w/o loading everything:
    QStringList mentionedAlready;
    for ( Q3CanvasItemList::Iterator it = nonlines.begin() ; it != nonlines.end() ; it++ ) {
        if ( (*it)->rtti() == GEVSmall::RTTI ) {
            GEVSmall* sm = ( GEVSmall* )(*it);
            if ( !mentionedAlready.contains( sm->getGroupFile()->codeName() ) ) {
                mentionedAlready << sm->getGroupFile()->codeName();
                sm->appendXMLMention( root );
            }
        }
    }
    // be sure to do the nonlines first so that the lines can reference them accurately:
    for ( Q3CanvasItemList::Iterator it = nonlines.begin() ; it != nonlines.end() ; it++ )
        appendXMLTo( root, *it );
    for ( Q3CanvasItemList::Iterator it = lines.begin() ; it != lines.end() ; it++ )
        appendXMLTo( root, *it, nonlines );
    return doc.toString( 4 );
}

QString GESheet::load ( QString filename )
{
    QFile file( filename );
    if ( !file.open( QIODevice::ReadOnly ) )
        return QString( "Cannot open file %1 for reading" ).arg( filename );
    QString everything = file.readAll();
    file.close();
    return loadFromString( everything );
}

QString GESheet::loadFromString ( QString fromMe )
{
    QDomDocument doc;
    if ( !doc.setContent( fromMe ) ) return "The data is not valid XML.";
    if ( doc.documentElement().nodeName() != "sheet" )
        return "The XML data does not describe a Group Explorer sheet.";

    QString tmp;
    if ( !try_get_attrib( tmp, doc.documentElement(), "width", "\\d+" ) )
        return "The sheet does not have a valid width.";
    int w = tmp.toInt();
    if ( !try_get_attrib( tmp, doc.documentElement(), "height", "\\d+" ) )
        return "The sheet does not have a valid height.";
    int h = tmp.toInt();
    resize( w, h );

    Q3CanvasItemList sofar;
    for ( QDomNode ch = doc.documentElement().firstChild() ; !ch.isNull() ;
          ch = ch.nextSibling() ) {
        if ( ch.nodeName() != "mention" ) {
            Q3CanvasItem* tmp = XMLToItem( ch, sofar );
            if ( tmp ) {
                sofar << tmp;
            } else {
                // before we return, clean up after ourselves:
                Q3CanvasItemList::Iterator it;
                for ( it = sofar.begin() ; it != sofar.end() ; ++it ) delete *it;
                // now return the error:
                return QString( "A \"%1\" item (#%2 in the sheet) is invalid." )
                       .arg( ch.nodeName() ).arg( sofar.count() + 1 );
            }
        }
    }
    for ( Q3CanvasItemList::Iterator it = sofar.begin() ; it != sofar.end() ; ++it )
        (*it)->setCanvas( this );
    for ( Q3CanvasItemList::Iterator it = sofar.begin() ; it != sofar.end() ; ++it )
        (*it)->show();
    /* -- this now happens automatically when setCanvas() is called in GEVSmalls
    for ( QCanvasItemList::Iterator it = sofar.begin() ; it != sofar.end() ; ++it )
        if ( (*it)->rtti() == GEVSmall::RTTI ) emit registerNewSmall( ( GEVSmall* )(*it) );
    */

    update();
    return QString::null;
}

void GESheet::noticeNewSmall ( GEVSmall* s )
{
    if ( s->canvas() == this ) emit registerNewSmall( s );
}

QPixmap GESheet::drawToPixmap ()
{
    QPixmap result( width(), height() );
    QPainter painter( &result );
    Q3CanvasItemList items = collisions( QRect( 0, 0, width(), height() ) );
    Q3CanvasItemList::const_iterator it = items.end();
    while ( it != items.begin() ) {
        --it;
        if ( corn != *it ) (*it)->draw( painter );
    }
    return result;
}

QString GESheet::formatMorphismName ( QString name )
{
    QRegExp re( "([a-zA-Z]+)([0-9]*)" );
    if ( re.exactMatch( name ) ) {
        QString result = "<i>" + re.cap( 1 ) + "</i>";
        if ( re.cap( 2 ).length() ) result += "<sub>" + re.cap( 2 ) + "</sub>";
        return result; // valid name
    }
    return QString::null; // invalid name
}

bool GESheet::morphismNameUsed ( QString name )
{
    QString formatted = formatMorphismName( name );
    Q3CanvasItemList all = allItems();
    for ( Q3CanvasItemList::Iterator it = all.begin() ; it != all.end() ; ++it )
        if ( (*it)->isVisible() && ( (*it)->rtti() == GECanvasMorphism::RTTI ) )
            if ( ( ( GECanvasMorphism* )(*it) )->getMorphism()->getName() == formatted )
                return TRUE;
    return FALSE;
}

QString GESheet::suggestMorphismName ()
{
    QStringList morphNames;
    Q3CanvasItemList all = allItems();
    for ( Q3CanvasItemList::Iterator it = all.begin() ; it != all.end() ; ++it )
        if ( (*it)->rtti() == GECanvasMorphism::RTTI )
            morphNames << ( ( GECanvasMorphism* )(*it) )->getMorphism()->getName();
    if ( !morphNames.contains( formatMorphismName( "f" ) ) )
        return "f";
    if ( !morphNames.contains( formatMorphismName( "g" ) ) )
        return "g";
    for ( uint i = 1 ; TRUE ; i++ ) {
        if ( !morphNames.contains( formatMorphismName( QString( "f%1" ).arg( i ) ) ) )
            return QString( "f%1" ).arg( i );
        if ( !morphNames.contains( formatMorphismName( QString( "g%1" ).arg( i ) ) ) )
            return QString( "g%1" ).arg( i );
    }
    return "This never happens--just making the compiler happy";
}

QStringList GESheet::scanForGroups ( QString filename )
{
    QStringList result;
    QFile file( filename );
    if ( !file.open( QIODevice::ReadOnly ) ) {
        qDebug( QString( "Cannot open file %1 for reading/loading" ).arg( filename ) );
    } else {
        QString tmp;
        char buf[2048];
        file.readLine( buf, sizeof( buf ) ); // ignore doc type line
        file.readLine( buf, sizeof( buf ) ); // ignore opening <sheet> tag
        file.readLine( buf, sizeof( buf ) ); // get first line containing "<mention>" (if any)
        tmp = buf;
        while ( tmp.contains( "<mention>" ) ) {
            int start = tmp.find( "<mention>" ) + 9;
            int finish = tmp.find( "</mention>" );
            result << tmp.mid( start, finish - start );
            file.readLine( buf, sizeof( buf ) ); // get nx line contain'g "<mention>" (if any)
            tmp = buf;
        }
    }
    return result;
}

QStringList GESheet::scanned;
QStringList GESheet::sheetsScanned ()
{
    return scanned;
}

QMap<QString,QStringList> GESheet::sheetToGroups;
QStringList GESheet::groupsInSheet ( QString filename )
{
    if ( !sheetToGroups.contains( filename ) ) {
        sheetToGroups[filename] = scanForGroups( filename );
        scanned << filename;
    }
    return sheetToGroups[filename];
}

bool GESheet::searchSheet ( QString filename, QString search4me,
                            bool wholeWord, bool caseSensitive )
{
    QFile file( filename );
    if ( !file.open( QIODevice::ReadOnly ) )
        return FALSE;//QString( "Cannot open file %1 for reading/searching" ).arg( filename );
    QString everything = file.readAll();
    file.close();

    QRegExp findSearchable( "<content>[^<]*<\\/content>" );
    search4me = QRegExp::escape( search4me );
    if ( wholeWord ) search4me = "\\b" + search4me + "\\b";
    QRegExp re( search4me );
    re.setCaseSensitive( caseSensitive );

    int i = -1;
    while ( ( i = findSearchable.search( everything, i + 1 ) ) > -1 ) {
        QString innards = everything.mid( i + 9, findSearchable.matchedLength() - 19 );
        if ( re.search( innards ) > -1 ) return TRUE;
    }
    return FALSE;
}


GEShCmdParameterized::GEShCmdParameterized ( QUndoStack* undoStack )
    : undoStack( undoStack )
{
}

void GEShCmdParameterized::setParameters ( Q3CanvasItemList items )
{
    parameters = items;
    if ( setup() )
        undoStack->push( this );
    else
        deleteLater();
}

bool GEShCmdParameterized::setup ()
{
    // subclasses may choose to put something here
    // this gets called after the parameters are set,
    // but before the command is invoked.
    // so creation of objects dependant on parameters could be done here, f.ex.
    return FALSE;
    // the return value tells if it succeeded; only successful setups get invoked.
    // if returning false, you should generally communicate a reason to the user.
}

Q3CanvasItem* GEShCmdParameterized::parameter ( int i )
{
    return ( ( i >= 0 ) && ( i < int( parameters.count() ) ) ) ? parameters[i] : 0;
}


GESheetView::GESheetView ( GESheet* s, QWidget* parent, const char* name, Qt::WFlags f )
    : Q3CanvasView( s, parent, name, f ), zoomLevel( 1.0 ),
      moving( FALSE ), resizeH( 0 ), resizeV( 0 ), cursor( Qt::ArrowCursor ),
      numParamsTotal( 0 ), numParamsLeft( 0 ), pending( 0 )
{
    viewport()->setMouseTracking( TRUE );
    viewport()->setFocusPolicy( Qt::StrongFocus );
    setFocusPolicy( Qt::StrongFocus );
}

void GESheetView::clear ()
{
    Q3CanvasItemList list = canvas()->allItems();
    for ( Q3CanvasItemList::Iterator it = list.begin() ; it != list.end() ; ++it )
        if ( *it ) delete *it;
}

void GESheetView::setSheet( GESheet* s )
{
    setCanvas( s );
}

GESheet* GESheetView::sheet ()
{
    return ( GESheet* )canvas();
}

void GESheetView::selectItems ( Q3CanvasItemList items )
{
    sheet()->setSelection( items );
    updateCanvas();
    emit selectionChanged( items );
}

Q3CanvasItemList GESheetView::selectedItems ()
{
    return sheet()->selectedItems();
}

void GESheetView::copySelection ()
{
    int missing;
    Q3CanvasItemList s = selectedItems();
    QString forClipboard = sheet()->itemsToXMLString( s, &missing );
    if ( missing ) {
        QMessageBox::warning( this, "Connection without endpoints",
            QString( ( ( s.count() - missing > 0 ) ?
              ( ( missing > 1 ) ?
                "Some of the items have not been added to the clipboard.\n" :
                "One of the items has not been added to the clipboard.\n" ) :
              "None of the items has been added to the clipboard.\n" ) ) +
            "This is because you cannot add a connection\n"
            "(either a connecting line or a morphism) to\n"
            "the clipboard without also adding both of its\n"
            "endpoints, because it cannot later be pasted\n"
            "without both endpoints.\n"
            "To prevent this, highlight all endpoints of all\n"
            "connecting lines or morphisms before issuing a\n"
            "Cut or Copy command.",
            "OK" );
        if ( !( s.count() - missing ) ) return;
    }
    QApplication::clipboard()->setText( forClipboard, QClipboard::Clipboard );
}

Q3CanvasItemList GESheetView::paste ()
{
    Q3CanvasItemList newstuff =
        GESheet::XMLStringToItems( QApplication::clipboard()->text() );
    for ( Q3CanvasItemList::Iterator it = newstuff.begin() ; it != newstuff.end() ; ++it )
        (*it)->setCanvas( canvas() );
    return newstuff;
}

void GESheetView::placeNewItem ( Q3CanvasItem* item )
{
    QRect rc( contentsX(), contentsY(), visibleWidth(), visibleHeight() );
    QRect canvasRect = inverseWorldMatrix().mapRect( rc );
    item->move( canvasRect.left() + QMIN( 100, canvasRect.width() / 3 ),
                canvasRect.top() + QMIN( 100, canvasRect.height() / 3 ) );
}

void GESheetView::getParametersFor ( GEShCmdParameterized* cmd, int numParams )
{
    pending = cmd;
    numParamsTotal = numParamsLeft = numParams;
    parameters.clear();
    setCursor( QCursor( Qt::CrossCursor ) );
}

void GESheetView::resizingCodes ( QPoint mouse, int& horizontal, int& vertical,
                                  int* origDims )
{
    horizontal = vertical = 0;
    // is there just one thing selected?
    Q3CanvasItemList sel = selectedItems();
    if ( sel.count() != 1 ) return;
    Q3CanvasItem* item = sel[0];
    // now does this item have a width and height that make it worth resizing?
    int w;
    int h;
    if ( !GESheet::canResizeItem( item, w, h ) ) return;
    if ( origDims ) {
        origDims[0] = int( item->x() );
        origDims[1] = int( item->y() );
        origDims[2] = w;
        origDims[3] = h;
    }
    // are we near any of its boundaries?
    const int dist = 3;
    if ( ( item->y() - dist <= mouse.y() ) && ( mouse.y() <= item->y() + h + dist ) ) {
        if ( abs( mouse.x() - int( item->x() ) ) <= dist )
            horizontal = -1;
        else if ( abs( mouse.x() - int( item->x() + w ) ) <= dist )
            horizontal = 1;
    }
    if ( ( item->x() - dist <= mouse.x() ) && ( mouse.x() <= item->x() + w + dist ) ) {
        if ( abs( mouse.y() - int( item->y() ) ) <= dist )
            vertical = -1;
        else if ( abs( mouse.y() - int( item->y() + h ) ) <= dist )
            vertical = 1;
    }
}

void GESheetView::contentsMouseDoubleClickEvent ( QMouseEvent* e )
{
    QPoint canvasPoint = inverseWorldMatrix().map( e->pos() );
    Q3CanvasItem* doubleClicked = sheet()->firstItemUnder( canvasPoint );
    if ( doubleClicked ) {
        if ( doubleClicked->rtti() == Q3CanvasItem::Rtti_Text ) {
            Q3CanvasText* textItem = ( Q3CanvasText* )doubleClicked;
            TextInputForm dialog;
            if ( dialog.exec( textItem->text(), "Edit text object from sheet" ) )
                if ( !dialog.text().isNull() )
                    emit changeTextItem( textItem, dialog.text() );
        } else if ( doubleClicked->rtti() == GEVSmall::RTTI ) {
            GEVSmall* smallv = ( GEVSmall* )doubleClicked;
            GEVLarge* large = smallv->visualizer->getLarge();
            large->show();
            large->setActiveWindow();
            large->raise();
            emit maybeChangedVisualizer();
        } else if ( doubleClicked->rtti() == GECanvasMorphism::RTTI ) {
            GECanvasMorphism* morph = ( GECanvasMorphism* )doubleClicked;
            emit editMorphism( morph );
        }
    }
}

void GESheetView::contentsMousePressEvent ( QMouseEvent* e )
{
    lastPos = movingStart = inverseWorldMatrix().map( e->pos() );
    Q3CanvasItem* topitem = sheet()->firstItemUnder( movingStart );
    // if we're selecting stuff for parameters, just do that and quit:
    if ( pending ) {
        if ( topitem ) {
            parameters << topitem;
            numParamsLeft--;
            emit addedParameter( topitem, numParamsTotal - numParamsLeft, numParamsTotal );
            if ( !numParamsLeft ) {
                pending->setParameters( parameters ); // this invokes it, so:
                pending = 0; // all done with that guy...ownership passed to undo stack
                unsetCursor(); // stop using the crosshairs
            }
        }
        return;
    }
    // if we're not selecting stuff, see if we're resizing (if not, we may be moving):
    resizingCodes( lastPos, resizeH, resizeV, origDims );
    if ( resizeH || resizeV ) {
        moving = FALSE;
    } else {
        moving = TRUE;
        Q3CanvasItemList emitMe;
        if ( topitem ) {
            if ( e->state() & Qt::ControlButton ) {
                // if they had the drag button pressed, do not move the item; start a drag:
                Q3CanvasItem* tmp = topitem;
                topitem = 0;
                ( new Q3TextDrag( GESheet::itemToXMLString( tmp ), this ) )->dragCopy();
                return;
            } else if ( e->state() & Qt::ShiftButton ) {
                // if they had multi-select button pressed, toggle the presence of topitem:
                emitMe = selectedItems();
                Q3CanvasItemList::Iterator seek = emitMe.find( topitem );
                if ( seek == emitMe.end() )
                    emitMe << topitem;
                else
                    emitMe.remove( seek );
            } else {
                // otherwise let the list of things to select be *only* topitem:
                emitMe << topitem;
            }
        }
        emit selectThese( emitMe );
    }
}

void GESheetView::contentsMouseMoveEvent ( QMouseEvent* e )
{
    QPoint p = inverseWorldMatrix().map( e->pos() );
    // update the highlighting-corners rectangle based on what's under the mouse
    sheet()->mouseOver( p );
    // now if we're choosing parameters, we don't allow moving/resising, so:
    if ( pending ) return;
    // otherwise, check to see what the current state of affairs is:
    if ( moving ) {
        // we are dragging the selection, so move the items and update the canvas
        Q3CanvasItemList sel = sheet()->selectedItems();
        Q3CanvasItemList::Iterator it;
        for ( it = sel.begin() ; it != sel.end() ; ++it )
            if ( !GESheet::isLineItem( *it ) )
                (*it)->moveBy( p.x() - lastPos.x(), p.y() - lastPos.y() );
        lastPos = p;
        updateCanvas();
    } else if ( resizeH || resizeV ) {
        // we are resizing an item, so change the item's shape and update the canvas
        Q3CanvasItemList sel = selectedItems();
        if ( sel.count() != 1 ) return;
        Q3CanvasItem* item = sel[0];
        int newx = int( item->x() );
        int newy = int( item->y() );
        int neww;
        int newh;
        GESheet::canResizeItem( item, neww, newh );
        if ( resizeH == 1 ) {
            neww = p.x() - newx;
        } else if ( resizeH == -1 ) {
            neww += newx - p.x();
            newx = p.x();
        }
        if ( resizeV == 1 ) {
            newh = p.y() - newy;
        } else if ( resizeV == -1 ) {
            newh += newy - p.y();
            newy = p.y();
        }
        if ( neww < 2 ) {
            neww = 2;
            newx = int( item->x() );
            newy = int( item->y() );
        }
        if ( newh < 2 ) {
            newh = 2;
            newx = int( item->x() );
            newy = int( item->y() );
        }
        item->move( newx, newy );
        GESheet::resizeItem( item, neww, newh );
        updateCanvas();
    } else {
        // we aren't moving or resizing, but we need to check if we should change the
        // cursor to indicate that the user could [not] resize the item under the mouse
        int h;
        int v;
        resizingCodes( p, h, v );
        if ( h != 0 ) {
            if ( v == h ) {
                changeCursor( Qt::SizeFDiagCursor );
            } else if ( v == -h ) {
                changeCursor( Qt::SizeBDiagCursor );
            } else {
                changeCursor( Qt::SizeHorCursor );
            }
        } else {
            if ( v != 0 ) {
                changeCursor( Qt::SizeVerCursor );
            } else {
                changeCursor( Qt::ArrowCursor );
            }
        }
    }
}

void GESheetView::changeCursor ( Qt::CursorShape newcursor )
{
    if ( cursor != newcursor ) {
        if ( newcursor == Qt::ArrowCursor ) {
            unsetCursor();
        } else {
            setCursor( QCursor( newcursor ) );
        }
        cursor = newcursor;
    }
}

void GESheetView::contentsMouseReleaseEvent ( QMouseEvent* /*e*/ )
{
    if ( moving )
        emit movedSelection( movingStart.x(), movingStart.y(), lastPos.x(), lastPos.y() );
    if ( resizeH || resizeV )
        emit resizedItem( selectedItems()[0], origDims );
    moving = FALSE;
    resizeH = resizeV = 0;
}

void GESheetView::contentsContextMenuEvent ( QContextMenuEvent* e )
{
    moving = FALSE;
    QPoint pos = inverseWorldMatrix().map( e->pos() );
    Q3CanvasItem* topitem = sheet()->firstItemUnder( pos );
    Q3PtrList<QAction> extras;
    if ( topitem && ( topitem->rtti() == GEVSmall::RTTI ) )
        extras = ( ( GEVSmall* )topitem )->actionsToExport();
    emit sheetContextMenu( e->globalPos(), extras );
}

void GESheetView::keyPressEvent ( QKeyEvent* e )
{
    if ( e->key() == Qt::Key_Escape ) {
        if ( pending ) {
            delete pending;
            pending = 0;
        }
        numParamsLeft = 0;
        unsetCursor();
        emit cancelledParameters();
        e->accept();
    }
    e->ignore();
}

void GESheetView::zoomIn ()
{
    zoomLevel *= 1.5;
    QMatrix wm;
    wm.scale( zoomLevel, zoomLevel );
    setWorldMatrix( wm );
}

void GESheetView::zoomOut ()
{
    zoomLevel /= 1.5;
    QMatrix wm;
    wm.scale( zoomLevel, zoomLevel );
    setWorldMatrix( wm );
}

void GESheetView::updateCanvas ()
{
    canvas()->update();
}

double GESheetView::zoom ()
{
    return zoomLevel;
}

