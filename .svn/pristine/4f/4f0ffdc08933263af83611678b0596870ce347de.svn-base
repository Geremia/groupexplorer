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


#if QT_VERSION >= 0x040000


#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>


/*
 *  On Mac OS X, all resources are embedded in the .app bundle, so that users have
 *  dnd installation.  On all other OS's, resources are in a resources file in the
 *  same directory as the application executable.  So we encapsulate the calculation
 *  of the resource path in this function, so that we don't need precompiler #if's
 *  sprinkled throughout code that anyone might ever look at.  *shudder*
 *  Note that this goes hand-in-hand with a script I have called embed_resources.sh,
 *  which is only used on Mac OS X, which copies the Resources directory
 *  (recursively) into <application>.app/Contents/Resources.  Something like this:
 *  that script, here is a copy of its contents:
 *
 *        test -d $1.app/Contents/Resources/ || mkdir -p $1.app/Contents/Resources/
 *        cp -R ...etc...
 *
 */
QString resourcesPath () {
    QString exePath = QCoreApplication::applicationDirPath();
    QString resPath;
#ifdef Q_WS_MACX
    // step out of the <application>.app/Contents/MacOS directory, then down into
    // the Resources directory, which should also be in the Contents directory.
    resPath = exePath + "/../Resources";
#else
    // assume the Resources directory is in the same directory as the executable.
    resPath = exePath + "/Resources";
#endif
    return QFileInfo( resPath ).absFilePath();
}

// This function is much like the above one, but it gives the path of the
// application, which is the .app bundle for OSX, and the executable otherwise.
QString applicationPath () {
        QString path = QCoreApplication::applicationDirPath();
#ifdef Q_WS_MACX
        path += "/../../";
#endif
        return QFileInfo( path ).absFilePath();
}

// This function is like the above two, but it gives the path (with filename too!) of a
// good place to put a configuration file (like MYPROGRAM.INI sort of files on Win).
// It takes the filename (without path or extension) given as argument and uses it in the
// following way:
//  On Windows:
//      the proposed name gets suffixed with ".CFG", and gets located in the
//      applicationPath(), above
//      e.g. "MyApp" --> "C:\Program Files\MyApp\MyApp.CFG"
//  On Mac OS X:
//      the proposed name gets lower cased, suffixed with ".config", and located in
//      ~/Library/Preferences/ (where the ~ is created using QDir::homeDirPath())
//      e.g. "MyApp" --> "~/Library/Preferences/myapp.config"
//  On any other operating system, which I assume is a Unix flavor:
//      the proposed name gets lower cased, and prefixed with ~/. (including the dot,
//      where the ~ is created using QDir::homeDirPath())
//      e.g. "MyApp" --> "~/.myapp"
QString configFileName ( QString proposed )
{
#if defined(Q_WS_WIN)
    return applicationPath() + "/" + proposed + ".CFG";
#elif defined(Q_WS_MACX)
    return QDir::homeDirPath() + "/Library/Preferences/" + proposed.lower() + ".config";
#else
    return QDir::homeDirPath() + "/." + proposed.lower();
#endif
}

QString naturalLanguageJoin( QStringList& list )
{
        switch ( list.count() ) {
                case 0: return "";
                case 1: return list[0];
                case 2: return list[0] + " and " + list[1];
        } // otherwise the usual list[0], list[1], ..., and list[size-1]
        QString result;
        for ( int i = 0 ; i < list.size() ; i++ ) {
                if ( i ) result += ", ";
                if ( i == list.size() - 1 ) result += QObject::tr( "and" ) + " ";
                result += list[i];
        }
        return result;
}

// see Blanchette & Summerfield pp.237-8;
// that's the starting point of this algorithm.
// fileTypes is like this form: "*.txt *.gif *.bmp"
QStringList findFilesRecursively ( QStringList paths, QString fileTypes ) {
    if ( fileTypes.isEmpty() ) fileTypes = "*";
    QStringList result, more;
    QStringList::Iterator it;
    for ( int i = 0 ; i < paths.size() ; i++ ) {
        QDir dir( paths[i] );
        more = dir.entryList( fileTypes, QDir::Files );
        for ( it = more.begin() ; it != more.end() ; ++it ) {
            result.append( paths[i] + "/" + *it );
        }
        more = dir.entryList( QDir::Dirs ).grep( QRegExp( "[^.]" ) );
        for ( it = more.begin() ; it != more.end() ; ++it ) {
            *it = paths[i] + "/" + *it;
        }
        more = findFilesRecursively( more, fileTypes );
        for ( it = more.begin() ; it != more.end() ; ++it ) {
            result.append( *it );
        }
    }
    return result;
}

// and this one is just the other one with the recursive parts trimmed out
QStringList findFiles ( QStringList paths, QString fileTypes ) {
    if ( fileTypes.isEmpty() ) fileTypes = "*";
    QStringList result, files;
    QStringList::Iterator it;
    for ( int i = 0 ; i < paths.size() ; i++ ) {
        QDir dir( paths[i] );
        files = dir.entryList( fileTypes, QDir::Files );
        for ( it = files.begin() ; it != files.end() ; ++it ) {
            result.append( paths[i] + "/" + *it );
        }
    }
    return result;
}

QStringList findFiles ( QString path, QString fileTypes ) {
    QStringList paths;
    paths << path;
    return findFiles( paths, fileTypes );
}

QStringList findFilesRecursively ( QString path, QString fileTypes ) {
    QStringList paths;
    paths << path;
    return findFilesRecursively( paths, fileTypes );
}

void assertWidgetSize ( QWidget* w )
{
    for ( QObject* tmp = w ; tmp ; tmp = tmp->parent() ) {
        if ( tmp->inherits( "QWidget" ) ) {
            QWidget* cast = ( QWidget* )tmp;
            cast->adjustSize();
        }
    }
}

QString removeHyperlinks ( QString fromThis )
{
        // doesn't remove ALL hyperlinks...just non-local ones
        // i.e. <a href="#anchorname">...</a> ones can stay,
        // but all others must go

        QString mycopy = fromThis;//QDeepCopy<QString>( fromThis );
        QRegExp open( "<\\s*a\\s+href\\s*=\\s*\"[^#][^\"]*\"\\s*>" );
        QRegExp close( "<\\s*\\/\\s*a\\s*>" );
        for ( int i = 0 ; ( i = open.search( mycopy, i ) ) > -1 ; ) {
                mycopy.replace( i, open.matchedLength(), "<u>" );
                int j = close.search( mycopy, i );
                if ( j > -1 ) {
                        mycopy.replace( j, close.matchedLength(), "</u>" );
                } else {
                        qWarning( "In myutils removeHyperlinks(): "
                                  "Error: open <a href=\"...\"> without close </a>" );
                }
        }
        return mycopy;
}

QString removeHTMLTags ( QString fromThis )
{
        // removes all HTML open and close tags from document,
        // leaving a pile of text.
        // this is useful for searching a web page...search this function's output.

        QString mycopy = fromThis;//QDeepCopy<QString>( fromThis );
        QRegExp open( "<\\s*\\w+(?:\\s+[^>]*)?>" );
        QRegExp close( "<\\s*\\/\\w+\\s*>" );
        return mycopy.replace( open, " " ).replace( close, " " );
}


#else


#include <qapplication.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <qregexp.h>
#include <qdeepcopy.h>


void assertWidgetSize ( QWidget* w )
{
    for ( QObject* tmp = w ; tmp ; tmp = tmp->parent() ) {
        if ( tmp->inherits( "QWidget" ) ) {
            QWidget* cast = ( QWidget* )tmp;
            cast->adjustSize();
        }
    }
}

// made obsolete by my discovery of QStyleSheet::escape()
//QString escapeHTML ( QString s ){
//    QString mycopy = QDeepCopy<QString>( s );
//    return mycopy.replace( "&", "&amp;" )
//                 .replace( "<", "&lt;" )
//                 .replace( ">", "&gt;" )
//                 .replace( "\"", "&quot;" );
//}

QString removeHyperlinks ( QString fromThis )
{
	// doesn't remove ALL hyperlinks...just non-local ones
	// i.e. <a href="#anchorname">...</a> ones can stay,
	// but all others must go

	QString mycopy = QDeepCopy<QString>( fromThis );
	QRegExp open( "<\\s*a\\s+href\\s*=\\s*\"[^#][^\"]*\"\\s*>" );
	QRegExp close( "<\\s*\\/\\s*a\\s*>" );
	for ( int i = 0 ; ( i = open.search( mycopy, i ) ) > -1 ; ) {
		mycopy.replace( i, open.matchedLength(), "<u>" );
		int j = close.search( mycopy, i );
		if ( j > -1 ) {
			mycopy.replace( j, close.matchedLength(), "</u>" );
		} else {
			qWarning( "In myutils removeHyperlinks(): "
			          "Error: open <a href=\"...\"> without close </a>" );
		}
	}
	return mycopy;
}

QString removeHTMLTags ( QString fromThis )
{
	// removes all HTML open and close tags from document,
	// leaving a pile of text.
	// this is useful for searching a web page...search this function's output.

	QString mycopy = QDeepCopy<QString>( fromThis );
	QRegExp open( "<\\s*\\w+(?:\\s+[^>]*)?>" );
	QRegExp close( "<\\s*\\/\\w+\\s*>" );
	return mycopy.replace( open, " " ).replace( close, " " );
}

QString toRGBHex ( QColor& c )
{
	QString r = QString( "%1" ).arg( c.red(), 0, 16 );
	QString g = QString( "%1" ).arg( c.green(), 0, 16 );
	QString b = QString( "%1" ).arg( c.blue(), 0, 16 );
	return ( ( r.length() == 1 ) ? "0" + r : r ) +
	       ( ( g.length() == 1 ) ? "0" + g : g ) +
	       ( ( b.length() == 1 ) ? "0" + b : b );
}

QColor fromRGBHex ( QString s )
{
	return QColor( s.mid( 0, 2 ).toInt( 0, 16 ),
	               s.mid( 2, 2 ).toInt( 0, 16 ),
				   s.mid( 4, 2 ).toInt( 0, 16 ), QColor::Rgb );
}

QString naturalLanguageJoin( QStringList& list )
{
	switch ( list.count() ) {
		case 0: return "";
		case 1: return list[0];
		case 2: return list[0] + " and " + list[1];
	} // otherwise the usual list[0], list[1], ..., and list[size-1]
	QString result;
	for ( uint i = 0 ; i < list.size() ; i++ ) {
		if ( i ) result += ", ";
		if ( i == list.size() - 1 ) result += QObject::tr( "and" ) + " ";
		result += list[i];
	}
	return result;
}


#define MYUTILS_BLOCKSIZE 8192


void FullQDebug ( QString d ) {
    int i = 0, j = d.findRev( '\n', MYUTILS_BLOCKSIZE );
    while ( i < (int)d.length() ) {
        if ( j == -1 )
            qDebug( d.mid( i, d.length() ) );
        else
            qDebug( d.mid( i, j - 1 ) );
        if ( j > -1 ) {
            i = j + 1;
            j = d.findRev( '\n', i + MYUTILS_BLOCKSIZE );
        } else
            i = d.length();
    }
}

void FullQWarning ( QString d ) {
    int i = 0, j = d.findRev( '\n', MYUTILS_BLOCKSIZE );
    while ( i < (int)d.length() ) {
        if ( j == -1 )
            qWarning( d.mid( i, d.length() ) );
        else
            qWarning( d.mid( i, j - 1 ) );
        if ( j > -1 ) {
            i = j + 1;
            j = d.findRev( '\n', i + MYUTILS_BLOCKSIZE );
        } else
            i = d.length();
    }
}

void FullQFatal ( QString d ) {
    int i = 0, j = d.findRev( '\n', MYUTILS_BLOCKSIZE );
    while ( i < (int)d.length() ) {
        if ( j == -1 )
            qFatal( d.mid( i, d.length() ) );
        else
            qFatal( d.mid( i, j - 1 ) );
        if ( j > -1 ) {
            i = j + 1;
            j = d.findRev( '\n', i + MYUTILS_BLOCKSIZE );
        } else
            i = d.length();
    }
}


/*
 *  On Mac OS X, all resources are embedded in the .app bundle, so that users have
 *  dnd installation.  On all other OS's, resources are in a resources file in the
 *  same directory as the application executable.  So we encapsulate the calculation
 *  of the resource path in this function, so that we don't need precompiler #if's
 *  sprinkled throughout code that anyone might ever look at.  *shudder*
 *  Note that this goes hand-in-hand with a script I have called embed_resources.sh,
 *  which is only used on Mac OS X, which copies the Resources directory
 *  (recursively) into <application>.app/Contents/Resources.  Something like this:
 *  that script, here is a copy of its contents:
 *
 *        test -d $1.app/Contents/Resources/ || mkdir -p $1.app/Contents/Resources/
 *        cp -R ...etc...
 *
 */
QString resourcesPath () {
    QString exePath = qApp->applicationDirPath();
    QString resPath;
#ifdef Q_WS_MACX
    // step out of the <application>.app/Contents/MacOS directory, then down into
    // the Resources directory, which should also be in the Contents directory.
    resPath = exePath + "/../Resources";
#else
    // assume the Resources directory is in the same directory as the executable.
    resPath = exePath + "/Resources";
#endif
    return QFileInfo( resPath ).absFilePath();
}

// This function is much like the above one, but it gives the path of the
// application, which is the .app bundle for OSX, and the executable otherwise.
QString applicationPath () {
        QString path = qApp->applicationDirPath();
#ifdef Q_WS_MACX
        path += "/../../../";
#endif
	return QFileInfo( path ).absFilePath();
}

// This function is like the above two, but it gives the path (with filename too!) of a
// good place to put a configuration file (like MYPROGRAM.INI sort of files on Win).
// It takes the filename (without path or extension) given as argument and uses it in the
// following way:
//  On Windows:
//      the proposed name gets suffixed with ".CFG", and gets located in the
//      applicationPath(), above
//      e.g. "MyApp" --> "C:\Program Files\MyApp\MyApp.CFG"
//  On Mac OS X:
//      the proposed name gets lower cased, suffixed with ".config", and located in
//      ~/Library/Preferences/ (where the ~ is created using QDir::homeDirPath())
//      e.g. "MyApp" --> "~/Library/Preferences/myapp.config"
//  On any other operating system, which I assume is a Unix flavor:
//      the proposed name gets lower cased, and prefixed with ~/. (including the dot,
//      where the ~ is created using QDir::homeDirPath())
//      e.g. "MyApp" --> "~/.myapp"
QString configFileName ( QString proposed )
{
#if defined(Q_WS_WIN)
    return applicationPath() + "/" + proposed + ".CFG";
#elif defined(Q_WS_MACX)
    return QDir::homeDirPath() + "/Library/Preferences/" + proposed.lower() + ".config";
#else
    return QDir::homeDirPath() + "/." + proposed.lower();
#endif
}

// see Blanchette & Summerfield pp.237-8;
// that's the starting point of this algorithm.
// fileTypes is like this form: "*.txt *.gif *.bmp"
QStringList findFilesRecursively ( QStringList paths, QString fileTypes ) {
	if ( fileTypes.isEmpty() ) fileTypes = "*";
    QStringList result, more;
    QStringList::Iterator it;
    for ( uint i = 0 ; i < paths.size() ; i++ ) {
        QDir dir( paths[i] );
        more = dir.entryList( fileTypes, QDir::Files );
        for ( it = more.begin() ; it != more.end() ; ++it ) {
            result.append( paths[i] + "/" + *it );
        }
		more = dir.entryList( QDir::Dirs ).grep( QRegExp( "[^.]" ) );
        for ( it = more.begin() ; it != more.end() ; ++it ) {
            *it = paths[i] + "/" + *it;
        }
        more = findFilesRecursively( more, fileTypes );
        for ( it = more.begin() ; it != more.end() ; ++it ) {
            result.append( *it );
        }
    }    
    return result;
}

// and this one is just the other one with the recursive parts trimmed out
QStringList findFiles ( QStringList paths, QString fileTypes ) {
	if ( fileTypes.isEmpty() ) fileTypes = "*";
	QStringList result, files;
	QStringList::Iterator it;
	for ( uint i = 0 ; i < paths.size() ; i++ ) {
		QDir dir( paths[i] );
		files = dir.entryList( fileTypes, QDir::Files );
        for ( it = files.begin() ; it != files.end() ; ++it ) {
            result.append( paths[i] + "/" + *it );
        }
	}
	return result;
}

QStringList findFiles ( QString path, QString fileTypes ) {
	QStringList paths;
	paths << path;
	return findFiles( paths, fileTypes );
}

QStringList findFilesRecursively ( QString path, QString fileTypes ) {
	QStringList paths;
	paths << path;
	return findFilesRecursively( paths, fileTypes );
}


#endif // QT_VERSION


