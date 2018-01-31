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


#include "multiplicationtable.h"
#include "sympanewidgets.h"
#include "gehelpwindow.h"
#include "gewhatsthis.h"
#include "ge.h"
#include <qlayout.h>
#include <qpixmap.h>
#include <q3scrollview.h>
#include <qpainter.h>
#include <qmessagebox.h>
#include <q3simplerichtext.h>
#include <qtabwidget.h>
#include <q3vbox.h>
#include <qlabel.h>
#include <qradiobutton.h>
#include <qcombobox.h>
#include <q3buttongroup.h>
#include <qslider.h>
#include <qcursor.h>
#include <qapplication.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qclipboard.h>
#include <q3dragobject.h>
#include <qdatetime.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3PointArray>
#include <Q3PtrList>
#include <Q3VBoxLayout>
//Added to help Qt4 out:
#include <q3mimefactory.h>


MTLargeView::MTLargeView ( GEVLarge* container, QWidget* parent, const char* name )
    : GEVLargeWidget( container, parent, name ), cellsize( 0 ), orderDirty( FALSE )
{
    Q3HBoxLayout* layout = new Q3HBoxLayout( this );
    Q3ScrollView* sv = new Q3ScrollView( this );
    layout->addWidget( sv );
    table = new ScrollableLabel( 0 );
    table->setText( "Generating table..." );
    sv->addChild( table );
    connect( table, SIGNAL(scroll(int,int)), sv, SLOT(scrollBy(int,int)) );
    connect( table, SIGNAL(startDrag(QPoint)), this, SLOT(tableStartDrag(QPoint)) );
    connect( table, SIGNAL(finishDrag(QPoint,QPoint)),
             this, SLOT(tableFinishDrag(QPoint,QPoint)) );

    addProperty( "orgz", "0" );
    addProperty( "zoom", "100" );
    addProperty( "colr", "0" );
    addProperty( "elts", QString::null );
    addProperty( "axis", "top" );

    GEWhatsThis::setup( table, "mt-view" );
}

void MTLargeView::copyImage ()
{
    QApplication::clipboard()->setPixmap( p, QClipboard::Clipboard );
}

QPoint MTLargeView::pointToCell ( QPoint point )
{
    if ( !p.width() || !p.height() ) return QPoint( -1, -1 );

    uint o = groupfile->group.order;
    QPoint result = QPoint( point.x() * o / p.width(), point.y() * o / p.height() );

    if ( result.x() < 0 ) result.setX( -1 );
    if ( result.y() < 0 ) result.setY( -1 );
    if ( result.x() >= int( o ) ) result.setX( o - 1 );
    if ( result.y() >= int( o ) ) result.setY( o - 1 );

    return result;
}

void MTLargeView::tableStartDrag ( QPoint from )
{
    QPoint cell = pointToCell( from );
    if ( bool( cell.x() ) != bool( cell.y() ) ) {
        qApp->setOverrideCursor(
            QCursor( cell.x() ? Qt::SizeHorCursor : Qt::SizeVerCursor ) );
    /*
    } else {
        ( new QImageDrag( p.convertToImage(), this ) )->dragCopy();
    */
    }
}

void MTLargeView::tableFinishDrag ( QPoint from, QPoint to )
{
    QPoint fcell = pointToCell( from );
    QPoint tcell = pointToCell( to );
    if ( bool( fcell.x() ) != bool( fcell.y() ) ) {
        qApp->restoreOverrideCursor();
        if ( fcell.x() ) {
            if ( !tcell.y() && ( tcell.x() > 0 ) ) swapElements( fcell.x(), tcell.x() );
        } else {
            if ( !tcell.x() && ( tcell.y() > 0 ) ) swapElements( fcell.y(), tcell.y() );
        }
    }
}

void MTLargeView::dumpAsyCode ()
{
#ifdef DUMP_ASY_FEATURE
    qDebug( "%s", QString( "\n"
                           "//-------------------------------------------------------\n"
                           "// Here begins text generated by Group Explorer %1\n"
                           "// on %2, the Asymptote code for the multiplication\n"
                           "// table for the group %3.\n"
                           "// It requires the the Asymptote file makemt.asy.\n"
                           "//-------------------------------------------------------\n"
                           "\n"
                           "include \"./makemt.asy\";\n"
                           "" )
                  .arg( GE_VERSION_STRING ).arg( QDateTime::currentDateTime().toString() )
                  .arg( groupfile->codeName() ).latin1() );
    for ( uint i = 0 ; i < groupfile->group.order ; i++ ) {
        QString row;
        if ( i )
            row = "                                          {";
        else
            row = "add( colorMultTable( table = new int[][]{ {";
        for ( uint j = 0 ; j < groupfile->group.order ; j++ ) {
            if ( j ) row += ",";
            row += QString::number( groupfile->group.op( i, j ) );
        }
        row += "}";
        if ( i == groupfile->group.order - 1 ) row += "}";
        row += ",";
        qDebug( "%s", row.latin1() );
    }
    QStringList names;
    for ( uint i = 0 ; i < groupfile->group.order ; i++ )
        names.append( groupfile->getRepresentation( i ).text );
    qDebug( "                     names = new string[]{ \"$%s$\" },",
            names.join( "$\", \"$" ).latin1() );
    QString perm;
    for ( uint i = 0 ; i < groupfile->group.order ; i++ ) {
        if ( i ) perm += ",";
        perm += QString::number( ord[i] );
    }
    qDebug( "                     rowperm = new int[]{ %s },", perm.latin1() );
    qDebug( "                     colperm = new int[]{ %s },", perm.latin1() );
    qDebug( "                     cellsize = ( 1cm, 1cm ) ) );\n" );
#endif
}

void MTLargeView::swapElements ( uint a, uint b )
{
    QStringList elts;
    for ( uint i = 0 ; i < ord.size() ; i++ ) {
        if ( i == a ) {
            elts << QString::number( ord[b] );
        } else if ( i == b ) {
            elts << QString::number( ord[a] );
        } else {
            elts << QString::number( ord[i] );
        }
    }
    change( "elts", elts.join( "-" ),
        QString( "change element order (swap %1 and %2)" )
        .arg( groupfile->getRepresentation( a ).HTML )
        .arg( groupfile->getRepresentation( b ).HTML ) );
}

uint MTLargeView::getCellSize ()
{
    QSize largest = largestElementText();
    return QMAX( largest.width(), largest.height() );
}

// the following value represents the fact that we want to warn the user if they're
// about to make a pixmap greater than 1000x1000 pixels:
uint MTLargeView::maximumEfficientSize = 1000;
// it is needed in the following method...
void MTLargeView::set ( QString key, QString oldvalue, QString value, QString desc )
{
    if ( key == "zoom" ) {
        uint newval = value.toInt();
        // but now check to be sure it's not too large:
        uint size = groupfile->group.order * getCellSize();
        uint maxGoodZoom = 100 * maximumEfficientSize / size;
        if ( ( newval > maxGoodZoom ) &&
             ( desc.isNull() || ( QMessageBox::warning(
                     0, "Very large multiplication table",
                     QString( "At %1% zoom, the multiplication table\n"
                              "will be %2x%3 pixels, which is quite large.\n"
                              "It may take some time to create, and the\n"
                              "image may require significant memory.\n"
                              "Would you rather view a %4% zoom version,\n"
                              "which will only be %5x%6 pixels?" )
                     .arg( newval )
                     .arg( size * newval / 100 )
                     .arg( size * newval / 100 )
                     .arg( maxGoodZoom )
                     .arg( size * maxGoodZoom / 100 )
                     .arg( size * maxGoodZoom / 100 ),
                     QMessageBox::Yes | QMessageBox::Default | QMessageBox::Escape,
                     QMessageBox::No ) == QMessageBox::Yes ) ) ) {
            newval = maxGoodZoom;
            change( key, QString::number( newval ), desc );
        }
        zoa->setEnabled( readProperty( "zoom" ).toInt() > 2 );
        zia->setEnabled( readProperty( "zoom" ).toInt() < 337 );
    } else if ( key == "elts" ) {
        if ( value.isEmpty() ) {
            updateElementOrder();
        } else {
            QStringList pieces = value.split( "-" );
            ord.resize( pieces.count() );
            for ( int i = 0 ; i < pieces.count() ; i++ ) ord[i] = pieces[i].toInt();
        }
    } else if ( key == "orgz" ) {
        int val = value.toInt();
        if ( val ) {
            ord = computeElementOrder( groupfile,
                groupfile->group.completeGens(
                    groupfile->group.neededGens(
                        groupfile->group.subgroups()[val - 1] ),
                    groupfile->group.elements() ) );
#if 0
            qDebug( QString( "Setting orgz to %1, subgroup %2\n"
                             "\tcomputed needed gens as %3, making %4\n"
                             "\tcompleted the gens as %5, making %6\n"
                             "\tthis gave element order %7" )
                    .arg( value )
                    .arg( elt_list_to_string( groupfile->group.subgroups()[val - 1] ) )
                    .arg( elt_list_to_string( groupfile->group.neededGens(
                        groupfile->group.subgroups()[val - 1] ) ) )
                    .arg( elt_list_to_string( groupfile->group.subgroupGenBy(
                        groupfile->group.neededGens(
                            groupfile->group.subgroups()[val - 1] ) ) ) )
                    .arg( elt_list_to_string( groupfile->group.completeGens(
                        groupfile->group.neededGens(
                            groupfile->group.subgroups()[val - 1] ),
                            groupfile->group.elements() ) ) )
                    .arg( elt_list_to_string( groupfile->group.subgroupGenBy(
                        groupfile->group.completeGens(
                            groupfile->group.neededGens(
                                groupfile->group.subgroups()[val - 1] ),
                                groupfile->group.elements() ) ) ) )
                    .arg( elt_list_to_string( ord ) ) );
#endif
            orderDirty = FALSE;
        } else {
            updateElementOrder();
        }
    }
    GEVLargeWidget::set( key, oldvalue, value, desc );
}

void MTLargeView::groupFileChanged ( GroupFile* gf )
{
    GEVLargeWidget::groupFileChanged( gf );
    orderDirty = TRUE;
}

void MTLargeView::updateContent ( QStringList changedKeys )
{
    // only need to change order if groupfile changed and we haven't yet reacted to that
    // with an appropriate order update; elts or orgz changes it in set():
    if ( changedKeys.contains( "groupfile" ) && orderDirty )
        updateElementOrder();
    // did the element order change?  this causes recomputing the inverse of that
    // ordering, and the pixmap that holds the top row of the table.
    if ( changedKeys.contains( "elts" ) || changedKeys.contains( "orgz" )
        || changedKeys.contains( "groupfile" ) )
        updateElementOrderInverse();
    // if the element order changed or the group changed, then update first row
    // pixmap, which is kept for speedy drawing.  Also update it if highlighting
    // has changed.
    if ( updateHighlighting( changedKeys ) || changedKeys.contains( "elts" )
        || changedKeys.contains( "orgz" ) || changedKeys.contains( "colr" )
        || changedKeys.contains( "groupfile" ) )
        updatePixmapQ();
    // if this function got called at all, then changedKeys.count() > 0, and so
    // we need to redraw the main view of things.
    updatePixmapP();
}

void MTLargeView::updatePixmapQ ()
{
    // The pixmap q is of size
    //   (getCellSize() + 1) * QSize( groupfile->group->order, 1 )
    // and it is the top row of the multiplication table.
    // This is good to precalculate, as it speeds table generation.
    // Note that the +1 in the above formula is to counterbalance an occasional
    // off-by-1 problem with QPainter's scaling.
    uint o = groupfile->group.order;
    uint c = getCellSize() + 1;
    q.resize( o * c, c );
    QPainter paint( &q );
    // first we must calculate all highlighting data:
    // and now we can draw the pixmaps:
    bool anyBackgroundUsed = highlightingTypeUsed( 0 );
    QString colr = readProperty( "colr" );
    for ( uint i = 0 ; i < o ; i++ ) {
        if ( isElementHighlightedBy( i, 1 ) ) {
            paint.setPen( QPen( elementHighlighting( i, 1 ), 3 ) );
        } else {
            paint.setPen( Qt::NoPen );
        }
        if ( anyBackgroundUsed ) {
            if ( isElementHighlightedBy( i, 0 ) ) {
                paint.setBrush( elementHighlighting( i, 0 ) );
            } else {
                paint.setBrush( colorGroup().background() );
            }
        } else {
            if ( colr == "0" ) {
                paint.setBrush( QColor( ordinv[i] * 360 / o, 100, 255, QColor::Hsv ) );
            } else if ( colr == "1" ) {
                uint level = ( ordinv[i] + 2 ) * 256 / ( o + 2 );
                paint.setBrush( QColor( level, level, level, QColor::Rgb ) );
            } else {
                paint.setBrush( colorGroup().background() );
            }
        }
        paint.drawRect( i * c, 0, c, c );
        if ( isElementHighlightedBy( i, 1 ) ) {
            paint.setPen( QPen( Qt::black, 1 ) );
            paint.setBrush( Qt::NoBrush );
            paint.drawRect( i * c + 3, 3, c - 6, c - 6 );
        }
        if ( isElementHighlightedBy( i, 2 ) ) {
            Q3PointArray pa( 0 );
            pa.putPoints( 0, 3, i * c, 0, i * c + 10, 0, i * c, 10 );
            paint.setPen( QPen( Qt::black, 1 ) );
            paint.setBrush( elementHighlighting( i, 2 ) );
            paint.drawConvexPolygon( pa );
        }
        drawElementText( i, &paint, QPoint( i * c + c / 2, c / 2 ), Qt::AlignCenter );
    }
}

void MTLargeView::updatePixmapP ()
{
    // This routine uses the prepared pixmap q to re-render the table to a pixmap.
    // It may be that sometimes you want to call this without calling updatePixmapQ()
    // first (e.g. in the case where just the zoom level has changed).
    //
    // begin by calculating the size of the pixmap, including sizes for coset separators
    uint c = getCellSize();
    uint o = groupfile->group.order;
    int zoom = readProperty( "zoom" ).toInt();
    int sepc = readProperty( "sepc" ).toInt() * c / 100;
    int orgz = readProperty( "orgz" ).toInt() - 1;
    uint size = o * c;
    uint subgsize = 0;
    if ( ( sepc > 0 ) && ( orgz > -1 ) ) {
        subgsize = groupfile->group.subgroups()[orgz].size();
        size += ( o / subgsize - 1 ) * sepc;
    }
    // now resize the pixmap and prepare to paint on it (this is where zoom is applied)
    p.resize( size * zoom / 100, size * zoom / 100 );
    QPainter paint( &p );
    paint.scale( zoom * 0.01, zoom * 0.01 );
    // now if there are coset separators, draw them as blank space
    if ( sepc > 0 ) {
        paint.setPen( Qt::NoPen );
        paint.setBrush( colorGroup().background() );
        for ( uint i = 1 ; i < o / subgsize ; i++ ) {
            paint.drawRect( i * c * subgsize + ( i - 1 ) * sepc, 0, sepc, size );
            paint.drawRect( 0, i * c * subgsize + ( i - 1 ) * sepc, size, sepc );
        }
    }
    // now copy each of the cells prepared in Q onto this pixmap as a grid
    uint x, y;
    for ( uint i = 0 ; i < o ; i++ ) {
        x = i * c;
        if ( subgsize ) x += sepc * ( i / subgsize );
        for ( uint j = 0 ; j < o ; j++ ) {
            y = j * c;
            if ( subgsize ) y += sepc * ( j / subgsize );
            GElt elt = groupfile->group.op( ord[j], ord[i] );
            paint.drawPixmap( x, y, q, elt * ( c + 1 ), 0, c + 1, c + 1 );
        }
    }
    // make that pixmap what we show:
    table->setPixmap( p );
    table->resize( p.size() );
}

GEltList MTLargeView::computeElementOrder ( GroupFile* gf, GEltList gens )
{
    GEltList generators = gens;
    if ( !generators.size() ) generators = gf->generatorlists[0];
    GEltList result;
    GEltList dims;
    gf->group.generateStructuredSubgroup( generators, result, dims, false );
    return result;
    // this necessitates recomputing the inverse, but that gets done elsewhere
}

QPoint MTLargeView::elementLocationPercent ( GElt e )
{
    if ( !ordinv.size() ) {
        if ( !groupfile ) return QPoint( 0, 0 );
        if ( !ord.size() ) updateElementOrder();
        updateElementOrderInverse();
    }
    uint ord = groupfile->group.order;
    uint idx = ordinv[e];
    int orgz = readProperty( "orgz" ).toInt() - 1;
    uint cell = ( orgz > -1 ) ? groupfile->group.subgroups()[orgz].size() : ord;
    double pct = ( orgz > -1 ) ? ( readProperty( "sepc" ).toInt() / 100.0 ) : 0.0;
    double locpct = ( idx + 0.5 + cell*pct*( idx / cell ) ) /
                    ( ord + cell*pct*( ord / cell - 1 ) );
    QPoint result( (int)( locpct * 100 ), 50 / ord );
    if ( readProperty( "axis" ) == "left" ) result = QPoint( result.y(), result.x() );
    return result;
}

void MTLargeView::updateElementOrder ()
{
    ord = computeElementOrder( groupfile );
    orderDirty = FALSE;
}

void MTLargeView::updateElementOrderInverse ()
{
    ordinv = perm_inverse( ord );
}

QPixmap MTLargeView::quickPixmap ( int w, int h, GroupFile* gf ) {
    // if the user did not pass dimension parameters, choose a sensible default
    if ( ( w < 0 ) || ( h < 0 ) ) w = h = QMAX( (uint)30, gf->group.order );
    QPixmap* p = new QPixmap( w, h );
    QPainter paint( p );
    paint.setPen( QPen( Qt::NoPen ) );
    uint o = gf->group.order;
    GEltList ord = computeElementOrder( gf );
    GEltList ordinv = perm_inverse ( ord );
    for ( uint i = 0 ; i < o ; i++ ) {
        for ( uint j = 0 ; j < o ; j++ ) {
            paint.setBrush( QBrush( QColor(
                    ordinv[gf->group.op( ord[j], ord[i] )] * 360 / o,
                    100, 255, QColor::Hsv ) ) );
            paint.drawRect(
                    i * w / o, j * h / o, ( i + 1 ) * w / o, ( j + 1 ) * h / o );
        }
    }
    return *p;
}

QPixmap MTLargeView::currentPixmap ()
{
    return p;
}

Q3PtrList<QAction> MTLargeView::actionsToExport ()
{
    Q3PtrList<QAction> result;
    
    zia = new QAction( QIcon( qPixmapFromMimeSource( "zoomin.png" ) ), "Zoom in", this );
    zia->setShortcut( QKeySequence( "Ctrl++" ) );
    connect( zia, SIGNAL(activated()), this, SLOT(zoomIn()) );
    result.append( zia );
    
    zoa = new QAction( QIcon( qPixmapFromMimeSource( "zoomout.png" ) ), "Zoom out", this );
    zoa->setShortcut( QKeySequence( "Ctrl+-" ) );
    connect( zoa, SIGNAL(activated()), this, SLOT(zoomOut()) );
    result.append( zoa );

    QAction* cpa = new QAction( QIcon( qPixmapFromMimeSource( "editcopy.png" ) ), "Copy",
                                this );
    cpa->setShortcut( QKeySequence( "Ctrl+C" ) );
    connect( cpa, SIGNAL(activated()), this, SLOT(copyImage()) );
    result.append( cpa );

#ifdef DUMP_ASY_FEATURE
    QAction* tmp = new QAction( QIcon( qPixmapFromMimeSource( "filesaveimage.png" ) ),
                                "Dump .asy code", this );
    tmp->setShortcut( QKeySequence( "Ctrl+D" ) );
    connect( tmp, SIGNAL(activated()), this, SLOT(dumpAsyCode()) );
    result.append( tmp );
#endif
    
    return result;
}

void MTLargeView::zoomIn ()
{
    uint zoom = readProperty( "zoom" ).toInt();
    zoom = zoom * 3 / 2;
    // cap it at three zooms in (337.5 = 100 * 3/2 * 3/2 * 3/2):
    if ( zoom > 337 ) zoom = 337;
    // fix the rounding error every time we come near 100:
    if ( ( 70 < zoom ) && ( zoom < 125 ) ) zoom = 100;
    change( "zoom", QString::number( zoom ),
            QString( "zoom in from %1% to %2%" )
            .arg( readProperty( "zoom" ) ).arg( zoom ) );
    // in the next line, we reread from mycopy because change() may be thwarted:
}

void MTLargeView::zoomOut ()
{
    uint zoom = readProperty( "zoom" ).toInt();
    zoom = zoom * 2 / 3;
    // cap it at 2 percent, because otherwise rounding would never let you zoom out!:
    if ( zoom < 2 ) zoom = 2;
    // fix the rounding error every time we come near 100:
    if ( ( 70 < zoom ) && ( zoom < 125 ) ) zoom = 100;
    change( "zoom", QString::number( zoom ),
            QString( "zoom out from %1% to %2%" )
            .arg( readProperty( "zoom" ) ).arg( zoom ) );
    // in the last line below, reread from mycopy because change() may be thwarted:
}


MTLargeControl::MTLargeControl ( GEVLarge* container, QWidget* parent, const char* name )
    : GEVLargeWidget( container, parent, name )
{
    Q3VBoxLayout* mainLayout = new Q3VBoxLayout( this, 0, 6 );

    QTabWidget* tabwidget = new QTabWidget( this );
    mainLayout->addWidget( tabwidget );

    // set up the tab page with the subset list box
    listBox = new SubsetListBox( container, 0, this );
    listBox->setHighlightTypes( QStringList()
        << "img/hightype-square-background.jpg:Background"
        << "img/hightype-square-border.jpg:Border"
        << "img/hightype-square-corner.jpg:Corner" );
    if ( container ) container->addLargeWidget( listBox, TRUE );
    tabwidget->addTab( listBox, "&Subsets" );

    // set up the tab page with all the other controls
    Q3VBox* othercontrols = new Q3VBox( this );
    othercontrols->setSpacing( 6 );
    othercontrols->setMargin( 11 );

    QLabel* sgr_instructions = new QLabel( "Organize by subgroup:", othercontrols );
    subgroups = new Q3ComboBox( FALSE, othercontrols );
    subgroups->insertItem( "none" );
    connect( subgroups, SIGNAL(activated(int)), this, SLOT(organizationChanged(int)) );
    sepCosLabel = new QLabel( "Separate cosets by:", othercontrols );
    separateCosets = new QSlider( 0, 100, 20, 0, Qt::Horizontal, othercontrols );
    separateCosets->setTracking( FALSE );
    connect( separateCosets, SIGNAL(valueChanged(int)),
             this, SLOT(separationChanged(int)) );

    Q3ButtonGroup* group = new Q3ButtonGroup(
        1, Qt::Horizontal, "Default coloration", othercontrols );
    colorChoices.append( new QRadioButton( "Spectrum/rainbow", group ) );
    colorChoices.append( new QRadioButton( "Grayscale", group ) );
    colorChoices.append( new QRadioButton( "None", group ) );
    connect( group, SIGNAL(clicked(int)), this, SLOT(colorizationChanged(int)) );
    colorChoices.at( 0 )->setChecked( TRUE );

    QWidget* space = new QWidget( othercontrols );
    space->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabwidget->addTab( othercontrols, "&Table" );

    // set up the Reset and Help buttons on the bottom
    Q3HBoxLayout* buttonLayout = new Q3HBoxLayout( mainLayout );

    QPushButton* helpButton = new QPushButton( "Help", this );
    buttonLayout->addWidget( helpButton );
    connect( helpButton, SIGNAL(clicked()), this, SLOT(showHelp()) );

    QPushButton* resetButton = new QPushButton( "Reset", this );
    buttonLayout->addWidget( resetButton );
    connect( resetButton, SIGNAL(clicked()), this, SIGNAL(reset()) );

    addProperty( "orgz", "0" );
    addProperty( "sepc", "0" );
    addProperty( "colr", "0" );

    GEWhatsThis::setup( sgr_instructions, "mt-organizeBySubgroups" );
    GEWhatsThis::setup( subgroups, "mt-organizeBySubgroups" );
    GEWhatsThis::setup( sepCosLabel, "mt-separateCosets" );
    GEWhatsThis::setup( separateCosets, "mt-separateCosets" );
    GEWhatsThis::setup( group, "mt-defaultColorization" );
    GEWhatsThis::setup( colorChoices.at( 0 ), "mt-defaultColorization" );
    GEWhatsThis::setup( colorChoices.at( 1 ), "mt-defaultColorization" );
    GEWhatsThis::setup( colorChoices.at( 2 ), "mt-defaultColorization" );
    GEWhatsThis::setup( helpButton, "largeWindow-helpButton" );
    GEWhatsThis::setup( resetButton, "largeWindow-resetButton" );
}

void MTLargeControl::set ( QString key, QString oldvalue, QString value, QString desc )
{
    if ( key == "orgz" ) {
        sepCosLabel->setEnabled( value != "0" );
        separateCosets->setEnabled( value != "0" );
        subgroups->setCurrentItem( value.toInt() );
        if ( value.toInt() ) change( "elts", QString::null, QString::null );
    } else if ( key == "sepc" ) {
        separateCosets->setValue( value.toInt() );
    } else if ( key == "colr" ) {
        colorChoices.at( value.toInt() )->setChecked( TRUE );
    }
    GEVLargeWidget::set( key, oldvalue, value, desc );
}

void MTLargeControl::colorizationChanged ( int id )
{
    change( "colr", QString::number( id ),
        "set default colorization to " + colorChoices.at( id )->text() );
}

void MTLargeControl::organizationChanged ( int id )
{
    change( "orgz", QString::number( id ),
        "organize by subgroup " + subgroups->text( id ) );
}

void MTLargeControl::separationChanged ( int value )
{
    change( "sepc", QString::number( value ), value ?
        QString( "separate cosets by %1%" ).arg( value ) :
        QString( "remove coset separation" ) );
}

void MTLargeControl::showHelp ()
{
    // I was going to make this sensitive to which tab is showing,
    // but if I did that then it would never show the whole rf-um-mt-options.html page.
    // Anyway, if I come up with a solution to that, then I can key off of
    // the tab widget's currentPageIndex() whether to show #subsetstab or #tabletab.
    GEHelpWindow::showHelp( "rf-um-mt-options.html" );
}

void MTLargeControl::groupFileChanged ( GroupFile* gf )
{
    GEVLargeWidget::groupFileChanged( gf );
    listBox->setGroupFile( groupfile );
    while ( subgroups->count() > 1 ) subgroups->removeItem( 1 );
    for ( int i = 0 ; i < groupfile->group.subgroups().count() ; i++ ) {
        subgroups->/*listBox()->*/insertItem(
            /*new RichListBoxItem(*/
                QString( "%1, a subgroup of order %2" )
                //"<nobr>%1, a subgroup of order %2</nobr>" )
                .arg( SubsetListBox::subsetTextName( 'H', i + 1 ) )
                .arg( groupfile->group.subgroups()[i].size() ) )/* )*/;
    }
    change( "orgz", "0", QString::null );
}


MTLarge::MTLarge ( QWidget* parent, const char* name )
    : GEVLarge( parent, name )
{
    addLargeWidget( view = new MTLargeView( this, splitter ), TRUE );
    addLargeWidget( new MTLargeControl( this, splitter ), FALSE );
}

QSize MTLarge::sizeHint () const
{
    return QSize( 600, 400 );
}

QPixmap MTLarge::currentPixmap ()
{
    return view->currentPixmap();
}

QPoint MTLarge::elementLocationPercent ( GElt e )
{
    return view->elementLocationPercent( e );
}


MTVisualizer::MTVisualizer ( GroupFile* gf )
    : GEVisualizer( gf )
{
}

QString MTVisualizer::typeName ()
{
    return "MultiplicationTable";
}

void MTVisualizer::createLarge ()
{
    large = new MTLarge();
}

void MTVisualizer::createSmall ()
{
    smallv = new MTSmall();
}

QPixmap MTVisualizer::quickPixmap ( int w, int h, int )
{
    return MTLargeView::quickPixmap( w, h, groupfile );
}


MTSmall::MTSmall ()
{
    topAction = new QAction( "Use top row for morphisms", this );
    connect( topAction, SIGNAL(activated()), this, SLOT(askedForTopRow()) );
    leftAction = new QAction( "Use left column for morphisms", this );
    connect( leftAction, SIGNAL(activated()), this, SLOT(askedForLeftColumn()) );

    addProperty( "axis", "top" );
}

Q3PtrList<QAction> MTSmall::actionsToExport ()
{
    Q3PtrList<QAction> result;
    result.append( ( readProperty( "axis" ) == "top" ) ? leftAction : topAction );
    return result;
}

void MTSmall::changeMorphismAxis ( bool top )
{
    change( "axis", top ? "top" : "left",
            QString( "use %1 for morphism arrows" )
            .arg( top ? "top row" : "left column" ) );
    canvas()->update();
}

bool MTSmall::morphismAxis ()
{
    return ( readProperty( "axis" ) == "top" );
}

void MTSmall::askedForTopRow ()
{
    emit pushThisCommand( new ToggleMTSmallOrientation( this ) );
}

void MTSmall::askedForLeftColumn ()
{
    emit pushThisCommand( new ToggleMTSmallOrientation( this ) );
}


ToggleMTSmallOrientation::ToggleMTSmallOrientation ( MTSmall* smallv )
    : smallv( smallv ), setToTopRow( setToTopRow )
{
    setText( QString( "use %1 for morphisms in multiplication table" )
             .arg( setToTopRow ? "top row" : "left column" ) );
    setToTopRow = !( smallv->morphismAxis() );
}

void ToggleMTSmallOrientation::redo ()
{
    smallv->changeMorphismAxis( setToTopRow );
}

void ToggleMTSmallOrientation::undo ()
{
    smallv->changeMorphismAxis( !setToTopRow );
}


