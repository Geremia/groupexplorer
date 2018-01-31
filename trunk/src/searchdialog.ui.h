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


void SearchDialog::init ()
{
    ResultsBox->setColumns( 1 );
    ResultsBox->setOrientation( Qt::Vertical );
    browser = new GEHelpBrowser( ResultsBox );
    browser->setMinimumHeight( 300 );
    connect( browser, SIGNAL(actionRequested(QStringList)),
             this, SLOT(resultsActionRequested(QStringList)) );
    resize( sizeHint() );
}


void SearchDialog::search()
{
    Q3ProgressDialog* progress = new Q3ProgressDialog( "Searching...", 0, 400, this,
                                                       (Qt::WindowFlags)0 );
    progress->setModal( TRUE );
    browser->setText( QString( "<h3>Results from help documents</h3>"
                               "%1"
                               "<h3>Results from group information</h3>"
                               "%2"
                               "<h3>Results from all sheets</h3>"
                               "%3"
                               "<h3>Results from all open windows</h3>"
                               "%4\n<br>\n" )
                      .arg( searchHelpDocuments( progress ) )
                      .arg( searchGroupInformation( progress ) )
                      .arg( searchAllSheets( progress ) )
                      .arg( searchAllOpenWindows( progress ) ) );
}


void SearchDialog::getReady()
{
    SearchHelpCheckBox->setChecked( TRUE );
    SearchSheetsCheckBox->setChecked( TRUE );
    SearchInfoCheckBox->setChecked( TRUE );
    SearchOpenCheckBox->setChecked( TRUE );
    show();
    setActiveWindow();
    raise();
    SearchEdit->setFocus();
}


void SearchDialog::getReadyForHelp()
{
    SearchHelpCheckBox->setChecked( TRUE );
    SearchSheetsCheckBox->setChecked( FALSE );
    SearchInfoCheckBox->setChecked( FALSE );
    SearchOpenCheckBox->setChecked( FALSE );
    show();
    setActiveWindow();
    raise();
    SearchEdit->setFocus();
}


void SearchDialog::resultsActionRequested( QStringList parameters )
{
    emit actionRequested( parameters, this );
}


void SearchDialog::searchHTML(
        QString html, QString link, QString defaultTitle, QStringList& result )
{
    QRegExp search4me( WholeWordCheckBox->isChecked() ?
                       "\\b" + QRegExp::escape( SearchEdit->text() ) + "\\b" :
                       QRegExp::escape( SearchEdit->text() ) );
    search4me.setCaseSensitive( CaseCheckBox->isChecked() );
    if ( search4me.search( removeHTMLTags( html ) ) > -1 ) {
        QRegExp title( "<\\s*title\\s*>([^>]+)<\\s*\\/\\s*title\\s*>" );
        result << QString( "<a href=\"%1\">%2</a>" )
                .arg( link )
                .arg( title.search( html ) > -1 ? title.cap( 1 ) : defaultTitle );
    }
}

QString SearchDialog::resultToList( QStringList result )
{
    return result.size() ?
            "<ul><li>" + result.join( "</li><li>" ) + "</li></ul>" :
            QString( "<p><i>no results found</i></p>" );
}


QString SearchDialog::searchHelpDocuments( Q3ProgressDialog* progress )
{
    if ( SearchHelpCheckBox->isChecked() ) {
        QStringList result;
        QStringList helpdocs = findFiles( resourcesPath() + "/help", "*.html" );
        for ( int i = 0 ; i < helpdocs.size() ; i++ ) {
            progress->setProgress( 100 * i / helpdocs.size() );
            QFile f( helpdocs[i] );
            if ( f.open( QIODevice::ReadOnly ) ) {
                QFileInfo fi( f );
                searchHTML( f.readAll(), "do openHelp " + fi.fileName(),
                            fi.fileName(), result );
            }
        }
        progress->setProgress( 100 );
        return resultToList( result );
    } else {
        progress->setProgress( 100 );
        return "<p><i>not searched</i></p>";
    }
}


QString SearchDialog::searchGroupInformation( Q3ProgressDialog* progress )
{
    if ( SearchInfoCheckBox->isChecked() ) {
        QStringList result;
        uint numGroups;
        emit requestNumGroups( numGroups );
        for ( uint i = 0 ; i < numGroups ; i++ ) {
            progress->setProgress( 100 + 100 * i / numGroups );
            QString html;
            QString code;
            emit requestGroupInfo( i, html, code );
            searchHTML( html, "do openGroup " + code,
                        GroupFile::decodePrimaryName( code ), result );
        }
        progress->setProgress( 200 );
        return resultToList( result );
    } else {
        progress->setProgress( 200 );
        return "<p><i>not searched</i></p>";
    }
}


QString SearchDialog::searchAllSheets( Q3ProgressDialog* progress )
{
    if ( SearchSheetsCheckBox->isChecked() ) {
        QStringList result;
        QStringList sheets;
        emit requestSheetPaths( sheets );
        sheets = findFilesRecursively( sheets );
        for ( int i = 0 ; i < sheets.count() ; i++ ) {
            progress->setProgress( 200 + 100 * i / sheets.count() );
            if ( GESheet::searchSheet( sheets[i], SearchEdit->text(),
                                       WholeWordCheckBox->isChecked(),
                                       CaseCheckBox->isChecked() ) )
                result << QString( "<a href=\"do openSheet %1\">%2</a>" )
                          .arg( sheets[i] ).arg( sheets[i] );
        }
        progress->setProgress( 300 );
        return resultToList( result );
    } else {
        progress->setProgress( 300 );
        return "<p><i>not searched</i></p>";
    }
}


QString SearchDialog::searchAllOpenWindows( Q3ProgressDialog* progress )
{
    if ( SearchOpenCheckBox->isChecked() ) {
        progress->setProgress( 400 );
        QStringList result;
        QWidgetList list = QApplication::topLevelWidgets();
        QWidgetList::iterator it = list.begin();  // iterate over the widgets
        QWidget* w;
        while ( ( w = *it ) ) {
            if ( w->isVisible() ) {
                if ( w->inherits( "GEHelpWindow" ) ) {
                    GEHelpWindow* hw = ( GEHelpWindow* )w;
                    QString title = hw->title();
                    result << ( "Help window: " +
                                ( title.isNull() ? QString( "untitled" ) : title ) );
                } else if ( w->inherits( "GESheetWindow" ) ) {
                    GESheetWindow* sw = ( GESheetWindow* )w;
                    QString title = sw->filename();
                    result << ( "Sheet window: " +
                                ( title.isNull() ? QString( "untitled" ) : title ) );
                }
            }
            ++it;
        }
        result.sort();
        return resultToList( result );
    } else {
        progress->setProgress( 400 );
        return "<p><i>not searched</i></p>";
    }
}


