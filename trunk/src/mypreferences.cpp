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


#include "mypreferences.h"


MyPreferences::MyPreferences (
        const QString& filename, const QString& format, const QString& version)
    : Preferences ( filename, format, version ) { }

MyPreferences::MyPreferences () : Preferences () { }

QStringList MyPreferences::getStringList (
        const QString& key, const QStringList& def ) {
    uint i = 0;
    QStringList result;
    QString each;
    while ( (each = getString( key + QString::number(i++) )) != "NULL" ) {
        result.append( each );
    }
    return result.size() ? result : def;
}

QStringList MyPreferences::getStringList ( const QString& key ) {
    QStringList def;
    return getStringList( key, def );
}

void MyPreferences::setStringList (
		const QString& key, const QStringList& value ) {
    int i;
    for ( i = 0 ; i < value.size() ; i++ ) {
        setString( key + QString::number(i), value[i] );
    }
    while ( getString( key + QString::number(i) ) != "NULL" ) {
        removeKey( key + QString::number(i++) );
    }
}

void MyPreferences::setStringList (
		const QString& key, const QString& value ) {
	QStringList tmp;
	tmp << value;
	setStringList( key, tmp );
}

void MyPreferences::removeStringList ( const QString& key )
{
	setStringList( key, QStringList() ); // length 0 == erases all entries
}

uint MyPreferences::getStringListLength ( const QString& key )
{
	uint i = 0;
	while ( getString( key + QString::number( i ) ) != "NULL" ) {
		i++;
	}
	return i;
}

QSize MyPreferences::getSize ( const QString& key, const QSize& def ) {
	return QSize( getNumber( key+"-sizewidth", def.width() ),
	              getNumber( key+"-sizeheight", def.height() ) );
}

QSize MyPreferences::getSize ( const QString& key ) {
	QSize def;
	return getSize( key, def );
}

void MyPreferences::setSize( const QString& key, const QSize& value ) {
	setNumber( key+"-sizewidth", value.width() );
	setNumber( key+"-sizeheight", value.height() );
}

QPoint MyPreferences::getPosition ( const QString& key, const QPoint& def ) {
	return QPoint( getNumber( key+"-positionx", def.x() ),
	               getNumber( key+"-positiony", def.y() ) );
}

QPoint MyPreferences::getPosition ( const QString& key ) {
	QPoint def;
	return getPosition( key, def );
}

void MyPreferences::setPosition( const QString& key, const QPoint& value ) {
	setNumber( key+"-positionx", value.x() );
	setNumber( key+"-positiony", value.y() );
}


void MyPreferences::setDefaultNumber ( const QString& key, const long def ) {
    setNumber( key, getNumber( key, def ) );
}

void MyPreferences::setDefaultBool ( const QString& key, const bool def ) {
    setBool( key, getBool( key, def ) );
}

void MyPreferences::setDefaultDouble ( const QString& key, const double def ) {
    setDouble( key, getDouble( key, def ) );
}

void MyPreferences::setDefaultString (
		const QString& key, const QString& def ) {
    setString( key, getString( key, def ) );
}

void MyPreferences::setDefaultStringList (
        const QString& key, const QStringList& def ) {
    setStringList( key, getStringList( key, def ) );
}

void MyPreferences::setDefaultStringList (
		const QString& key, const QString& def ) {
	QStringList tmp;
	tmp << def;
	setStringList( key, getStringList( key, tmp ) );
}

void MyPreferences::setDefaultSize ( const QString& key, const QSize& def ) {
	setSize( key, getSize( key, def ) );
}

void MyPreferences::setDefaultPosition (
		const QString& key, const QPoint& def ) {
	setPosition( key, getPosition( key, def ) );
}

