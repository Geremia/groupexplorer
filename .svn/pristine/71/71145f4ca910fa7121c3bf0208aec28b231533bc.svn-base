//Added by qt3to4:
#include <Q3TextStream>
#include <QMoveEvent>
#include <QResizeEvent>
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


GEHelpWindow* GEHelpWindow::mainHelpWindow = 0;


void GEHelpWindow::init ()
{
    delete statusBar(); // no use for it here
    // in fact, this thing only inherits QMainWindow (as opposed to QWidget)
    // because only QMainWindows can have toolbars...I therefore think that
    // QMainWindow is a deceptive name...makes it sound like you can only have one.
    // Perhaps just QWindow would be better.
    browser = new GEHelpBrowser( this, resourcesPath() + "/help" );
    browser->setTextFormat( Qt::RichText );
    setCentralWidget( browser );
    
    connect( HomeAction, SIGNAL(activated()), browser, SLOT(home()) );
    connect( BackAction, SIGNAL(activated()), browser, SLOT(backward()) );
    connect( ForwardAction, SIGNAL(activated()), browser, SLOT(forward()) );
    connect( FontIncreaseAction, SIGNAL(activated()), browser, SLOT(zoomIn()) );
    connect( FontDecreaseAction, SIGNAL(activated()), browser, SLOT(zoomOut()) );
    connect( CopyAction, SIGNAL(activated()), browser, SLOT(copy()) );
    connect( helpSearchAction, SIGNAL(activated()),
             MainWindow::original(), SLOT(helpSearchAction_activated()) );
    
    Q3ToolBar* toolBar = toolBars( Qt::DockTop ).at( 0 );
    toolBar->addSeparator();
    
    undoStack = new QUndoStack( this );
#ifndef NO_QT_UNDO
    QAction* undoAction = undoStack->createUndoAction( browser );
    QAction* redoAction = undoStack->createRedoAction( browser );
    undoAction->setName( "undoAction" );
    redoAction->setName( "redoAction" );
    undoAction->setIcon( QIcon( qPixmapFromMimeSource( "undo" ) ) );
    redoAction->setIcon( QIcon( qPixmapFromMimeSource( "redo" ) ) );
    undoAction->setAccel( tr( "Ctrl+Z" ) );
    redoAction->setAccel( tr( "Ctrl+Y" ) );
    toolBar->addSeparator();
    (new ActionForwarder( this, undoAction ) )->addTo( toolBar );
    (new ActionForwarder( this, redoAction ) )->addTo( toolBar );
    Edit->insertSeparator();
    undoAction->addTo( Edit );
    redoAction->addTo( Edit );
#endif
    connect( browser, SIGNAL(actionRequested(QStringList)),
             this, SLOT(browserRequestedAction(QStringList)) );
    
    if ( mainHelpWindow ) {
        mainHelpWindow->notifyAboutNewWindow( this );
    }
    
    // and place What's This? button on toolbar
    toolBar->addSeparator();
    Q3WhatsThis::whatsThisButton( toolBar );
    // Then you add GE-specific What's This? help tips to things by doing:
    GEWhatsThis::setup( toolBar->child( "HomeAction_action_button" ),
                        "helpWindow-HomeAction" );
    GEWhatsThis::setup( toolBar->child( "BackAction_action_button" ),
                        "helpWindow-BackAction" );
    GEWhatsThis::setup( toolBar->child( "ForwardAction_action_button" ),
                        "helpWindow-ForwardAction" );
    GEWhatsThis::setup( toolBar->child( "FontIncreaseAction_action_button" ),
                        "helpWindow-FontIncreaseAction" );
    GEWhatsThis::setup( toolBar->child( "FontDecreaseAction_action_button" ),
                        "helpWindow-FontDecreaseAction" );
    GEWhatsThis::setup( toolBar->child( "CopyAction_action_button" ),
                        "helpWindow-CopyAction" );
    GEWhatsThis::setup( toolBar->child( "SaveAsHTMLAction_action_button" ),
                        "helpWindow-SaveAsHTMLAction" );
    GEWhatsThis::setup( toolBar->child( "PrintAction_action_button" ),
                        "helpWindow-PrintAction" );
    GEWhatsThis::setup( toolBar->child( "FindEdit" ),
                        "helpWindow-FindEdit" );
    GEWhatsThis::setup( toolBar->child( "FindButton" ),
                        "helpWindow-FindEdit" );
    GEWhatsThis::setup( toolBar->child( "helpSearchAction_action_button" ),
                        "helpWindow-helpSearchAction" );
    GEWhatsThis::setup( toolBar->child( "undoAction_action_button" ),
                        "helpWindow-undoAction" );
    GEWhatsThis::setup( toolBar->child( "redoAction_action_button" ),
                        "helpWindow-redoAction" );
    GEWhatsThis::setup( browser, "helpWindow-browser" );
    GEWhatsThis::setup( statusBar(), "helpWindow-statusBar" );
}


void GEHelpWindow::showHelp( QString doc )
{
    GEHelpWindow::getMainHelpWindow()->showTo( doc );
}


void GEHelpWindow::goTo( QString ref )
{
    browser->setSource( ref );
    setCaption( browser->documentTitle().isEmpty() ?
                QString( "Group Explorer Help" ) : browser->documentTitle() );
}

void GEHelpWindow::showTo( QString ref )
{
    goTo( ref );
    // if it was a link command, don't bother bringing this window to the fore;
    // if the link command wants that to happen, it will handle it itself.
    if ( !ref.startsWith( "do " ) ) {
        // but if it was not a link command, then we must bring this window
        // to the fore so that the user can see the document loaded:
        showAndFocus();
    }
}

void GEHelpWindow::showAndFocus ()
{
    show();
    setActiveWindow();
    raise();
    browser->setFocus();
}


void GEHelpWindow::browserRequestedAction( QStringList parameters )
{
    emit actionRequested( parameters, this );
}


GEHelpWindow* GEHelpWindow::getMainHelpWindow()
{
    // be sure the main help window exists:
    if ( !mainHelpWindow ) {
        mainHelpWindow = new GEHelpWindow ();
        // ensure that the main help window has the contents page as its home:
        mainHelpWindow->goTo( "index.html" );
    }
    return mainHelpWindow;
}

void GEHelpWindow::setText( QString doc )
{
    browser->setText( doc );
    if ( !doc.startsWith( "<qt type=detail>" ) )
        setCaption( browser->documentTitle().isEmpty() ?
                    QString( "Group Explorer Help" ) : browser->documentTitle() );
}


void GEHelpWindow::scrollToAnchor ( QString anchor )
{
    browser->scrollToAnchor( anchor );
}


void GEHelpWindow::notifyAboutNewWindow( GEHelpWindow * newwin )
{
    emit newHelpWindowCreated( newwin );
}


void GEHelpWindow::pushCommand( QUndoCommand * cmd )
{
//    qDebug( "help window is pushing command %s", cmd->text().latin1() );
    undoStack->push( cmd );
}


void GEHelpWindow::resizeEvent( QResizeEvent * e )
{
    Q3MainWindow::resizeEvent( e );
    emit gotResized();
}


void GEHelpWindow::moveEvent( QMoveEvent * e )
{
    Q3MainWindow::moveEvent( e );
    emit gotMoved();
}


void GEHelpWindow::SaveAsHTMLAction_activated()
{
    QString filename = Q3FileDialog::getSaveFileName( applicationPath(),
                                                     "HTML (*.htm *.html)" );
    if ( !filename.isEmpty() ) {
        QFile file( filename );
        if ( file.open( QIODevice::WriteOnly ) ) {
            Q3TextStream stream( &file );
            stream << removeHyperlinks( browser->text() );
            file.close();
        } else {
            QMessageBox::warning(
                    this,
                    "Error saving to file",
                    "There was an error opening the file\n"
                    "<tt>" + filename + "</tt>\n"
                    "for writing.  The HTML was not saved.",
                    QMessageBox::Ok, Qt::NoButton );
        }
    }
}


void GEHelpWindow::PrintAction_activated()
{
    Printing::dispatchPrintRichText( browser->text() );
}


void GEHelpWindow::FileCloseAction_activated()
{
    hide();
}


void GEHelpWindow::FindAction_activated()
{
    FindEdit->setFocus();
}


void GEHelpWindow::FindButton_clicked()
{
    browser->find( FindEdit->text(), FALSE, FALSE );
}


QString GEHelpWindow::title ()
{
    return browser->documentTitle();
}


