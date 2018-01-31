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


#include "progresssplashscreen.h"
#include <qapplication.h>


ProgressSplashScreen::ProgressSplashScreen( Qt::WFlags f )
    : BlankSplashScreen( f ),
    pixmapLabel( this ), textLabel( this ), progressBar( 1000, this ),
    phaseProgress( 0 )
{
    setMargin( 3 );
    setSpacing( 3 );
    starts.append( 0 );
    stops.append( 1000 );
}

void ProgressSplashScreen::setText( QString text ) {
    textContent = text;
    textLabel.setText( text );
    repaint();
    qApp->processEvents(); // or this may not show up in Windows
}

void ProgressSplashScreen::setProgress( int thousandths ) {
    progressBar.setProgress( thousandths );
    repaint();
    qApp->processEvents(); // or this may not show up in Windows
}

void ProgressSplashScreen::setPixmapName( QString data ) {
    pixmapLabel.setText( QString( "<img src=\"%1\" align=\"center\">" )
                         .arg( data ) );
    repaint();
}

void ProgressSplashScreen::enterNewPhase( uint start, uint stop ) {
    int delta = stops.last() - starts.last(), curStart = starts.last();
    starts.append( curStart + delta*start/100 );
    stops.append( curStart + delta*stop/100 );
//    qDebug( QString( "enterNewPhase(%1,%2) gives ...[%3,%4]" )
//            .arg( start ).arg( stop ).arg( starts.last() ).arg( stops.last() ) );
    phaseProgressChanged( 0 );
}

void ProgressSplashScreen::finishPhase() {
    phaseProgressChanged( 100 );
    starts.resize( starts.size() - 1 );
    stops.resize( stops.size() - 1 );
//    qDebug( QString( "finishPhase() gives ...[%1,%2]" )
//            .arg( starts.last() ).arg( stops.last() ) );
}
    
void ProgressSplashScreen::phaseProgressChanged( uint percent ) {
    phaseProgress = percent;
//    qDebug( QString( "phaseProgressChanged(%1) comes to %2" )
//            .arg( percent )
//            .arg( starts.last() + (stops.last() - starts.last())*percent/100 ) );
    setProgress( starts.last() + (stops.last() - starts.last())*percent/100 );
}

void ProgressSplashScreen::progressMessageChanged( QString message ) {
    textLabel.setText( QString( "%1 (%2)" ).arg( textContent ).arg( message ) );
    repaint();
    qApp->processEvents(); // or this may not show up in Windows
}
