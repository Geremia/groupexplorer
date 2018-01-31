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


#include "myutils.h"
#include "grouptable.h"
#include "idealist.h"
#include "mathtableitem.h"
#include "gehelpwindow.h"
#include "ge.h"
#include <q3progressdialog.h>
#include <q3popupmenu.h>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include <qregexp.h>
#include <qevent.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <QContextMenuEvent>
#include <QPixmap>
#include <math.h>
//Added to help Qt4 version:
#include <q3mimefactory.h>


#define NO_SORTING "--"


GroupTable* GroupTable::firstTable = 0;


GroupTable::GroupTable ( int numRows, int numCols, QWidget * parent,
                         const char * name )
    : Q3Table( numRows, numCols, parent, name )
{
    undoStack = 0;//new QUndoStack( this );
    // set up necessary properties:
    setRowMovingEnabled( TRUE );
    setColumnMovingEnabled( TRUE );
    setSelectionMode( Single );
    // initialize built-in table columns:
    tableColumns
            << "Name"
            << "Order"
            << "Definition"
            << "Name as phrase"
            << "Cayley diagram"
            << "Multiplication table"
            << "Object of symmetry"
            << "Cycle graph"
            << "Other names"
            << "Filename"
            << "Path"
            << "Author"
            << "Date modified";
    numColumns = tableColumns.size(); // i.e. num built-in columns
    // add appropriate GroupIdea table columns:
    for ( uint i = 0 ; i < GroupIdeas::listAll()->count() ; i++ ) {
        tableColumns << GroupIdeas::listAll()->at(i)->title;
    }
    // initialize some members to empty:
    lastPrefs = 0;
    colJustSorted = -1;
    // make some connections, because we're not building this widget in designer:
    connect( this, SIGNAL(doubleClicked(int,int,int,const QPoint&)),
             this, SLOT(doubleClickedRow(int,int)) );
    connect( horizontalHeader(), SIGNAL( clicked(int) ),
             this, SLOT(columnHeaderClicked(int)) );
    connect( horizontalHeader(), SIGNAL(indexChange(int,int,int)),
             this, SLOT(columnsRearranged(int,int,int)) );
    connect( horizontalHeader(), SIGNAL(sizeChange(int,int,int)),
             this, SLOT(columnSizeChanged(int,int,int)) );
    connect( verticalHeader(), SIGNAL(indexChange(int,int,int)),
             this, SLOT(rowsRearranged(int,int,int)) );
    connect( this, SIGNAL(contextMenuRequested(int,int,const QPoint&)),
             this, SLOT(contextMenuRequested(int,int,const QPoint&)) );
    connect( this, SIGNAL(valueChanged(int,int)),
             this, SLOT(postEditCommand(int,int)) );
    horizontalHeader()->installEventFilter( this );
    // record this one if it's new
    if ( !firstTable ) firstTable = this;
}

bool GroupTable::eventFilter ( QObject* o, QEvent* e )
{
    if ( ( o == horizontalHeader() ) && ( e->type() == QEvent::ContextMenu ) ) {
        QContextMenuEvent* cme = (QContextMenuEvent*)e;
        int column = columnAt( cme->pos().x() + contentsX() );
        QString colname = horizontalHeader()->label( column );
        QMenu contextMenu( this );
        connect( &contextMenu, SIGNAL(activated(int)), this, SLOT(headerMenu(int)) );
        bool found = FALSE;
        if ( !colname.isNull() ) {
            contextMenu.insertItem( "Hide " + colname, 1000 + column );
            found = TRUE;
        }
        if ( lastPrefs ) {
            if ( lastPrefs->getUserDefinedColumns().contains( colname ) ) {
                contextMenu.insertItem( "Rename " + colname,
                                        5000 + colNameToPos[colname] );
                contextMenu.insertItem( "Delete " + colname,
                                        3000 + colNameToPos[colname] );
                found = TRUE;
            }
        }
        if ( found ) contextMenu.insertSeparator();
        found = FALSE;
        for ( int i = 0 ; i < horizontalHeader()->count() ; i++ ) {
            if ( ( i != column ) && isColumnHidden( i ) ) {
                contextMenu.insertItem( "Show " + horizontalHeader()->label( i ),
                                        2000 + i );
                found = TRUE;
            }
        }
        if ( lastPrefs ) {
            if ( found ) contextMenu.insertSeparator();
            contextMenu.insertItem( "Create new column", 4000 );
        }
        contextMenu.exec( cme->globalPos() );
        return TRUE; // true means we handled this event, so don't pass it on
    }
    // call parent class's filter...necessary in tables:
    return Q3Table::eventFilter( o, e );
}

void GroupTable::headerMenu ( int id )
{
    int numcols = horizontalHeader()->count();
    if ( ( 1000 <= id ) && ( id < 1000 + numcols ) ) {
        undoStack->push( new GTCmdHideShowColumn( this, id - 1000, TRUE ) );
    } else if ( ( 2000 <= id ) && ( id < 2000 + numcols ) ) {
        undoStack->push( new GTCmdHideShowColumn( this, id - 2000, FALSE ) );
    } else if ( ( 3000 <= id ) && ( id < 3000 + numcols ) ) {
        // I'll assume id - 3000 points to a user-defined column...
        // can't see how it wouldn't...*shrug*
        undoStack->push( new GTCmdDeleteColumn( this, id - 3000 ) );
    } else if ( id == 4000 ) {
        bool ok;
        QString text = QInputDialog::getText(
                "Create new column",
                "You are creating a new column for the main group table.\n"
                "Enter the heading for this new column:",
                QLineEdit::Normal, QString::null, &ok, this );
        if ( !ok ) return; // user pressed cancel
        if ( text.isEmpty() ) {
            // user entered nothing
            QMessageBox::warning( this, "Empty column name not allowed",
                                  "You did not enter a column name.\n"
                                  "A new column has not been created." );
            return;
        }
        if ( colNameToPos.contains( text ) ) {
            QMessageBox::warning( this, "Column name in use",
                                  "A column by the name <b>" + text + "</b> "
                                  "already exists.\n"
                                  "A new column has not been created.  "
                                  "You will need to choose a different name." );
            return;
        }
        // passed all above checks, so let's make the thing:
        undoStack->push( new GTCmdCreateColumn( this, text ) );
    } else if ( ( 5000 <= id ) && ( id < 5000 + numcols ) ) {
        // I'll assume id - 5000 points to a user-defined column...
        // can't see how it wouldn't...*shrug*
        bool ok;
        QString oldname = horizontalHeader()->label( id - 5000 );
        QString text = QInputDialog::getText(
                "Rename column",
                "You are renaming the column \"" + oldname + "\""
                " in the main group table.\n"
                "Enter the new heading for this column:",
                QLineEdit::Normal, oldname, &ok, this );
        if ( !ok ) return; // user pressed cancel
        if ( text.isEmpty() ) {
            // user entered nothing
            QMessageBox::warning( this, "Empty column name not allowed",
                                  "You did not enter a column name.\n"
                                  "The column has not been renamed." );
            return;
        }
        if ( colNameToPos.contains( text ) ) {
            QMessageBox::warning( this, "Column name in use",
                                  "A column by the name <b>" + text + "</b> "
                                  "already exists.\n"
                                  "The column has not been renamed.  "
                                  "You will need to choose a different name." );
            return;
        }
        // passed all above checks, so let's make the thing:
        undoStack->push( new GTCmdRenameColumn( this, id - 5000, text ) );
    }
}

GroupFile* GroupTable::findGroup ( QString code, bool warning )
{
    for ( int i = 0 ; firstTable && ( i < firstTable->groups.size() ) ; i++ ) {
        if ( firstTable->groups[i].codeName() == code )
            return &(firstTable->groups[i]);
    }
    if ( warning ) {
        if ( firstTable ) { // table exists, group not in it
            QMessageBox::warning( 0, "Group not found",
                QString( "The group that you need, %1 by\n"
                         "%2, is not currently loaded.\n"
                         "If you have it on your computer, you will need\n"
                         "to set Group Explorer's preferences to ensure\n"
                         "that group is loaded at startup.\n"
                         "These include preferences about search directories,\n"
                         "file size, and group order." )
                .arg( GroupFile::decodePrimaryName( code ) )
                .arg( GroupFile::decodeAuthor( code ) ),
                QMessageBox::Ok, QMessageBox::NoButton );
        } else {
            QMessageBox::warning( 0, "Group library not available",
                "You tried to open a group, but no group library exists.\n"
                "This should never happen, and should be reported as a\n"
                "bug to " GE_AUTHOR " at " GE_AUTHOR_EMAIL ".",
                QMessageBox::Ok, QMessageBox::NoButton );
        }
    }
    return 0;
}

int GroupTable::numGroups ()
{
    return firstTable ? ( firstTable->groups.size() ) : 0;
}

GroupFile* GroupTable::getGroup ( uint i )
{
    return ( firstTable && ( (int)i < firstTable->groups.size() ) ) ?
           &(firstTable->groups[i]) : 0;
}

int GroupTable::findGroupRow ( QString code )
{
    QString priname = GroupFile::decodePrimaryName( code );
    QString author = GroupFile::decodeAuthor( code );
    for ( int i = 0 ; i < numRows() ; i++ ) {
        GroupFile* gf = groupAtRow( i );
        if ( ( gf->getPrimaryName().text == priname ) && ( gf->author == author ) )
            return i;
    }
    return -1;
}


QStringList GroupTable::getBuiltInColumnNames () {
    return tableColumns;
}

void GroupTable::setPreferences ( GEPreferences *prefs ) {
    lastPrefs = prefs;
}
    
void GroupTable::loadGroups ( QStringList paths ) {
    QStringList absFilePaths = findFilesRecursively( paths, "*.group" );
    QStringList::Iterator it;
    emit progressChanged( 10 );
    int maxGroupFileSize = -1;
    int maxGroupOrder = -1;
    if ( lastPrefs ) {
        lastPrefs->setGroup( "MainWindow" );
        maxGroupFileSize = lastPrefs->getNumber( "maximumGroupFileSize", -1 );
        maxGroupOrder = lastPrefs->getNumber( "maximumGroupOrder", -1 );
    }
    uint numskips = 0;
    for ( it = absFilePaths.begin() ; it != absFilePaths.end() ; ++it ) {
        QFile f( *it );
        QFileInfo fi( f );
        // first scan the file for group order and make sure that is not too large
        if ( maxGroupOrder > -1 ) {
            if ( !f.open( QIODevice::ReadOnly ) ) {
                qWarning( QString( "Could not open file %1" ).arg( *it )
                          .toLatin1().constData() );
                continue;
            }
            QString tmp;
            char buf[2048];
            QRegExp re( "\\s*<row>([0-9 ]+)<\\/row>\\s*" );
            const int Max = 50;
            int max = Max;
            do {
                f.readLine( buf, sizeof( buf ) );
                tmp = buf;
                max--;
            } while ( ( max > 0 ) && !re.exactMatch( tmp ) );
            if ( max ) {
                int order = re.cap( 1 ).split( ' ' ).count();
                if ( order > maxGroupOrder ) {
                    emit messageChanged( "skipping " + fi.baseName( TRUE ) );
                    numskips++;
                    emit progressChanged(
                        10 + 90 * ( groups.size() + numskips ) / absFilePaths.size() );
                    continue;
                }
            } else {
                qWarning( QString( "Warning: could not find multiplication table "
                                   "in first %1 lines of file %2" )
                          .arg( Max ).arg( *it ).toLatin1().constData() );
            }
            f.close();
        }
        // next check to be see the file itself is not too large
        if ( maxGroupFileSize > -1 ) {
            if ( fi.size() > (uint)maxGroupFileSize ) {
                int powerOf10 = (int)pow( 10, floor( log10( fi.size() ) ) );
                int newSize = (int)( ceil( (double)fi.size() /
                                           (double)powerOf10 ) * powerOf10 );
                emit hideSplashScreen(); // or we won't see the dialog box!
                int choice = QMessageBox::warning(
                        0,
                        "Group file too large",
                        QString( "The following group file is "
                                 "<b>larger than %1 bytes</b>:<br>\n"
                                 "<tt>%2</tt>\n"
                                 "(Actual size: %3 bytes)<br>\n"
                                 "<br>\n"
                                 "Be careful with large group files, because they "
                                 "may contain group data for enormous groups that "
                                 "will slow down the operation of Group Explorer "
                                 "as a whole.<br>\n"
                                 "<br>\n"
                                 "What should Group Explorer do with this file?" )
                        .arg( maxGroupFileSize ).arg( *it ).arg( fi.size() ),
                        "Ignore once",
                        "Load once",
                        QString( "Load anything up to %1 bytes" ).arg( newSize ),
                        0, 0 );
                emit reshowSplashScreen(); // ok now it can come back
                if ( choice == 0 ) {
                    // just skip to next for loop iteration--don't load the file
                    continue;
                }
                // if choice == 1 we just move on to loading the file
                if ( choice == 2 ) {
                    // before moving on to load the file, up the maximum
                    lastPrefs->setNumber( "maximumGroupFileSize", 
                                          maxGroupFileSize = newSize );
                }
            }
        }
        // since both those tests passed, load the file
        GroupFile gf;
        GroupFile* duplicate;
        if ( !gf.load( *it ) ) continue;
        emit messageChanged( gf.fileInfo.baseName( TRUE ) );
        if ( ( duplicate = findGroup( gf.codeName(), FALSE ) ) ) { // no warning msgs
            // uh-oh...we already have one with this name + author! problem!
            emit hideSplashScreen(); // or we won't see the dialog box!
            QMessageBox::critical(
                0, "Duplicate group files",
                QString( "The following two group files contain groups\n"
                         "whose primary names and author's names match\n"
                         "exactly:<br>\n"
                         "<br>\n"
                         "<tt>%1</tt><br>\n"
                         "<tt>%2</tt><br>\n"
                         "<br>\n"
                         "This is not legal in Group Explorer, and thus\n"
                         "the latter file has not been loaded.  Alter one\n"
                         "of the files in order to have them both show up\n"
                         "the next time you start up Group Explorer." )
                .arg( duplicate->fileInfo.absFilePath() )
                .arg( gf.fileInfo.absFilePath() ),
                QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton );
            emit reshowSplashScreen(); // ok now it can come back
            emit progressChanged(
                10 + 90 * ( groups.size() + numskips ) / absFilePaths.size() );
            continue; // abandon the one we just loaded--don't put it in groups[]
        }
        groups.append( gf );
        emit progressChanged(
            10 + 90 * ( groups.size() + numskips ) / absFilePaths.size() );
    }
}


void GroupTable::fillColumn ( uint col )
        // this routine assumes lastPrefs is defined...
        // if !lastPrefs, some of what this routine is supposed to do won't happen
        // (no errors, just missing functionality)
{
    int c = colNameToStdPos[colPosToName[col]];
    QStringList userCols;
    if ( lastPrefs ) userCols = lastPrefs->getUserDefinedColumns();
    
    // if this is not at startup, we need to show a progress dialog:
    Q3ProgressDialog* progress = 0;
    if ( !isAtStartup ) {
        progress = new Q3ProgressDialog(
                QString( "Computing \"%1\" values..." )
                .arg( horizontalHeader()->label( col ) ),
                0, groups.count(), 0, 0 );
        progress->setModal( TRUE );
    }
    
    // for each group, drop the appropriate information in the cell:
    for ( int r = 0 ; r < (int)groups.count() ; r++ ) {
        uint orig = isAtStartup ? r : rowOriginalPosition( r );
        GETableItem* geti;
        switch ( c ) {
        case 0:            
            geti = new GETableItem( this, orig, groups[orig].getPrimaryName(), 16 );
            break;
        case 1:
            geti = new GETableItem( this, orig, groups[orig].group.order );
            geti->setLink( "rf-groupterms.html#order" );
            break;
        case 2:
            geti = new GETableItem(
                    this, orig, groups[orig].getPrimaryDefinition() );
            geti->setLink( "rf-groupterms.html#gensrels" );
            break;
        case 3:
            geti = new GETableItem( this, orig, groups[orig].phrasename );
            break;
        case 4:
            geti = new GETableItem( this, orig, qPixmapFromMimeSource(
                "CayleyDiagram/" + groups[orig].codeName() ) );
            geti->setLink( "rf-groupterms.html#cayleydiagram" );
            break;
        case 5:
            geti = new GETableItem( this, orig, qPixmapFromMimeSource(
                "MultiplicationTable/" + groups[orig].codeName() ) );
            geti->setLink( "rf-groupterms.html#multtable" );
            break;
        case 6:
            geti = new GETableItem( this, orig, qPixmapFromMimeSource(
                "SymmetryObject/" + groups[orig].codeName() ) );
            geti->setLink( "rf-groupterms.html#symmetryobject" );
            break;
        case 7:
            geti = new GETableItem( this, orig, qPixmapFromMimeSource(
                "CycleGraph/" + groups[orig].codeName() ) );
            geti->setLink( "rf-groupterms.html#cyclegraph" );
            break;
        case 8:
            geti = new GETableItem( this, orig, groups[orig].getOtherNames() );
            break;
        case 9:
            geti = new GETableItem( this, orig, groups[orig].fileInfo.fileName() );
            break;
        case 10:
            geti = new GETableItem( this, orig, groups[orig].fileInfo.dirPath() );
            break;
        case 11:
            geti = new GETableItem( this, orig, groups[orig].author );
            break;
        case 12:
            geti = new GETableItem(
                    this, orig, groups[orig].fileInfo.lastModified().toString() );
            break;
        default:
            if ( c < (int)tableColumns.size() ) {
                geti = new GETableItem(
                        this, orig, 
                        groups[orig].ideas[c-numColumns].computedValue );
                geti->setLink( QString( "do openGroupComputation %1 %2" )
                               .arg( groups[orig].codeName() )
                               .arg( groups[orig].ideas[c-numColumns].factory->title ) );
            } else if ( lastPrefs ) {
                geti = new GETableItem(
                        this, orig,
                        lastPrefs->getColumnValue( userCols[c-tableColumns.size()],
                                                   groups[orig].codeName() ),
                        Q3TableItem::OnTyping );
            } else { // shouldn't happen, but to be paranoid:
                geti = new GETableItem( this, orig, "--" );
            }
        }
        setItem( r, col, geti );
        if ( !isAtStartup ) {
            progress->setProgress( r+1 );
            qApp->processEvents();
        }
        emit progressChanged( 100 * (r+1) / groups.count() );
    }
    colNameToFilled[horizontalHeader()->label( col )] = TRUE;
    if ( !isAtStartup ) delete progress;       
}


void GroupTable::validatePreferences ()
{
    if ( !lastPrefs ) return; // silly to call this w/no prefs
    QStringList mCols = lastPrefs->getColumnsMentioned();
    // if any positions are doubly occupied, shift some column(s) to right end:
    int rightmost = -1; // helps to start by finding farthest-right column
    for ( int i = 0 ; i < mCols.size() ; i++ ) {
        int pos = lastPrefs->getColumnPosition( mCols[i] );
        if ( pos > rightmost ) rightmost = pos;
    }
    colPosToName.clear(); // temporary use right now...will fix it @ end of this func
    for ( int i = 0 ; i < mCols.size() ; i++ ) {
        int pos = lastPrefs->getColumnPosition( mCols[i], -1 );
        if ( pos < 0 ) pos = 0; // thus it will be fixed in a moment
        // extend colPosToName[] to reach the pos mentioned:
        while ( pos >= (int)colPosToName.size() ) colPosToName << QString::null;
        if ( colPosToName[pos].isNull() ) {
            // have seen things past this, but nothing at this location, so OK:
            colPosToName[pos] = mCols[i];
        } else {
            // uh-oh, duplicate positions...put this guy at the far right end,
            // and increase what that right end is:
            rightmost++;
            while ( rightmost > (int)colPosToName.size() )
                colPosToName << QString::null; // this makes it one shy
            colPosToName << mCols[i];          // this makes it the right size
        }
    } // so far we've just recorded info...we write to preferences below
    // if any positional gaps occur, shift higher columns downward:
    for ( int i = colPosToName.size() - 1 ; i >= 0 ; i-- ) {
        if ( colPosToName[i].isNull() ) {
            // a positional gap...let's fix this
            for ( int j = i + 1 ; j < (int)colPosToName.size() ; j++ ) {
                colPosToName[j-1] = colPosToName[j];
            }
            colPosToName.pop_back();
        }
    }
    // if any built-in column or idea has no position, put it at right end:
    for ( int i = 0 ; i < tableColumns.size() ; i++ ) {
        if ( !colPosToName.contains( tableColumns[i] ) ) {
            colPosToName << tableColumns[i];
        }
    }
    // update preferences
    // (now the colPosToName map is the standard, so make lastPrefs reflect it)
    for ( int i = 0 ; i < colPosToName.size() ; i++ ) {
        lastPrefs->setColumnPosition( colPosToName[i], i );
    }
    // clear and refill other column maps:
    // (ensure colNameToFilled, colNameToPos, colNameToStdPos are up-to-date)
    colNameToPos.clear();
    QStringList userCols = lastPrefs->getUserDefinedColumns();
    for ( int i = 0 ; i < colPosToName.size() ; i++ ) {
        colNameToPos[colPosToName[i]] = i;
        if ( !colNameToFilled.contains( colPosToName[i] ) )
            colNameToFilled[colPosToName[i]] = FALSE;
        if ( !colNameToStdPos.contains( colPosToName[i] ) ) {
            int pos = tableColumns.findIndex( colPosToName[i] );
            if ( pos > -1 ) {
                colNameToStdPos[colPosToName[i]] = pos;
            } else {
                int pos2 = userCols.findIndex( colPosToName[i] );
                if ( pos2 > -1 ) {
                    colNameToStdPos[colPosToName[i]] = tableColumns.size() + pos2;
                } else {
                    // this should never happen
                    colNameToStdPos[colPosToName[i]] = -1;
                }
            }
        }
    }
}
        
void GroupTable::updateColumnMaps ()
{
    // update column maps to reflect most recent column ordering by user:
    for ( int i = 0 ; i < horizontalHeader()->count() ; i++ ) {
        colPosToName[i] = horizontalHeader()->label( i );
        colNameToPos[colPosToName[i]] = i;
    }
}
    
void GroupTable::fillTable ()
        // this routine assumes lastPrefs is defined...
        // if !lastPrefs, much of what this routine is supposed to do won't happen
        // (no errors, just missing functionality)
{
    isAtStartup = TRUE;
    
    setNumRows( groups.count() );
    emit enterNewPhase( 0, 30 );
    for ( uint j = 0 ; j < GroupIdeas::listAll()->count() ; j++ ) {
        emit messageChanged( QString( "Computing for each group: %1" )
                             .arg( GroupIdeas::listAll()->at( j )->title ) );
        for ( int i = 0 ; i < groups.count() ; i++ ) {
            if ( !j ) groups[i].ideas.clear();
            if ( (int)j >= groups[i].ideas.count() ) {
                // we ask the above question because maybe the call we're about to make
                // will cause some groups to ask questions of one another, and therefore
                // the computation would be done for some groups when we get to them.
                // see, for example, the propagation inherent in idealist.cpp:SolvableGIF.
                groups[i].ideas <<
                    GroupIdeas::listAll()->at(j)->consider( groups[i] );
            }
        }
        emit progressChanged( 100 * (j+1) / GroupIdeas::listAll()->count() );
    }
    emit finishPhase();
    
    // the main work is done by these two routines:
    emit enterNewPhase( 30, 100 );
    validatePreferences();    
    applyPreferences();
    emit finishPhase();
    
    if ( lastPrefs ) {
        lastPrefs->setGroup( "MainWindowColumns" );
        if ( lastPrefs->getString( "sortedby" ) == NO_SORTING ) {
            setRowOrder( lastPrefs->getStringList( "groupatrow" ) );
        } else {
            sortColumn( colNameToPos[lastPrefs->getString( "sortedby" )],
                        lastPrefs->getBool( "sortedascending" ) );
        }
    }
    emit progressChanged( 100 );
    
    isAtStartup = FALSE;
}

void GroupTable::showColumn ( int col )
{
    Q3Table::showColumn( col );
    if ( !colNameToFilled[colPosToName[col]] ) {
        emit messageChanged( QString( "column %1 of %2" )
                             .arg( numColsComputed+1 )
                             .arg( numColsToCompute ) );
        emit enterNewPhase( 100 * numColsComputed / numColsToCompute,
                            100 * (numColsComputed+1) / numColsToCompute );
        fillColumn( col );
        if ( lastPrefs->getColumnWidth( colPosToName[col] ) > -1 ) {
            horizontalHeader()->resizeSection(
                    col, lastPrefs->getColumnWidth( colPosToName[col] ) );
        } else {
            adjustColumn( col );
            lastPrefs->setColumnWidth(
                    colPosToName[col], horizontalHeader()->sectionSize( col ) );
        }
        numColsComputed++;
        emit finishPhase();
    }
    // let's say the smallest a column can be is 20 pixels
    if ( horizontalHeader()->sectionSize( col ) < 20 ) {
        horizontalHeader()->resizeSection( col, 20 );
        if ( lastPrefs ) lastPrefs->setColumnWidth( colPosToName[col], 20 );
    }
    if ( lastPrefs ) lastPrefs->setColumnVisible( colPosToName[col], TRUE );
}

void GroupTable::hideColumn ( int col )
{
    Q3Table::hideColumn( col );
    if ( lastPrefs ) lastPrefs->setColumnVisible( colPosToName[col], FALSE );
}

void GroupTable::removeColumn ( int col )
{
    Q3Table::removeColumn( col );
    if ( lastPrefs ) {
        lastPrefs->deleteColumn( colPosToName[col] );
        validatePreferences();
    }
    updateColumnMaps ();
}

void GroupTable::appendColumn ( QString name )
{
    int newnum = numCols();
    setNumCols( newnum + 1 );
    horizontalHeader()->setLabel( newnum, name );
    if ( lastPrefs ) {
        lastPrefs->createColumn( name );
        validatePreferences();
    }
    showColumn( newnum );
    void updateColumnMaps ();
}

void GroupTable::appendColumn ( QString name, QStringList values, int width )
{
    int newnum = numCols();
    appendColumn( name );
    if ( width > -1 ) {
        horizontalHeader()->resizeSection( numCols() - 1, width );
        updateContents();
    }
    for ( int i = 0 ; i < numRows() ; i++ ) {
        if ( lastPrefs ) {
            lastPrefs->setColumnValue(
                    name, groupAtRow( i )->codeName(), values[i] );
        }
        setText( i, newnum, values[i] );
    }
}
    
void GroupTable::hideAndShowColumns ()
{
    if ( !lastPrefs ) return; // can't tell what should be visible w/o preferences
    // hide and show the appropriate columns
    numColsComputed = 0;
    for ( int i = 0 ; i < numCols() ; i++ ) {
        if ( lastPrefs->getColumnVisible( colPosToName[i] ) ) {
            showColumn( i );
        } else {
            hideColumn( i );
        }
    }
    for ( int j = 0 ; j < numRows() ; j++ ) adjustRow( j );
}

void GroupTable::applyPreferences ()
{
    if ( !lastPrefs ) return; // silly to run this with no preferences
    validatePreferences();
    // get all columns in right position via swapping
    setNumCols( QMAX( numCols(), (int)colPosToName.size() ) );
    numColsToCompute = 0;
    for ( int i = 0 ; i < colPosToName.size() ; i++ ) {
        int foundColAt = -1; // looking for colPosToName[i] in headers
        for ( int j = 0 ; j < horizontalHeader()->count() ; j++ ) {
            if ( horizontalHeader()->label( j ) == colPosToName[i] ) {
                foundColAt = j;
                break;
            }
        }
        if ( foundColAt > -1 ) {
            if ( foundColAt != (int)i ) {
                // move already-existing column to where we want it
                swapColumns( i, foundColAt );
            } // don't swap the thing with itself
        } else {
            // initialize column i, since it doesn't exist anywhere yet
            horizontalHeader()->setLabel( i, colPosToName[i] );
            horizontalHeader()->setClickEnabled( i );
            colNameToFilled[colPosToName[i]] = FALSE; // just to be sure
            // we will handle column width when column is first shown (showColumn())
            if ( lastPrefs->getColumnVisible( colPosToName[i] ) ) numColsToCompute++;
        }
    }
    setNumCols( colPosToName.size() );
    hideAndShowColumns();
}

void GroupTable::applyPreferences ( GEPreferences *p )
{
    // first copy the given preferences over our old ones, because this routine
    // is called from a QUndoCommand that remembers the old ones for us:
    lastPrefs = p;
    applyPreferences();
}

void GroupTable::doubleClickedRow ( int row, int col )
{
    // first, if an editable item was double-clicked, do nothing, so that the
    // default behavior of editing that cell will take place uninterrupted:
    Q3TableItem* i = item( row, col );
    if ( i && ( i->editType() != Q3TableItem::Never ) ) return;
    // otherwise, we continue, opening the group from that row:
    int idx = rowOriginalPosition( row );
    if ( ( idx > -1 ) && ( idx < (int)groups.size() ) ) {
        emit requestedGroupInfo( groups[idx].codeName() );
    }
}

void GroupTable::columnHeaderClicked ( int column ) {
    // check to see if the column is sortable:
    GETableItem* geti = (GETableItem*)item( 0, column );
    if ( geti->key().isNull() ) {
        // then no key in this column will be non-null, which is the signal
        // indicating the column is not sortable, so don't:
        return;
    }
    // if we haven't quit yet, it's sortable, so do so:
    GTCmdSortColumn* newcmd = new GTCmdSortColumn( 
            this, column, (colJustSorted == column) ? !wayJustSorted : TRUE,
            colJustSorted, wayJustSorted );
    if ( undoStack ) {
        undoStack->push( newcmd );
    } else {
        newcmd->redo();
    }
}

void GroupTable::sortColumn( int column, bool ascending )
{
    Q3Table::sortColumn( column, ascending, TRUE );
    colJustSorted = column;
    wayJustSorted = ascending;
    horizontalHeader()->setSortIndicator( column, 
                                          ascending ? Qt::AscendingOrder : Qt::DescendingOrder );
    for ( int i = 0 ; i < numRows() ; i++ ) adjustRow( i );
    saveRowsInPreferences();
}

void GroupTable::sortColumn( int column ) {
    sortColumn( column, (colJustSorted == column) ? !wayJustSorted : TRUE );   
}


int GroupTable::rowOriginalPosition( int row )
{
    // this obviously depends on the fact that this group table had better only use
    // descendants of GETableItem for its items!  ensure this takes place!
    // it also assumes there's something in the table, but calling this assumes that
    GETableItem* geti = (GETableItem*)item( row, 0 );
    return geti->originalRow();
}


void GroupTable::columnsRearranged( int, int from, int to ) {
    GTCmdReorderColumns* newcmd = new GTCmdReorderColumns( this, from, to );
    if ( undoStack ) {
        undoStack->push( newcmd );
    } else {
        newcmd->redo();
    }
    updateColumnMaps();
}

void GroupTable::columnSizeChanged( int section, int oldsize, int newsize )
{
    undoStack->push( new GTCmdResizeColumn( this, section, oldsize, newsize ) );
}

void GroupTable::saveColumnsInPreferences () {
    updateColumnMaps();
    if ( lastPrefs ) {
        for ( int i = 0 ; i < colPosToName.size() ; i++ ) {
            lastPrefs->setColumnPosition( colPosToName[i], i );
            lastPrefs->setColumnWidth(
                    colPosToName[i], horizontalHeader()->sectionSize( i ) );
        }
    }
    validatePreferences();
    // whether columns are visible or not is handled by the options dialog
}


void GroupTable::rowsRearranged ( int, int from, int to ) {
    GTCmdReorderRows* newcmd = new GTCmdReorderRows(
            this, from, to, colJustSorted, wayJustSorted );
    if ( undoStack ) {
        undoStack->push( newcmd );
    } else {
        newcmd->redo();
    }
    colJustSorted = -1;
    horizontalHeader()->setSortIndicator( -1, Qt::AscendingOrder ); // clears sort indicator
}

void GroupTable::saveRowsInPreferences () {
    if ( lastPrefs ) {
        lastPrefs->setGroup( "MainWindowColumns" );
        if ( colJustSorted == -1 ) {
            lastPrefs->setString( "sortedby", NO_SORTING );
            lastPrefs->setStringList( "groupatrow", getRowOrder() );
        } else {
            lastPrefs->setString( "sortedby", colPosToName[colJustSorted] );
            lastPrefs->setBool( "sortedascending", wayJustSorted );
        }
    }
}


QStringList GroupTable::getRowOrder () {
    QStringList order;
    for ( int i = 0 ; i < groups.count() ; i++ ) {
        order << groups[rowOriginalPosition( i )].codeName();
    }
    return order;
}

void GroupTable::setRowOrder ( const QStringList& order ) {
    for ( int i = 0 ; i < order.size() ; i++ ) {
        QString name = GroupFile::decodePrimaryName( order[i] ),
                author = GroupFile::decodeAuthor( order[i] );
        for ( int j = i ; j < numRows() ; j++ ) {
            int orig = rowOriginalPosition( j );
            if ( ( groups[orig].getPrimaryName().text == name )
              && ( groups[orig].author == author ) ) {
                swapRows( i, j );
                break; // inner loop only
            }
        }
        adjustRow( i );
    }
    colJustSorted = -1;
    horizontalHeader()->setSortIndicator( -1, Qt::AscendingOrder ); // clears sort indicator
    updateContents();
}

void GroupTable::setUndoStack ( QUndoStack* stack )
{
    // does not take ownership
    undoStack = stack;
}

void GroupTable::contextMenuRequested ( int row, int col, const QPoint& pos )
{
    GETableItem* geti = (GETableItem*)item( row, col );
    if ( !geti->getLink().isNull() ) {
        popupRow = row;
        popupCol = col;
        QMenu contextMenu( this );
        contextMenu.insertItem( "&Explain this...", this, SLOT(explain()) );
        contextMenu.exec( pos );
    }
}

void GroupTable::explain ()
{
    GETableItem* geti = (GETableItem*)item( popupRow, popupCol );
    QString link = geti->getLink();
    if ( !link.isNull() ) GEHelpWindow::showHelp( link );
}

GroupFile* GroupTable::groupAtRow ( int row )
{
    GETableItem* geti = (GETableItem*)item( row, 0 );
    return &(groups[geti->originalRow()]);
}

void GroupTable::postEditCommand ( int row, int col )
{
    undoStack->push( new GTCmdEditCell( this, lastPrefs, row, col ) );
}

void GroupTable::updateColumnsAndSorting ()
{
    // update not just the column maps, but also the colJustSorted variable,
    // and the horizontalHeader()'s sort indicator
    QString sortedcolname =
            ( colJustSorted > -1 ) ? colPosToName[colJustSorted] : QString::null;
    updateColumnMaps();
    colJustSorted = colNameToPos[sortedcolname];
    horizontalHeader()->setSortIndicator(
            colJustSorted, horizontalHeader()->sortIndicatorOrder() );
}

void GroupTable::swapColumns ( int col1, int col2 )
{
    Q3Table::swapColumns( col1, col2, TRUE );
    updateColumnsAndSorting();
}

void GroupTable::setColumnName( int column, QString name )
{
    if ( lastPrefs )
        lastPrefs->renameColumn( horizontalHeader()->label( column ), name );
    horizontalHeader()->setLabel( column, name );
}

void GroupTable::setCustomValue ( QString colname, QString groupcode, QString value )
{
    int column = colNameToPos[colname];
    int row = findGroupRow( groupcode );
    if ( ( column > -1 ) && ( row > -1 ) &&
         ( column < numCols() ) && ( row < numRows () ) )
        setText( row, column, value );
}

QString GroupTable::selectionAsHTML ()
{
    Q3TableSelection ts = selection( 0 );
    if ( ( ts.topRow() == ts.bottomRow() ) && ( ts.leftCol() == ts.rightCol() ) ) {
        GETableItem* geti = ( GETableItem* )item( ts.topRow(), ts.leftCol() );
        return geti->asHTML();
    }
    QString result = "<table>\n";
    for ( int r = ts.topRow() ; r <= ts.bottomRow() ; r++ ) {
        result += "    <tr>\n";
        for ( int c = ts.leftCol() ; c <= ts.rightCol() ; c++ ) {
            if ( !isColumnHidden( c ) ) {
                GETableItem* geti = ( GETableItem* )item( r, c );
                if ( geti ) {
                    result += "        <td>" + geti->asHTML() + "</td>\n";
                } else {
                    qDebug( QString( "GroupTable::selectionAsHTML() error: "
                                     "item( %1, %2 ) == 0" ).arg( r ).arg( c )
                            .toLatin1().constData() );
                    result += "        <td>ERROR: MISSING!</td>\n";
                }
            }
        }
        result += "    </tr>\n";
    }
    result += "</table>";
    return result;
}


GTCmdEditCell::GTCmdEditCell (
        GroupTable* table, GEPreferences* prefs, int row, int col )
    : table( table )
{
    colname = table->horizontalHeader()->label( col );
    groupcode = table->groupAtRow( row )->codeName();
    oldvalue = prefs->getColumnValue( colname, groupcode );
    newvalue = table->text( row, col );
    setText( "enter " + newvalue + " in cell" );
}

void GTCmdEditCell::redo ()
{
    table->setCustomValue( colname, groupcode, newvalue );
}

void GTCmdEditCell::undo ()
{
    table->setCustomValue( colname, groupcode, oldvalue );
}


GTCmdSortColumn::GTCmdSortColumn (
        GroupTable *table, int column, bool ascending,
        int colJustSorted, bool wayJustSorted )
    : table( table ), sortByThisColumn( column ), ascending( ascending ),
    colJustSorted( colJustSorted ), wayJustSorted( wayJustSorted )
{
    oldRowOrder = table->getRowOrder();
    setText( QString( "%1 groups by %2" )
             .arg( ascending ? "sort" : "reverse-sort" )
             .arg( table->horizontalHeader()->label( sortByThisColumn ) ) );
}

void GTCmdSortColumn::redo () {
    table->sortColumn( sortByThisColumn, ascending );
}

void GTCmdSortColumn::undo () {
    if ( colJustSorted == -1 ) {
        qDebug( "Using a GTCmdSortColumn to reorder the rows one by one\n" );
        table->setRowOrder( oldRowOrder );
    } else {
        table->sortColumn( colJustSorted, wayJustSorted );
    }
}


GTCmdReorderRows::GTCmdReorderRows (
        GroupTable *table, int draggedFrom, int draggedTo,
        int colJustSorted, bool wayJustSorted )
    : table( table ),
    colJustSorted( colJustSorted ), wayJustSorted( wayJustSorted ),
    draggedThisRow( draggedFrom ), firstRedo( TRUE )
{
    intoThisRow = ( draggedFrom < draggedTo ) ? draggedTo - 1 : draggedTo;
    setText( QString( "move row %1 to row %2" )
             .arg( draggedThisRow + 1 ).arg( intoThisRow + 1 ) );
}

void GTCmdReorderRows::pushRow ( int from, int to ) {
    int last = -1, delta = ( ( from < to ) ? 1 : -1 ), pastEnd = to + delta;
    for ( int i = from ; i != pastEnd ; i += delta ) {
        if ( last > -1 ) table->swapRows( last, i, TRUE );
        last = i;
    }
    table->horizontalHeader()->setSortIndicator( -1, Qt::Ascending );
    table->updateContents();
}
    
void GTCmdReorderRows::redo () {
    // don't do this the first time because QTable actually does this automatically,
    // and our posting this to the undo stack is a REaction, not an action itself.
    // any future "redo()" actions, however, are actual REdo's, not the inital DO.
    if ( !firstRedo )
        pushRow( draggedThisRow, intoThisRow );
    else
        firstRedo = FALSE;
    table->saveRowsInPreferences();
}

void GTCmdReorderRows::undo () {
    if ( colJustSorted == -1 ) {
        pushRow( intoThisRow, draggedThisRow ); // backwards, of course
    } else {
        table->sortColumn( colJustSorted, wayJustSorted );
    }
    table->saveRowsInPreferences();
}


GTCmdReorderColumns::GTCmdReorderColumns (
        GroupTable *table, int draggedFrom, int draggedTo )
    : table( table ), draggedThisColumn( draggedFrom ), firstRedo( TRUE )
{
    intoThisColumn = ( draggedFrom < draggedTo ) ? draggedTo - 1 : draggedTo;
    setText( "move column " + table->horizontalHeader()->label( intoThisColumn ) );
}

void GTCmdReorderColumns::pushColumn ( int from, int to ) {
    int last = -1, delta = ( ( from < to ) ? 1 : -1 ), pastEnd = to + delta;
    for ( int i = from ; i != pastEnd ; i += delta ) {
        if ( last > -1 ) table->swapColumns( last, i );
        last = i;
    }
    table->updateContents();
}
    
void GTCmdReorderColumns::redo () {
    // don't do this the first time because QTable actually does this automatically,
    // and our posting this to the undo stack is a REaction, not an action itself.
    // any future "redo()" actions, however, are actual REdo's, not the inital DO.
    if ( !firstRedo )
        pushColumn( draggedThisColumn, intoThisColumn );
    else {
        firstRedo = FALSE;
        table->updateColumnsAndSorting();
    }
    table->saveColumnsInPreferences();
}

void GTCmdReorderColumns::undo () {
    pushColumn( intoThisColumn, draggedThisColumn ); // backwards, of course
    table->saveColumnsInPreferences();
}


GTCmdHideShowColumn::GTCmdHideShowColumn (
        GroupTable* table, int column, bool hide )
    : table( table ), column( column ), hide( hide )
{
    QString colname = table->horizontalHeader()->label( column );
    setText( QString( "%1 column %2" ).arg( hide ? "hide" : "show" ).arg( colname ) );
}

void GTCmdHideShowColumn::redo ()
{
    if ( hide ) table->hideColumn( column ); else table->showColumn( column );
}

void GTCmdHideShowColumn::undo ()
{
    if ( !hide ) table->hideColumn( column ); else table->showColumn( column );
}


GTCmdDeleteColumn::GTCmdDeleteColumn ( GroupTable* table, int column )
    : table( table ), column( column )
{
    colname = table->horizontalHeader()->label( column );
    setText( "delete column " + colname );
}

void GTCmdDeleteColumn::redo ()
{
    // we can assume rows will be in same order if this thing is undone, so just
    // pile all current values in a stringlist:
    for ( int i = 0 ; i < table->numRows() ; i++ ) {
        values << table->text( i, column );
    }
    table->removeColumn( column );
}

void GTCmdDeleteColumn::undo ()
{
    column = table->numCols();
    table->appendColumn( colname, values );
}


GTCmdRenameColumn::GTCmdRenameColumn ( GroupTable* table, int column, QString text )
    : table( table ), column( column ), newname( text )
{
    oldname = table->horizontalHeader()->label( column );
    setText( "rename column " + oldname + " to " + newname );
}

void GTCmdRenameColumn::redo ()
{
    table->setColumnName( column, newname );
}

void GTCmdRenameColumn::undo ()
{
    table->setColumnName( column, oldname );
}


GTCmdCreateColumn::GTCmdCreateColumn ( GroupTable* table, QString colname )
    : table( table ), colname( colname )
{
    setText( "create column " + colname );
}

void GTCmdCreateColumn::redo ()
{
    table->appendColumn( colname );
}

void GTCmdCreateColumn::undo ()
{
    // it should be the rightmost still
    table->removeColumn( table->numCols() - 1 );
}
    

GTCmdApplyPreferences::GTCmdApplyPreferences (
        GroupTable* table, GEPreferences* oldPrefs, GEPreferences* newPrefs )
    : table( table )
{
    oldPrefCopy = new GEPreferences(
            QString::null, QString::null, QString::null, QStringList() );
    *oldPrefCopy = *oldPrefs; // overwrite data
    newPrefCopy = new GEPreferences(
            QString::null, QString::null, QString::null, QStringList() );
    *newPrefCopy = *newPrefs; // overwrite data
    setText( "change options/appearance" );
}

void GTCmdApplyPreferences::redo ()
{
    table->applyPreferences( newPrefCopy );
}

void GTCmdApplyPreferences::undo ()
{
    table->applyPreferences( oldPrefCopy );
}


GTCmdResizeColumn::GTCmdResizeColumn (
        GroupTable* table, int column, int oldsize, int newsize )
    : table( table ), column( column ), oldsize( oldsize ), newsize( newsize ),
    firstRedo( TRUE )
{
    setText( "resize column " + table->horizontalHeader()->label( column ) );
}

void GTCmdResizeColumn::redo ()
{
    if ( !firstRedo ) {
        table->horizontalHeader()->resizeSection( column, newsize );
        table->updateContents();
    } else {
        firstRedo = FALSE;
    }
    table->saveColumnsInPreferences();
}

void GTCmdResizeColumn::undo ()
{
    table->horizontalHeader()->resizeSection( column, oldsize );
    table->updateContents();
    table->saveColumnsInPreferences();
}

bool GTCmdResizeColumn::mergeMeWith ( QUndoCommand* c )
{
    if ( c->id() != id() ) return FALSE;
    
    GTCmdResizeColumn* rcc = (GTCmdResizeColumn*)c;
    if ( column != rcc->column ) return FALSE;
    
    newsize = rcc->newsize;
    return TRUE;
}

int GTCmdResizeColumn::id ()
{
    return 1;
}

