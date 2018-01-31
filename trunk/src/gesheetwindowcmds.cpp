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


#include "gesheetwindowcmds.h"
#include <qmessagebox.h>


GEShCmdInsertText::GEShCmdInsertText ( GESheetWindow* window )
    : window( window )
{
    setText( "insert new text box" );
    GESheet* sheet = window->sheet();
    textBox = new GECanvasText( sheet );
    textBox->setText( "Double-click to edit this text" );
    textBox->setZ( 0 );
    //textBox->move( QMIN( 100, sheet->width() / 3 ), QMIN( 100, sheet->height() / 3 ) );
    window->placeNewItem( textBox );
}

void GEShCmdInsertText::redo ()
{
    textBox->show();
    window->update();
}

void GEShCmdInsertText::undo ()
{
    textBox->hide();
    window->update();
}


GEShCmdInsertRect::GEShCmdInsertRect ( GESheetWindow* window )
    : window( window )
{
    setText( "insert new rectangle" );
    GESheet* sheet = window->sheet();
    box = new GECanvasRect( sheet );
    box->setSize( 100, 50 );
    //box->move( QMIN( 50, sheet->width() / 4 ),
    //           QMIN( 50, sheet->height() / 4 ) );
    box->setZ( -1 );
    window->placeNewItem( box );
}

void GEShCmdInsertRect::redo ()
{
    box->show();
    window->update();
}

void GEShCmdInsertRect::undo ()
{
    box->hide();
    window->update();
}


GEShCmdInsertVisualizer::GEShCmdInsertVisualizer (
        GESheetWindow* window, GEVSmall* visualizer )
    : window( window ), visualizer( visualizer )
{
    setText( "insert new visualizer" );
    visualizer->setSheet( window->sheet() );
    //visualizer->move( 10, 10 );
    window->placeNewItem( visualizer );
}

void GEShCmdInsertVisualizer::redo ()
{
    visualizer->show();
    window->update();
}

void GEShCmdInsertVisualizer::undo ()
{
    visualizer->hide();
    window->update();
}


GEShCmdZoomIn::GEShCmdZoomIn ( GESheetWindow* window )
    : window( window )
{
    setText( "zoom in" );
}

void GEShCmdZoomIn::redo ()
{
    window->zoomIn();
}

void GEShCmdZoomIn::undo ()
{
    window->zoomOut();
}


GEShCmdZoomOut::GEShCmdZoomOut ( GESheetWindow* window )
    : window( window )
{
    setText( "zoom out" );
}

void GEShCmdZoomOut::redo ()
{
    window->zoomOut();
}

void GEShCmdZoomOut::undo ()
{
    window->zoomIn();
}


GEShCmdMovedSelection::GEShCmdMovedSelection (
        GESheetWindow* window, int fromX, int fromY, int toX, int toY )
    : window( window ), firstTime( TRUE ),
      fromX( fromX ), fromY( fromY ), toX( toX ), toY( toY )
{
    setText( "move sheet object(s)" );
}

void GEShCmdMovedSelection::redo ()
{
    if ( firstTime ) {
        firstTime = FALSE;
    } else {
        Q3CanvasItemList sel = window->sheet()->selectedItems();
        Q3CanvasItemList::Iterator it;
        for ( it = sel.begin() ; it != sel.end() ; ++it )
            if ( !GESheet::isLineItem( *it ) )
                (*it)->moveBy( toX - fromX, toY - fromY );
        window->update();
    }
}

void GEShCmdMovedSelection::undo ()
{
    Q3CanvasItemList sel = window->sheet()->selectedItems();
    Q3CanvasItemList::Iterator it;
    for ( it = sel.begin() ; it != sel.end() ; ++it )
        if ( !GESheet::isLineItem( *it ) )
            (*it)->moveBy( fromX - toX, fromY - toY );
    window->update();
}


GEShCmdSelectItems::GEShCmdSelectItems ( GESheetView* sheetview, Q3CanvasItemList items )
    : sheetview( sheetview ), newsel( items )
{
    setText( "select sheet object(s)" );
    oldsel = sheetview->selectedItems();
}

void GEShCmdSelectItems::redo ()
{
    sheetview->selectItems( newsel );
}

void GEShCmdSelectItems::undo ()
{
    sheetview->selectItems( oldsel );
}


GEShCmdChangeText::GEShCmdChangeText ( Q3CanvasText* textitem, QString newtext )
    : textitem( textitem ), newtext( newtext )
{
    setText( "change text box contents" );
    oldtext = textitem->text();
}

void GEShCmdChangeText::redo ()
{
    textitem->setText( newtext );
    textitem->canvas()->update();
}

void GEShCmdChangeText::undo ()
{
    textitem->setText( oldtext );
    textitem->canvas()->update();
}


GEShCmdResizeText::GEShCmdResizeText ( Q3CanvasText* textitem, bool increase )
    : textitem( textitem ), increase( increase )
{
    setText( QString( "%1 font size" ).arg( increase ? "increase" : "decrease" ) );
}

void GEShCmdResizeText::redo ()
{
    QFont font = textitem->font();
    font.setPointSize( font.pointSize() + ( increase ? 2 : -2 ) );
    textitem->setFont( font );
    textitem->canvas()->update();
}

void GEShCmdResizeText::undo ()
{
    QFont font = textitem->font();
    font.setPointSize( font.pointSize() - ( increase ? -2 : 2 ) );
    textitem->setFont( font );
    textitem->canvas()->update();
}


GEShCmdCutItems::GEShCmdCutItems ( GESheetView* sheetview )
    : sheetview( sheetview )
{
    items = sheetview->selectedItems();
    setText( "cut sheet item(s)" );
    // record any connecting lines/morphisms that touch any of these guys:
    Q3CanvasItemList all = sheetview->canvas()->allItems();
    for ( Q3CanvasItemList::Iterator it = items.begin() ; it != items.end() ; ++it ) {
        for ( Q3CanvasItemList::Iterator it2 = all.begin() ; it2 != all.end() ; ++it2 ) {
            if ( ( (*it2)->rtti() == Q3CanvasItem::Rtti_Line )
              || ( (*it2)->rtti() == GECanvasMorphism::RTTI ) ) {
                GECanvasLine* cnxn = ( GECanvasLine* )(*it2);
                if ( ( cnxn->getFromItem() == *it ) || ( cnxn->getToItem() == *it ) )
                    if ( !cnxns.contains( cnxn ) ) cnxns << cnxn;
            }
        }
    }
}

uint GEShCmdCutItems::numberOfConnections ()
{
    return cnxns.count();
}

void GEShCmdCutItems::redo ()
{
    // the putting on the clipboard happens before this, in GESheetView
    for ( Q3CanvasItemList::Iterator it = items.begin() ; it != items.end() ; ++it )
        (*it)->hide();
    for ( Q3CanvasItemList::Iterator it = cnxns.begin() ; it != cnxns.end() ; ++it )
        (*it)->hide();
    sheetview->updateCanvas();
    sheetview->selectItems( Q3CanvasItemList() );
}

void GEShCmdCutItems::undo ()
{
    for ( Q3CanvasItemList::Iterator it = items.begin() ; it != items.end() ; ++it )
        (*it)->show();
    for ( Q3CanvasItemList::Iterator it = cnxns.begin() ; it != cnxns.end() ; ++it )
        (*it)->show();
    sheetview->updateCanvas();
    sheetview->selectItems( items );
}


GEShCmdPasteItems::GEShCmdPasteItems ( Q3CanvasItemList items )
    : items( items )
{
    setText( "paste sheet item(s)" );
}

void GEShCmdPasteItems::redo ()
{
    // the pasting happens before this, in GESheetView
    for ( Q3CanvasItemList::Iterator it = items.begin() ; it != items.end() ; ++it )
        (*it)->show();
    if ( items.count() ) items[0]->canvas()->update();
}

void GEShCmdPasteItems::undo ()
{
    for ( Q3CanvasItemList::Iterator it = items.begin() ; it != items.end() ; ++it )
        (*it)->hide();
    if ( items.count() ) items[0]->canvas()->update();
}


GEShCmdResizedItem::GEShCmdResizedItem ( Q3CanvasItem* item, int* origDims )
    : firstTime( TRUE ), item( item )
{
    for ( int i = 0 ; i < 4 ; i++ ) origD[i] = origDims[i];
    newD[0] = int( item->x() );
    newD[1] = int( item->y() );
    GESheet::canResizeItem( item, newD[2], newD[3] );
    setText( "resize sheet item" );
}

void GEShCmdResizedItem::redo ()
{
    if ( firstTime ) {
        firstTime = FALSE;
    } else {
        item->move( newD[0], newD[1] );
        GESheet::resizeItem( item, newD[2], newD[3] );
        item->canvas()->update();
    }
}

void GEShCmdResizedItem::undo ()
{
    item->move( origD[0], origD[1] );
    GESheet::resizeItem( item, origD[2], origD[3] );
    item->canvas()->update();
}


GEShCmdPushSelectionZ::GEShCmdPushSelectionZ ( GESheetView* sheetview, bool backwards )
    : backwards( backwards )
{
    setText( backwards ? "push selection back" : "bring selection forward" );
    items = sheetview->selectedItems();
}

void GEShCmdPushSelectionZ::redo ()
{
    for ( Q3CanvasItemList::Iterator it = items.begin() ; it != items.end() ; ++it )
        (*it)->setZ( (*it)->z() + ( backwards ? -1 : 1 ) );
    if ( items.count() ) items[0]->canvas()->update();
}

void GEShCmdPushSelectionZ::undo ()
{
    for ( Q3CanvasItemList::Iterator it = items.begin() ; it != items.end() ; ++it )
        (*it)->setZ( (*it)->z() - ( backwards ? -1 : 1 ) );
    if ( items.count() ) items[0]->canvas()->update();
}


GEShCmdConnectWithLine::GEShCmdConnectWithLine ( QUndoStack* undoStack )
    : GEShCmdParameterized( undoStack )
{
    setText( "insert connecting line" );
}

bool GEShCmdConnectWithLine::setup ()
{
    if ( parameters[0] == parameters[1] ) {
        QMessageBox::information( 0, "Cannot create connecting line",
                                  "You must choose two <i>different</i> sheet items\n"
                                  "to connect with a line.\n"
                                  "You chose the same item twice.", QMessageBox::Ok );
        return FALSE; // this is failure
    } else {
        line = new GECanvasLine( parameters[0]->canvas() );
        line->setFromItem( parameters[0] );
        line->setToItem( parameters[1] );
        return TRUE; // this is success
    }
}

void GEShCmdConnectWithLine::redo ()
{
    line->show();
    line->canvas()->update();
}

void GEShCmdConnectWithLine::undo ()
{
    line->hide();
    line->canvas()->update();
}


GEShCmdConnectWithMorphism::GEShCmdConnectWithMorphism (
        QUndoStack* undoStack, QString morphName )
    : GEShCmdParameterized( undoStack ), tmp( morphName )
{
    setText( "insert homomorphism" );
}

bool GEShCmdConnectWithMorphism::setup ()
{
    if ( parameters[0] == parameters[1] ) {
        QMessageBox::information( 0, "Cannot create homomorphism",
                                  "You must choose two <i>different</i> visualizers\n"
                                  "to connect with a homomorphism.\n"
                                  "You chose the same object twice.", QMessageBox::Ok );
        return FALSE; // this is failure
    } else if ( ( parameters[0]->rtti() == GEVSmall::RTTI )
             && ( parameters[1]->rtti() == GEVSmall::RTTI ) ) {
        morphism = new GECanvasMorphism( parameters[0]->canvas() );
        morphism->getMorphism()->setName( tmp );
        morphism->setFromSmall( ( GEVSmall* )( parameters[0] ) );
        morphism->setToSmall( ( GEVSmall* )( parameters[1] ) );
        morphism->updateLabel();
        return TRUE; // this is success
    } else {
        QMessageBox::information( 0, "Cannot create homomorphism",
                                  "You must choose two different visualizers\n"
                                  "to connect with a homomorphism.\n"
                                  "Try again.", QMessageBox::Ok );
        return FALSE; // this is failure
    }
}

void GEShCmdConnectWithMorphism::redo ()
{
    morphism->show();
    morphism->canvas()->update();
}

void GEShCmdConnectWithMorphism::undo ()
{
    morphism->hide();
    morphism->canvas()->update();
}


GEShCmdChangeMorphism::GEShCmdChangeMorphism (
        GECanvasMorphism* morphism, GEltList domgens, GEltList codgens,
        bool name, bool domcod, bool epimon, bool gens, bool many )
    : morphism( morphism ), newdomgens( domgens ), newcodgens( codgens ),
      newname( name ), newdomcod( domcod ), newepimon( epimon ), newgens( gens ),
      newmany( many )
{
    olddomgens = morphism->domgens.copy();
    oldcodgens = morphism->codgens.copy();
    oldname = morphism->showName;
    olddomcod = morphism->showDomCod;
    oldepimon = morphism->showEpiMon;
    oldgens = morphism->showGens;
    oldmany = morphism->manyLines;
    setText( "change morphism" );
}

void GEShCmdChangeMorphism::redo ()
{
    morphism->domgens = newdomgens;
    morphism->codgens = newcodgens;
    morphism->getMorphism()->setMap( morphism->buildMap( newdomgens, newcodgens ) );
    morphism->showName = newname;
    morphism->showDomCod = newdomcod;
    morphism->showEpiMon = newepimon;
    morphism->showGens = newgens;
    morphism->manyLines = newmany;
    morphism->updateLabel();
    morphism->canvas()->update();
}

void GEShCmdChangeMorphism::undo ()
{
    morphism->domgens = olddomgens;
    morphism->codgens = oldcodgens;
    morphism->getMorphism()->setMap( morphism->buildMap( olddomgens, oldcodgens ) );
    morphism->showName = oldname;
    morphism->showDomCod = olddomcod;
    morphism->showEpiMon = oldepimon;
    morphism->showGens = oldgens;
    morphism->manyLines = oldmany;
    morphism->updateLabel();
    morphism->canvas()->update();
}


