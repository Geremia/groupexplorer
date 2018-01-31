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
 *  Nathan Carter made this class, stealing the general idea and a bunch of the code
 *  from Qt's QSplashScreen class.  See the comments in Progresssplashscreen.{h,cpp} for
 *  more information.
 *  This class specializes that one to have a QPixmap, a QProgressBar, and a QLabel,
 *  vertically stacked.
 */


#ifndef PROGRESSSPLASHSCREEN_H
#define PROGRESSSPLASHSCREEN_H


#include "blanksplashscreen.h"
#include <qlabel.h>
#include <q3progressbar.h>
#include <qpixmap.h>
#include <q3valuevector.h>


class ProgressSplashScreen : public BlankSplashScreen
{
    Q_OBJECT
    
public:
    ProgressSplashScreen ( Qt::WFlags f = 0 );
    
    void setText( QString text );
    void setProgress( int thousandths );
    void setPixmapName( QString data );
    
public slots:
    void phaseProgressChanged( uint percent );
    void progressMessageChanged( QString message );
    void enterNewPhase( uint start, uint stop );
    void finishPhase();
    
private:
    QLabel pixmapLabel;
    QLabel textLabel;
    Q3ProgressBar progressBar;
    QString textContent;
    
    uint numPhases;
    Q3ValueVector<uint> starts, stops;
    int phaseProgress;
};


#endif // PROGRESSSPLASHSCREEN_H
