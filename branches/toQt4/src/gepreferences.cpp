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


#include "gepreferences.h"
#include <myutils.h>
#include <qregexp.h>
#include <QMessageBox>


GEPreferences::GEPreferences ( const QString& filename,
                               const QString& format,
                               const QString& version,
                               const QStringList& colNames )
    : MyPreferences( filename, format, version )
{
    // now that we've loaded the user's information from the file, we can
    // fill in any values that weren't there already with sensible defaults:
    setGroup( "MainWindow" );
    setDefaultPosition( "MainWindow", QPoint( 100, 100 ) );
    setDefaultSize( "MainWindow", QSize( 600, 400 ) );
    setDefaultBool( "maximized", FALSE );
    setDefaultStringList( "groupsearchpaths", resourcesPath() + "/groups" );
    setDefaultStringList( "sheetsearchpaths", resourcesPath() + "/sheets" );
    setDefaultNumber( "maximumGroupFileSize", 125000 );
    setDefaultNumber( "maximumGroupOrder", 60 );
    setDefaultString( "username", "Unnamed User" );
    setDefaultString( "useremail", "" );
    setDefaultBool( "firstTime", TRUE );
    setGroup( "MainHelpWindow" );
    setDefaultPosition( "MainHelpWindow", QPoint( 200, 200 ) );
    setDefaultSize( "MainHelpWindow", QSize( 400, 400 ) );
    setDefaultBool( "maximized", FALSE );
    setGroup( "MainWindowColumns" );

    QStringList colsVisByDefault;
    colsVisByDefault << "Name"
                     << "Order"
                     << "Definition"
                     << "Cayley diagram"
                     << "Multiplication table"
                     << "Object of symmetry"
                     << "Cycle graph";
    for ( int i = 0 ; i < colNames.size() ; i++ ) {
        setDefaultColumnVisible( colNames[i], colsVisByDefault.contains( colNames[i] ) );
        setDefaultColumnPosition( colNames[i], i );
        // and this line checks to be sure that invisible columns are marked as such:
        // (if something gets inadvertently zeroed, you want to be able to find it)
        if ( !getColumnWidth( colNames[i] ) ) setColumnVisible( colNames[i], FALSE );
    }
    setDefaultString( "sortedby", "Order" );
    setDefaultBool( "sortedascending", TRUE );
}

GEPreferences::GEPreferences ()
{
}

GEPreferences* GEPreferences::createCopy ()
{
    GEPreferences* result = new GEPreferences();
    *result = *this;
    return result;
}

void GEPreferences::debugGroup ()
{
    QStringList k = getGroupKeys();
    for ( int i = 0 ; i < k.size() ; i++ ) {
        qDebug( "\t" + k[i] + "\t=>\t" + getString( k[i] ) );
    }
}

void GEPreferences::setColumnVisible ( const QString& key, const bool value ) {
    setGroup( "MainWindowColumns" );
    setNumber( key+"-visible", value );
}

bool GEPreferences::getColumnVisible ( const QString& key, const bool def ) {
    setGroup( "MainWindowColumns" );
    return getNumber( key+"-visible", def );
}

void GEPreferences::setColumnPosition ( const QString& key, const int value ) {
    setGroup( "MainWindowColumns" );
    setNumber( key+"-position", value );
}

int GEPreferences::getColumnPosition ( const QString& key, const int def ) {
    setGroup( "MainWindowColumns" );
    return getNumber( key+"-position", def );
}

void GEPreferences::setColumnWidth ( const QString& key, const int value )
{
    setGroup( "MainWindowColumns" );
    setNumber( key+"-width", value );
}

int GEPreferences::getColumnWidth ( const QString& key, const int def )
{
    setGroup( "MainWindowColumns" );
    return getNumber( key+"-width", def );
}
    
void GEPreferences::setDefaultColumnVisible ( const QString& key, const bool def )
{
    setGroup( "MainWindowColumns" );
    setColumnVisible( key, getColumnVisible( key, def ) );
}

void GEPreferences::setDefaultColumnPosition ( const QString& key, const uint def ) {
    setGroup( "MainWindowColumns" );
    setColumnPosition( key, getColumnPosition( key, def ) );
}

QStringList GEPreferences::getColumnsMentioned () {
    setGroup( "MainWindowColumns" );
    return getGroupKeys() // get all keys for all preferences in MainWindowColumns
            .grep( QRegExp( "-visible$" ) ) // find columns mentioned as visible/not
            .gres( QRegExp( "^(.*)-visible$" ), "\\1" ); // keep only the names
}

void GEPreferences::setGroupNotes ( const QString& code, const QString& notes )
{
    setGroup( "GroupNotes" );
    setString( code, notes );
}

QString GEPreferences::getGroupNotes ( const QString& code, const QString& def )
{
    setGroup( "GroupNotes" );
    return getString( code, def );
}

void GEPreferences::setGroupRep ( const QString& code, const uint repNum,
                                  const MathTextList reps )
{
    setGroup( "GroupNamingSchemes" );
    QStringList tmp;
    for ( int i = 0 ; i < reps.size() ; i++ )
        tmp << reps[i].text;
    setStringList( QString( "%1-%2-" ).arg( code ).arg( repNum ), tmp );
}

MathTextList GEPreferences::getGroupRep ( const QString& code, const uint repNum )
{
    setGroup( "GroupNamingSchemes" );
    QStringList tmp = getStringList( QString( "%1-%2-" ).arg( code ).arg( repNum ) );
    MathTextList result;
    for ( int i = 0 ; i < tmp.size() ; i++ ) result.append( MathText( tmp[i] ) );
    return result;
}

uint GEPreferences::numGroupReps ( const QString& code )
{
    setGroup( "GroupNamingSchemes" );
    uint i = 0;
    while ( getStringListLength( QString( "%1-%2-" ).arg( code ).arg( i ) ) ) {
        i++;
    }
    return i;
}

void GEPreferences::removeGroupRep ( const QString& code, const uint repNum )
{
    setGroup( "GroupNamingSchemes" );
    uint n = numGroupReps( code );
    for ( uint i = repNum + 1 ; i < n ; i++ ) {
        setGroupRep( code, i-1, getGroupRep( code, i ) );
    }
    removeStringList( QString( "%1-%2-" ).arg( code ).arg( n-1 ) );
}

void GEPreferences::insertGroupRep ( const QString& code, const uint beforeThis,
                                     MathTextList rep )
{
    setGroup( "GroupNamingSchemes" );
    uint n = numGroupReps( code );
    for ( uint i = n ; i > beforeThis ; i-- ) {
        setGroupRep( code, i, getGroupRep( code, i-1 ) );
    }
    setGroupRep( code, beforeThis, rep );
}

uint GEPreferences::getCurrentGroupRep ( const QString& code )
{
    setGroup( "GroupNamingSchemes" );
    return getNumber( "currep-" + code, 0 );
}

void GEPreferences::setCurrentGroupRep ( const QString& code, const uint repNum )
{
    setGroup( "GroupNamingSchemes" );
    setNumber( "currep-" + code, repNum );
}

void GEPreferences::createColumn ( const QString& colname )
{
    setGroup( "MainWindowColumns" );
    // first add colname to the list of userdefined columns
    QStringList tmp = getStringList( "userdefined" );
    tmp << colname;
    setStringList( "userdefined", tmp );
    // then give it sensible -visible and -position entries (TRUE and rightmost)
    setColumnPosition( colname, getColumnsMentioned().size() );
    setColumnVisible( colname, TRUE );
}

void GEPreferences::deleteColumn ( const QString& colname )
{
    setGroup( "MainWindowColumns" );
    // first remove colname from the list of userdefined columns
    QStringList tmp = getStringList( "userdefined" );
    tmp.erase( tmp.find( colname ) );
    setStringList( "userdefined", tmp );
    // then remove its -visible and -position entries, but recording its old
    // position so that we might bump all higher-positioned columns down by one
    removeKey( colname + "-visible" );
    int i = getColumnPosition( colname );
    removeKey( colname + "-position" );
    removeKey( colname + "-width" );
    tmp = getColumnsMentioned();
    for ( int j = 0 ; j < tmp.size() ; j++ ) {
        int p = getColumnPosition( tmp[j] );
        if ( p > i ) setColumnPosition( tmp[j], p - 1 );
    }
    // then remove the ColValues-* group of the appropriate name
    setGroup( "ColValues-" + colname );
    removeGroup();
}

void GEPreferences::renameColumn ( const QString& oldname, const QString& newname )
{
    // copy position, width, and visibility over from old to new:
    setColumnPosition( newname, getColumnPosition( oldname ) );
    setColumnWidth( newname, getColumnWidth( oldname ) );
    setColumnVisible( newname, getColumnVisible( oldname ) );
    // copy values over from old to new:
    setGroup( "ColValues-" + oldname );
    QStringList keys = getGroupKeys();
    QStringList vals;
    for ( int i = 0 ; i < keys.size() ; i++ ) vals << getString( keys[i] );
    setGroup( "ColValues-" + newname );
    for ( int i = 0 ; i < keys.size() ; i++ ) setString( keys[i], vals[i] );
    // delete all traces of old:
    deleteColumn( oldname );
    // append new name to list of all user defined columns:
    setGroup( "MainWindowColumns" );
    QStringList tmp = getStringList( "userdefined" );
    tmp << newname;
    setStringList( "userdefined", tmp );
}


QStringList GEPreferences::getUserDefinedColumns ()
{
    setGroup( "MainWindowColumns" );
    return getStringList( "userdefined" );
}

void GEPreferences::setColumnValue ( const QString& colname,
                                     const QString& groupcode,
                                     const QString& value )
{
    setGroup( "ColValues-" + colname );
    setString( groupcode, value );
}

QString GEPreferences::getColumnValue ( const QString& colname, 
                                        const QString& groupcode,
                                        const QString& def )
{
    setGroup( "ColValues-" + colname );
    return getString( groupcode, def );
}

