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


#include "modelviewwidgets.h"
#include "gewhatsthis.h"
#include <qlayout.h>
#include <qslider.h>
#include <qcheckbox.h>
#include <qlabel.h>
//Added by qt3to4:
#include <Q3VBoxLayout>


ModelViewControl::ModelViewControl (
        GEVLarge* container, QWidget* parent, const char* name )
    : GEVLargeWidget( container, parent, name )
{
    addProperty( "zoom", "250" );
    addProperty( "line", "3" );
    addProperty( "node", "100" );
    addProperty( "fogl", "15" );
    addProperty( "fogs", "Y" );

    Q3VBoxLayout* mainLayout = new Q3VBoxLayout( this, 0, 11 );
    mainLayout->setAutoAdd( TRUE ); 

    QLabel* zoom_label = new QLabel( "Zoom level:", this ); 
    zoomSlider = new QSlider( 100, 400, 20, readProperty( "zoom" ).toInt(),
                              Qt::Horizontal, this ); 
    zoomSlider->setTracking( FALSE );
    connect( zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(zoomChanged(int)) );

    QLabel* line_label = new QLabel( "Line thickness:", this ); 
    thickSlider = new QSlider( 1, 10, 2, readProperty( "line" ).toInt(),
                               Qt::Horizontal, this ); 
    thickSlider->setTracking( FALSE );
    connect( thickSlider, SIGNAL(valueChanged(int)), this, SLOT(thicknessChanged(int)) );

    QLabel* node_label = new QLabel( "Node radius:", this ); 
    radiusSlider = new QSlider( 1, 150, 10, readProperty( "node" ).toInt(),
                                Qt::Horizontal, this ); 
    radiusSlider->setTracking( FALSE );
    connect( radiusSlider, SIGNAL(valueChanged(int)), this, SLOT(radiusChanged(int)) );

    fogCheckBox = new QCheckBox( "Use this much fog:", this );
    fogCheckBox->setChecked( readProperty( "fogs" ) == "Y" );
    connect( fogCheckBox, SIGNAL(clicked()), this, SLOT(fogStatusChanged()) );
    fogSlider = new QSlider( 10, 50, 5, readProperty( "fogl" ).toInt(),
                             Qt::Horizontal, this );
    fogSlider->setTracking( FALSE );
    connect( fogSlider, SIGNAL(valueChanged(int)), this, SLOT(fogLevelChanged(int)) );

    GEWhatsThis::setup( zoom_label, "modelView-zoomControls" );
    GEWhatsThis::setup( zoomSlider, "modelView-zoomControls" );
    GEWhatsThis::setup( line_label, "modelView-lineControls" );
    GEWhatsThis::setup( thickSlider, "modelView-lineControls" );
    GEWhatsThis::setup( node_label, "modelView-nodeControls" );
    GEWhatsThis::setup( radiusSlider, "modelView-nodeControls" );
    GEWhatsThis::setup( fogCheckBox, "modelView-fogControls" );
    GEWhatsThis::setup( fogSlider, "modelView-fogControls" );
}

void ModelViewControl::set ( QString key, QString oldvalue, QString value, QString desc )
{
    if ( key == "zoom" ) {
        zoomSlider->setValue( value.toInt() );
    } else if ( key == "line" ) {
        thickSlider->setValue( value.toInt() );
    } else if ( key == "node" ) {
        radiusSlider->setValue( value.toInt() );
    } else if ( key == "fogl" ) {
        fogSlider->setValue( value.toInt() );
    } else if ( key == "fogs" ) {
        fogCheckBox->setChecked( value == "Y" );
        fogSlider->setEnabled( value == "Y" );
    }
    GEVLargeWidget::set( key, oldvalue, value, desc );
}

void ModelViewControl::zoomChanged ( int zoom )
{
    change( "zoom", QString::number( zoom ), 
        QString( "zoom camera from distance %1 to %2" )
        .arg( readProperty( "zoom" ).toInt() / 100.0 ).arg( zoom / 100.0 ) );
}

void ModelViewControl::thicknessChanged ( int thickness )
{
    change( "line", QString::number( thickness ),
        QString( "change line thickness from %1 to %2 pixels" )
        .arg( readProperty( "line" ) ).arg( thickness ) );
}

void ModelViewControl::radiusChanged ( int radius )
{
    change( "node", QString::number( radius ),
        QString( "change base node radius from %1 to %2" )
        .arg( readProperty( "node" ).toInt() / 100.0 ).arg( radius / 100.0 ) );
}

void ModelViewControl::fogLevelChanged ( int level )
{
    change( "fogl", QString::number( level ),
        QString( "change fog level from %1 to %2" )
        .arg( readProperty( "fogl" ).toInt() / 10.0 ).arg( level / 10.0 ) );
}

void ModelViewControl::fogStatusChanged ()
{
    bool on = fogCheckBox->isChecked();
    change( "fogs", on ? "Y" : "N", on ? "turn on fog" : "turn off fog" );
}


