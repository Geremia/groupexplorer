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


#include "gevisualizerlist.h"
#include "multiplicationtable.h"
#include "cyclegraph.h"
#include "symmetryobject.h"
#include "cayleydiagram.h"
#include "grouptable.h"
#include <mainwindow.h>


#include "gevisualizer.h"
//Added by qt3to4:
#include <QPixmap>


//
// First the class GEVisualizers, on which GEVMimeSourceFactory depends:
//

GEVisualizer* GEVisualizers::getNewVisualizer ( QString typeName )
{
    if ( typeName == "CayleyDiagram" ) {
        return new CDVisualizer();
    } else if ( typeName == "CycleGraph" ) {
        return new CGVisualizer();
    } else if ( typeName == "MultiplicationTable" ) {
        return new MTVisualizer();
    } else if ( typeName == "SymmetryObject" ) {
        return new OSVisualizer();
    } else {
        qWarning( "Error: No such visualizer type: " + typeName );
        return 0; // bad
    }
}

QStringList GEVisualizers::allVisualizerNames ()
{
    return QStringList() << "CayleyDiagram"
                         << "CycleGraph"
                         << "MultiplicationTable"
                         << "SymmetryObject";
}

QString GEVisualizers::printableName ( QString origName )
{
    if ( origName == "CayleyDiagram" ) {
        return "Cayley Diagram";
    } else if ( origName == "CycleGraph" ) {
        return "Cycle Graph";
    } else if ( origName == "MultiplicationTable" ) {
        return "Multiplication Table";
    } else if ( origName == "SymmetryObject" ) {
        return "Object of Symmetry";
    } else {
        qWarning( "Error: No such visualizer type: " + origName );
        return "(Unknown Visualizer)"; // bad
    }
}

QString GEVisualizers::helpURL ( QString vizTypeName )
{
    if ( vizTypeName == "CayleyDiagram" ) {
        return "rf-groupterms.html#cayleydiagram";
    } else if ( vizTypeName == "CycleGraph" ) {
        return "rf-groupterms.html#cyclegraph";
    } else if ( vizTypeName == "MultiplicationTable" ) {
        return "rf-groupterms.html#multtable";
    } else if ( vizTypeName == "SymmetryObject" ) {
        return "rf-groupterms.html#symmetryobject";
    } else {
        qWarning( "Error: No such visualizer type: " + vizTypeName );
        return "(Unknown Visualizer)"; // bad
    }
}

int GEVisualizers::numIndices ( QString vizTypeName, GroupFile* gf )
{
    GEVisualizer* v = getNewVisualizer( vizTypeName );
    v->setGroup( gf );
    int result = v->numIndices();
    delete v;
    return result;
}

//
// And now GEVMimeSourceFactory:
//

GEVMimeSourceFactory::GEVMimeSourceFactory ( MainWindow* mainwin )
    : main( mainwin )
{
}

const QMimeSource* GEVMimeSourceFactory::data ( const QString& abs_name ) const
{
    // first, do we already have this information?
    const QMimeSource* qms = Q3MimeSourceFactory::data( abs_name );
    if ( qms ) return qms;
    
    // since we do not have it, we must create it; parse abs_name to start:
    QStringList s = QStringList::split( "/", abs_name );
    if ( ( s.size() < 2 ) || ( s.size() > 5 ) ) {
        qWarning( "GEVMimeSourceFactory::data(\"" + abs_name + "\")"
                  " has wrong number of slashes in it (expected 1 to 4)" );
        return 0;
    }

    // try to fetch index, width, and height
    int index = 0;
    int width = -1;
    int height = -1;
    bool ok;
    if ( s.size() > 2 ) {
        index = s[2].toUInt( &ok );
        if ( !ok ) {
            qWarning( "GEVMimeSourceFactory::data(\"" + abs_name + "\")"
                      " gives invalid index (" + s[2] + " not an unsigned int)" );
            return 0;
        }
    }
    if ( s.size() > 3 ) {
        height = width = s[3].toUInt( &ok );
        if ( !ok ) {
            qWarning( "GEVMimeSourceFactory::data(\"" + abs_name + "\")"
                      " gives invalid width (" + s[3] + " not an unsigned int)" );
            return 0;
        }
    }
    if ( s.size() > 4 ) {
        height = s[4].toUInt( &ok );
        if ( !ok ) {
            qWarning( "GEVMimeSourceFactory::data(\"" + abs_name + "\")"
                      " gives invalid height (" + s[4] + " not an unsigned int)" );
            return 0;
        }
    }
    
    // given abs_name of format visualizer_name/group_code, we make a visualizer:
    GEVisualizer* v = GEVisualizers::getNewVisualizer( s[0] );
    if ( !v ) return 0; // error message already given by getNewVisualizer()
    
    // and we'd like to be able to apply it to a group, if we have a main window:
    if ( !main ) {
        qWarning( "GEVMimeSourceFactory cannot find group without main window" );
        return 0;
    }
    
    // ah, at last, everything we need!  we can finish:
    GroupFile* gf = GroupTable::findGroup( s[1], FALSE ); // FALSE == no warning msgs
    if ( gf ) {
        v->setGroup( gf );
        ((Q3MimeSourceFactory*)this)->setPixmap( abs_name,
            v->quickPixmap( width, height, index ) );
        // (why typecast in the line above? to keep the compiler from warning me)
    } else {
        QPixmap banner( width, height ); 
        QPainter p( &banner );
        Q3SimpleRichText srt(
            "<center>Group " + GroupFile::decodePrimaryName( s[1] ) +
            " not loaded</center>", QFont() );
        srt.setWidth( &p, width );
        QBrush b( QColor( 200, 200, 200, QColor::Rgb ) );
        srt.draw( &p, 0, ( height - srt.height() ) / 2,
                  QRect( 0, 0, width, height ), QColorGroup(), &b );
        ((Q3MimeSourceFactory*)this)->setPixmap( abs_name, banner );
    }
    delete v;
    
    // now that we've stored that information, a recursive call will find it and
    // encode it properly for returning:
    return data( abs_name );
}


