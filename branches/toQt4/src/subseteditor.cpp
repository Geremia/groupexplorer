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


#include "subseteditor.h"
#include "utilpieces.h"
#include <qlayout.h>
#include <qpushbutton.h>
#include <q3dragobject.h>
#include <qapplication.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QLabel>


#include "sympanewidgets.h"


GEltListBox::GEltListBox ( GroupFile* groupfile,
        QWidget* parent, const char *name )
    : Q3ListBox( parent, name ), groupfile( groupfile )
{
    viewport()->setAcceptDrops( TRUE );
}

GElt GEltListBox::getElement ( uint index )
{
    Q3ListBoxItem* qlbi = item( index );
    if ( qlbi->rtti() != RichListBoxItem::RTTI ) {
        qWarning( QString( "GEltListBox::getElement(%1) cannot get tag for \"%2\"" )
                  .arg( index ).arg( text( index ) ) );
        return groupfile->group.order; // an invalid result
    }
    RichListBoxItem* rlbi = (RichListBoxItem*)qlbi;
    return rlbi->getTag();
}

void GEltListBox::setElements ( GEltList elts )
{
    clear();
    for ( uint i = 0 ; i < elts.size() ; i++ ) addElement( elts[i] );
}

void GEltListBox::addElement ( GElt e )
{
    RichListBoxItem* rlbi = new RichListBoxItem(
        groupfile->getRepresentation( e ).HTML, this );
    rlbi->setTag( e );
}

void GEltListBox::removeElement ( GElt e )
{
    for ( uint i = 0 ; i < count() ; i++ ) {
        if ( getElement( i ) == e ) {
            delete item( i );
            return;
        }
    }
}

GEltList GEltListBox::getElements ()
{
    GEltList result( count() );
    for ( uint i = 0 ; i < count() ; i++ ) result[i] = getElement( i );
    return result;
}

void GEltListBox::contentsMousePressEvent ( QMouseEvent* e )
{
    if ( e->button() == Qt::LeftButton )
        dragPos = e->pos();
    Q3ListBox::contentsMousePressEvent( e );
}

void GEltListBox::contentsMouseMoveEvent ( QMouseEvent* e )
{
    if ( e->state() & Qt::LeftButton &&
         ( ( e->pos() - dragPos ).manhattanLength() >
           QApplication::startDragDistance() ) )
        startDrag();
    Q3ListBox::contentsMouseMoveEvent( e );
}

void GEltListBox::contentsDragEnterEvent ( QDragEnterEvent* e )
{
    e->accept( e->provides( "text/x-groupexplorer-gelt" ) );
}

void GEltListBox::contentsDropEvent ( QDropEvent* e )
{
    QString eltnum;
    if ( !Q3TextDrag::decode( e, eltnum ) ) return;

    GElt elt;
    bool ok;
    elt = eltnum.toUInt( &ok );
    if ( !ok ) return;

    QWidget* fromWidget = e->source();
    if ( fromWidget && ( fromWidget != this )
                    && fromWidget->inherits( "GEltListBox" ) ) {
        GEltListBox* gelb = (GEltListBox*)fromWidget;
        gelb->removeElement( elt );
        addElement( elt );
    }
}

void GEltListBox::startDrag ()
{
    if ( selectedItem() ) {
        RichListBoxItem* rlbi = (RichListBoxItem*)selectedItem();
        Q3TextDrag *drag = new Q3TextDrag( QString::number( rlbi->getTag() ), this );
        drag->setSubtype( "x-groupexplorer-gelt" );
        drag->setPixmap( rlbi->getPixmap() );
        drag->drag();
    }
}


SubsetEditor::SubsetEditor ( GroupSubset* subset,
        QWidget* parent, const char *name, bool modal, Qt::WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    setCaption( "Edit subset" );

    Q3GridLayout* subseteditorlayout = new Q3GridLayout( this, 4, 2, 11, 6 );

    QLabel* tmp;
    tmp= new QLabel(
        "Customize the contents of " + subset->getName() + " by dragging "
        "elements into it our out of it below.", this );
    tmp->setAlignment( int( Qt::AlignCenter ) );
    subseteditorlayout->addMultiCellWidget( tmp, 0, 0, 0, 1 );

    tmp = new QLabel( "Elements inside " + subset->getName(), this );
    tmp->setAlignment( int( Qt::AlignCenter ) );
    subseteditorlayout->addWidget( tmp, 1, 0 );
    tmp = new QLabel( "Elements outside " + subset->getName(), this );
    tmp->setAlignment( int( Qt::AlignCenter ) );
    subseteditorlayout->addWidget( tmp, 1, 1 );

    inside = new GEltListBox( subset->getGroup(), this );
    GEltListBox* outside = new GEltListBox( subset->getGroup(), this );
    subseteditorlayout->addWidget( inside, 2, 0 );
    subseteditorlayout->addWidget( outside, 2, 1 );

    QPushButton* ok = new QPushButton( "OK", this );
    QPushButton* cancel = new QPushButton( "Cancel", this );
    ok->setDefault( TRUE );
    connect( ok, SIGNAL(clicked()), this, SLOT(accept()) );
    connect( cancel, SIGNAL(clicked()), this, SLOT(reject()) );
    subseteditorlayout->addWidget( cancel, 3, 0 );
    subseteditorlayout->addWidget( ok, 3, 1 );
    
    inside->setElements( subset->elements() );
    GEltList remainder( subset->getGroup()->group.order - subset->elements().size() );
    uint count = 0;
    for ( uint i = 0 ; i < subset->getGroup()->group.order ; i++ )
        if ( !subset->elements().contains( i ) ) remainder[count++] = i;
    outside->setElements( remainder );
}

GEltList SubsetEditor::getElements ()
{
    return inside->getElements();
}


