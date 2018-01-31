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

#ifndef GEVISUALIZERLIST_H
#define GEVISUALIZERLIST_H


#include <gevlarge.h>
#include <qmime.h>


//#include "gevisualizer.h"
class GEVisualizer;


//
// This class is a factory for every subclass of GEVisualizer
//

class GEVisualizers
{
    
public:
    static GEVisualizer* getNewVisualizer ( QString typeName );
    // later you call setGroup() or decode() or stuff like that

    static QStringList allVisualizerNames ();
    static QString printableName ( QString origName );
        // e.g. "MultiplicationTable" |-> "Multiplication Table"
    static int numIndices ( QString vizTypeName, GroupFile* gf );
    static QString helpURL ( QString vizTypeName );
    
};


//
// This class uses the above class to provide default pixmaps for various
// visualizers for various groups
//

class MainWindow;

class GEVMimeSourceFactory : public Q3MimeSourceFactory
{
    
public:
    GEVMimeSourceFactory ( MainWindow* mainwin = 0 );
    
    virtual const QMimeSource* data ( const QString& abs_name ) const;
    
private:
    MainWindow* main;
    
};


#endif // GEVISUALIZERLIST_H

