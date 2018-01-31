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


#include "cdwidgets.h"
#include "cayleydiagram.h"
#include "groupfile.h"
#include "utilpieces.h"
#include "mathtableitem.h"
#include "myutils.h"
#include "glmodelview.h"
#include "gewhatsthis.h"
#include "sympanewidgets.h"
#include <math.h>
#include <qlayout.h>
#include <q3listbox.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <q3hbox.h>
#include <q3vbox.h>
#include <qslider.h>
#include <qcheckbox.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <QLabel>
#include <Q3PtrList>
#include <QPixmap>
#include <Q3VBoxLayout>
#include <Q3MemArray>
//Added by me to help porting:
#include <q3mimefactory.h>


QString encode_three ( GEltList gens, Q3MemArray<uint> axes, GEltList ordr )
{
    return ( gens.size() || axes.size() || ordr.size() ) ?
           elt_list_to_code( gens ) + "," +
           elt_list_to_code( axes ) + "," +
           elt_list_to_code( ordr ) : QString( "" );
}
void decode_three ( QString code, GEltList& gens, Q3MemArray<uint>& axes, GEltList& ordr )
{
    QStringList three = code.split( "," );
    gens = ( three.count() > 0 ) ? code_to_elt_list( three[0] ) : GEltList();
    axes = ( three.count() > 1 ) ? code_to_elt_list( three[1] ) : GEltList();
    ordr = ( three.count() > 2 ) ? code_to_elt_list( three[2] ) : GEltList();
}


/*
PushyTable::PushyTable ( QWidget* parent )
    : Q3Table( parent )
{
}

QSize PushyTable::minimumSizeHint () const
{
    return QSize( contentsWidth() + verticalHeader()->width(),
                  contentsHeight() + horizontalHeader()->height() );
}
*/


GeneratorControl::GeneratorControl (
        GEVLarge* container, GroupFile* gf, QWidget* parent, const char* name )
    : GEVLargeWidget( container, parent, name )
{
    addProperty( "gtbl", QString::null, FALSE );
    addProperty( "arro", QString::null, FALSE );
    //addProperty( "head", "100" );
    addProperty( "chnk", "0" );
    //addProperty( "labs", "Y" );
    //addProperty( "labz", "12" );
    addProperty( "mult", "R" );

    axisNames = CDAxisWrapper::getAllAxisNames();
    axisPictures = CDAxisWrapper::getAllAxisPictures();

    Q3VBoxLayout* lay = new Q3VBoxLayout( this, 6, 11 );

    diagram = new QComboBox( this );
    // this gets filled when you call groupFileChanged( gf ) at end of this ctor
    connect( diagram, SIGNAL(activated(int)), this, SLOT(diagramChanged(int)) );
    lay->addWidget( diagram );

    QLabel* priority_label = new QLabel( "Generate diagram this way:", this );
    lay->addWidget( priority_label );

    table = new Q3Table( this );
    table->setRowMovingEnabled( TRUE );
    table->setColumnMovingEnabled( FALSE );
    table->setNumCols( 3 );
    table->setSelectionMode( Q3Table::NoSelection );
    table->horizontalHeader()->setLabel( 0, "Generator" );
    table->horizontalHeader()->setLabel( 1, "Axis" );
    table->horizontalHeader()->setLabel( 2, "Order" );
    connect( table, SIGNAL(contextMenuRequested(int,int,const QPoint&)),
             this, SLOT(contextMenuRequested(int,int,const QPoint&)) );
    connect( table->verticalHeader(), SIGNAL(indexChange(int,int,int)),
             this, SLOT(rowsRearranged(int,int,int)) );
    lay->addWidget( table );
    lay->setStretchFactor( table, 2 );

    QLabel* arrows_label = new QLabel( "Show these arrows:", this );
    lay->addWidget( arrows_label );
    Q3HBox* arrowBox = new Q3HBox( this );
    arrows = new Q3ListBox( arrowBox );
    connect( arrows, SIGNAL(selectionChanged()),
             this, SLOT(enableDisableAddRemoveButtons()) );
    Q3VBox* buttons = new Q3VBox( arrowBox );
    addButton = new QPushButton( "Add...", buttons );
    connect( addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()) );
    removeButton = new QPushButton( "Remove", buttons );
    connect( removeButton, SIGNAL(clicked()), this, SLOT(removeButtonClicked()) );
    lay->addWidget( arrowBox );
    /* MOVED THIS STUFF INTO VIEW TAB; SEE CDLargeControl CONSTRUCTOR IN cayleydiagram.cpp
    QLabel* arrowhead_label = new QLabel( "Arrowhead placement:", this );
    lay->addWidget( arrowhead_label );
    headSlider = new QSlider( 10, 100, 10, readProperty( "head" ).toInt(),
                              Qt::Horizontal, this );
    headSlider->setTracking( FALSE );
    connect( headSlider, SIGNAL(valueChanged(int)), this, SLOT(headPosChanged(int)) );
    lay->addWidget( headSlider );
    */
    Q3HBox* multBox = new Q3HBox( this );
    QLabel* mult_label = new QLabel( "Arrows mean ", multBox );
    QComboBox* mult = new QComboBox( multBox );
    mult->insertItem( "right multiplication" );
    mult->insertItem( "left multiplication" );
    connect( mult, SIGNAL(activated(int)), this, SLOT(changeMult(int)) );
    lay->addWidget( multBox );

    /* MOVED THIS STUFF INTO VIEW TAB; SEE CDLargeControl CONSTRUCTOR IN cayleydiagram.cpp
    labelCheckBox = new QCheckBox( "Show labels of this size:", this );
    labelCheckBox->setChecked( readProperty( "labs" ) == "Y" );
    connect( labelCheckBox, SIGNAL(clicked()), this, SLOT(labelStatusChanged()) );
    lay->addWidget( labelCheckBox );
    labelSlider = new QSlider( 10, 40, 1, readProperty( "labz" ).toInt(),
                               Qt::Horizontal, this );
    labelSlider->setTracking( FALSE );
    connect( labelSlider, SIGNAL(valueChanged(int)), this, SLOT(labelSizeChanged(int)) );
    lay->addWidget( labelSlider );
    */

    chunkLabel = new QLabel( "Chunk this subgroup:", this );
    lay->addWidget( chunkLabel );
    chunk = new QComboBox( this );
    // this gets filled when you call groupFileChanged( gf ) at end of this ctor
    connect( chunk, SIGNAL(activated(int)), this, SLOT(changeChunk(int)) );
    lay->addWidget( chunk );

    GEWhatsThis::setup( diagram, "cd-diagramChooser" );
    GEWhatsThis::setup( priority_label, "cd-priorityTable" );
    GEWhatsThis::setup( table, "cd-priorityTable" );
    GEWhatsThis::setup( arrows_label, "cd-arrowsControls" );
    GEWhatsThis::setup( arrows, "cd-arrowsControls" );
    GEWhatsThis::setup( addButton, "cd-arrowsControls" );
    GEWhatsThis::setup( removeButton, "cd-arrowsControls" );
    //GEWhatsThis::setup( arrowhead_label, "cd-headPosSlider" );
    //GEWhatsThis::setup( headSlider, "cd-headPosSlider" );
    //GEWhatsThis::setup( labelCheckBox, "cd-labelSizeControls" );
    //GEWhatsThis::setup( labelSlider, "cd-labelSizeControls" );
    GEWhatsThis::setup( chunkLabel, "cd-chunkControls" );
    GEWhatsThis::setup( chunk, "cd-chunkControls" );
    GEWhatsThis::setup( mult_label, "cd-multBox" );
    GEWhatsThis::setup( mult, "cd-multBox" );

    groupFileChanged( gf );
}

void GeneratorControl::reset ()
{
    resetDiagram();
    resetGenerators();
    resetArrows();
    gTableChanged();
    arrowsChanged();
}

void GeneratorControl::niceDiagram ( GroupFile* groupfile, GEltList& gens,
                                     GEltGrid& grid, GEltList& axes, GEltList& ordr )
{
    axes.resize( gens.size() );
    ordr.resize( gens.size() );
    grid = CDLargeView::generateOrbitGrid( groupfile, gens );
    switch ( gens.size() ) {
        case 2:
            // try to alleviate 2xN case:
            if ( ( grid.dimension( 0 ) == 2 ) && ( grid.dimension( 1 ) > 2 ) ) {
                GEltList testgens( 2 );
                testgens[0] = gens[1];
                testgens[1] = gens[0];
                GEltGrid test = CDLargeView::generateOrbitGrid( groupfile, testgens );
                if ( test.dimension( 0 ) > 2 ) {
                    grid = test;
                    gens = testgens;
                }
            }
            break;
        /*
         *  FORGET THIS -- THESE 3D HEURISTICS ONLY WORK 40% OF THE TIME -- JUST DO GRIDS
         *
        case 3:
            // try to alleviate 2x2xN case:
            if ( ( grid.dimension( 0 ) == 2 ) && ( grid.dimension( 1 ) == 2 )
              && ( grid.dimension( 2 ) > 2 ) ) {
                GEltList testgens( 3 );
                testgens[0] = gens[2];
                testgens[1] = gens[0];
                testgens[2] = gens[1];
                GEltGrid test = CDLargeView::generateOrbitGrid( groupfile, testgens );
                if ( ( test.dimension( 0 ) > 2 ) || ( test.dimension( 1 ) > 2 ) ) {
                    grid = test;
                    gens = testgens;
                }
            }
            // try to alleviate 2xNxM case:
            if ( ( grid.dimension( 0 ) == 2 )
              && ( grid.dimension( 1 ) > 2 ) && ( grid.dimension( 2 ) > 2 ) ) {
                GEltList testgens( 3 );
                testgens[0] = gens[1];
                testgens[1] = gens[2];
                testgens[2] = gens[0];
                GEltGrid test = CDLargeView::generateOrbitGrid( groupfile, testgens );
                if ( test.dimension( 0 ) > 2 ) {
                    grid = test;
                    gens = testgens;
                }
            }
            // try to alleviate Nx2xM case:
            if ( ( grid.dimension( 0 ) > 2 )
              && ( grid.dimension( 1 ) == 2 )
              && ( grid.dimension( 2 ) > 2 ) ) {
                GEltList testgens( 3 );
                testgens[0] = gens[0];
                testgens[1] = gens[2];
                testgens[2] = gens[1];
                GEltGrid test = CDLargeView::generateOrbitGrid( groupfile, testgens );
                if ( ( test.dimension( 0 ) > 2 ) && ( test.dimension( 1 ) > 2 ) )
                    grid = test;
            }
            break;
         *
         */
    }
    for ( uint i = 0 ; i < gens.size() ; i++ ) ordr[i] = gens.size() - 1 - i;

    // and now change the axes to best suit the generators:
    uint i;
    switch ( gens.size() ) {
        case 1: // 1D case: circles
            axes[0] = 3; // Circular x,y
            break;
        case 2: // 2D case: polar, cylindrical, or rectangular
            if ( grid.dimension( 0 ) == 2 ) {
                if ( grid.dimension( 1 ) > 2 ) {
                    axes[1] = 2; // Linear z
                    axes[0] = 3; // Circular x,y
                    break;
                }
            } else if ( grid.dimension( 1 ) == 2 ) {
                axes[1] = 6; // Rotational x,y
                axes[0] = 0; // Linear x
                break;
            }
            // rectangular cases fall through to here:
            axes[1] = 0;
            axes[0] = 1;
            break;
        /*
         *  FORGET THIS -- THESE 3D HEURISTICS ONLY WORK 40% OF THE TIME -- JUST DO GRIDS
         *
        case 3: // 3D case: cylindrical or rectangular solids
            if ( grid.dimension( 0 ) == 2 ) {
                if ( grid.dimension( 1 ) == 2 ) {
                    if ( grid.dimension( 2 ) > 2 ) {
                        axes[0] = 2; // Linear z
                        axes[1] = 0; // Linear y
                        axes[2] = 6; // Rotational x,y
                        ordr[1] = 2; // Linear x is inside Rotational x,y
                        ordr[2] = 1; // Rotational x,y is outside Linear x
                        break;
                    }
                } else {
                    if ( grid.dimension( 2 ) < 4 ) {
                        axes[0] = 2; // Linear z
                        axes[1] = 6; // Rotational in x,y
                        axes[2] = 0; // Linear x
                        break;
                    }
                }
            } else {
                if ( grid.dimension( 1 ) == 2 ) {
                    if ( grid.dimension( 2 ) == 2 ) {
                        axes[0] = 6; // Rotational in x,y
                        axes[1] = 0; // Linear x
                        axes[2] = 2; // Linear z;
                        break;
                    }
                } else {
                    if ( grid.dimension( 2 ) < 4 ) {
                        axes[0] = 2; // Linear z;
                        axes[1] = 6; // Rotational in x,y
                        axes[2] = 0; // Linear x
                        break;
                    }
                }
            }
            // rectangular cases fall through to here:
         *
         */
        default:
            for ( i = 0 ; i < gens.size() ; i++ ) axes[i] = i % 3; // Rectangular
    }
}

void GeneratorControl::resetGenerators ()
{
    if ( !groupfile ) {
        setGenerators( GEltList() ); // clear it out
        return;
    }
    // find a good set of generators:
    GEltList mygens = groupfile->generatorlists.count() ? groupfile->generatorlists[0]
                                                        : groupfile->group.findGens();
    // farm the hard work out:
    GEltGrid grid;
    niceDiagram( groupfile, mygens, grid, axes, orderperm );
    // save our changes and make the table reflect them:
    setGenerators( mygens );
    for ( uint i = 0 ; i < mygens.size() ; i++ ) {
        updateTableCell( i, 1 );
        updateTableCell( i, 2 );
    }
    checkChunkOrder();
}

void GeneratorControl::checkChunkOrder ()
{
    // chunking only permitted if order is inside...outside, perfectly linearly
    // because otherwise it is not certain that chunking is possible; depends on the gp
    for ( uint i = 0 ; i < orderperm.size() ; i++ ) {
        if ( orderperm[orderperm.size() - 1 - i] != i ) {
            change( "chnk", "0", QString::null );
            chunkLabel->setEnabled( false );
            chunk->setEnabled( false );
            return;
        }
    }
    chunkLabel->setEnabled( true );
    chunk->setEnabled( true );
}

void GeneratorControl::resetArrows ()
{
    /*
    setArrows( GEltList() );
    if ( groupfile )
        setArrows( groupfile->generatorlists.count() ? groupfile->generatorlists[0]
                                                     : groupfile->group.findGens() );
    */
    if ( groupfile ) {
        int indx = readProperty( "indx" ).toInt();
        /*
        qDebug( "%s", QString( " ** GeneratorControl::resetArrows() with indx==%1 "
                               "and groupfile==%2 and cays[%3].getArrows==%4" )
                      .arg( indx ).arg( uint(groupfile) ).arg( indx-1 )
                      .arg( indx && groupfile ?
                            elt_list_to_string( groupfile->cays[indx-1].getArrows() ) :
                            QString( "N/A" ) ).latin1() );
        */
        if ( ( indx < int(groupfile->cays.count()) ) && ( indx != -1 ) ) {
            setArrows( groupfile->cays[indx].getArrows() );
        } else {
            setArrows( generators );
        }
    } else {
        setArrows( GEltList() );
    }
}

void GeneratorControl::resetDiagram ()
{
    diagram->clear();
    diagram->insertItem( "Generate diagram" );
    if ( groupfile )
        for ( int i = 0 ; i < groupfile->cays.count() ; i++ )
            diagram->insertItem( "Use " + groupfile->cays[i].getName() );
    diagram->setCurrentItem( 0 );
    table->setEnabled( TRUE );
    chunk->setEnabled( TRUE );
    chunkLabel->setEnabled( TRUE );
}

void GeneratorControl::set ( QString key, QString oldvalue, QString value, QString desc )
{
    /*
    qDebug( "GeneratorControl::set( \"" + key + "\", \"" + oldvalue + "\", \"" +
            value + "\", \"" + desc + "\" )" );
    */
    if ( key == "gtbl" ) {
        if ( value.isNull() ) {
            resetGenerators();
            gTableChanged();
        } else {
            decode_three( value, generators, axes, orderperm );
            setGenerators( generators );
            setAxes( axes );
            setOrder( orderperm );
        }
    } else if ( key == "arro" ) {
        if ( value.isNull() ) {
            resetArrows();
            arrowsChanged();
        } else {
            setArrows( code_to_elt_list( value ) );
        }
    } else if ( key == "chnk" ) {
        chunk->setCurrentItem( value.toInt() );
    /*
    } else if ( key == "labs" ) {
        labelCheckBox->setChecked( value == "Y" );
    } else if ( key == "labz" ) {
        labelSlider->setValue( value.toInt() );
    } else if ( key == "head" ) {
        headSlider->setValue( value.toInt() );
    */
    } else if ( key == "indx" ) {
        int v = value.toInt();
        v = ( ( v == int(groupfile->cays.count()) ) || ( v == -1 ) ) ? 0 : ( v + 1 );
        diagram->setCurrentItem( v );
        table->setEnabled( !v );
        chunk->setEnabled( !v );
        chunkLabel->setEnabled( !v );
    }
    GEVLargeWidget::set( key, oldvalue, value, desc );
}

/*
void GeneratorControl::updateContent ( QStringList changedKeys )
{
    // this actually gets handled adequately in set(...)
}
*/

void GeneratorControl::groupFileChanged ( GroupFile* gf )
{
    GEVLargeWidget::groupFileChanged( gf );
    reset();
    assertWidgetSize( table );
}

QString orderText ( int i, int outOf )
{
    if ( outOf < 1 ) {
        qWarning( QString( "Error in orderText( %1, %2 ): second parameter too small" )
                  .arg( i ).arg( outOf ) );
        return "ERROR:TOO-FEW";
    }
    if ( outOf > 21 ) {
        qWarning( QString( "Error in orderText( %1, %2 ): second parameter too large" )
                  .arg( i ).arg( outOf ) );
        return "ERROR:TOO-MANY";
    }
    if ( ( i < 0 ) || ( i >= outOf ) ) {
        qWarning( QString( "Error in orderText( %1, %2 ): first parameter not in 0..%3" )
                  .arg( i ).arg( outOf ).arg( outOf - 1 ) );
        return "ERROR:BAD-INDEX";
    }
    if ( outOf == 1 ) return "N/A";
    if ( outOf == 2 ) return i ? "inside" : "outside";
    // outOf > 2
    if ( i == 0 ) return "outermost";
    if ( i == outOf - 1 ) return "innermost";
    if ( ( outOf % 2 ) && ( i == ( outOf - 1 ) / 2 ) ) return "middle";
    static QStringList ordinals;
    if ( !ordinals.count() )
        ordinals << "first" << "second"  << "third"  << "fourth" << "fifth"
                 << "sixth" << "seventh" << "eighth" << "ninth"  << "tenth";
    if ( i < outOf / 2 ) return ordinals[i] + " outermost";
    return ordinals[outOf - i] + " innermost";
}

void GeneratorControl::updateTableCell ( int row, int col )
{
    if ( ( col < 0 ) || ( col > 2 ) || ( row < 0 ) || ( row >= table->numRows() ) )
        return;

    if ( col == 2 ) {
        table->setText( row, 2, orderText( orderperm[row], table->numRows() ) );
    } else if ( col == 1 ) {
        table->setPixmap( row, 1, qPixmapFromMimeSource( axisPictures[axes[row]] ) );
        table->setText( row, 1, axisNames[axes[row]] );
    } else {
        table->setItem( row, 0, new GETableItem(
            table, 0, groupfile->getRepresentation( generators[row] ) ) );
    }
    table->adjustRow( row );
}

void GeneratorControl::setGenerators ( GEltList gens )
{
    // take the generators and update the table's left column:
    generators = gens.copy();
    orbitGrid = CDLargeView::generateOrbitGrid( groupfile, generators );
    table->setNumRows( generators.size() );
    for ( uint i = 0 ; i < generators.size() ; i++ ) updateTableCell( i, 0 );
    // now extend or contract the axes list and orderperm to match the generators list
    if ( axes.size() > generators.size() ) {
        axes.resize( generators.size() );
    } else if ( axes.size() < generators.size() ) {
        uint oldsize = axes.size();
        axes.resize( generators.size() );
        for ( uint i = oldsize ; i < axes.size() ; i++ ) axes[i] = 0;
    }
    for ( uint i = 0 ; i < axes.size() ; i++ ) updateTableCell( i, 1 );
    if ( orderperm.size() > generators.size() ) {
        orderperm = iota( generators.size() ); // this could be more sophisticated, but...
    } else if ( orderperm.size() < generators.size() ) {
        uint oldsize = orderperm.size();
        orderperm.resize( generators.size() );
        for ( uint i = oldsize ; i < orderperm.size() ; i++ ) orderperm[i] = i;
    }
    for ( uint i = 0 ; i < orderperm.size() ; i++ ) updateTableCell( i, 2 );
    table->adjustColumn( 0 );
    table->adjustColumn( 1 );
    table->adjustColumn( 2 );
    int left, top, right, bottom;
    table->getContentsMargins( &left, &top, &right, &bottom );
    table->setMinimumSize( table->contentsWidth() + table->verticalHeader()->width()
                         + left + right + 1,
                           table->contentsHeight() + table->horizontalHeader()->height()
                         + top + bottom + 1 );
    // and update the contents of the chunking list box
    int lastSelection = chunk->currentItem();
    chunk->clear();
    chunk->insertItem( "(no chunking)" );
    if ( !groupfile ) return;
    QString sofar;
    GEltListList sgrs = groupfile->group.subgroups();
    GEltList sgrgens;
    for ( uint i = 0 ; i + 1 < generators.size() ; i++ ) {
        if ( i ) sofar += ", ";
        sofar += groupfile->getRepresentation( generators[i] ).text;
        sgrgens.resize( sgrgens.size() + 1 );
        sgrgens[sgrgens.size() - 1] = generators[i];
        GEltList sgr = groupfile->group.subgroupGenBy( sgrgens );
        int j;
        for ( j = 0 ; j < sgrs.count() ; j++ )
            if ( elt_sets_are_equal( sgrs[j], sgr ) ) break;
        chunk->insertItem( QString( "%1 generated by { %2 }" )
                           .arg( j < sgrs.count() ?
                                 QString( "H_%1," ).arg( j + 1 ) : QString( "Subgroup" ) )
                           .arg( sofar ) );
    }
    if ( generators.size() ) chunk->insertItem( "The whole group" );
    chunk->setCurrentItem( QMIN( lastSelection, chunk->count() - 1 ) );
}

void GeneratorControl::setAxes ( Q3MemArray<uint> ax )
{
    // note that this routine never changes the size of the axes list, because the size
    // of the axes list is a function of the size of the generators list.
    // thus this one just copies out of the given list into the current one, truncating
    // the given one if need be, and padding it with zeros if need be.
    for ( uint i = 0 ; i < axes.size() ; i++ ) axes[i] = ( i < ax.size() ) ? ax[i] : 0;
}

void GeneratorControl::setOrder ( GEltList order )
{
    orderperm = order.copy();
    checkChunkOrder();
}

void GeneratorControl::setArrows ( GEltList arro )
{
    arrows->clear();
    if ( !groupfile ) return;
    for ( uint i = 0 ; i < arro.size() ; i++ ) {
        RichListBoxItem* rlbi = new RichListBoxItem(
            groupfile->getRepresentation( arro[i] ).HTML, arrows );
        rlbi->setTag( arro[i] );
    }
    enableDisableAddRemoveButtons();
}

void GeneratorControl::fixAnyGeneratorProblems ( uint lastGoodGenIndex )
{
    // for each generator after the last good one, be sure it makes sense
    for ( uint i = lastGoodGenIndex + 1 ; i < generators.size() ; i++ ) {
        // first figure out everything generated by generators 0 through i
        GEltList initialSegment = generators.copy();
        initialSegment.resize( i );
        GEltList sgr = groupfile->group.subgroupGenBy( initialSegment );
        if ( sgr.size() == groupfile->group.order ) {
            // if that's everything, we need no more rows
            setGenerators( initialSegment );
        } else {
            // otherwise, be sure the next generator is legit (i.e. not yet used)
            if ( sgr.contains( generators[i] ) ) {
                // if not, find one who is (unused)
                GEltList extended = groupfile->group.completeGens(
                    initialSegment, groupfile->group.elements() );
                generators[i] = extended[i];
            }
        }
    }
    // if that does not suffice to generate the group, append more rows + generators
    if ( groupfile->group.subgroupGenBy( generators ).size() < groupfile->group.order )
        generators = groupfile->group.completeGens(
            generators, groupfile->group.elements() );
    setGenerators( generators );
}

void appendSubgroups ( GroupFile* g, QMenu& m )
{
    QMenu* sm = m.addMenu( "Organize by" );
    for ( int i = 0 ; i < g->group.subgroups().count() ; i++ ) {
        QAction* ac = sm->addAction(
            QString( "%1, a subgroup of order %2" )
            .arg( SubsetListBox::subsetTextName( 'H', i + 1 ) )
            .arg( g->group.subgroups()[i].size() ) );
        ac->setData( 10000+i );
    }
}
void GeneratorControl::contextMenuRequested ( int row, int col, const QPoint& pos )
{
    QMenu menu( this );
    int result = -1;
    QAction* resultAction;

    if ( ( col < 0 ) || ( col > 2 ) || ( row < 0 ) || ( row >= table->numRows() ) ) {

        appendSubgroups( groupfile, menu );
        resultAction = menu.exec( pos );
        if ( resultAction != NULL ) result = resultAction->data().toInt();

    } else if ( col == 2 ) {

        if ( !orderperm.size() ) { // I believe I've stomped this out, but just to be safe:
            qWarning( "GeneratorControl::contextMenuRequested(): empty orderperm" );
            return;
        }
        for ( uint i = 0 ; i < orderperm.size() ; i++ )
            menu.insertItem( orderText( i, table->numRows() ), i );
        menu.insertSeparator();
        appendSubgroups( groupfile, menu );

        resultAction = menu.exec( pos );
        if ( resultAction != NULL ) result = resultAction->data().toInt();
        if ( ( result > -1 ) && ( result < 9999 ) ) {
            int swapper = -1;
            for ( uint i = 0 ; i < orderperm.size() ; i++ ) {
                if ( orderperm[i] == (uint)result ) {
                    swapper = i;
                    break;
                }
            }
            if ( swapper > -1 ) {
                orderperm[swapper] = orderperm[row];
                orderperm[row] = result;
                updateTableCell( swapper, 2 );
                updateTableCell( row, 2 );
                gTableChanged( "change order" );
            }
        }

    } else if ( col == 1 ) {

        for ( int i = 0 ; i < axisNames.count() ; i++ ) {
            if ( ( orbitGrid.dimension( orbitGrid.numDimensions() - 1 - row ) > 2 )
              || ( i < 3 ) ) {
                QAction* tmp = new QAction( QIcon( qPixmapFromMimeSource( axisPictures[i] ) ),
                                            axisNames[i], this );
                tmp->setData( i );
                menu.addAction( tmp );
                //menu.insertItem( new RichCustomMenuItem(
                //    "<nobr><img src=\"" + axisPictures[i] +
                //    "\"> " + axisNames[i] + "</nobr>" ), i );
            }
        }
        menu.insertSeparator();
        appendSubgroups( groupfile, menu );

        resultAction = menu.exec( pos );
        if ( resultAction != NULL ) result = resultAction->data().toInt();
        if ( ( result > -1 ) && ( result < 9999 ) ) {
            axes[row] = (uint)result;
            updateTableCell( row, 1 );
            gTableChanged( "change axis" );
        }

    } else {

        GEltList initialSegment = generators.copy();
        initialSegment.resize( row );
        GEltList sgr = groupfile->group.subgroupGenBy( initialSegment );

        for ( GElt g = 0 ; g < groupfile->group.order ; g++ ) {
            if ( !sgr.contains( g ) ) {
                QAction* tmp = new QAction( groupfile->getRepresentation( g ).text, this );
                tmp->setData( g );
                menu.addAction( tmp );
                //menu.insertItem( new RichCustomMenuItem(
                //    groupfile->getRepresentation( g ).HTML ), g );
            }
        }
        menu.insertSeparator();
        appendSubgroups( groupfile, menu );

        resultAction = menu.exec( pos );
        if ( resultAction != NULL ) result = resultAction->data().toInt();
        if ( ( result > -1 ) && ( result < 9999 ) ) {
            generators[row] = (GElt)result;
            setGenerators( generators );
            fixAnyGeneratorProblems( row );
            gTableChanged( "change generator" );
        }

    }

    if ( result >= 10000 ) {
        GEltList sgr = groupfile->group.subgroups()[result-10000];
        GEltList gns = groupfile->group.neededGens( sgr );
        GEltList cgs = groupfile->group.completeGens( gns, groupfile->group.elements() );
        setArrows(cgs);
        arrowsChanged( QString::null );
        setGenerators( cgs );
        Q3MemArray<uint> list( cgs.size() );
        for ( uint i = 0 ; i < list.size() ; i++ ) list[i] = list.size() - 1 - i;
        setOrder( list );
        for ( uint i = 0 ; i < list.size() ; i++ ) list[i] = i % 3;
        setAxes( list );
        gTableChanged( "organize by subgroup" );
    }

}

void GeneratorControl::rowsRearranged ( int, int from, int to )
{
    // the change has already happened visually; we must simply catch the internal data
    // up to what's on-screen:
    int direction;
    if ( from < to ) {
        direction = 1;
        to--; // this is because the row you're moving disappears, so indices change!!!
        // I know, it's bizarre, but it's wrong without this line.  phooey.
    } else {
        direction = -1;
    }
    for ( int i = from ; i != to ; i += direction ) {
        GElt tmpg = generators[i];
        generators[i] = generators[i + direction];
        generators[i + direction] = tmpg;
        uint tmpa = axes[i];
        axes[i] = axes[i + direction];
        axes[i + direction] = tmpa;
        tmpa = orderperm[i];
        orderperm[i] = orderperm[i + direction];
        orderperm[i + direction] = tmpa;
    }
    // now check to be sure that this setup is legitimate, as far as generator lists go:
    fixAnyGeneratorProblems( (uint) QMIN( from, to ) );
    gTableChanged( "rearrange generators" );
}

void GeneratorControl::addButtonClicked ()
{
    if ( !groupfile ) return;
    QMenu menu( this );
    GEltList arrs = getArrows();
    for ( uint i = 1 ; i < groupfile->group.order ; i++ ) {
        if ( !arrs.contains( i ) ) {
            QAction* tmp = new QAction( groupfile->getRepresentation( i ).text, this );
            tmp->setData( i );
            menu.addAction( tmp );
            //menu.insertItem( new RichCustomMenuItem(
            //    groupfile->getRepresentation( i ).HTML ), i );
        }
    }
    QAction* resultAction =
            menu.exec( addButton->mapToGlobal( QPoint( 0, addButton->height() ) ) );
    if ( resultAction != NULL ) {
        int result = resultAction->data().toInt();
        RichListBoxItem* rlbi = new RichListBoxItem(
            groupfile->getRepresentation( (GElt)result ).HTML, arrows );
        rlbi->setTag( (GElt)result );
        arrowsChanged( "add an arrow" );
    }
}

void GeneratorControl::removeButtonClicked ()
{
    if ( arrows->selectedItem() ) {
        delete arrows->selectedItem();
        arrowsChanged( "remove an arrow" );
    }
}

void GeneratorControl::enableDisableAddRemoveButtons ()
{
    removeButton->setEnabled( arrows->selectedItem() );
    addButton->setEnabled(
        groupfile && ( arrows->count() < groupfile->group.order - 1 ) );
}

GEltList GeneratorControl::getArrows ()
{
    GEltList result( arrows->count() );
    for ( uint i = 0 ; i < result.size() ; i++ )
        result[i] = ( (RichListBoxItem*)( arrows->item( i ) ) )->getTag();
    return result;
}

void GeneratorControl::changeChunk ( int toThis )
{
    if ( groupfile ) {
        QStringList resetThese;
        for ( uint i = 0 ; i < groupfile->group.order ; i++ ) {
            QString test = QString( "N%1" ).arg( i );
            if ( readProperty( test ).length() ) resetThese << test;
        }
        if ( resetThese.count() ) {
            int answer = QMessageBox::question(
                this, "Chunking must move nodes",
                QString( "You have manually edited the positions of\n"
                         "%1 node%2 in the diagram.  Those nodes will\n"
                         "be automatically moved back to their original\n"
                         "locations if you proceed with chunking.\n"
                         "\n"
                         "Proceed with chunking?" )
                         .arg( resetThese.count() )
                         .arg( ( resetThese.count() > 1 ) ? "s" : "" ),
                QMessageBox::Yes | QMessageBox::Default,
                QMessageBox::No | QMessageBox::Escape );
            if ( answer == QMessageBox::Yes ) {
                for ( int i = 0 ; i < resetThese.count() ; i++ )
                    change( resetThese[i], QString::null, "restore node position" );
            } else {
                chunk->setCurrentItem( readProperty( "chnk" ).toInt() );
                return;
            }
        }
    }
    chunkChanged( toThis ? "set chunking level" : "turn off chunking" );
}

void GeneratorControl::changeMult ( int toThis )
{
    change( "mult", toThis ? "L" : "R", "change multiplication order" );
}

void GeneratorControl::diagramChanged ( int toThis )
{
    change( "indx",
            QString::number( toThis ? toThis - 1 : groupfile->cays.count() ),
            "choose new diagram" );
    resetArrows();
    arrowsChanged();
}

/*
void GeneratorControl::labelSizeChanged ( int toThis )
{
    change( "labz", QString::number( toThis ), "change label size" );
}

void GeneratorControl::labelStatusChanged ()
{
    bool on = labelCheckBox->isChecked();
    change( "labs", on ? "Y" : "N", on ? "turn labels on" : "turn labels off" );
}

void GeneratorControl::headPosChanged ( int toThis )
{
    change( "head", QString::number( toThis ), "change arrowhead position" );
}
*/

void GeneratorControl::gTableChanged ( QString reason )
{
    change( "gtbl", encode_three( generators, axes, orderperm ), reason );
}

void GeneratorControl::arrowsChanged ( QString reason )
{
    change( "arro", elt_list_to_code( getArrows() ), reason );
}

void GeneratorControl::chunkChanged ( QString reason )
{
    change( "chnk", QString::number( chunk->currentItem() ), reason );
}


CDAxisWrapper::CDAxisWrapper ( GLSceneNode* parent )
    : chunk( FALSE ), axistype( 99999 )
{
    if ( parent ) parent->addChild( this );
    name = "no-axis"; // note all names this object ever has should end in "-axis"
    // see cayleydiagram.cpp for reliance on this (search for "-axis")
    setColor( 0, 0, 0, 0.25 );
}

void CDAxisWrapper::chunkThis ( bool on )
{
    chunk = on;
}

void CDAxisWrapper::preRender ()
{
    // see if any children are also CDAxisWrappers:
    GLSceneNode* childCDAW = 0;
    for ( uint i = 0 ; i < children.count() ; i++ ) {
        if ( children.at( i )->name.endsWith( "-axis" ) ) {
            childCDAW = children.at( i );
            break;
        }
    }
    // if so, and they are rotational, then we must ensure that OUR scaling is normalized
    // for THEIR sake; they are the ones that make their local coord system have axes
    // not parallel to those of the parent
    if ( childCDAW && childCDAW->name.startsWith( "rotational-" ) ) {
        translate( 0.5, 0.5, 0.5 );
        normalizeScaling();
        translate( -0.5, -0.5, -0.5 );
    }
    // ok all done my stuff; do superclass stuff
    GLSceneNode::preRender();
}

void CDAxisWrapper::renderMe ()
{
    if ( chunk ) {
        bool tmpx, tmpy, tmpz;
        axesUsed( tmpx, tmpy, tmpz );
        float xmin = tmpx ? 0.02 : 0.35;
        float ymin = tmpy ? 0.02 : 0.35;
        float zmin = tmpz ? 0.02 : 0.35;
        float xmax = 1 - xmin;
        float ymax = 1 - ymin;
        float zmax = 1 - zmin;
        glBegin( GL_POLYGON );
            glVertex3f( xmin, ymin, zmin );
            glVertex3f( xmin, ymax, zmin );
            glVertex3f( xmax, ymax, zmin );
            glVertex3f( xmax, ymin, zmin );
        glEnd();
        glBegin( GL_POLYGON );
            glVertex3f( xmin, ymin, zmax );
            glVertex3f( xmin, ymax, zmax );
            glVertex3f( xmax, ymax, zmax );
            glVertex3f( xmax, ymin, zmax );
        glEnd();
        glBegin( GL_POLYGON );
            glVertex3f( xmin, ymin, zmin );
            glVertex3f( xmin, ymin, zmax );
            glVertex3f( xmax, ymin, zmax );
            glVertex3f( xmax, ymin, zmin );
        glEnd();
        glBegin( GL_POLYGON );
            glVertex3f( xmin, ymax, zmin );
            glVertex3f( xmin, ymax, zmax );
            glVertex3f( xmax, ymax, zmax );
            glVertex3f( xmax, ymax, zmin );
        glEnd();
        glBegin( GL_POLYGON );
            glVertex3f( xmin, ymin, zmin );
            glVertex3f( xmin, ymin, zmax );
            glVertex3f( xmin, ymax, zmax );
            glVertex3f( xmin, ymax, zmin );
        glEnd();
        glBegin( GL_POLYGON );
            glVertex3f( xmax, ymin, zmin );
            glVertex3f( xmax, ymin, zmax );
            glVertex3f( xmax, ymax, zmax );
            glVertex3f( xmax, ymax, zmin );
        glEnd();
    }
}

void CDAxisWrapper::axesUsed ( bool& x, bool& y, bool& z )
{
    uint s = 0; // sibling count -- how many more of me are there?
    if ( parent )
        for ( uint i = 0 ; i < parent->children.count() ; i++ )
            if ( parent->children.at( i )->name == name ) s++;
    x = y = z = 0;
    if ( name.endsWith( "-axis" ) ) {
        switch ( axistype ) {
            case 0: x |= 1; break;
            case 1: y |= 1; break;
            case 2: z |= 1; break;
            case 3: x |= ( s > 2 ); y |= 1; break;
            case 4: x |= ( s > 2 ); z |= 1; break;
            case 5: y |= ( s > 2 ); z |= 1; break;
            case 6: x |= ( s > 2 ); y |= 1; break;
            case 7: x |= ( s > 2 ); z |= 1; break;
            case 8: y |= 1; z |= ( s > 2 ); break;
        }
    }
    for ( uint i = 0 ; i < children.count() ; i++ ) {
        if ( children.at( i )->name.endsWith( "-axis" ) ) {
            bool tmpx, tmpy, tmpz;
            ((CDAxisWrapper*)children.at( i ))->axesUsed( tmpx, tmpy, tmpz );
            x |= tmpx; y |= tmpy; z |= tmpz;
        }
    }
}

void CDAxisWrapper::placeMe ( uint axis, uint myIndex, uint outOf )
{
    // this assumes we're working in the cube [0,1]^3
    axistype = axis;
    double numerator = (double)myIndex;
    double denominator = (double)( outOf ? outOf : 1 ); // paranoia
    double s;
    double t;
    switch ( axis ) {
        case 0:
            name = "linear-x-axis";
            s = 2 / ( 3 * denominator - 1 );
            t = 3 * numerator / ( 3 * denominator - 1 );
            translate( t, 0, 0 );
            scale( s, 1, 1 );
            break;
        case 1:
            name = "linear-y-axis";
            s = 2 / ( 3 * denominator - 1 );
            t = 3 * ( denominator - 1 - numerator ) / ( 3 * denominator - 1 );
            translate( 0, t, 0 );
            scale( 1, s, 1 );
            break;
        case 2:
            name = "linear-z-axis";
            s = 2 / ( 3 * denominator - 1 );
            t = 3 * ( denominator - 1 - numerator ) / ( 3 * denominator - 1 );
            translate( 0, 0, t );
            scale( 1, 1, s );
            break;
        case 3:
            name = "circular-xy-axis";
            t = 2 * M_PI * ( 0.25 - numerator / denominator );
            s = sqrt( 1.0 - cos( 2.0 * M_PI / denominator ) ) * 0.5;
            translate( 0.5 + cos( t ) * 0.5 / ( 1 + s ),
                       0.5 + sin( t ) * 0.5 / ( 1 + s ),
                       0 );
            scale( s / ( s + 1 ), s / ( s + 1 ), 1 );
            translate( -0.5, -0.5, 0 );
            break;
        case 4:
            name = "circular-xz-axis";
            t = 2 * M_PI * ( 0.25 - numerator / denominator );
            s = sqrt( 1.0 - cos( 2.0 * M_PI / denominator ) ) * 0.5;
            translate( 0.5 + cos( t ) * 0.5 / ( 1 + s ),
                       0,
                       0.5 + sin( t ) * 0.5 / ( 1 + s ) );
            scale( s / ( s + 1 ), 1, s / ( s + 1 ) );
            translate( -0.5, 0, -0.5 );
            break;
        case 5:
            name = "circular-yz-axis";
            t = 2 * M_PI * ( 0.25 - numerator / denominator );
            s = sqrt( 1.0 - cos( 2.0 * M_PI / denominator ) ) * 0.5;
            translate( 0,
                       0.5 + sin( t ) * 0.5 / ( 1 + s ),
                       0.5 + cos( t ) * 0.5 / ( 1 + s ) );
            scale( 1, s / ( s + 1 ), s / ( s + 1 ) );
            translate( 0, -0.5, -0.5 );
            break;
        case 6:
            name = "rotational-xy-axis";
            t = 360.0 * ( - 0.25 - numerator / denominator );
            s = sqrt( 1.0 - cos( 2.0 * M_PI / denominator ) ) * 0.5;
            translate( 0.5, 0.5, 0.5 );
            rotate( 0, 0, t );
            translate( - 0.5 + 0.5 * s / ( s + 1 ), 0, 0 );
            scale( s / ( s + 1 ), s / ( s + 1 ), 1 );
            translate( -0.5, -0.5, -0.5 );
            break;
        case 7:
            name = "rotational-xz-axis";
            t = 360.0 * ( - 0.25 - numerator / denominator );
            s = sqrt( 1.0 - cos( 2.0 * M_PI / denominator ) ) * 0.5;
            translate( 0.5, 0.5, 0.5 );
            rotate( 0, t, 0 );
            translate( - 0.5 + 0.5 * s / ( s + 1 ), 0, 0 );
            scale( s / ( s + 1 ), 1, s / ( s + 1 ) );
            translate( -0.5, -0.5, -0.5 );
            break;
        case 8:
            name = "rotational-yz-axis";
            t = 360.0 * ( - 0.25 - numerator / denominator );
            s = sqrt( 1.0 - cos( 2.0 * M_PI / denominator ) ) * 0.5;
            translate( 0.5, 0.5, 0.5 );
            rotate( t, 0, 0 );
            translate( 0, - 0.5 + 0.5 * s / ( s + 1 ), 0 );
            scale( 1, s / ( s + 1 ), s / ( s + 1 ) );
            translate( -0.5, -0.5, -0.5 );
            break;
    }
}

QStringList CDAxisWrapper::getAllAxisNames ()
{
    return QStringList() << "Linear in x"     << "Linear in y"     << "Linear in z"
                         << "Circular in x,y" << "Circular in x,z" << "Circular in y,z"
                         << "Rotated in x,y"  << "Rotated in x,z"  << "Rotated in y,z";
}

QStringList CDAxisWrapper::getAllAxisPictures ()
{
    return QStringList() << "img/axis-x.png"   << "img/axis-y.png"   << "img/axis-z.png"
                         << "img/axis-xy.png"  << "img/axis-xz.png"  << "img/axis-yz.png"
                         << "img/axis-rxy.png" << "img/axis-rxz.png" << "img/axis-ryz.png";
}


CDNode::CDNode ( GLSceneNode* diagram, bool simple )
    : radius( 1.0 ), label( "" ), useLabels( TRUE ), diagram( diagram )
{
    highlight[0] = highlight[1] = highlight[2] = FALSE;
    name = "cdnode";
    if ( simple ) {
        setColor( 0.5, 0.5, 0.5 );
        actualNode = 0;
    } else {
        addChild( actualNode = new GLSphere() );
        actualNode->setDiffuse( 1, 1, 1 );
    }
    setRadius( 1.0 );
    clearAbsolutePosition();
}

void CDNode::placeActualNode ()
{
    if ( actualNode ) {
        actualNode->reset();
        actualNode->translate( myx, myy, myz );
        actualNode->scale( 0.25 * radius, 0.25 * radius, 0.25 * radius );
    }
}

void CDNode::setRadius ( double r )
{
    radius = r;
    placeActualNode();
}

void CDNode::setLabel ( QString l )
{
    label = l;
}

QString CDNode::getLabel ()
{
    return label;
}

void CDNode::setSize ( uint s )
{
    if ( s ) {
        font.setPointSize( s );
        useLabels = TRUE;
    } else {
        useLabels = FALSE;
    }
}

bool CDNode::containsSphere ( GLSceneNode* sphere )
{
    return ( sphere == actualNode );
}

bool CDNode::containedByChunk ( GLSceneNode* chunk )
{
    GLSceneNode* p = this;
    while ( p->parent ) {
        if ( p->parent == chunk ) return true;
        p = p->parent;
    }
    return false;
}

void CDNode::setAbsolutePosition ( double x, double y, double z )
{
    reset();
    inverseRelativeCoordinates( x, y, z, myx, myy, myz/*, diagram*/ );
    translate( myx, myy, myz );
    normalizeScaling();
    translate( -myx, -myy, -myz );
    placeActualNode();
}

void CDNode::clearAbsolutePosition ()
{
    myx = myy = myz = 0.5;
    placeActualNode();
}

void CDNode::getAbsolutePositionAndRadius ( double& x, double& y, double& z, double& r,
                                            GLSceneNode* relativeTo )
{
    relativeCoordinates( myx, myy, myz, x, y, z, relativeTo );
    double rx, ry, rz;
    relativeCoordinates( myx + 0.24 * radius, myy, myz, rx, ry, rz, relativeTo );
    // I use 0.24 in above line to make sure arrows actually reach just a hair inside
    rx -= x;
    ry -= y;
    rz -= z;
    r = sqrt( rx * rx + ry * ry + rz * rz );
}

void CDNode::getDiagramPositionAndRadius ( double& x, double& y, double& z, double& r )
{
    getAbsolutePositionAndRadius( x, y, z, r, diagram );
}

void CDNode::preRender ()
{
    reset();
    translate( myx, myy, myz );
    normalizeScaling();
    translate( -myx, -myy, -myz );

    GLSceneNode::preRender();
}

void CDNode::renderMe ()
{
    GLSceneNode::renderMe();
    if ( actualNode ) {
        double myrad = radius * 0.3;
        if ( highlight[1] ) { // ring around node
            GLdouble xx, xy, xz, yx, yy, yz, zx, zy, zz;
            screenXYZ( myx, myy, myz, xx, xy, xz, yx, yy, yz, zx, zy, zz );
            glColor3f( highcolor[1].red() / 255.0,
                       highcolor[1].green() / 255.0,
                       highcolor[1].blue() / 255.0 );
            glBegin( GL_LINE_LOOP );
                for ( GLdouble theta = 0 ; theta < 2*M_PI ; theta += 0.4 )
                    glVertex3f( myx + ( xx * cos( theta ) + yx * sin( theta ) ) * myrad,
                                myy + ( xy * cos( theta ) + yy * sin( theta ) ) * myrad,
                                myz + ( xz * cos( theta ) + yz * sin( theta ) ) * myrad );
            glEnd();
            myrad = radius * 0.35; // so square will go outside circle
        }
        if ( highlight[2] ) { // square around node
            GLdouble xx, xy, xz, yx, yy, yz, zx, zy, zz;
            screenXYZ( myx, myy, myz, xx, xy, xz, yx, yy, yz, zx, zy, zz );
            glColor3f( highcolor[2].red() / 255.0,
                       highcolor[2].green() / 255.0,
                       highcolor[2].blue() / 255.0 );
            xx *= myrad;  xy *= myrad;  xz *= myrad;
            yx *= myrad;  yy *= myrad;  yz *= myrad;
            glBegin( GL_LINE_LOOP );
                glVertex3f( myx + xx + yx, myy + xy + yy, myz + xz + yz );
                glVertex3f( myx + xx - yx, myy + xy - yy, myz + xz - yz );
                glVertex3f( myx - xx - yx, myy - xy - yy, myz - xz - yz );
                glVertex3f( myx - xx + yx, myy - xy + yy, myz - xz + yz );
            glEnd();
        }
    } else {
        glPointSize( 4.0 );
        glBegin( GL_POINTS );
            glVertex3f( myx, myy, myz );
        glEnd();
    }
}

void CDNode::postRender ()
{
    if ( actualNode && renderingOn && useLabels ) {
        double point = 0.25 * radius;
        glDisable( GL_LIGHTING );
        glColor3f( 0.0, 0.0, 0.0 );
        renderingOn->renderText( myx + point, myy + point, myz + point, label, font );
        glEnable( GL_LIGHTING ); // should check to be sure it was on...but whatever
    }

    GLSceneNode::postRender();
}

void CDNode::setHighlighting ( uint htype, QColor color )
{
    //int h2, s2, v2; color.getHsv( h2, s2, v2 );
    //qDebug( QString( "CDNode[%1]->setHighlighting( %2, h%3s%4v%5 )" )
    //        .arg( label ).arg( htype ).arg( h2 ).arg( s2 ).arg( v2 ) );
    if ( htype < 3 ) {
        highlight[htype] = TRUE;
        int h, s, v;
        color.getHsv( &h, &s, &v );
        highcolor[htype] = QColor( h, 255, htype ? 100 : 200, QColor::Hsv );
        if ( !htype ) {
            actualNode->setDiffuse( highcolor[0].red() / 255.0,
                                    highcolor[0].green() / 255.0,
                                    highcolor[0].blue() / 255.0 );
            //qDebug( QString( "\tset node's color to r%1g%2b%3" )
            //        .arg( highcolor[0].red() / 255.0 )
            //        .arg( highcolor[0].green() / 255.0 )
            //        .arg( highcolor[0].blue() / 255.0 ) );
        }
    }
}

void CDNode::clearHighlighting ( uint htype )
{
    //qDebug( QString( "CDNode[%1]->clearHighlighting( %2 )" ).arg( label ).arg( htype ) );
    highlight[htype] = FALSE;
    if ( !htype ) actualNode->setDiffuse( 1, 1, 1 );
}


CDArrow::CDArrow ( CDNode* from, CDNode* to, GLSceneNode* diagram/*, bool simple*/ )
    : fromNode( from ), toNode( to ), diagram( diagram ), order( 0 ), headPos( 1.0 )//,
      //simple( simple )
{
    name = "cdarrow";
    decideArcing();
}

void CDArrow::setArcing ( GLdouble amount,
                          GLdouble centerX, GLdouble centerY, GLdouble centerZ )
{
    arcCenterX = centerX;
    arcCenterY = centerY;
    arcCenterZ = centerZ;
    // We check to see if the arc center is on the line in question;
    // this would make arcing ineffectual and must be changed.
    setupInternalValues();
    /*
    qDebug( QString( "setArcing( %1, %2, %3, %4 )" )
            .arg( amount, 6, 'f', 3 ).arg( centerX, 6, 'f', 3 )
            .arg( centerY, 6, 'f', 3 ).arg( centerZ, 6, 'f', 3 ) );
    qDebug( QString( "in arrow from %1, %2, %3 to %4, %5, %6" )
            .arg( fx, 6, 'f', 3 ).arg( fy, 6, 'f', 3 ).arg( fz, 6, 'f', 3 )
            .arg( tx, 6, 'f', 3 ).arg( ty, 6, 'f', 3 ).arg( tz, 6, 'f', 3 ) );
    */
    arcAmount = amount;
    GLdouble dirx = tx - fx, diry = ty - fy, dirz = tz - fz;
    GLdouble ftcx = arcCenterX - fx, ftcy = arcCenterY - fy, ftcz = arcCenterZ - fz;
    unitize( dirx, diry, dirz );
    unitize( ftcx, ftcy, ftcz );
    if ( ( ( dirx < 0.0 ) && ( ftcx > 0.0 ) ) || ( ( dirx > 0.0 ) && ( ftcx < 0.0 ) ) ) {
        ftcx *= -1.0;
        ftcy *= -1.0;
        ftcz *= -1.0;
    }
    if ( ( QABS( dirx - ftcx ) < 0.1 )
      && ( QABS( diry - ftcy ) < 0.1 )
      && ( QABS( dirz - ftcz ) < 0.1 ) ) {
        // problem.  here we must change things.
        GLdouble x, y, z;
        if ( dirx || diry ) {
            crossproduct( dirx, diry, dirz, 0.0, 0.0, 1.0, x, y, z );
        } else {
            crossproduct( dirx, diry, dirz, 0.0, 1.0, 0.0, x, y, z );
        }
        arcCenterX = ( tx + fx ) / 2.0 + x;
        arcCenterY = ( ty + fy ) / 2.0 + y;
        arcCenterZ = ( tz + fz ) / 2.0 + z;
        /*
        qDebug( QString( "\tmodified that center to %1, %2, %3" )
                .arg( arcCenterX, 6, 'f', 3 ).arg( arcCenterY, 6, 'f', 3 )
                .arg( arcCenterZ, 6, 'f', 3 ) );
        */
    }
    // now we compute the vector that points in the direction we'll inflate the arc.
    // that is, various percentages of this vector will get added to the straight line
    // to produce an arced line.
    // First, find the nearest point on the line f---t to arcCenter
    GLdouble p = dirx * ( arcCenterX - fx )
               + diry * ( arcCenterY - fy )
               + dirz * ( arcCenterZ - fz );
    GLdouble nearx = fx + p * dirx, neary = fy + p * diry, nearz = fz + p * dirz;
    // now compute arcV (vector in direction arc will fluff from line) w/length == radius
    arcVX = nearx - arcCenterX;  arcVY = neary - arcCenterY;  arcVZ = nearz - arcCenterZ;
    GLdouble radius = sqrt( ( fx - tx ) * ( fx - tx ) / 4.0 +
                            ( fy - ty ) * ( fy - ty ) / 4.0 +
                            ( fz - tz ) * ( fz - tz ) / 4.0 +
                            arcVX * arcVX + arcVY * arcVY + arcVZ * arcVZ );
    unitize( arcVX, arcVY, arcVZ );
    arcVX *= radius;  arcVY *= radius;  arcVZ *= radius;
}
void CDArrow::setArcing ( GLdouble amount )
{
    setArcing( amount, arcCenterX, arcCenterY, arcCenterZ );
}

GLdouble CDArrow::getArcing ()
{
    return arcAmount;
}

void CDArrow::setHeadPosition ( GLdouble percent )
{
    headPos = percent;
}

GLdouble CDArrow::getHeadPosition ()
{
    return headPos;
}

void CDArrow::myArrowPath ( double percent, GLdouble& x, GLdouble& y, GLdouble& z,
                            double arcLevel )
{
    // assumes setupInternalValues() has been called
    // First, we find out what the answer would be if this were a line
    x = ( 1.0 - percent ) * fx + percent * tx;
    y = ( 1.0 - percent ) * fy + percent * ty;
    z = ( 1.0 - percent ) * fz + percent * tz;
    // if there is any arcing, we adjust for that
    if ( arcLevel ) {
        /*
        GLdouble length = sqrt( ( x - arcCenterX ) * ( x - arcCenterX )
                              + ( y - arcCenterY ) * ( y - arcCenterY )
                              + ( z - arcCenterZ ) * ( z - arcCenterZ ) );
        if ( length ) {
            // figure out how long this should be: a linear interpolation between two
            // lengths: the first is the distance from the arc center to the from-point,
            // the second is the distance from the arc center to the to-point; if these
            // distances are the same (we're on a circle) then we're just preserving radius
            GLdouble lengthDesired =
                ( 1.0 - percent ) * sqrt( ( fx - arcCenterX ) * ( fx - arcCenterX ) +
                                          ( fy - arcCenterY ) * ( fy - arcCenterY ) +
                                          ( fz - arcCenterZ ) * ( fz - arcCenterZ ) ) +
                percent * sqrt( ( tx - arcCenterX ) * ( tx - arcCenterX ) +
                                ( ty - arcCenterY ) * ( ty - arcCenterY ) +
                                ( tz - arcCenterZ ) * ( tz - arcCenterZ ) );
            // what if we rescaled the vector to the desired length?
            // this fluffs the line segment to a portion of a circle (or ellipse?)
            GLdouble newX = arcCenterX + ( x - arcCenterX ) * lengthDesired / length;
            GLdouble newY = arcCenterY + ( y - arcCenterY ) * lengthDesired / length;
            GLdouble newZ = arcCenterZ + ( z - arcCenterZ ) * lengthDesired / length;
            // so do that fluffing, but do it not just once; do it arcLevel times
            x += ( newX - x ) * arcLevel;
            y += ( newY - y ) * arcLevel;
            z += ( newZ - z ) * arcLevel;
        }
        */
        GLdouble avlen = sqrt( arcVX * arcVX + arcVY * arcVY + arcVZ * arcVZ );
        if ( avlen > 0.0 ) {
            GLdouble ftlen = sqrt( ( tx - fx ) * ( tx - fx )
                                 + ( ty - fy ) * ( ty - fy )
                                 + ( tz - fz ) * ( tz - fz ) );
            GLdouble angle = 2.0 * atan( ftlen / avlen / 2.0 );
            GLdouble anotherX = ( 1.0 - percent ) * cos( M_PI_2 + angle / 2.0 )
                              + percent * cos( M_PI_2 - angle / 2.0 );
            GLdouble arcFactor = arcLevel * (
                sqrt( 1.0 - anotherX * anotherX ) - sin( M_PI_2 + angle / 2.0 ) );
            x += arcFactor * arcVX;
            y += arcFactor * arcVY;
            z += arcFactor * arcVZ;
        }
    }
}
void CDArrow::myArrowPath ( double percent, GLdouble& x, GLdouble& y, GLdouble& z )
{
    myArrowPath( percent, x, y, z, arcAmount );
}
void CDArrow::arrowPath ( double percent, GLdouble& x, GLdouble& y, GLdouble& z,
                          double arcLevel )
{
    setupInternalValues();
    myArrowPath( percent, x, y, z, arcLevel );
}

double CDArrow::myInvasions ( Q3PtrList<CDNode> nodes, double arcLevel )
{
    double result = 0;
    GLdouble x, y, z;
    GLdouble* nx = new GLdouble[nodes.count()];
    GLdouble* ny = new GLdouble[nodes.count()];
    GLdouble* nz = new GLdouble[nodes.count()];
    GLdouble* nr = new GLdouble[nodes.count()];
    uint skip1, skip2;
    skip1 = skip2 = nodes.count();
    for ( uint i = 0 ; i < nodes.count() ; i++ ) {
        if ( nodes.at( i ) == fromNode ) {
            skip1 = i;
        } else if ( nodes.at( i ) == toNode ) {
            skip2 = i;
        } else {
            nodes.at( i )->getDiagramPositionAndRadius( nx[i], ny[i], nz[i], nr[i] );
        }
    }
    for ( double p = 0.0 ; p < 1.01 ; p += 0.02 ) {
        myArrowPath( p, x, y, z, arcLevel );
        for ( uint i = 0 ; i < nodes.count() ; i++ ) {
            if ( ( i != skip1 ) && ( i != skip2 ) ) {
                double dist = sqrt( ( x - nx[i] ) * ( x - nx[i] )
                                  + ( y - ny[i] ) * ( y - ny[i] )
                                  + ( z - nz[i] ) * ( z - nz[i] ) );
                if ( dist < 2 * nr[i] ) result += 2 * nr[i] - dist;
            }
        }
    }
    delete nx;
    delete ny;
    delete nz;
    delete nr;
    return result;
}
double CDArrow::myInvasions ( Q3PtrList<CDNode> nodes )
{
    return myInvasions( nodes, arcAmount );
}

double CDArrow::bestHigherArcing ( Q3PtrList<CDNode> nodes, double thanThis )
{
    /*
    qDebug( QString( "arcing level check for arrow %1 --> %2" )
            .arg( fromNode->getLabel() ).arg( toNode->getLabel() ) );
    */
    double best = myInvasions( nodes, thanThis );
    double bestarc = thanThis;
    for ( double more = 0.5 ; more < 2.25 ; more += 0.5 ) {
        double badness = myInvasions( nodes, thanThis + more );
        /*
        double diff = 0.0;
        for ( double p = 0.0 ; p < 1.04 ; p += 0.05 ) {
            double x1, y1, z1, x2, y2, z2;
            myArrowPath( p, x1, y1, z1, thanThis );
            myArrowPath( p, x2, y2, z2, thanThis + more );
            diff += sqrt( ( x1 - x2 ) * ( x1 - x2 )
                        + ( y1 - y2 ) * ( y1 - y2 )
                        + ( z1 - z2 ) * ( z1 - z2 ) );
        }
        qDebug( QString( "\twhen arc == %1 (diff %3), badness == %2" )
                .arg( thanThis + more, 6, 'f', 3 ).arg( badness, 6, 'f', 3 )
                .arg( diff, 6, 'f', 3 ) );
        */
        if ( badness < best - 0.1 ) { // require some noticeable change
            best = badness;
            bestarc = thanThis + more;
        }
    }
    for ( double more = -0.5 ; more > -1.25 ; more -= 0.5 ) {
        double badness = myInvasions( nodes, thanThis + more );
        if ( badness < best - 0.1 ) { // require some noticeable change
            best = badness;
            bestarc = thanThis + more;
        }
    }
    /*
    qDebug( QString( "\t\tthe best of these was with arc %1 (so that's my result)" )
            .arg( bestarc, 6, 'f', 3 ) );
    */
    return bestarc;
}
double CDArrow::bestHigherArcing ( Q3PtrList<CDNode> nodes )
{
    return bestHigherArcing( nodes, arcAmount );
}

void CDArrow::decideArcing ()
{
    // must do this stuff from renderMe() in order to set up fx,fy,fz,...
    if ( !fromNode || !toNode ) return;
    setupInternalValues();

    // find first common parent
    //qDebug( QString( "looking at %1 --> %2" )
    //        .arg( (uint)fromNode ).arg( (uint)toNode ) );
    setArcing( 0.0, 0.0, 0.0, 0.0 );
    GLSceneNode* fpar = fromNode->parent;
    GLSceneNode* tpar = toNode->parent;
    while ( fpar && tpar && fpar->parent && tpar->parent &&
            ( fpar->parent != tpar->parent ) ) {
        //qDebug( QString( "\tparents not equal: %1 != %2; must move further up" )
        //        .arg( (uint)(fpar->parent) ).arg( (uint)(tpar->parent) ) );
        fpar = fpar->parent;
        tpar = tpar->parent;
    }
    if ( fpar && tpar && fpar->parent && tpar->parent &&
         ( fpar->parent == tpar->parent ) ) {
        //qDebug( QString( "\tfinally equal: %1 == %2" )
        //        .arg( (uint)(fpar->parent) ).arg( (uint)(tpar->parent) ) );
        if ( ( fpar->name.startsWith( "rotational-" )
            || fpar->name.startsWith( "circular-" )
            || tpar->name.startsWith( "rotational-" )
            || tpar->name.startsWith( "circular-" ) )
          && ( fpar->parent->children.count() > 2 ) ) {
            //qDebug( QString( "\tyes, they're circular (e.g. %1) in there (%2)" )
            //        .arg( fpar->name ).arg( fpar->parent->name ) );
            int idx1 = fpar->parent->children.find( fpar );
            int idx2 = tpar->parent->children.find( tpar );
            if ( idx1 > idx2 ) qSwap( idx1, idx2 );
            //qDebug( QString( "\tidx1 %1 idx2 %2 (ct %3)" )
            //        .arg( idx1 ).arg( idx2 )
            //        .arg( tpar->parent->children.count() ) );
            if ( ( idx2 == idx1 + 1 )
              || ( idx2 == idx1 + ( int )( tpar->parent->children.count() ) - 1 ) ) {
                GLdouble ctrx, ctry, ctrz;
                tpar->parent->relativeCoordinates(
                    0.5, 0.5, 0.5, ctrx, ctry, ctrz, diagram );
                //qDebug( QString( "\t<0.5,0.5,0.5> in %1 is <%2,%3,%4> in diagram" )
                //        .arg( tpar->parent->name ).arg( ctrx, 4, 'f', 2 )
                //        .arg( ctry, 4, 'f', 2 ).arg( ctrz, 4, 'f', 2 ) );
                setArcing( 1.0, ctrx, ctry, ctrz );
            }
        }// else qDebug( QString( "\tno, they're not circular (e.g. %1) in there (%2)" )
         //              .arg( fpar->name ).arg( fpar->parent->name ) );
    }// else qDebug( QString( "\tone became zero! %1, %2" )
     //              .arg( (uint)fpar ).arg( (uint)tpar ) );
    //qDebug( "\t" + QString::number( arcAmount ) );
}
void CDArrow::decideArcing ( Q3PtrList<CDNode> nodes )
{
    /*
    if ( simple ) {
        setArcing( 0, 0, 0, 0 );
        return;
    }
    */
    decideArcing();
    setArcing( bestHigherArcing( nodes ) );
}

void CDArrow::setOrder ( uint o )
{
    order = o;
}

uint CDArrow::getOrder ()
{
    return order;
}

void CDArrow::renderMe ()
{
    // get the endpoints, provided the end nodes exist
    if ( !fromNode || !toNode ) return;
    setupInternalValues();

    // try to get good vector for making arrowhead face camera:
    GLdouble cx, cy, cz; // (c stands for camera)
    cameraPosition( cx, cy, cz );
    GLdouble hx, hy, hz;
    myArrowPath( headPos, hx, hy, hz );
    myArrowPath( headPos - 0.05, ux, uy, uz );
    ux = hx - ux;  uy = hy - uy;  uz = hz - uz;
    unitize( ux, uy, uz );
    if ( cx || cy || cz ) {
        unitize( cx, cy, cz );
    } else if ( uy || uz ) {
        cx = 1.0;  cy = 0.0;  cz = 0.0;
    } else {
        cx = 0.0;  cy = 1.0;  cz = 0.0;
    }
    GLdouble px, py, pz; // perpendicular
    crossproduct( cx, cy, cz, ux, uy, uz, px, py, pz );
    unitize( px, py, pz );

    glBegin( GL_LINE_STRIP );
        for ( double p = 0.0 ; p < 1.025 ; p += 0.05 ) {
            GLdouble x, y, z;
            myArrowPath( p, x, y, z );
            glVertex3f( x, y, z );
        }
    glEnd();
    if ( order > 2 ) {
        glBegin( GL_LINE_STRIP );
            glVertex3f( hx + px * 0.05 - ux * 0.12,
                        hy + py * 0.05 - uy * 0.12,
                        hz + pz * 0.05 - uz * 0.12 );
            glVertex3f( hx, hy, hz );
            glVertex3f( hx - px * 0.05 - ux * 0.12,
                        hy - py * 0.05 - uy * 0.12,
                        hz - pz * 0.05 - uz * 0.12 );
        glEnd();
    }
    /*
    glBegin( GL_LINES );
        glVertex3f( arcCenterX + 0.05, arcCenterY, arcCenterZ );
        glVertex3f( arcCenterX - 0.05, arcCenterY, arcCenterZ );
        glVertex3f( arcCenterX, arcCenterY + 0.05, arcCenterZ );
        glVertex3f( arcCenterX, arcCenterY - 0.05, arcCenterZ );
        glVertex3f( arcCenterX, arcCenterY, arcCenterZ + 0.05 );
        glVertex3f( arcCenterX, arcCenterY, arcCenterZ - 0.05 );
    glEnd();
    */
}

void CDArrow::setupInternalValues ()
{
    // get the endpoints, provided the end nodes exist
    if ( !fromNode || !toNode ) return;
    fromNode->getDiagramPositionAndRadius( fx, fy, fz, fr ); // (f stands for from)
    toNode->getDiagramPositionAndRadius( tx, ty, tz, tr ); // (t stands for to)
    // get the direction vector "from-->to", unitized (u stands for unit)
    ux = tx - fx; uy = ty - fy; uz = tz - fz;
    unitize( ux, uy, uz );
    // indent a bit on each side:
    fx += ux * fr;  fy += uy * fr;  fz += uz * fr;
    tx -= ux * tr;  ty -= uy * tr;  tz -= uz * tr;
}


