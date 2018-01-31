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


#include "ge.h"
#include "gehelpbrowser.h"
#include <qdir.h>


GEHelpBrowser::GEHelpBrowser ( QWidget *parent, QString helpPath )
    : Q3TextBrowser( parent )
{
	if ( !Q3MimeSourceFactory::defaultFactory()->filePath().contains( helpPath ) )
		Q3MimeSourceFactory::defaultFactory()->addFilePath( helpPath );
}

void GEHelpBrowser::setSource ( const QString &name )
{
    /*
    // translate relative source description to sort-of-absolute one
    QString trueName = QDir::cleanDirPath( currentDir + name );
    qDebug( "interpreting \"" + name + "\" in \"" + currentDir +
            "\" gives \"" + trueName + "\"" );
    int lastSlash = QMAX( trueName.findRev( "/" ), trueName.findRev( "\\" ) );
    currentDir = trueName.left( lastSlash + 1 );
    */
    // now act on it, either emitting a request for action, or navigating
    QStringList pieces = name.split( " " );
    if ( pieces[0] == "do" ) {
        pieces.remove( pieces.begin() );
        emit actionRequested( pieces );
    } else {
        Q3TextBrowser::setSource( name );
        if ( !text().length() ) { // error: should have found the page
            setText( QString(
                "<html>\n"
                "<title>Help error</title>\n"
                "<body>\n"
                "<h1>Help error</h1>\n"
                "<h3>The help file you were trying to access "
                "does not exist.</h3>\n"
                "<hr>\n"
                "<p>Please report this error to %2 via the email address %3.  "
                "In your email, please mention the following internal address "
                "for the file.</p>\n"
                "<blockquote><code>%1</code></blockquote>\n"
                "<p>You can use the back button in the toolbar above to return "
                "to the last page you were browsing.</p>\n"
                "<hr>\n"
                "</body>\n"
                "</html>\n" )
                .arg( name ).arg( GE_AUTHOR ).arg( GE_AUTHOR_EMAIL ) );
        }
    }
}


