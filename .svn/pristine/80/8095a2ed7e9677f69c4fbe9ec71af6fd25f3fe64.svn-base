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


#include "gewhatsthis.h"
#include "gehelpwindow.h"
#include "utilpieces.h"


TextReference* GEWhatsThis::ref = 0;

GEWhatsThis::GEWhatsThis ( QWidget* widget, const QString& text )
    : Q3WhatsThis( widget )
{
    myText = text;
}


QString GEWhatsThis::text ( const QPoint& )
{
    return myText;
}


bool GEWhatsThis::clicked ( const QString& page )
{
    // if user clicked somewhere besides a hyperlink, close this window:
    if ( page.isEmpty() ) return TRUE;
    // user clicked on hyperlink whose URL is in the page variable, so go to it:
    GEHelpWindow::showHelp( page );
    return FALSE;
}

bool GEWhatsThis::setup ( QObject* obj, QString key )
{
    if ( !ref ) ref = new TextReference( resourcesPath() + "/help/whatsthis.data" );
    if ( !obj || !obj->inherits( "QWidget" ) ) {
//        if ( !obj )
//            qDebug( QString( "GEWhatsThis::setup( %1, \"%2\" ) bombed "
//                             "because !first param" ).arg( (uint)obj ).arg( key ) );
//        else
//            qDebug( QString( "GEWhatsThis::setup( %1, \"%2\" ) bombed "
//                             "because second param doesn't inherit QWidget" )
//                    .arg( (uint)obj ).arg( key ) );
        return FALSE;
    }
    QString data = ref->lookup( key );
    bool result = !data.isNull();
    if ( result ) {
//        qDebug( key + ": " + data );
        (void) new GEWhatsThis( ( QWidget* )obj, data );
    }
//    else qDebug( key + " gave NULL" );
    return result;
}


GEListBoxWhatsThis::GEListBoxWhatsThis ( Q3ListBox* listbox, const QString& text )
    : GEWhatsThis( listbox, text ), listbox( listbox )
{
}


QString GEListBoxWhatsThis::text ( const QPoint& point )
{
    QString result;
    Q3ListBoxItem* qlbi;
    if ( point.x() || point.y() ) {
        // try to find list box item @ the point they clicked; get its what's-this:
        qlbi = listbox->itemAt( point );
    } else {
        // use the currently selected list box item instead:
        qlbi = listbox->selectedItem();
    }
    // does that list box item have what's-this info for us?  try:
    if ( qlbi ) {
        RichListBoxItem* rlbi = (RichListBoxItem*)qlbi;
        result = rlbi->getWhatsThisHTML();
    }
    // if you did not find any, give the whats this info for the whole list box:
    if ( result.isNull() ) result = GEWhatsThis::text( point );
    return result;
}

