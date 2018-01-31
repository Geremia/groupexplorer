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


/*
 *  Miscellaneous utilities not associated with GE, but that are helpful for
 *  [Qt] programming in general
 */


#ifndef MYUTILS_H
#define MYUTILS_H


#include <qstring.h>

#if QT_VERSION >= 0x040000

#include <QWidget>

// I'm only porting over these utils one at a time.

QString naturalLanguageJoin( QStringList& list );

QStringList findFiles ( QString path, QString fileTypes = QString::null );
QStringList findFiles ( QStringList paths, QString fileTypes = QString::null );
QStringList findFilesRecursively ( QString path, QString fileTypes = QString::null );
QStringList findFilesRecursively ( QStringList paths, QString fileTypes = QString::null );

QString resourcesPath ();
QString applicationPath ();
QString configFileName ( QString proposed );

void assertWidgetSize ( QWidget* w );

QString removeHyperlinks ( QString fromThis );
QString removeHTMLTags ( QString fromThis );

#else

#include <qstringlist.h>
#include <qmap.h>
#include <qcolor.h>
#include <qwidget.h>


void assertWidgetSize ( QWidget* w );

//QString escapeHTML ( QString s );
// made obsolete by my discovery of QStyleSheet::escape()

QString removeHyperlinks ( QString fromThis );
QString removeHTMLTags ( QString fromThis );
QString toRGBHex ( QColor& c );
QColor fromRGBHex ( QString s );

QString naturalLanguageJoin( QStringList& list );

void FullQDebug ( QString d );
void FullQWarning ( QString d );
void FullQFatal ( QString d );

QString resourcesPath ();
QString applicationPath ();
QString configFileName ( QString proposed );

QStringList findFiles (
		QString path, QString fileTypes = QString::null );
QStringList findFiles (
		QStringList paths, QString fileTypes = QString::null );
QStringList findFilesRecursively (
		QString path, QString fileTypes = QString::null );
QStringList findFilesRecursively (
		QStringList paths, QString fileTypes = QString::null );

template<class X, class Y> QString debugMap ( QMap<X,Y> m ) {
	QString result;
	QValueList<X> keys = m.keys();
	for ( uint i = 0 ; i < keys.size() ; i++ ) {
		result += QString( " [%1 => %2]" ).arg( keys[i] ).arg( m[keys[i]] );
	}
	return result.mid( 1 );
}

template<class X> QString debugList ( QValueList<X> lst ) {
	QString result;
	for ( uint i = 0 ; i < lst.size() ; i++ ) {
		result += QString( " [%1 => %2]" ).arg( i ).arg( lst[i] );
	}
	return result.mid( 1 );
}


#endif // QT_VERSION

#endif // MYUTILS_H
