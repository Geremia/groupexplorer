//Added by qt3to4:
#include <QCloseEvent>
#include <QPixmap>
#include <Q3PtrList>
#include <QDebug>
#include <QToolButton>
#include <Q3VBox>
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

void GEVLarge::init()
{
    delete statusBar(); // no use for it here
    
    isIndependent = FALSE;
    visualizer = 0;
    // isIndependent means that this GEVLarge is not connected to a GEVSmall that
    // exists in a sheet anywhere, and thus when it is closed, it should destroy
    // itself so that its associated GEVisualizer can also die and free up memory.
    typeName = "Visualizer"; // descendants should call setTypeName() *after* this
    
    Q3VBox* vbox = new Q3VBox( this );
    setCentralWidget( vbox );

    toolBar = new QToolBar( vbox );

    //setCentralWidget( splitter = new QSplitter( Qt::Horizontal, this ) );
    splitter = new QSplitter( Qt::Horizontal, vbox );
    splitter->setOpaqueResize( TRUE );
    
    toolBar->addAction( fileSaveAction );
    toolBar->addAction( filePrintAction );
    toolBar->addSeparator();
    toolBar->addWidget( Q3WhatsThis::whatsThisButton( toolBar ) );
#ifndef NO_QT_UNDO
    QAction *undoAction = QtUndoManager::manager()->createUndoAction( this );
    QAction *redoAction = QtUndoManager::manager()->createRedoAction( this );
    undoAction->setName( "undoAction" );
    redoAction->setName( "redoAction" );
    undoAction->setAccel( tr( "Ctrl+Z" ) );
    redoAction->setAccel( tr( "Ctrl+Y" ) );
    toolBar->addSeparator();
    undoAction->addTo( toolBar );
    redoAction->addTo( toolBar );
    //editMenu->insertSeparator();//there's nothing in the darn edit menu yet
    undoAction->addTo( editMenu );
    redoAction->addTo( editMenu );
#endif
    undoStack = new QtUndoStack( this );

    GEWhatsThis::setup( toolBar->child( "fileSaveAction_action_button" ),
                        "largeWindow-fileSaveAction" );
    GEWhatsThis::setup( toolBar->child( "filePrintAction_action_button" ),
                        "largeWindow-filePrintAction" );
    GEWhatsThis::setup( toolBar->child( "undoAction_action_button" ),
                        "largeWindow-undoAction" );
    GEWhatsThis::setup( toolBar->child( "redoAction_action_button" ),
                        "largeWindow-redoAction" );
    GEWhatsThis::setup( statusBar(), "largeWindow-statusBar" );
}


QPixmap GEVLarge::currentPixmap ()
{
    qWarning( "GEVLarge::currentPixmap() should be overridden in subclasses; "
              "right now returning a plain gray pixmap" );
    QImage result( 50, 50, 32 );
    result.fill( 0x505050 );
    return QPixmap( result );
}

void GEVLarge::fileSave()
{
    SaveImageSettingsDialog dia( this );
    QPixmap tmp = currentPixmap();
    dia.exec( tmp, applicationPath() );
}


void GEVLarge::filePrint()
{
    PrintImageSettingsDialog dia( this );
    QPixmap tmp = currentPixmap();
    dia.exec( tmp );
}


void GEVLarge::fileExit()
{
    close();
}


void GEVLarge::editCopy()
{
}


void GEVLarge::helpManualAction_activated ()
{
    GEHelpWindow::showHelp( "index.html" );
}

void GEVLarge::helpTutorialsAction_activated ()
{
    GEHelpWindow::showHelp( "tu-index.html" );
}


void GEVLarge::setIndependent( bool i )
{
    isIndependent = i;
}


void GEVLarge::closeEvent( QCloseEvent * e )
{
    if ( isIndependent ) deleteLater();
    e->accept();
}


void GEVLarge::addLargeWidget ( GEVLargeWidget * w, bool expanding )
{
    largeWidgets.append( w );
    
    connect( this, SIGNAL(changed(QString,QString,QString)),
             w, SLOT(processChange(QString,QString,QString)) );
    connect( w, SIGNAL(changed(QString,QString,QString)),
             this, SIGNAL(childChanged(QString,QString,QString)) );
    connect( this, SIGNAL(groupFileChanged(GroupFile*)),
             w, SLOT(groupFileChanged(GroupFile*)) );
    if ( w->inherits( "SubsetListBox" ) ) {
        connect( w, SIGNAL(getAllMorphismsToMe(Q3PtrList<GECanvasMorphism>&)),
                 this, SIGNAL(getAllMorphismsToMe(Q3PtrList<GECanvasMorphism>&)) );
        connect( w, SIGNAL(getAllMorphismsFromMe(Q3PtrList<GECanvasMorphism>&)),
                 this, SIGNAL(getAllMorphismsFromMe(Q3PtrList<GECanvasMorphism>&)) );
        connect( this, SIGNAL(suggestComputedSubset(GEltList,QString)),
                 w, SLOT(suggestComputedSubset(GEltList,QString)) );
    }
    connect( w, SIGNAL(reset()), this, SIGNAL(reset()) );
    
    if ( !expanding ) splitter->setResizeMode( w, QSplitter::KeepSize );
    
    Q3PtrList<QAction> tmp = w->actionsToExport();
    if ( !tmp.isEmpty() ) {
        editMenu->insertSeparator();
        toolBar->addSeparator();
        for ( QAction* a = tmp.first() ; a ; a = tmp.next() ) {
            if ( a->menuText().isEmpty() ) {
                // this means that it's not an action, but a flag for a separator
                editMenu->insertSeparator();
                toolBar->addSeparator();
            } else {
                // it's really an action, so insert it
                a->addTo( editMenu );
                a->addTo( toolBar );
            }
        }
    }
    
    adjustSize();
}


void GEVLarge::processChange ( QString key, QString value, QString desc )
{
    emit changed( key, value, desc );
}


void GEVLarge::pushUndoCommand ( QtCommand * cmd )
{
    undoStack->push( cmd );
}


void GEVLarge::vizGroupFileChanged ( GroupFile * gf )
{
    setCaption( QString( "%1 for %2" )
                .arg( typeName ).arg( gf->getPrimaryName().text ) );
    emit groupFileChanged( gf );
}


void GEVLarge::setTypeName( QString n )
{
    typeName = n;
}


QStringList GEVLarge::keys()
{
    QStringList result;
    for ( uint i = 0 ; i < largeWidgets.count() ; i++ ) {
        QStringList news = largeWidgets.at( i )->keys();
        for ( int j = 0 ; j < news.count() ; j++ )
            if ( !result.contains( news[j] ) ) result << news[j];
    }
    return result;
}


QPoint GEVLarge::elementLocationPercent ( GElt /*e*/ )
{
    return QPoint( -1, -1 ); // subclasses should override!
}


void GEVLarge::ideasPushed ()
{
    /*
    QPopupMenu m( this );
    QPtrList<IdeaFactory<GEVisualizer> >* vifs = VisualizerIdeas::listAll();
    for ( uint i = 0 ; i < vifs->count() ; i++ ) {
        IdeaFactory<GEVisualizer>* fact = vifs->at( i );
        Idea<GEVisualizer> idea = fact->consider( *visualizer );
        if ( idea.applies ) m.insertItem( fact->question, i );
    }
    if ( !m.count() ) {
        m.insertItem( "(no ideas available right now)", -1 );
        m.exec( mapToGlobal( centralWidget()->pos() ) );
        return;
    }
    int result = m.exec( mapToGlobal( centralWidget()->pos() ) );
    if ( result > -1 ) {
        IdeaFactory<GEVisualizer>* fact = vifs->at( result );
        Idea<GEVisualizer> idea = fact->consider( *visualizer );
        if ( idea.longDescription.isNull() ) {
            QString command = idea.computedValue;
            if ( command.startsWith( "do " ) ) command = command.mid( 3 );
            LinkCommand* cmd = LinkCommands::getNewCommand(
                QStringList::split( " ", command ), this );
            if ( cmd ) {
                cmd->redo();
                delete cmd;
            }
        } else {
            GEHelpWindow* hw = new GEHelpWindow();
            hw->setText( QString( "<h1>%1</h1>\n<hr>\n%2" )
                         .arg( fact->title ).arg( idea.longDescription ) );
            hw->showAndFocus();
        }
    }
    */
}


