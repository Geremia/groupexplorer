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


#include "insvisdialog.h"
#include "groupfile.h"
#include "utilpieces.h"
#include "grouptable.h"
#include "gevisualizerlist.h"
#include <q3listbox.h>
#include <qpushbutton.h>
#include <qregexp.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>


InsertVisualizerDialog::InsertVisualizerDialog ( QWidget* parent )
    : QDialog( parent )
{
    setCaption( "Choose a group and a visualizer" );

    Q3VBoxLayout* mainLay = new Q3VBoxLayout( this ); 
    mainLay->setSpacing( 6 );
    mainLay->setMargin( 11 );
    QLabel* lab = new QLabel(
        "Choose a group from the list on the left,\n"
        "and then choose a visualizer for that group\n"
        "from the list on the right.", this );
    mainLay->addWidget( lab );

    Q3HBoxLayout* listsLay = new Q3HBoxLayout( mainLay );
    listsLay->setSpacing( 6 );
    groupList = new Q3ListBox( this ); 
    listsLay->addWidget( groupList );
    for ( int i = 0 ; i < GroupTable::numGroups() ; i++ ) 
        new RichListBoxItem(
            "<nobr>" + GroupTable::getGroup( i )->getPrimaryName().HTML + "</nobr>",
            groupList );
    connect( groupList, SIGNAL(currentChanged(Q3ListBoxItem*)),
             this, SLOT(groupChanged()) );
    vizList = new Q3ListBox( this ); 
    listsLay->addWidget( vizList );
    connect( vizList, SIGNAL(currentChanged(Q3ListBoxItem*)),
             this, SLOT(vizChanged()) );

    Q3HBoxLayout* buttonLay = new Q3HBoxLayout( mainLay );
    buttonLay->setSpacing( 6 );
    OkButton = new QPushButton( "OK", this ); 
    OkButton->setEnabled( FALSE );
    connect( OkButton, SIGNAL(clicked()), this, SLOT(accept()) );
    CancelButton = new QPushButton( "Cancel", this ); 
    connect( CancelButton, SIGNAL(clicked()), this, SLOT(reject()) );
    QWidget* space = new QWidget( this ); 
    space->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
    buttonLay->addWidget( space );
    buttonLay->addWidget( CancelButton );
    buttonLay->addWidget( OkButton );
}

void InsertVisualizerDialog::vizChanged()
{
    OkButton->setEnabled( bool( vizList->selectedItem() ) );
}

void InsertVisualizerDialog::groupChanged()
{
    vizList->clear();
    GroupFile* gf = groupFile();
    if ( gf ) {
        QStringList names = GEVisualizers::allVisualizerNames();
        for ( int j = 0 ; j < names.count() ; j++ ) {
            int num = QMAX( GEVisualizers::numIndices( names[j], gf ), 1 );
            for ( int k = 0 ; k < num ; k++ )
                vizList->insertItem( ( num == 1 ) ? names[j] :
                    QString( "%1 (%2 of %3)" ).arg( names[j] ).arg( k + 1 ).arg( num ) );
        }
    }
}

QString InsertVisualizerDialog::visualizerName ()
{
    QString text = vizList->selectedItem()->text();
    QRegExp re( "(.+) \\(\\d+ of \\d+\\)" );
    return re.exactMatch( text ) ? re.cap( 1 ) : text;
}

int InsertVisualizerDialog::visualizerIndex ()
{
    QString text = vizList->selectedItem()->text();
    QRegExp re( ".+ \\((\\d+) of \\d+\\)" );
    return re.exactMatch( text ) ? ( re.cap( 1 ).toInt() - 1 ) : 0;
}

GroupFile* InsertVisualizerDialog::groupFile ()
{
    Q3ListBoxItem* item = groupList->selectedItem();
    return item ? GroupTable::getGroup( groupList->index( item ) ) : 0;
}

GEVisualizer* InsertVisualizerDialog::createVisualizer ()
{
    QString name = visualizerName();
    GroupFile* gf = groupFile();
    GEVisualizer* result = GEVisualizers::getNewVisualizer( name );
    result->setGroup( gf );
    if ( GEVisualizers::numIndices( name, gf ) > 0 )
        result->processChange( "indx",
            QString::number( visualizerIndex() ), QString::null );
    return result;
}


