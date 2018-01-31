//Added by qt3to4:
#include <Q3PtrList>
#include <QPixmap>
#include <Q3PopupMenu>
#include <QCloseEvent>
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
/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/


void GESheetWindow::fileNew()
{
    // not using this method right now...maybe not ever...will delete later if so
}


void GESheetWindow::fileOpen()
{
    // not using this method right now...maybe not ever...will delete later if so
}


bool GESheetWindow::saveToFile ( QString filename )
{
    QString error = sheetview->sheet()->save( filename );
    if ( error.isNull() ) {
        contentsDirty = FALSE;
        saveFileName = filename;
        statusBar()->message( "Saved sheet to " + filename );
        return TRUE;
    }
    statusBar()->message( "Error attempting to save to " + filename );
    QMessageBox::warning( this, "Error saving to file",
                          "There was an error while attempting to save:\n"
                          + error + "\n"
                          "The sheet was not saved.",
                          QMessageBox::Ok, Qt::NoButton );
    return FALSE;
}


bool GESheetWindow::trySave ()
{
    return saveFileName.isNull() ? trySaveAs() : saveToFile( saveFileName );
}


QString GESheetWindow::filename ()
{
    return saveFileName;
}


void GESheetWindow::fileSave()
{
    trySave();
}


void GESheetWindow::fileSaveImage()
{
    SaveImageSettingsDialog dia( this );
    QPixmap tmp = sheetview->sheet()->drawToPixmap();
    if ( dia.exec( tmp, applicationPath() ) == 2 ) {
        statusBar()->message( "Image saved successfully" );
    } else {
        statusBar()->message( "Canceled image save" );
    }
}


bool GESheetWindow::trySaveAs ()
{
    QString filename = Q3FileDialog::getSaveFileName( applicationPath(),
                                                     "Group Explorer sheets (*.sheet)" );
    if ( filename.isEmpty() ) {
        statusBar()->message( "Canceled save" );
        return FALSE;
    }

    if ( !filename.endsWith( ".sheet" ) ) filename += ".sheet";
    QFile check( filename );
    if ( check.exists() ) {
        int yesAnyway = QMessageBox::warning(
            this, "Warning: Overwrite file?",
            QString( "You are about to save your sheet over the following "
                     "existing file.\n"
                     "%1\n"
                     "Do you want to overwrite that file?" )
            .arg( filename ),
            QMessageBox::Yes, QMessageBox::Cancel | QMessageBox::Escape );
        if ( yesAnyway != QMessageBox::Yes ) {
            statusBar()->message( "Canceled save" );
            return FALSE;
        }
    }

    return saveToFile( filename );
}


void GESheetWindow::fileSaveAs()
{
    trySaveAs();
}


void GESheetWindow::filePrint()
{
    //Printing::dispatchPrintCanvas( *(sheetview->sheet()) );
    PrintImageSettingsDialog dia( this );
    QPixmap tmp = sheetview->sheet()->drawToPixmap();
    if ( dia.exec( tmp ) == QDialog::Accepted ) {
        statusBar()->message( "Sent sheet print job" );
    } else {
        statusBar()->message( "Canceled print" );
    }
}


void GESheetWindow::fileClose()
{
    close();
}


bool GESheetWindow::saveOnClosing ()
{
    if ( contentsDirty ) {
        int userInput = QMessageBox::warning(
            this, "Sheet has been modified",
            "The sheet has been modified since the last save.\n"
            "Would you like to save a copy before closing?",
            QMessageBox::Yes | QMessageBox::Default,
            QMessageBox::No,
            QMessageBox::Cancel | QMessageBox::Escape );
        // the following line ensures that any Yes/No answer will persist, so that if
        // this function gets called before the window closes, the close event won't
        // trigger another showing of the same question dialog:
        if ( userInput != QMessageBox::Cancel ) contentsDirty = FALSE;
        // if they don't want to quit, or if they cancel a desired save,
        // then it's not OK to close the window:
        if ( ( userInput == QMessageBox::Cancel ) ||
           ( ( userInput == QMessageBox::Yes ) && !trySave() ) ) return FALSE;
    }
    // otherwise return that it's OK to close this window:
    return TRUE;
}


void GESheetWindow::closeEvent ( QCloseEvent* ce )
{
    if ( saveOnClosing() ) ce->accept(); else ce->ignore();
}


Q3PtrList<GESheetWindow> GESheetWindow::allInstances;
bool GESheetWindow::tryToQuitApplication ()
{
    for ( GESheetWindow* win = allInstances.first() ; win ; win = allInstances.next() )
        if ( !win->saveOnClosing() ) return FALSE;
    return TRUE;
}


void GESheetWindow::editCut()
{
    editCopy();
    statusBar()->message( "Cut item(s) out of sheet and into clipboard" );
    GEShCmdCutItems* cmd = new GEShCmdCutItems( sheetview );
    uint extras = cmd->numberOfConnections();
    if ( extras ) {
        QMessageBox::warning( this, "Connections not added to clipboard",
            QString( "The selection you cut from the sheet\n"
                     "also required Group Explorer to remove\n"
                     "%1 %2 (connecting line or morphism).\n"
                     "However the extra %3 were not added\n"
                     "to the clipboard.\n"
                     "To prevent this, select the items at the\n"
                     "ends of each connectionbefore executing\n"
                     "the Cut command.\n" )
            .arg( extras )
            .arg( ( extras > 1 ) ? "connection" : "connections" )
            .arg( ( extras > 1 ) ? "connection" : "connections" ) );
    }
    undoStack->push( cmd );
    contentsDirty = TRUE;
}


void GESheetWindow::DeleteAction_activated()
{
    statusBar()->message( "Delete item(s) from sheet" );
    GEShCmdCutItems* cmd = new GEShCmdCutItems( sheetview );
    cmd->setDescription( "delete item(s)" );
    undoStack->push( cmd );
    contentsDirty = TRUE;
}


void GESheetWindow::editCopy()
{
    statusBar()->message( "Copied item(s) to clipboard" );
    sheetview->copySelection();
}


void GESheetWindow::editPaste()
{
    Q3CanvasItemList pasted = sheetview->paste();
    if ( pasted.count() ) {
        statusBar()->message( "Pasted items from clipboard" );
        undoStack->push( new GEShCmdPasteItems( sheetview->paste() ) );
        contentsDirty = TRUE;
    } else {
        statusBar()->message( "No items pasted" );
    }
}


void GESheetWindow::editFind()
{
    // think this guy is a dinosaur that needs to die
}


void GESheetWindow::helpManualAction_activated()
{
    statusBar()->message( "Opened help" );
    GEHelpWindow::showHelp( "index.html" );
}


void GESheetWindow::helpTutorialsAction_activated()
{
    statusBar()->message( "Opened tutorials" );
    GEHelpWindow::showHelp( "tu-index.html" );
}


void GESheetWindow::init()
{
    saveFileName = QString::null;
    contentsDirty = FALSE;
    allInstances.append( this );
    //delete statusBar();

    sheetview = new GESheetView( 0, centralWidget() );
    centralWidget()->layout()->addWidget( sheetview );
    connect( sheetview, SIGNAL(movedSelection(int,int,int,int)),
             this, SLOT(movedSelection(int,int,int,int)) );
    connect( sheetview, SIGNAL(selectThese(Q3CanvasItemList)),
             this, SLOT(selectThese(Q3CanvasItemList)) );
    connect( sheetview, SIGNAL(changeTextItem(Q3CanvasText*,QString)),
             this, SLOT(changeTextItem(Q3CanvasText*,QString)) );
    connect( sheetview, SIGNAL(maybeChangedVisualizer()),
             this, SLOT(maybeChangedVisualizer()) );
    connect( sheetview, SIGNAL(sheetContextMenu(QPoint,Q3PtrList<QAction>)),
             this, SLOT(sheetContextMenu(QPoint,Q3PtrList<QAction>)) );
    connect( sheetview, SIGNAL(resizedItem(Q3CanvasItem*,int*)),
             this, SLOT(resizedItem(Q3CanvasItem*,int*)) );
    connect( sheetview, SIGNAL(addedParameter(Q3CanvasItem*,int,int)),
             this, SLOT(addedParameter(Q3CanvasItem*,int,int)) );
    connect( sheetview, SIGNAL(cancelledParameters()),
             this, SLOT(cancelledParameters()) );
    connect( sheetview, SIGNAL(editMorphism(GECanvasMorphism*)),
             this, SLOT(editMorphism(GECanvasMorphism*)) );

    increaseTextSizeAction->setEnabled( FALSE );
    decreaseTextSizeAction->setEnabled( FALSE );
    connect( sheetview, SIGNAL(selectionChanged(Q3CanvasItemList)),
             this, SLOT(selectionChanged(Q3CanvasItemList)) );

    Q3ToolBar* toolBar = toolBars( Qt::DockTop ).at( 0 );

    undoStack = new QtUndoStack( this );
#ifndef NO_QT_UNDO
    QAction* undoAction = undoStack->createUndoAction( sheetview );
    QAction* redoAction = undoStack->createRedoAction( sheetview );
    undoAction->setName( "undoAction" );
    redoAction->setName( "redoAction" );
    undoAction->setAccel( tr( "Ctrl+Z" ) );
    redoAction->setAccel( tr( "Ctrl+Y" ) );
    toolBar->addSeparator();
    undoAction->addTo( toolBar );
    redoAction->addTo( toolBar );
    editMenu->insertSeparator();
    undoAction->addTo( editMenu );
    redoAction->addTo( editMenu );
#endif
    GEWhatsThis::setup( toolBar->child( "fileSaveAction_action_button" ),
                        "sheetWindow-fileSaveAction" );
    GEWhatsThis::setup( toolBar->child( "filePrintAction_action_button" ),
                        "sheetWindow-filePrintAction" );
    GEWhatsThis::setup( toolBar->child( "fileSaveImageAction_action_button" ),
                        "sheetWindow-fileSaveImageAction" );
    GEWhatsThis::setup( toolBar->child( "DeleteAction_action_button" ),
                        "sheetWindow-DeleteAction" );
    GEWhatsThis::setup( toolBar->child( "editCutAction_action_button" ),
                        "sheetWindow-editCutAction" );
    GEWhatsThis::setup( toolBar->child( "editCopyAction_action_button" ),
                        "sheetWindow-editCopyAction" );
    GEWhatsThis::setup( toolBar->child( "editPasteAction_action_button" ),
                        "sheetWindow-editPasteAction" );
    GEWhatsThis::setup( toolBar->child( "zoomInAction_action_button" ),
                        "sheetWindow-zoomInAction" );
    GEWhatsThis::setup( toolBar->child( "zoomOutAction_action_button" ),
                        "sheetWindow-zoomOutAction" );
    GEWhatsThis::setup( toolBar->child( "helpManualAction_action_button" ),
                        "sheetWindow-helpManualAction" );
    GEWhatsThis::setup( toolBar->child( "helpTutorialsAction_action_button" ),
                        "sheetWindow-helpTutorialsAction" );
    GEWhatsThis::setup( toolBar->child( "helpSheetsAction_action_button" ),
                        "sheetWindow-helpSheetsAction" );
    GEWhatsThis::setup( toolBar->child( "undoAction_action_button" ),
                        "sheetWindow-undoAction" );
    GEWhatsThis::setup( toolBar->child( "redoAction_action_button" ),
                        "sheetWindow-redoAction" );
    GEWhatsThis::setup( statusBar(), "sheetWindow-statusBar" );
    GEWhatsThis::setup( sheetview, "sheetWindow-sheetview" );
}


void GESheetWindow::destroy()
{
    allInstances.remove( this );
    sheet()->deleteLater();
}


void GESheetWindow::setSheet ( GESheet* s )
{
    sheetview->setSheet( s );
    connect( s, SIGNAL(registerNewSmall(GEVSmall*)),
             this, SLOT(registerNewSmall(GEVSmall*)) );
}


GESheet* GESheetWindow::sheet ()
{
    return ( GESheet* )( sheetview->canvas() );
}


void GESheetWindow::update ()
{
    sheetview->updateCanvas();
}


void GESheetWindow::placeNewItem ( Q3CanvasItem* item )
{
    sheetview->placeNewItem( item );
}


void GESheetWindow::insertText ()
{
    statusBar()->message( "Inserted new text item into sheet" );
    undoStack->push( new GEShCmdInsertText( this ) );
    contentsDirty = TRUE;
}


void GESheetWindow::insertRect ()
{
    statusBar()->message( "Inserted new rectangle item into sheet" );
    undoStack->push( new GEShCmdInsertRect( this ) );
    contentsDirty = TRUE;
}


void GESheetWindow::insertLine ()
{
    statusBar()->message( "Choose two sheet items to connect with a line" );
    sheetview->getParametersFor( new GEShCmdConnectWithLine( undoStack ), 2 );
}


void GESheetWindow::insertMorphism ()
{
    bool ok;
    QString morphName = QInputDialog::getText(
        "Enter homomorphism name/symbol",
        "What name (or symbol) should this morphism have?\n"
        "Using something standard like f or g (or f1, f2, etc.)\n"
        "is a good idea because readers expect such notation.",
        QLineEdit::Normal, sheetview->sheet()->suggestMorphismName(), &ok, this );
    if ( !ok ) return; // the user cancelled the action
    QString actualName = GESheet::formatMorphismName( morphName );
    if ( actualName.isNull() ) {
        QMessageBox::warning( this, "Invalid morphism name",
                              "The name you give a morphism must be\n"
                              "a sequence of letters followed by an\n"
                              "optional sequence of numbers.\n"
                              "No space or punctuation is allowed.\n",
                              QMessageBox::Ok, Qt::NoButton );
        insertMorphism(); // recur -- try again
        return;
    } else {
        if ( sheetview->sheet()->morphismNameUsed( actualName ) ) {
            QMessageBox::warning( this, "Duplicate morphism name",
                                  "The name you chose is already in use\n"
                                  "in the current sheet.  You will need\n"
                                  "to chose a different morphism name.\n",
                                  QMessageBox::Ok, Qt::NoButton );
            insertMorphism(); // recur -- try again
            return;
        } else {
            // finally, we can let them choose domain and range now:
            statusBar()->message( "Choose two visualizers to connect with a morphism" );
            sheetview->getParametersFor(
                new GEShCmdConnectWithMorphism( undoStack, actualName ), 2 );
        }
    }
}


void GESheetWindow::insertVisualizer ()
{
    statusBar()->message( "Inserting a visualizer..." );
    InsertVisualizerDialog dia;
    if ( dia.exec() ) {
        // for now, I ignore any information and just huck in an MT one
        GEVisualizer* viz = dia.createVisualizer();
        undoStack->push( new GEShCmdInsertVisualizer( this, viz->getSmall() ) );
        contentsDirty = TRUE;
    }
}


void GESheetWindow::zoomIn_activated ()
{
    statusBar()->message( "Zoomed in on the sheet" );
    undoStack->push( new GEShCmdZoomIn( this ) );
}


void GESheetWindow::zoomOut_activated ()
{
    statusBar()->message( "Zoomed out from the sheet" );
    undoStack->push( new GEShCmdZoomOut( this ) );
}


void GESheetWindow::zoomIn ()
{
    sheetview->zoomIn();
    zoomInAction->setEnabled( sheetview->zoom() < 10 );
    zoomOutAction->setEnabled( sheetview->zoom() > 0.1 );
}


void GESheetWindow::zoomOut ()
{
    sheetview->zoomOut();
    zoomInAction->setEnabled( sheetview->zoom() < 10 );
    zoomOutAction->setEnabled( sheetview->zoom() > 0.1 );
}


void GESheetWindow::movedSelection ( int fromX, int fromY, int toX, int toY )
{
    if ( !sheetview->selectedItems().count() ) return;
    if ( ( fromX != toX ) || ( fromY != toY ) ) {
        statusBar()->message( "Moved sheet item(s)" );
        undoStack->push( new GEShCmdMovedSelection( this, fromX, fromY, toX, toY ) );
        contentsDirty = TRUE;
    }
}


void GESheetWindow::resizeSheetAction_activated()
{
    SheetSizeDialog dia;
    dia.setSize( sheetview->canvas()->width(), sheetview->canvas()->height() );
    if ( dia.exec() ) {
        QSize s = dia.getSize();
        if ( ( s.width() != sheetview->canvas()->width() )
          || ( s.height() != sheetview->canvas()->height() ) ) {
            sheetview->canvas()->resize( s.width(), s.height() );
            update();
            contentsDirty = TRUE;
        }
    }
}


void GESheetWindow::selectionChanged ( Q3CanvasItemList items )
{
    bool oneTextItem = ( items.count() == 1 )
                    && ( items[0]->rtti() == Q3CanvasItem::Rtti_Text );
    increaseTextSizeAction->setEnabled( oneTextItem );
    decreaseTextSizeAction->setEnabled( oneTextItem );
    DeleteAction->setEnabled( items.count() > 0 );
    editCopyAction->setEnabled( items.count() > 0 );
    editCutAction->setEnabled( items.count() > 0 );
    pushBackwardAction->setEnabled( items.count() > 0 );
    bringForwardAction->setEnabled( items.count() > 0 );
}


void GESheetWindow::selectThese ( Q3CanvasItemList items )
{
    statusBar()->message( "Changed sheet selection" );
    undoStack->push( new GEShCmdSelectItems( sheetview, items ) );
}


void GESheetWindow::changeTextItem ( Q3CanvasText* textItem, QString newtext )
{
    statusBar()->message( "Modified contents of text item" );
    undoStack->push( new GEShCmdChangeText( textItem, newtext ) );
    contentsDirty = TRUE;
}


void GESheetWindow::maybeChangedVisualizer ()
{
    // just to be safe, we must do this:
    statusBar()->message( "Opened large visualizer" );
    contentsDirty = TRUE;
    // I suppose this could be done better, with signals from GEVSmalls all the way up,
    // but it's not worth it for so small a gain right now.
}


void GESheetWindow::increaseText ()
{
    statusBar()->message( "Increased font size for text item" );
    undoStack->push( new GEShCmdResizeText(
        ( Q3CanvasText* )( sheetview->selectedItems()[0] ), TRUE ) );
    contentsDirty = TRUE;
}


void GESheetWindow::decreaseText ()
{
    statusBar()->message( "Decreased font size for text item" );
    undoStack->push( new GEShCmdResizeText(
        ( Q3CanvasText* )( sheetview->selectedItems()[0] ), FALSE ) );
    contentsDirty = TRUE;
}


void GESheetWindow::sheetContextMenu ( QPoint here, Q3PtrList<QAction> extraActions )
{
    Q3PopupMenu menu( this );

    if ( DeleteAction->isEnabled() ) DeleteAction->addTo( &menu );
    if ( editCutAction->isEnabled() ) editCutAction->addTo( &menu );
    if ( editCopyAction->isEnabled() ) editCopyAction->addTo( &menu );
    editPasteAction->addTo( &menu );
    menu.insertSeparator();

    if ( decreaseTextSizeAction->isEnabled() || increaseTextSizeAction->isEnabled() ) {
        if ( decreaseTextSizeAction->isEnabled() ) decreaseTextSizeAction->addTo( &menu );
        if ( increaseTextSizeAction->isEnabled() ) increaseTextSizeAction->addTo( &menu );
        menu.insertSeparator();
    }

    zoomInAction->addTo( &menu );
    zoomOutAction->addTo( &menu );
    if ( pushBackwardAction->isEnabled() ) pushBackwardAction->addTo( &menu );
    if ( bringForwardAction->isEnabled() ) bringForwardAction->addTo( &menu );

    if ( extraActions.count() ) {
        menu.insertSeparator();
        for ( QAction* a = extraActions.first() ; a ; a = extraActions.next() )
            a->addTo( &menu );
    }

    menu.exec( here );
}


void GESheetWindow::resizedItem ( Q3CanvasItem* item, int* origDims )
{
    int w;
    int h;
    bool sanity = GESheet::canResizeItem( item, w, h );
    if ( ( ( origDims[0] != item->x() ) || ( origDims[1] != item->y() )
        || ( origDims[2] != w ) || ( origDims[3] != h ) ) && sanity ) {
        statusBar()->message( "Resized rectangular sheet item" );
        undoStack->push( new GEShCmdResizedItem( item, origDims ) );
        contentsDirty = TRUE;
    }
}


void GESheetWindow::bringForward_activated ()
{
    statusBar()->message( "Brought sheet item forward" );
    undoStack->push( new GEShCmdPushSelectionZ( sheetview, FALSE ) );
    contentsDirty = TRUE;
}


void GESheetWindow::pushBackward_activated ()
{
    statusBar()->message( "Pushed sheet item backward" );
    undoStack->push( new GEShCmdPushSelectionZ( sheetview, TRUE ) );
    contentsDirty = TRUE;
}


void GESheetWindow::addedParameter ( Q3CanvasItem*, int num, int outOf )
{
    if ( num == outOf ) {
        statusBar()->message( QString( "Chose final item (of %1)" ).arg( outOf ) );
        contentsDirty = TRUE;
    } else if ( num == 1 ) {
        statusBar()->message( QString( "Chose first item (of %1)" ).arg( outOf ) );
    } else {
        statusBar()->message( QString( "Chose item %1 (of %2)" ).arg( num ).arg( outOf ) );
    }
}


void GESheetWindow::cancelledParameters ()
{
    statusBar()->message( "Cancelled action" );
}


bool GESheetWindow::loadSheet ( QString filename )
{
    // we use 0 as the parent to the messagebox so that even before this object has been
    // placed on screen (maybe not even visible) the client can call this method, and then
    // ditch this object on failure
    QString error = sheetview->sheet()->load( filename );
    if ( error.isNull() ) {
        saveFileName = filename;
        contentsDirty = FALSE;
        statusBar()->message( "Sheet loaded successfully" );
        return TRUE; // success
    }
    QMessageBox::warning( 0, "Error loading from file",
                          QString( "There was an error while attempting to load:\n"
                                   "%1\n"
                                   "No sheet was loaded." ).arg( error ),
                          QMessageBox::Ok, Qt::NoButton );
    return FALSE;
}


void GESheetWindow::editMorphism ( GECanvasMorphism* m )
{
    MorphismEditDialog dia;
    if ( dia.exec( m ) ) {
        GEltList newdoms = dia.getDomGens();
        GEltList newcods = dia.getCodGens();
        bool sName = dia.getShowName();
        bool sDomCod = dia.getShowDomCod();
        bool sEpiMon = dia.getShowEpiMon();
        bool sGens = dia.getShowGens();
        bool sMany = dia.getManyLines();
        if ( !elt_lists_are_equal( newdoms, m->domgens )
          || !elt_lists_are_equal( newcods, m->codgens )
          || ( sName != m->showName ) || ( sDomCod != m->showDomCod )
          || ( sEpiMon != m->showEpiMon ) || ( sGens != m->showGens )
          || ( sMany != m->manyLines ) ) {
            undoStack->push( new GEShCmdChangeMorphism(
                m, newdoms, newcods, sName, sDomCod, sEpiMon, sGens, sMany ) );
            statusBar()->message( "Edited morphism" );
            contentsDirty = TRUE;
        } else {
            statusBar()->message( "Left morphism unchanged" );
        }
    }
}


void GESheetWindow::pushSmallCommand ( QtCommand* cmd )
{
    // a small visualizer asked us to push this command onto our stack
    undoStack->push( cmd );
}


void GESheetWindow::registerNewSmall ( GEVSmall* smallv )
{
    // thanks for letting us know about this guy; that may be crucial to his functionality
    connect( smallv, SIGNAL(pushThisCommand(QtCommand*)),
             this, SLOT(pushSmallCommand(QtCommand*)) );
}


GESheetWindow* GESheetWindow::createNewWindow ( int sheetWidth, int sheetHeight )
{
    GESheetWindow* result = new GESheetWindow( 0, 0, Qt::WType_TopLevel | Qt::WDestructiveClose );
    GESheet* sh = new GESheet( sheetWidth, sheetHeight );
    result->setSheet( sh );
    return result;
}


void GESheetWindow::helpSheetsAction_activated()
{
    statusBar()->message( "Opened help on sheets" );
    GEHelpWindow::showHelp( "rf-um-sheetwindow.html" );
}
