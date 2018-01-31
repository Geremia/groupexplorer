//Added by qt3to4:
#include <Q3ValueList>
#include <QResizeEvent>
#include <QMoveEvent>
#include <QCloseEvent>
//Added by me to help qt3to4:
#include <QDesktopWidget>
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


MainWindow* MainWindow::firstInstance = 0;


void MainWindow::fileNew()
{
    GESheetWindow::createNewWindow()->show();
    statusBar()->message( "Created new sheet" );
}


void MainWindow::openSheet ( QString filename )
{
    if ( filename.isEmpty() ) return;
    GESheetWindow* win = GESheetWindow::createNewWindow();
    if ( win->loadSheet( filename ) ) {
        win->show();
        statusBar()->message( "Opened sheet" );
    } else {
        delete win;
        statusBar()->message( "Cancelled opening sheet" );
    }
}


void MainWindow::fileOpen()
{
    openSheet( Q3FileDialog::getOpenFileName( applicationPath(),
                                             "Group Explorer sheets (*.sheet)" ) );
}


void MainWindow::fileSave()
{
    preferences->flush();
}


void MainWindow::fileSaveAs()
{
    // what is this doing here?  what is it for?
}


void MainWindow::closeEvent( QCloseEvent* ce )
{
    // this gives us the chance to save any modifications to any open sheets:
    if ( GESheetWindow::tryToQuitApplication() )
        ce->accept();
    else
        ce->ignore();
}


void MainWindow::editPaste()
{
    // not sure what this is intended to do...I don't think it's necessary
}


void MainWindow::editFind()
{
    searchDialog.getReady();
    statusBar()->message( "Opened search window" );
}


void MainWindow::helpAbout()
{
    QMessageBox::about( this,
                        QString( "About Group Explorer %1" )
                        .arg( GE_VERSION_STRING ),
                        QString( "Group Explorer is visualization software\n"
                                 "for the abstract algebra classroom,\n"
                                 "created by %1,\n%2" )
                        .arg( GE_AUTHOR )
                        .arg( GE_AUTHOR_EMAIL ) );
    statusBar()->message( "Viewed \"about\" screen" );
}


void MainWindow::createGlobalObjects() {
    // here we make some function calls and don't use the results.
    // this is because each of these functions creates some data the first time it
    // gets called, and that data is then ready for all future calls to the function.
    // (it is nice to do this up front during splash screen time with progress bar.)
    GroupIdeas::listAll();
    emit progressChanged( 100 );
    // add more here
}


void MainWindow::setConfigFilename ( QString filename )
{
    configFilename = filename;
}

void MainWindow::processConfigFile () {
    // load user's preferences from this source:
    QString format = "GroupExplorer";
    QString version = "2.0";
    QStringList colNames = mainTable->getBuiltInColumnNames();
    emit progressChanged( 10 );
    preferences = new GEPreferences( configFilename, format, version, colNames );
    // note that the GEPreferences() constructor handles ensuring sensible defaults
    if ( !preferences->fileState() || !preferences->formatState() ) {
        qWarning( "MainWindow::processConfigFile: "
                  "No configuration file loaded!  Assuming a blank one." );
    }
    emit progressChanged( 80 );
    // use any preferences applicable to this object and its charges:
    preferences->setGroup( "MainWindow" );
    move( preferences->getPosition( "MainWindow" ) );
    resize( preferences->getSize( "MainWindow" ) );
    QDesktopWidget dw; // for checking if the window is on screen;
    // if you had multiple screens before but not now, it might be offscreen, so fix:
    if ( dw.screenNumber( this ) == -1 )
        move( dw.screenGeometry( dw.primaryScreen() ).topLeft() );
    if ( preferences->getBool( "maximized" ) ) {
        setWindowState( windowState() | Qt::WindowMaximized );
    }
    preferences->setGroup( "MainHelpWindow" );
    GEHelpWindow::getMainHelpWindow()->move(
            preferences->getPosition( "MainHelpWindow" ) );
    GEHelpWindow::getMainHelpWindow()->resize(
            preferences->getSize( "MainHelpWindow" ) );
    if ( preferences->getBool( "maximized" ) ) {
        GEHelpWindow::getMainHelpWindow()->setWindowState(
                windowState() | Qt::WindowMaximized );
    }
    emit progressChanged( 100 );
}


void MainWindow::loadSheets( QStringList, uint, uint )
{
    preferences->setGroup( "MainWindow" );
    QStringList absFilePaths = findFilesRecursively(
        preferences->getStringList( "sheetsearchpaths" ), "*.sheet" );
    for ( int i = 0 ; i < absFilePaths.count() ; i++ ) {
        emit progressChanged( 10 + 90 * i / absFilePaths.count() );
        GESheet::groupsInSheet( absFilePaths[i] ); // do computation in advance + remember
    }
    emit progressChanged( 100 );
}


void MainWindow::checkFirstTime ()
{
    preferences->setGroup( "MainWindow" );
    if ( preferences->getBool( "firstTime" ) )
        GEHelpWindow::showHelp( "welcome.html" );
    preferences->setGroup( "MainWindow" );
    preferences->setBool( "firstTime", FALSE );
}


void MainWindow::setup()
{
    // make a splash screen before you do anything
    ProgressSplashScreen splash( Qt::WStyle_StaysOnTop );
    splash.setPixmapName( "biglogo.png" );
    QObject::connect( this, SIGNAL( progressChanged(uint) ),
                      &splash, SLOT( phaseProgressChanged(uint) ) );
    QObject::connect( this, SIGNAL( messageChanged(QString) ),
                      &splash, SLOT( progressMessageChanged(QString) ) );
    QObject::connect( this, SIGNAL( enterNewPhase(uint,uint) ),
                      &splash, SLOT( enterNewPhase(uint,uint) ) );
    QObject::connect( this, SIGNAL( finishPhase() ),
                      &splash, SLOT( finishPhase() ) );
    QObject::connect( mainTable, SIGNAL( progressChanged(uint) ),
                      &splash, SLOT( phaseProgressChanged(uint) ) );
    QObject::connect( mainTable, SIGNAL( messageChanged(QString) ),
                      &splash, SLOT( progressMessageChanged(QString) ) );
    QObject::connect( mainTable, SIGNAL( enterNewPhase(uint,uint) ),
                      &splash, SLOT( enterNewPhase(uint,uint) ) );
    QObject::connect( mainTable, SIGNAL( finishPhase() ),
                      &splash, SLOT( finishPhase() ) );
    QObject::connect( mainTable, SIGNAL( hideSplashScreen() ),
                      &splash, SLOT( hide() ) );
    QObject::connect( mainTable, SIGNAL( reshowSplashScreen() ),
                      &splash, SLOT( show() ) );
    splash.centerOnDesktop();
    splash.show();
    
    // 1. create all global objects (icons, ideas, etc.)
//    qDebug( "CREATING OBJECTS" );
    emit enterNewPhase( 0, 5 );
    splash.setText( "Creating objects..." );
    createGlobalObjects();
    emit finishPhase();
    qApp->processEvents();
    
    // 2. process config file
//    qDebug( "PROCESSING CONFIG FILE" );
    emit enterNewPhase( 5, 10 );
    splash.setText( "Processing configuration file..." );
    processConfigFile();
    emit finishPhase();
    qApp->processEvents();
    
    // 3. load all groups accessible from search paths
//    qDebug( "LOADING GROUPS" );
    emit enterNewPhase( 10, 35 );
    splash.setText( "Finding and loading groups..." );
    mainTable->setPreferences( preferences );
    preferences->setGroup( "MainWindow" );
    mainTable->loadGroups( preferences->getStringList( "groupsearchpaths" ) );
    emit finishPhase();
    qApp->processEvents();
    
    // 4. create all global objects (icons, ideas, etc.)
//    qDebug( "CREATING GLOBAL OBJECTS" );
    emit enterNewPhase( 35, 80 );
    splash.setText( "Populating main window..." );
    mainTable->fillTable();
    emit finishPhase();
    qApp->processEvents();
    
    // 5. find and record data about all sheets accessible from search paths
//    qDebug( "LOADING SHEETS" );
    emit enterNewPhase( 80, 100 );
    splash.setText( "Finding and processing sheets..." );
    loadSheets( QStringList(), 0, 100 );
    emit finishPhase();
    qApp->processEvents();
    
    // when done doing startup stuff, proceed as follows...
    show();
    splash.finish( this );
    checkFirstTime();
}


void MainWindow::init () {
    // prepare additional default mime source factory
    Q3MimeSourceFactory::addFactory( new GEVMimeSourceFactory( this ) );
    // initialize all private values
    if ( !firstInstance ) firstInstance = this; // should be only 1 anyhow
    preferences = 0;
    undoStack = new QtUndoStack( this );
    setConfigFilename( configFileName( "GroupExplorer" ) );
    optionsFirstTime = TRUE;
    // I stole this code from stuff uic generated:
    mainTable = new GroupTable( 0, 0, centralWidget(), "mainTable" );
    mainTable->setUndoStack( undoStack );
    centralWidget()->layout()->addWidget( mainTable );
    //MainWindowLayout->addWidget( mainTable );
    // make connections that can't be done in designer
    connect( mainTable, SIGNAL(requestedGroupInfo(QString)),
             this, SLOT(openGroupInfoWindow(QString)) );
    connect( &prefsDialog, SIGNAL(preferencesChanged(Q3ValueList<uint>)),
             this, SLOT(preferencesChanged(Q3ValueList<uint>)) );
    listenToHelpWindow( GEHelpWindow::getMainHelpWindow() );
    connect( &searchDialog, SIGNAL(actionRequested(QStringList,QWidget*)),
             this, SLOT(doLinkCommand(QStringList,QWidget*)) );
    connect( &searchDialog, SIGNAL(requestNumGroups(uint&)),
             this, SLOT(provideNumGroups(uint&)) );
    connect( &searchDialog, SIGNAL(requestGroupInfo(uint,QString&,QString&)),
             this, SLOT(provideGroupInfo(uint,QString&,QString&)) );
    connect( &searchDialog, SIGNAL(requestSheetPaths(QStringList&)),
             this, SLOT(provideSheetPaths(QStringList&)) );
    connect( GEHelpWindow::getMainHelpWindow(),
             SIGNAL(newHelpWindowCreated(GEHelpWindow*)),
             this,
             SLOT(listenToHelpWindow(GEHelpWindow*)) );
    connect( GEHelpWindow::getMainHelpWindow(),
             SIGNAL(gotResized()),
             this,
             SLOT(help_resizeEvent()) );
    connect( GEHelpWindow::getMainHelpWindow(),
             SIGNAL(gotMoved()),
             this,
             SLOT(help_moveEvent()) );
    // make undo/redo actions and put them on toolbar & menu
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
    editMenu->insertSeparator();
    undoAction->addTo( editMenu );
    redoAction->addTo( editMenu );
#endif
    // and place What's This? button on toolbar
    toolBar->addSeparator();
    Q3WhatsThis::whatsThisButton( toolBar );
    // Then you add GE-specific What's This? help tips to things by doing this:
    GEWhatsThis::setup( mainTable, "mainTable" );
    GEWhatsThis::setup( toolBar->child( "newSheetAction_action_button" ),
                        "mainWindow-newSheet" );
    GEWhatsThis::setup( toolBar->child( "fileOpenAction_action_button" ),
                        "mainWindow-fileOpen" );
    GEWhatsThis::setup( toolBar->child( "saveSettingsAction_action_button" ),
                        "mainWindow-saveSettings" );
    GEWhatsThis::setup( toolBar->child( "editCopyAction_action_button" ),
                        "mainWindow-editCopy" );
    GEWhatsThis::setup( toolBar->child( "editSearchAction_action_button" ),
                        "mainWindow-editSearch" );
    GEWhatsThis::setup( toolBar->child( "helpManualAction_action_button" ),
                        "mainWindow-helpManual" );
    GEWhatsThis::setup( toolBar->child( "helpTutorialsAction_action_button" ),
                        "mainWindow-helpTutorials" );
    GEWhatsThis::setup( toolBar->child( "helpSearchAction_action_button" ),
                        "mainWindow-helpSearch" );
    GEWhatsThis::setup( toolBar->child( "optionsAction_action_button" ),
                        "mainWindow-options" );
    GEWhatsThis::setup( toolBar->child( "undoAction_action_button" ),
                        "mainWindow-undoAction" );
    GEWhatsThis::setup( toolBar->child( "redoAction_action_button" ),
                        "mainWindow-redoAction" );
    GEWhatsThis::setup( statusBar(), "mainWindow-statusBar" );
}


void MainWindow::destroy () {
    preferences->flush();
    delete preferences;
}


void MainWindow::optionsAction_activated () {
    if ( optionsFirstTime ) {
        prefsDialog.readPreferences( preferences );
        prefsDialog.resize( sizeHint() );
        QPoint ctr = rect().center();
        prefsDialog.move( ctr.x() - prefsDialog.width() / 2,
                          ctr.y() - prefsDialog.height() / 2 );
        optionsFirstTime = FALSE;
    }
    statusBar()->message( "Opened options window" );
    prefsDialog.niceShow();
}


void MainWindow::resizeEvent ( QResizeEvent* e ) {
    if ( preferences ) {
        // when this object is created, preferences doesn't exist,
        // and yet resizeEvent() may get called.  d'oh.  hence this "if".
        preferences->setGroup( "MainWindow" );
        preferences->setBool( "maximized", isMaximized() );
        if ( !isMaximized() ) {
            preferences->setSize( "MainWindow", size() );
        }
    }
    Q3MainWindow::resizeEvent( e );
}

void MainWindow::moveEvent ( QMoveEvent* e ) {
    if ( preferences ) {
        // when this object is created, preferences doesn't exist,
        // and yet moveEvent() may get called.  d'oh.  hence this "if".
        preferences->setGroup( "MainWindow" );
        preferences->setBool( "maximized", isMaximized() );
        if ( !isMaximized() ) {
            preferences->setPosition( "MainWindow", pos() );
        }
    }
    Q3MainWindow::moveEvent( e );
}


void MainWindow::help_resizeEvent () {
    if ( preferences ) {
        // in case main window gets created (and therefore sized) before the
        // preferences object has been created
        preferences->setGroup( "MainHelpWindow" );
        preferences->setBool( "maximized",
                              GEHelpWindow::getMainHelpWindow()->isMaximized() );
        if ( !isMaximized() ) {
            preferences->setSize( "MainHelpWindow", 
                                  GEHelpWindow::getMainHelpWindow()->size() );
        }
    }
}

void MainWindow::help_moveEvent () {
    if ( preferences ) {
        // in case main window gets created (and therefore placed) before the
        // preferences object has been created
        preferences->setGroup( "MainHelpWindow" );
        preferences->setBool( "maximized", 
                              GEHelpWindow::getMainHelpWindow()->isMaximized() );
        if ( !isMaximized() ) {
            preferences->setPosition( "MainHelpWindow", 
                                      GEHelpWindow::getMainHelpWindow()->pos() );
        }
    }
}


void MainWindow::preferencesChanged( Q3ValueList<uint> categories )
{
    MWCmdApplyNewPreferences* prefchangecmd = new MWCmdApplyNewPreferences(
            this, prefsDialog.oldPreferences(), preferences, categories );
    undoStack->push( prefchangecmd );
}

void MainWindow::applyPreferenceCategory ( uint cat )
{
    switch ( cat ) {
    case PreferencesDialog::PGFiles:
        QMessageBox::warning(
                this,
                "Warning: Some changes will not take effect",
                "<b>Warning:</b> The changes you made to the "
                "paths Group Explorer examines when loading "
                "will not take effect until the next time "
                "you run Group Explorer." );
        break;
    case PreferencesDialog::PGOther:
        QMessageBox::warning(
                this,
                "Warning: Some changes will not take effect",
                "<b>Warning:</b> If you made changes to the "
                "maximum group order or file size Group Explorer "
                "uses when loading, those changes will not take "
                "effect until the next time you run Group Explorer." );
        break;
    }
    statusBar()->message( "Changed preferences" );
}


void MainWindow::openGroupInfoWindow( QString code, QString anchor )
{
    GroupFile* gf = GroupTable::findGroup( code );
    if ( gf ) {
        if ( !groupInfoWindows.contains( code ) )
            groupInfoWindows[code] = new GEHelpWindow( 0, 0, 0 );
        GEHelpWindow* hw = groupInfoWindows[code];
        hw->setText( gf->groupInfoHTML( preferences ) );
        if ( !anchor.isNull() ) hw->scrollToAnchor( anchor );
        hw->showAndFocus();
        statusBar()->message( "Opened group info for " + gf->getPrimaryName().text );
    } else {
        statusBar()->message( "Could not open group \"" +
                              GroupFile::decodePrimaryName( code ) + "\"" );
    }
}


void MainWindow::openGroupInfoWindow( QString code )
{
    openGroupInfoWindow( code, QString::null );
}


void MainWindow::helpManualAction_activated()
{
    GEHelpWindow::showHelp( "index.html" );
    statusBar()->message( "Opened help" );
}


void MainWindow::helpTutorialsAction_activated()
{
    GEHelpWindow::showHelp( "tu-index.html" );
    statusBar()->message( "Opened tutorials" );
}


void MainWindow::doLinkCommand( QStringList parameters, QWidget* originator )
{
    // NOTE!!!!!!!!!!!!!!!!
    // this is only HERE (as opposed to elsewhere) because it has access to undo stacks.
    // if your link commands don't ever need to be pushed onto an undo stack, you can
    // run them yourself; create them like in the next line, then call redo() in them.
    LinkCommand* newguy = LinkCommands::getNewCommand( parameters, originator );
    if ( newguy ) { // in case erroneous command identifier gives newguy == 0
        if ( newguy->belongsOnStack() ) {
            // if the originator is one of my help windows, and can therefore keep
            // locally this command on its own undo stack, let's do that:
            if ( originator->className() == "GEHelpWindow" ) {
                GEHelpWindow* hw = (GEHelpWindow*)originator;
                hw->pushCommand( newguy ); // gets auto-run by this call
            } else {
                undoStack->push( newguy ); // gets auto-run by this call
            }
        } else {
            newguy->redo(); // manually run him
            delete newguy; // no one owns him, so better clean up here; bye-bye!
        }
    } else {
        qWarning( "Error: No link command created for \""
                  + parameters.join( " " ) + "\"" );
    }
}


void MainWindow::listenToHelpWindow( GEHelpWindow * hw )
{
    connect( hw, SIGNAL(actionRequested(QStringList,QWidget*)),
             this, SLOT(doLinkCommand(QStringList,QWidget*)) );
}


/*
GroupFile* MainWindow::findGroup( QString code )
{
    return mainTable->findGroup( code );
}
*/

void MainWindow::provideNumGroups( uint& n )
{
    n = mainTable->numRows();
}

void MainWindow::provideGroupInfo( uint i, QString& html, QString& code )
{
    html = mainTable->groupAtRow( i )->groupInfoHTML( preferences );
    code = mainTable->groupAtRow( i )->codeName();
}

void MainWindow::provideSheetPaths( QStringList& paths )
{
    preferences->setGroup( "MainWindow" );
    paths = preferences->getStringList( "sheetsearchpaths" );
}


void MainWindow::helpSearchAction_activated()
{
    searchDialog.getReadyForHelp();
    statusBar()->message( "Opened search window" );
}


MainWindow* MainWindow::original()
{
    return firstInstance;
}


void MainWindow::editCopyAction_activated()
{
    Q3TextDrag* data = new Q3TextDrag( mainTable->selectionAsHTML() );
    data->setSubtype( "html" );
    QApplication::clipboard()->setData( data, QClipboard::Clipboard );
    statusBar()->message( "Copied data from group library" );
}


