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


#include "cayleydiagram.h"
#include "gehelpwindow.h"
#include "gewhatsthis.h"
#include "modelviewwidgets.h"
#include "sympanewidgets.h"
#include "glscenenodes.h"
#include "ge.h"
#include <qlayout.h>
#include <qpixmap.h>
#include <q3vbox.h>
#include <qpushbutton.h>
#include <qgl.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qimage.h>
#include <q3dragobject.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <qtabwidget.h>
#include <qmessagebox.h>
#include <qslider.h>
#include <qcheckbox.h>
#include <qdatetime.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3ValueList>
#include <Q3PtrList>
#include <Q3VBoxLayout>
#include <Q3MemArray>
#include <math.h>
//Added by me to help porting:
#include <q3mimefactory.h>

#ifdef DUMP_ASY_FEATURE
#include <QDebug>
#endif


CDLargeView::CDLargeView ( GEVLarge* container, QWidget* parent, const char* name )
    : GEVLargeWidget( container, parent, name ), dragging( 0 ),
      needToReform( FALSE ), needToUpdateFog( FALSE ), diagramRadius( 0.0 )
{
    Q3VBoxLayout* layout = new Q3VBoxLayout( this );

    addProperty( "zoom", "250" );
    addProperty( "line", "3" );
    addProperty( "node", "100" );
    addProperty( "fogl", "15" );
    addProperty( "fogs", "Y" );

    //addProperty( "gtbl", QString::null );
    addProperty( "head", "100" );
    addProperty( "chnk", "0" );
    addProperty( "labs", "Y" );
    addProperty( "labz", "12" );
    addProperty( "mult", "R" );

    // create and set up OpenGL model view widget
    viewer = new GLModelView( this );
    usedDims[0] = usedDims[1] = usedDims[2] = FALSE;
    placeCamera();
    viewer->setLineWidth( 3.0 );
    viewer->setClearColor( 0.94, 0.78, 0.78 );
    viewer->setUseFog();
    calibrateFog( viewer, readProperty( "fogl" ) );
    viewer->setMinimumSize( 200, 200 );
    layout->addWidget( viewer );
    //connect( viewer, SIGNAL(startDrag(QPoint)), this, SLOT(objectStartDrag()) );
    viewer->setDragAllowed( FALSE );
    viewer->setOtherButtonState( /*Qt::ButtonState( Qt::LeftButton | Qt::ShiftButton )*/
                                 (int)Qt::LeftButton | (int)Qt::ShiftModifier );
    connect( viewer, SIGNAL(startOther(QPoint)),
             this, SLOT(startEditDrag(QPoint)) );
    connect( viewer, SIGNAL(duringOther(QPoint,QPoint)),
             this, SLOT(duringEditDrag(QPoint,QPoint)) );
    connect( viewer, SIGNAL(finishOther(QPoint,QPoint)),
             this, SLOT(finishEditDrag(QPoint,QPoint)) );
    //new CDTipFlagger( viewer, this );

    diagram = new GLSceneNode();
    diagram->addChild( nodesholder = new CDAxisWrapper() );
    nodesholder->translate( -1.25, -1.25, -1.25 );
    nodesholder->scale( 2.5, 2.5, 2.5 );
    diagram->addChild( arrowsholder = new GLSceneNode() );
    viewer->addChild( diagram );

    // place lights in scene
    GLLight* tmp;
    QGLContext* c = ( QGLContext* )( viewer->context() );
    viewer->addFixedChild( tmp = new GLLight(  2.0,  2.0,  2.0, 1.0, c ) );
    tmp->setDiffuse( 0.5, 0.5, 0.5, 1.0 );
    viewer->addFixedChild( tmp = new GLLight(  2.0, -2.0, -2.0, 1.0, c ) );
    tmp->setDiffuse( 0.5, 0.5, 0.5, 1.0 );
    viewer->addFixedChild( tmp = new GLLight( -2.0,  2.0, -2.0, 1.0, c ) );
    tmp->setDiffuse( 0.5, 0.5, 0.5, 1.0 );
    viewer->addFixedChild( tmp = new GLLight( -2.0, -2.0,  2.0, 1.0, c ) );
    tmp->setDiffuse( 0.5, 0.5, 0.5, 1.0 );

    GEWhatsThis::setup( viewer, "cd-view" );
}

/*
void CDLargeView::considerTooltip ( const QPoint& p, CDTipFlagger* useMe )
{
    bool overflow;
    Q3ValueList<GLSceneNode*> hits =
        viewer->pickClick( (GLdouble)p.x(), (GLdouble)p.y(), &overflow );
    if ( !overflow ) {
        // sufficiently few things under mouse pointer, so find first _node_ there:
        QStringList elementNames;
        QStringList cosetNames;
        for ( int i = 0 ; i < hits.count() ; i++ ) {
            int eltloc = -1;
            int cosloc = -1;
            for ( uint j = 0 ; j < nodes.count() ; j++ ) {
                if ( ( eltloc == -1 ) && nodes.at( j )
                  && nodes.at( j )->containsSphere( hits[i] ) ) eltloc = j;
                if ( ( cosloc == -1 ) && nodes.at( j )
                  && nodes.at( j )->containedByChunk( hits[i] ) ) cosloc = j;
            }
            if ( eltloc > -1 )
                elementNames <<
                    ( "<nobr>" + groupfile->getRepresentation( (GElt)eltloc ).HTML
                      + "</nobr>" );
            if ( cosloc > -1 )
                cosetNames <<
                    ( "<nobr>" + groupfile->getRepresentation( (GElt)cosloc ).HTML
                      + "</nobr>" );
        }
        if ( elementNames.count() ) {
            if ( elementNames.count() > 1 ) {
                elementNames[0] = "<b>In front:</b> " + elementNames[0];
                if ( elementNames.count() > 2 ) {
                    QStringList::iterator one = elementNames.begin();
                    one++;
                    elementNames.insert( one, "<b>Others behind:</b>" );
                    for ( int i = 2 ; i < elementNames.count() ; i++ )
                        elementNames[i] = " &nbsp; &nbsp; " + elementNames[i];
                } else {
                    elementNames[1] = "<b>Behind:</b> " + elementNames[1];
                }
            }
            useMe->tip( QRect( p.x() - 2, p.y() - 2, 5, 5 ),
                        "<nobr>" + elementNames.join( "</nobr><br>\n<nobr>" ) +
                        "</nobr>" );
        } else if ( cosetNames.count() ) {
            if ( cosetNames.count() > 1 ) {
                cosetNames[0] = "<b>In front:</b> " + cosetNames[0] + chunkCosetName;
                if ( cosetNames.count() > 2 ) {
                    QStringList::iterator one = cosetNames.begin();
                    one++;
                    cosetNames.insert( one, "<b>Others behind:</b>" );
                    for ( int i = 2 ; i < cosetNames.count() ; i++ )
                        cosetNames[i] = " &nbsp; &nbsp; "
                                      + cosetNames[i] + chunkCosetName;
                } else {
                    cosetNames[1] = "<b>Behind:</b> " + cosetNames[1] + chunkCosetName;
                }
            } else {
                cosetNames[0] += chunkCosetName;
            }
            useMe->tip( QRect( p.x() - 2, p.y() - 2, 5, 5 ),
                        "<nobr>" + cosetNames.join( "</nobr><br>\n<nobr>" ) +
                        "</nobr>" );
        }
    }
}
*/

QPoint CDLargeView::elementLocationPercent ( GElt e )
{
    if ( e < nodes.count() ) {
        double x, y, z, r;
        nodes.at( e )->getAbsolutePositionAndRadius( x, y, z, r );
        viewer->worldToScreen( x, y, z, x, y, z );
        QRect viewport = viewer->viewportRectangle();
        return QPoint( int( 100 * x / viewport.width() ),
                       100 - int( 100 * y / viewport.height() ) );
    } else {
        return QPoint( 0, 0 );
    }
}

/*
void CDLargeView::objectStartDrag ()
{
    //QPixmap* tmp = new QPixmap( currentPixmap() );
    //currentP = currentPixmap();
    currentP = viewer->renderPixmap();
    qDebug( "drag: saving a copy to /Users/nathancarter/Desktop/GEqDebug.png..." );
    currentP.convertToImage().save( "/Users/nathancarter/Desktop/GEqDebug.png", "PNG" );
    qDebug( "done saving copy" );
    ( new QImageDrag( currentP.convertToImage(), this ) )->drag();
}
*/

void CDLargeView::copyImage ()
{
    //QPixmap* tmp = new QPixmap( currentPixmap() );
    //currentP = currentPixmap();
    currentP = viewer->renderPixmap();
    //qDebug( "drag: saving a copy to /Users/nathancarter/Desktop/GEqDebug.png..." );
    //currentP.convertToImage().save( "/Users/nathancarter/Desktop/GEqDebug.png", "PNG" );
    //qDebug( "done saving copy" );
    QApplication::clipboard()->setPixmap( currentP, QClipboard::Clipboard );
}

void CDLargeView::dumpAsyCode ()
{
#ifdef DUMP_ASY_FEATURE
    qDebug( "%s", QString( "\n"
                           "//--------------------------------------------------------\n"
                           "// Here begins text generated by Group Explorer %1\n"
                           "// on %2, the asymptote code for the Cayley diagram\n"
                           "// for the group %3.\n"
                           "// It requires the Asymptote file makecd.asy.\n"
                           "//--------------------------------------------------------\n"
                           "\n"
                           "include \"./makecd.asy\";\n"
                           "\n"
                           "size( 150, 0 );\n"
                           "\n"
                           "camera = (0,-5,0); // for truly 3D stuff, move as needed\n"
                           "currentprojection = perspective( camera );\n"
                           "" )
                  .arg( GE_VERSION_STRING ).arg( QDateTime::currentDateTime().toString() )
                  .arg( groupfile->codeName() ).latin1() );
    qDebug( "picture CD1 = \n"
            "    CayleyDiagram(\n"
            "        nodeLocs = new triple[] {" );
    for ( uint i = 0 ; i < nodes.count() ; i++ ) {
        double x, y, z, r;
        nodes.at( i )->getDiagramPositionAndRadius( x, y, z, r );
        qDebug( "%s", QString( "            (%1,%2,%3)," )
                      .arg( x ).arg( z ).arg( y ).latin1() ); // permute on purpose
    }
    qDebug( "            }," );
    qDebug( "        nodeNames = new string[] {" );
    for ( uint i = 0 ; i < nodes.count() ; i++ )
        qDebug( "            \"$" + groupfile->getRepresentation( i ).text + "$\"," );
    qDebug( "            }," );
    qDebug( "        arrows = new triple[][] {" );
    QStringList ordersDump, pensDump;
    GEltList arrs = code_to_elt_list( readProperty( "arro" ) );
    uint arridx = 0;
    for ( uint i = 0 ; i < arrs.count() ; i++ ) {
        uint numArrs = groupfile->group.order;
        uint order = arrows.at( arridx )->getOrder();
        if ( order == 2 ) numArrs /= 2;
        QColor c;
        c.setHsv( i * 360 / arrs.size(), 255, 200 );
        for ( uint j = 0 ; j < numArrs ; j++ ) {
            double x, y, z, x2, y2, z2;
            CDArrow* thisA = arrows.at( arridx+j );
            thisA->arrowPath( 0, x, y, z, thisA->getArcing() );
            if ( thisA->getArcing() == 0 ) {
                thisA->arrowPath( 1, x2, y2, z2, thisA->getArcing() );
                qDebug( "%s", QString( "            { (%1,%2,%3), (%4,%5,%6) }," )
                              .arg( x ).arg( z ).arg( y ).arg( x2 ).arg( z2 ).arg( y2 )
                              .latin1() );
                // the above x,y,z is permuted to x,z,y on purpose (in both cases)
            } else {
                thisA->arrowPath( 0.333, x2, y2, z2, thisA->getArcing() );
                qDebug( "%s", QString( "            { (%1,%2,%3), (%4,%5,%6)," )
                              .arg( x ).arg( z ).arg( y ).arg( x2 ).arg( z2 ).arg( y2 )
                              .latin1() );
                // the above x,y,z is permuted to x,z,y on purpose (in both cases)
                thisA->arrowPath( 0.666, x, y, z, thisA->getArcing() );
                thisA->arrowPath( 1, x2, y2, z2, thisA->getArcing() );
                qDebug( "%s", QString( "              (%1,%2,%3), (%4,%5,%6) }," )
                              .arg( x ).arg( z ).arg( y ).arg( x2 ).arg( z2 ).arg( y2 )
                              .latin1() );
                // the above x,y,z is permuted to x,z,y on purpose (in both cases)
            }
            ordersDump << QString::number( order );
            pensDump << QString( "rgb( %1, %2, %3 )" )
                        .arg( c.red() / 255.0 ).arg( c.green() / 255.0 )
                        .arg( c.blue() / 255.0 );
        }
        arridx += numArrs;
    }
    qDebug( "            },\n"
            "        orders = new int[] { %s },",
            ordersDump.join( "," ).latin1() );
    qDebug( "        arrowPens = new pen[] {\n"
            "            %s\n"
            "            },",
            pensDump.join( ",\n            " ).latin1() );
    qDebug( "        arrowThickness = 1.5//,\n"
            "        // If you want no BSP occluding, then uncomment these\n"
            "        // line(s) and tweak numbers as fits your diagram:\n"
            "        // (also uncomment the comma 3 lines above this)\n"
            "        // orderDepth = false,\n"
            "        // arrowMargin = 1.75mm,\n"
            "        // nodeSize = 0.15\n"
            "        );\n"
            "\n"
            "add( CD1 );\n"
            "shipout( bbox( 10, invisible ) );\n"
            "" );
#endif
}

void CDLargeView::startEditDrag ( QPoint orig )
{
    if ( readProperty( "chnk" ) != "0" ) {
        QMessageBox::information(
            this, "Cannot move nodes while chunked",
            "You cannot move nodes while the diagram\n"
            "is chunked.  Turn off chunking first.",
            QMessageBox::Ok );
        return;
    }

    bool overflow;
    Q3ValueList<GLSceneNode*> hits =
        viewer->pickClick( (GLdouble)orig.x(), (GLdouble)orig.y(), &overflow );

    if ( !overflow ) { // ensures there are not too many things under mouse pointer
        if ( !hits.count() ) return;

        if ( hits[0]->parent && ( hits[0]->parent->name == "cdnode" ) ) {
            // this means we've clicked on a node and will drag it
            dragging = hits[0];
            dragging->relativeCoordinates( 0, 0, 0, dragX, dragY, dragZ );
            double x, y;
            viewer->worldToScreen( dragX, dragY, dragZ, x, y, screenZ );
            /*
            qDebug( QString( "starting drag at world %1,%2,%3 == screen %4,%5,%6" )
                    .arg( dragX, 5, 'f', 3 ).arg( dragY, 5, 'f', 3 )
                    .arg( dragZ, 5, 'f', 3 )
                    .arg( x, 5, 'f', 3 ).arg( y, 5, 'f', 3 ).arg( screenZ, 5, 'f', 3 ) );
            */
        } else if ( hits[0]->name == "cdarrow" ) {
            dragging = hits[0];
            // compute the on-screen vector that would give us 1 more level of arcing
            double x1, y1, z1, x2, y2, z2, arcing = ( ( CDArrow* )dragging )->getArcing();
            ( ( CDArrow* )dragging )->arrowPath( 0.5, x1, y1, z1, arcing );
            dragging->relativeCoordinates( x1, y1, z1, x1, y1, z1 );
            ( ( CDArrow* )dragging )->arrowPath( 0.5, x2, y2, z2, arcing + 1.0 );
            dragging->relativeCoordinates( x2, y2, z2, x2, y2, z2 );
            viewer->worldToScreen( x1, y1, z1, x1, y1, z1 );
            viewer->worldToScreen( x2, y2, z2, x2, y2, z2 );
            screenArcDragX = x2 - x1;
            screenArcDragY = y1 - y2; // y is always upside-down
        }
    }
}

void CDLargeView::duringEditDrag ( QPoint prev, QPoint now )
{
    if ( !dragging ) return;
    if ( dragging->parent && dragging->parent->name == "cdnode" ) {
        viewer->screenToWorld( now.x(), viewer->height() - now.y(), screenZ,
                               dragX, dragY, dragZ );
        /*
        qDebug( QString( "    more drag at world %1,%2,%3 == screen %4,%5,%6" )
                .arg( dragX, 5, 'f', 3 ).arg( dragY, 5, 'f', 3 ).arg( dragZ, 5, 'f', 3 )
                .arg( now.x(), 5, 'f', 3 ).arg( viewer->height() - now.y(), 5, 'f', 3 )
                .arg( screenZ, 5, 'f', 3 ) );
        */
        ( ( CDNode* )( dragging->parent ) )->setAbsolutePosition( dragX, dragY, dragZ );
        viewer->updateGL();
    } else if ( dragging->name == "cdarrow" ) {
        if ( screenArcDragX || screenArcDragY ) {
            int dx = now.x() - prev.x(), dy = now.y() - prev.y();
            GLdouble darc = ( dx * screenArcDragX + dy * screenArcDragY )
                / ( screenArcDragX * screenArcDragX + screenArcDragY * screenArcDragY );
                // this is like dividing by the square root twice, once to normalize the
                // dot product, and once to ask what percent of <scrArDrX,scrArDrY> it is
            CDArrow* a = ( CDArrow* )dragging;
            a->setArcing( a->getArcing() + darc );
        }
        viewer->updateGL();
    }
}

void CDLargeView::finishEditDrag ( QPoint /*orig*/, QPoint /*now*/ )
{
    if ( !dragging ) return;
    GLSceneNode* justBeenDragged = dragging;
    dragging = 0;
    for ( uint j = 0 ; j < nodes.count() ; j++ ) {
        if ( nodes.at( j ) && nodes.at( j )->containsSphere( justBeenDragged ) ) {
            change( QString( "N%1" ).arg( j ),
                    QString( "%1 %2 %3" ).arg( dragX ).arg( dragY ).arg( dragZ ),
                    "move node" );
            return;
        }
    }
    for ( uint j = 0 ; j < arrows.count() ; j++ ) {
        if ( arrows.at( j ) == justBeenDragged ) {
            change( QString( "A%1" ).arg( j ),
                    QString::number( arrows.at( j )->getArcing() ),
                    "change arcing amount" );
            return;
        }
    }
}

void CDLargeView::reset ()
{
    //qDebug( "CDLargeView::reset()" );
    viewer->resetScene();
}

Q3PtrList<QAction> CDLargeView::actionsToExport ()
{
    Q3PtrList<QAction> result;

    QAction* cpa = new QAction( QIcon( qPixmapFromMimeSource( "editcopy.png" ) ),
                                "Copy", this );
    cpa->setShortcut( QKeySequence( "Ctrl+C" ) );
    connect( cpa, SIGNAL(activated()), this, SLOT(copyImage()) );
    result.append( cpa );

#ifdef DUMP_ASY_FEATURE
    QAction* tmp = new QAction( QIcon( qPixmapFromMimeSource( "filesaveimage.png" ) ),
                                "Dump .asy code", QKeySequence( "Ctrl+D" ), this );
    connect( tmp, SIGNAL(activated()), this, SLOT(dumpAsyCode()) );
    result.append( tmp );
#endif

    return result;
}

CDArrow* CDLargeView::getArrow ( GElt generator, GElt sourceNode )
{
    if ( groupfile ) {
        uint index = generator * groupfile->group.order + sourceNode;
        if ( index < arrows.count() ) return arrows.at( index );
    }
    return 0;
}

void CDLargeView::calibrateFog ( GLModelView* mv, QString level )
{
    GLdouble fogStart = mv->distanceFromEye( 0.0, 0.0, 0.0 );
    mv->setFogValues( 0.94, 0.78, 0.78,
                      fogStart - 1.25, fogStart + ( 60 - level.toInt() ) * 1.25 / 10.0 );
}

uint chunkingRecursion ( GLSceneNode* insideMe, uint targetLevel )
{
    uint lowerLevel = 0;
    for ( uint i = 0 ; i < insideMe->children.count() ; i++ ) {
        uint low = chunkingRecursion( insideMe->children.at( i ), targetLevel );
        if ( low > lowerLevel ) lowerLevel = low;
    }
    lowerLevel++;
    /*
    qDebug( QString( "                                     " ).left( lowerLevel*4 ) +
            QString( "chunkingRecursion( %1 [%2], %3 ) == %4" )
            .arg( (uint)insideMe ).arg( insideMe->name ).arg( targetLevel )
            .arg( lowerLevel ) );
    */
    if ( insideMe->name.endsWith( "-axis" ) ) {
        ( ( CDAxisWrapper* )insideMe )->chunkThis( lowerLevel == targetLevel );
        /*
        qDebug( QString( "                                   " ).left( lowerLevel*4 ) +
                QString( "ends with -axis, so chunk set to %1" )
                .arg( lowerLevel == targetLevel ? "TRUE" : "FALSE" ) );
        */
    } else {
        /*
        qDebug( QString( "                                   " ).left( lowerLevel*4 ) +
                "does not end with -axis (no chunk)" );
        */
    }
    return lowerLevel;
}
void CDLargeView::set ( QString key, QString oldvalue, QString value, QString desc )
{
    //qDebug( "CDLargeView::set( \"" + key + "\", \"" + oldvalue + "\", \"" + value +
    //        "\", \"" + desc + "\" )" );
    bool ok;
    int N = key.mid( 1 ).toInt( &ok );
    if ( ok && ( key[0] == 'N' ) ) {
        // moving node N's position
        if ( value.isNull() ) {
            if ( nodes.at( N ) ) nodes.at( N )->clearAbsolutePosition();
            // nodes.at( N ) should be true, but be careful
        } else {
            if ( nodes.at( N ) ) {
                // nodes.at( N ) should be true, but be careful
                QStringList xyzstrs = QStringList::split( ' ', value );
                if ( xyzstrs.count() < 3 ) return; // should never happen, but better safe
                double x, y, z;
                x = xyzstrs[0].toDouble();
                y = xyzstrs[1].toDouble();
                z = xyzstrs[2].toDouble();
                nodes.at( N )->setAbsolutePosition( x, y, z );
            }
        }
    } else if ( ok && ( key[0] == 'A' ) ) {
        if ( value.isEmpty() ) {
            if ( arrows.at( N ) ) arrows.at( N )->decideArcing( nodes );
            // arrows.at( N ) should be true, but be careful
        } else {
            if ( arrows.at( N ) ) arrows.at( N )->setArcing( value.toDouble() );
            // arrows.at( N ) should be true, but be careful
        }
    } else if ( key == "node" ) {
        double r = value.toInt() / 100.0;
        for ( uint i = 0 ; i < nodes.count() ; i++ ) nodes.at( i )->setRadius( r );
    } else if ( key == "labs" ) {
        uint size = ( value == "Y" ) ? readProperty( "labz" ).toInt() : 0;
        for ( uint i = 0 ; i < nodes.count() ; i++ ) nodes.at( i )->setSize( size );
    } else if ( key == "labz" ) {
        uint size = readProperty( "labz" ).toInt();
        for ( uint i = 0 ; i < nodes.count() ; i++ ) nodes.at( i )->setSize( size );
    } else if ( key == "line" ) {
        viewer->setLineWidth( double( readProperty( "line" ).toInt() ) );
    } else if ( key == "zoom" ) {
        placeCamera();
        calibrateFog( viewer, readProperty( "fogl" ) );
    } else if ( key == "fogl" ) {
        calibrateFog( viewer, readProperty( "fogl" ) );
    } else if ( key == "fogs" ) {
        viewer->setUseFog( value == "Y" );
        calibrateFog( viewer, readProperty( "fogl" ) );
    } else if ( key == "gtbl" ) {
        if ( value.isNull() || oldvalue.isNull() ) {
            reformDiagram();
        } else {
            arrangeNodes();
            for ( uint i = 0 ; i < arrows.count() ; i++ )
                arrows.at( i )->decideArcing( nodes );
        }
    } else if ( key == "indx" ) {
        reformDiagram();
        int indx = value.toInt();
        if ( indx < (int)groupfile->cays.count() )
            change( "arro", elt_list_to_code( groupfile->cays[indx].getArrows() ),
                    QString::null );
    } else if ( key == "arro" ) {
        arrangeArrows();
    } else if ( key == "mult" ) {
        arrangeArrows();
    } else if ( key == "head" ) {
        GLdouble percent = value.toInt() * 0.01;
        for ( uint i = 0 ; i < arrows.count() ; i++ )
            arrows.at( i )->setHeadPosition( percent );
    }
    GEVLargeWidget::set( key, oldvalue, value, desc );
}

void CDLargeView::groupFileChanged ( GroupFile* gf )
{
    GEVLargeWidget::groupFileChanged( gf );
    needToReform = TRUE;
    needToUpdateFog = TRUE;
}

void CDLargeView::placeCamera ()
{
    double dist = ( 500 - readProperty( "zoom" ).toInt() ) / 100.0;
    //qDebug( QString( "Dimensions used: x? %1 y? %2 z? %3 (with dist==%4)" )
    //        .arg( usedDims[0] ? "Y" : "N" ).arg( usedDims[1] ? "Y" : "N" )
    //        .arg( usedDims[2] ? "Y" : "N" ).arg( dist ) );
    if ( usedDims[0] && usedDims[1] && usedDims[2] ) {
        if ( diagramRadius ) dist *= diagramRadius / sqrt( 3.0 );
        viewer->setCameraPosition( dist, dist, dist );
        if ( needToUpdateFog ) change( "fogs", "Y", QString::null );
        needToUpdateFog = FALSE;
        return;
    }
    if ( !usedDims[2] ) {
        viewer->setCameraPosition( 0, 0, dist * 1.25 );
    } else if ( !usedDims[1] ) {
        viewer->setCameraPosition( 0, dist * 1.25, 0 );
    } else {
        viewer->setCameraPosition( dist * 1.25, 0, 0 );
    }
    if ( needToUpdateFog ) change( "fogs", "N", QString::null );
    needToUpdateFog = FALSE;
    calibrateFog( viewer, readProperty( "fogl" ) );
}

void CDLargeView::reformDiagram ()
{
    arrows.clear();
    arrowsholder->children.clear();
    nodes.clear();
    nodesholder->children.clear();
    double r = readProperty( "node" ).toInt() / 100.0;
    for ( GElt g = 0 ; g < groupfile->group.order ; g++ ) {
        CDNode* node = new CDNode( diagram );
        node->setRadius( r );
        node->setLabel( groupfile->getRepresentation( g ).text );
        nodes.append( node );
    }
    arrangeNodes();
    arrangeArrows();
    needToReform = FALSE;
}

void CDLargeView::disassembleOrbitGridLayout ( GLSceneNode* startHere )
{
    // this routine deletes all the CDAxisWrapper objects that had been piled up to
    // lay out the orbit grid (so we can make a new pile) without deleting the CDNode
    // objects that inevitably lie at the bottom of the hierarchy (those we keep stored
    // in the nodes member variable, so we don't have to keep recreating them)
    if ( !startHere ) {
        if ( !nodesholder ) return;
        startHere = nodesholder;
    }
    for ( int i = startHere->children.count() - 1 ; i >= 0 ; i-- ) {
        if ( startHere->children.at( i )->name == "cdnode" ) {
            startHere->children.take( i );
        } else {
            disassembleOrbitGridLayout( startHere->children.at( i ) );
            startHere->children.remove( i );
        }
    }
}

void CDLargeView::arrangeNodes ()
{
    usedDims[0] = usedDims[1] = usedDims[2] = FALSE;
    disassembleOrbitGridLayout();
    viewer->resetScene();

    int D = readProperty( "indx" ).toInt();
    if ( ( D == (int)groupfile->cays.count() ) || ( D == -1 ) ) {
        QString gtbl = readProperty( "gtbl" );
        if ( gtbl.isNull() ) {
            orbitGrid = GEltGrid();
        } else {
            QStringList gparts = QStringList::split( ",", gtbl );
            while ( gparts.size() < 3 ) gparts << ""; // in case trivial group
            orbitGrid = generateOrbitGrid( groupfile, code_to_elt_list( gparts[0] ) );
            Q3MemArray<uint> axes = code_to_elt_list( gparts[1] );
            GEltList orderperm = code_to_elt_list( gparts[2] );
            //orderperm = perm_inverse( orderperm );//happens in layOutOrbitGrid
            layOutOrbitGrid( orbitGrid, axes, orderperm, nodes, nodesholder );

            for ( uint i = 0 ; i < axes.size() ; i++ ) {
                switch ( axes[i] ) {
                    case 0: usedDims[0] = TRUE; break;
                    case 1: usedDims[1] = TRUE; break;
                    case 2: usedDims[2] = TRUE; break;
                    case 3: case 6: usedDims[0] = usedDims[1] = TRUE; break;
                    case 4: case 7: usedDims[0] = usedDims[2] = TRUE; break;
                    case 5: case 8: usedDims[1] = usedDims[2] = TRUE; break;
                }
            }
        }
    } else {
        for ( GElt g = 0 ; g < groupfile->group.order ; g++ ) {
            GLdouble x, y, z;
            groupfile->cays[D].getNodePosition( g, x, y, z );
            CDAxisWrapper* wrap = new CDAxisWrapper( nodesholder );
            wrap->scale( 0.2, 0.2, 0.2 );
            wrap->addChild( nodes.at( g ) );
            nodes.at( g )->setAbsolutePosition( x, y, z );
            if ( QABS( x ) > 0.1 ) usedDims[0] = TRUE;
            if ( QABS( y ) > 0.1 ) usedDims[1] = TRUE;
            if ( QABS( z ) > 0.1 ) usedDims[2] = TRUE;
        }
    }

    for ( uint i = 0 ; i < arrows.count() ; i++ ) arrows.at( i )->decideArcing( nodes );

    // recompute diagram radius every time we arrange the nodes (helps with zooming):
    diagramRadius = 0.0;
    for ( uint i = 0 ; i < nodes.count() ; i++ ) {
        double x, y, z, r;
        nodes.at( i )->getAbsolutePositionAndRadius( x, y, z, r );
        r += sqrt( x*x + y*y + z*z );
        if ( r > diagramRadius ) diagramRadius = r;
    }

    placeCamera();
    viewer->updateGL();
}

void CDLargeView::arrangeArrows ()
{
    //qDebug( "CDLargeView::arrangeArrows()" );
    arrows.clear();
    arrowsholder->children.clear();
    if ( !groupfile ) return;
    GEltList arrs = code_to_elt_list( readProperty( "arro" ) );
    bool right = ( readProperty( "mult" ) == "R" );
    QColor c;
    for ( uint i = 0 ; i < arrs.size() ; i++ ) {
        c.setHsv( i * 360 / arrs.size(), 255, 100 );
        uint o = groupfile->group.orderOf( arrs[i] );
        for ( GElt g = 0 ; g < groupfile->group.order ; g++ ) {
            GElt ga = right ? groupfile->group.op( g, arrs[i] )
                            : groupfile->group.op( arrs[i], g );
            if ( ( o > 2 ) || ( g < ga ) ) {
                CDArrow* a = new CDArrow( nodes.at( g ), nodes.at( ga ), diagram );
                a->setColor( c.red() / 255.0, c.green() / 255.0, c.blue() / 255.0, 1.0 );
                a->setOrder( o );
                a->decideArcing( nodes );
                a->setHeadPosition( readProperty( "head" ).toInt() * 0.01 );
                arrowsholder->addChild( a );
                arrows.append( a );
            }
        }
    }
    viewer->updateGL();
}

void CDLargeView::updateContent ( QStringList changedKeys )
{
    //qDebug( "CDLargeView::updateContent( <\"" + changedKeys.join( "\", \"" ) + "\"> )" );
    if ( updateHighlighting( changedKeys ) && groupfile ) {
        for ( GElt g = 0 ; g < groupfile->group.order ; g++ ) {
            CDNode* gnode = nodes.at( g );
            for ( uint htype = 0 ; htype < 3 ; htype++ ) {
                if ( isElementHighlightedBy( g, htype ) ) {
                    gnode->setHighlighting( htype, elementHighlighting( g, htype ) );
                } else {
                    gnode->clearHighlighting( htype );
                }
            }
        }
    }
    if ( needToReform ) reformDiagram();
    if ( changedKeys.contains( "groupfile" ) ||
         changedKeys.contains( "gtbl" ) ||
         changedKeys.contains( "chnk" ) )
    {
        uint level = readProperty( "chnk" ).toInt();
        // We need the +3 in next line for these two reasons:
        // First, CDNode is a node with a node inside, so that's two extra.
        // Second, the lowest things are 1's, not 0's; another extra.
        chunkingRecursion( nodesholder, level ? level + 3 : 0 );
        // Also note that zero would ordinarily chunk things of size 0, i.e. individual
        // nodes, but we don't ever want to do that; that should mean no chunking, so
        // we make 0 stay 0, so no chunking occurs.
        if ( level ) {
            if ( level >= orbitGrid.numDimensions() ) {
                chunkCosetName = "[ERROR]";
            } else {
                GEltList sgrgens( level );
                GEltList index;
                index.fill( 0, orbitGrid.numDimensions() );
                for ( uint i = 0 ; i < level ; i++ ) {
                    if ( i ) index[index.size()-i] = 0;
                    index[index.size()-i-1] = 1;
                    sgrgens[i] = orbitGrid[index];
                }
                GEltList sgr = groupfile->group.subgroupGenBy( sgrgens );
                int j;
                GEltListList sgrs = groupfile->group.subgroups();
                for ( j = 0 ; j < sgrs.count() ; j++ )
                    if ( elt_sets_are_equal( sgrs[j], sgr ) ) break;
                if ( j < sgrs.count() )
                    chunkCosetName = QString( "<i>H</i><sub>%1</sub>" ).arg( j + 1 );
                else if ( j == sgrs.count() )
                    chunkCosetName = groupfile->getPrimaryName().HTML;
                else
                    chunkCosetName = "[ERROR]";
            }
        } else {
            chunkCosetName = "[NONE]";
        }
    }
    viewer->updateGL();
}

GEltGrid CDLargeView::generateOrbitGrid ( GroupFile* G, GEltList gens )
{
    // can't do anything w/o the group
    if ( !G ) return GEltGrid();
    // we must reverse the generators list because generateStructuredSubgroup behaves
    // backwards of what Cayley diagrams would like it to
    if ( !gens.size() ) gens = G->generatorlists[0].copy();
    /*
    GEltList generators( gens.size() );
    for ( uint i = 0 ; i < gens.size() ; i++ ) generators[i] = gens[gens.size() - 1 - i];
    */
    // if there are none of those, it must be the trivial group:
    if ( !gens.size() ) {
        GEltList size( 1 );  size[0] = 1;
        GEltGrid result( size );
        size[0] = 0;  result[size] = 0;
        //qDebug( "must be trivial group; returning " + result.toString() );
        return result;
    }
    
    GEltList data;
    GEltList dims;
    G->group.generateStructuredSubgroup( gens, data, dims );
    GEltGrid result( dims, data );
    //qDebug( elt_list_to_string( generators ) + " ==> " + result.toString() );
    return result;
#if 0
    // if there is only one, it must be a cyclic group; that's easy:
    if ( generators.size() == 1 ) {
        GEltList reslist = G->group.orbitOf( generators[0] );
        GEltList size( 1 );  size[0] = reslist.size();
        GEltGrid result( size );
        for ( uint i = 0 ; i < reslist.size() ; i++ ) {
            size[0] = i;
            result[size] = reslist[i];
        }
        //qDebug( "returning this grid: " + result.toString() );
        return result;
    }
    // first generate grid from generators 1 through size-1
    //qDebug( "supposed to use these gens: " + elt_list_to_string( generators ) );
    GEltList prefix = generators.copy();
    prefix.resize( prefix.size() - 1 );
    //qDebug( "but recurring only with these for now: " +
    //        elt_list_to_string( prefix ) );
    GEltGrid subgrid = generateOrbitGrid( G, prefix );
    // now up its number of dimensions by sticking a 1 on the front
    GEltList newdims( subgrid.numDimensions() + 1 );
    newdims[0] = 1;
    for ( uint i = 0 ; i < subgrid.numDimensions() ; i++ )
        newdims[i + 1] = subgrid.dimension( i );
    subgrid.redimension( newdims );
    // now multiply that whole grid repeatedly by generator 0 until exhausted
    GEltList subgroup = G->group.subgroupGenBy( generators );
    GEltGrid subgridcopy( subgrid );
    uint max = subgridcopy.flatsize();
    GEltGrid firstTier( subgridcopy );
    do {
        /*
        qDebug( QString( "walking from tier %1 using generator %2" )
                .arg( firstTier.toString() )
                .arg( generators[generators.size() - 1] ) );
        */
        do {
            for ( uint i = 0 ; i < max ; i++ )
                subgridcopy.flatlookup( i ) = G->group.op(
                    subgridcopy.flatlookup( i ), generators[generators.size() - 1] );
            subgrid.concat( subgridcopy );
            //qDebug( "\tgot: " + subgridcopy.toString() );
        } while ( !firstTier.contains( G->group.op(
            subgridcopy.flatlookup( 0 ), generators[generators.size() - 1] ) ) );
        // now if you're not done, find a new element and start over until you are done
        if ( subgrid.flatsize() < subgroup.size() ) {
            GElt newelt;
            for ( uint i = 0 ; i < subgroup.size() ; i++ ) {
                if ( !subgrid.contains( subgroup[i] ) ) {
                    newelt = subgroup[i];
                    break;
                }
            }
            GElt changer = G->group.op( G->group.inv( subgridcopy.flatlookup( 0 ) ),
                                        newelt );
            for ( uint i = 0 ; i < max ; i++ )
                subgridcopy.flatlookup( i ) = G->group.op( subgridcopy.flatlookup( i ),
                                                           changer );
            //qDebug( "the grid " + subgrid.toString() +
            //        " was not all of the subgroup " +
            //        elt_list_to_string( subgroup ) +
            //        ", so I extended as follows:" );
            subgrid.concat( subgridcopy );
            firstTier = GEltGrid( subgridcopy );
            //qDebug( "changed tier to " + firstTier.toString() );
            //qDebug( "\t" + subgrid.toString() );
        }
    } while ( subgrid.flatsize() < subgroup.size() );
    //GEltList tmp = G->group.subgroupGenBy( generators );
    //qDebug( "after applying first gen that became: " + subgrid.toString() );
    return subgrid;
#endif
}

void CDLargeView::layOutOrbitGrid ( GEltGrid grid, Q3MemArray<uint> axes,
                                    GEltList orderperm, Q3PtrList<CDNode> nodes,
                                    GLSceneNode* insideThis,
                                    GEltList index, uint depth )
{
    // this assumes we have one sphere for each element of the group already created
    // and sitting in the nodes list, in order (GElt g goes with sphere nodes[g]).
    if ( !depth ) {
        // initialize everything, first time only, at top of recursion tree
        orderperm = perm_inverse( orderperm );
        index.fill( 0, axes.size() );
        /*
        qDebug( QString( "\t\t\t\t\t\t\t\t" ).left( depth ) +
                QString( "layOutOrbitGrid( grid = %1, axes = %2, ordr = %3, ..., "
                         "index = %4, depth = %5 )" )
                .arg( grid.toString() ).arg( elt_list_to_string( axes ) )
                .arg( elt_list_to_string( orderperm ) )
                .arg( elt_list_to_string( index ) ).arg( depth ) );
        */
    }
    if ( depth >= orderperm.size() ) { // >= instead of == just for paranoia
        /*
        qDebug( QString( "\t\t\t\t\t\t\t\t" ).left( depth ) +
                QString( "placing nodes.at( grid[index] ) == nodes.at( %1 )" )
                .arg( grid[index] ) );
        */
        insideThis->addChild( nodes.at( grid[index] ) );
    } else {
        uint dim = orderperm[orderperm.size() - 1 - depth]; // OK index b/c above paranoia
        uint size = grid.dimension( dim );
        for ( index[dim] = 0 ; index[dim] < size ; index[dim]++ ) {
            /*
            qDebug( QString( "\t\t\t\t\t\t\t\t" ).left( depth ) +
                    QString( "looping dim %1, axis %2, index %3" )
                    .arg( dim ).arg( axes[orderperm[depth]] )
                    .arg( elt_list_to_string( index ) ) );
            */
            CDAxisWrapper* tmp = new CDAxisWrapper( insideThis );
            tmp->placeMe( axes[orderperm[depth]], index[dim], size );
            layOutOrbitGrid( grid, axes, orderperm, nodes, tmp, index, depth + 1 );
        }
    }
#if 0
    if ( index.size() > axes.size() ) return; // paranoia
    if ( index.size() == axes.size() ) {
        insideThis->addChild( nodes.at( grid[index] ) );
    } else {
        GEltList longerIndex = index.copy();
        longerIndex.resize( longerIndex.size() + 1 );
        for ( uint i = 0 ; i < grid.dimension( index.size() ) ; i++ ) {
            CDAxisWrapper* tmp = new CDAxisWrapper( insideThis );
            tmp->placeMe( axes[index.size()], i,
                          grid.dimension( index.size() ) );
            longerIndex[index.size()] = i;
            layOutOrbitGrid( grid, axes, orderperm, nodes, tmp, longerIndex );
        }
    }
#endif
}

GLModelView* CDLargeView::quickviewer = 0;
GLSceneNode* CDLargeView::quickdiagram = 0;
GLSceneNode* CDLargeView::quicknodesholder = 0;
GLSceneNode* CDLargeView::quickarrowsholder = 0;
QPixmap CDLargeView::quickPixmap ( int w, int h, GroupFile* gf, int index )
{
    //qDebug( "" );
    //qDebug( "Working on " + gf->getPrimaryName().text );
    generateOrbitGrid( gf, GEltList() );

    if ( w < 0 ) w = 30;
    if ( h < 0 ) h = 30;

    if ( !quickviewer ) {
        quickviewer = new GLModelView();
        quickviewer->setLineWidth( 1.0 );
        quickviewer->setClearColor( 0.94, 0.78, 0.78 );
        quickviewer->setUseFog();
        quickdiagram = new GLSceneNode();
        quickdiagram->addChild( quicknodesholder = new CDAxisWrapper() );
        quicknodesholder->translate( -1.25, -1.25, -1.25 );
        quicknodesholder->scale( 2.5, 2.5, 2.5 );
        quickdiagram->addChild( quickarrowsholder = new GLSceneNode() );
        quickviewer->addChild( quickdiagram );
        // don't need lights because not using spheres--see CDNode() ctor w/"simple" param
    }
    GEltList gens = gf->generatorlists[0];

    // this is like what reformDiagram/arrangeNodes/placeCamera does:
    quicknodesholder->children.clear();
    Q3PtrList<CDNode> nodes;
    for ( GElt g = 0 ; g < gf->group.order ; g++ )
        nodes.append( new CDNode( quickdiagram, TRUE ) ); // TRUE = no spheres, just dots
    GEltList arrows;

    bool dim[3];
    dim[0] = dim[1] = dim[2] = FALSE;
    if ( ( index == (int)gf->cays.count() ) || ( index == -1 ) ) {
        GEltGrid grid;
        GEltList axes;
        GEltList orderperm;
        GeneratorControl::niceDiagram( gf, gens, grid, axes, orderperm );
        //orderperm = perm_inverse( orderperm );//happens in layOutOrbitGrid
        layOutOrbitGrid( grid, axes, orderperm, nodes, quicknodesholder );
        for ( uint i = 0 ; i < axes.size() ; i++ ) {
            switch ( axes[i] ) {
                case 0: dim[0] = TRUE; break;
                case 1: dim[1] = TRUE; break;
                case 2: dim[2] = TRUE; break;
                case 3: case 6: dim[0] = dim[1] = TRUE; break;
                case 4: case 7: dim[0] = dim[2] = TRUE; break;
                case 5: case 8: dim[1] = dim[2] = TRUE; break;
            }
        }
        arrows = gens;
    } else {
        for ( GElt g = 0 ; g < gf->group.order ; g++ ) {
            GLdouble x, y, z;
            gf->cays[index].getNodePosition( g, x, y, z );
            quicknodesholder->addChild( nodes.at( g ) );
            nodes.at( g )->setAbsolutePosition( x, y, z );
            nodes.at( g )->setRadius( 0.1 );
            if ( QABS( x ) > 0.1 ) dim[0] = TRUE;
            if ( QABS( y ) > 0.1 ) dim[1] = TRUE;
            if ( QABS( z ) > 0.1 ) dim[2] = TRUE;
        }
        arrows = gf->cays[index].getArrows();
    }
    if ( dim[0] && dim[1] && dim[2] ) {
        quickviewer->setCameraPosition( 2.5, 2.5, 2.5 );
    } else if ( !dim[2] ) {
        quickviewer->setCameraPosition( 0.0, 0.0, 3.125 );
    } else if ( !dim[1] ) {
        quickviewer->setCameraPosition( 0.0, 3.125, 0.0 );
    } else {
        quickviewer->setCameraPosition( 3.125, 0.0, 0.0 );
    }
    calibrateFog( quickviewer, "15" );

    // here we accomplish what arrangeArrows does:
    quickarrowsholder->children.clear();
    QColor c;
    for ( uint i = 0 ; i < arrows.size() ; i++ ) {
        c.setHsv( i * 360 / arrows.size(), 255, 100 );
        uint o = gf->group.orderOf( arrows[i] );
        for ( GElt g = 0 ; g < gf->group.order ; g++ ) {
            GElt ga = gf->group.op( g, arrows[i] );
            if ( ( o > 2 ) || ( g < ga ) ) {
                CDArrow* a =
                    new CDArrow( nodes.at( g ), nodes.at( ga ), quickdiagram/*, TRUE*/ );
                a->setColor( c.red() / 255.0, c.green() / 255.0, c.blue() / 255.0, 1.0 );
                a->setOrder( o );
                a->decideArcing( nodes );
                quickarrowsholder->addChild( a );
            }
        }
    }
    quickviewer->resize( w, h );
    QPixmap p = quickviewer->renderPixmap();
    if ( ( p.width() != w ) || ( p.height() != h ) ) {
        int mw = QMIN( w, p.width() );
        int mh = QMIN( h, p.height() );
        QPixmap q( mw, mh );
        QPainter paint( &q );
        paint.drawPixmap(
            0, 0, p, p.width() / 2 - mw / 2, p.height() / 2 - mh / 2, mw, mh );
        p = q;
    }
    return p;
}

QPixmap CDLargeView::currentPixmap ()
{
    return viewer->renderPixmap();
}

bool CDLargeView::event ( QEvent *event )
{
    // stole this code right from the Qt4 ToolTips example
    // then modified it to fit my OpenGL situation
    if ( event->type() != QEvent::ToolTip )
        return QWidget::event( event );
    QHelpEvent *helpEvent = static_cast<QHelpEvent*>(event);
    QPoint p = helpEvent->pos();
    bool overflow;
    Q3ValueList<GLSceneNode*> hits =
        viewer->pickClick( (GLdouble)p.x(), (GLdouble)p.y(), &overflow );
    if ( !overflow ) {
        // sufficiently few things under mouse pointer, so find first _node_ there:
        QStringList elementNames;
        QStringList cosetNames;
        for ( int i = 0 ; i < hits.count() ; i++ ) {
            int eltloc = -1;
            int cosloc = -1;
            for ( uint j = 0 ; j < nodes.count() ; j++ ) {
                if ( ( eltloc == -1 ) && nodes.at( j )
                  && nodes.at( j )->containsSphere( hits[i] ) ) eltloc = j;
                if ( ( cosloc == -1 ) && nodes.at( j )
                  && nodes.at( j )->containedByChunk( hits[i] ) ) cosloc = j;
            }
            if ( eltloc > -1 )
                elementNames <<
                    ( "<nobr>" + groupfile->getRepresentation( (GElt)eltloc ).HTML
                      + "</nobr>" );
            if ( cosloc > -1 )
                cosetNames <<
                    ( "<nobr>" + groupfile->getRepresentation( (GElt)cosloc ).HTML
                      + "</nobr>" );
        }
        if ( elementNames.count() ) {
            if ( elementNames.count() > 1 ) {
                elementNames[0] = "<b>In front:</b> " + elementNames[0];
                if ( elementNames.count() > 2 ) {
                    QStringList::iterator one = elementNames.begin();
                    one++;
                    elementNames.insert( one, "<b>Others behind:</b>" );
                    for ( int i = 2 ; i < elementNames.count() ; i++ )
                        elementNames[i] = " &nbsp; &nbsp; " + elementNames[i];
                } else {
                    elementNames[1] = "<b>Behind:</b> " + elementNames[1];
                }
            }
            QToolTip::showText( helpEvent->globalPos(),
            //useMe->tip( QRect( p.x() - 2, p.y() - 2, 5, 5 ),
                        "<nobr>" + elementNames.join( "</nobr><br>\n<nobr>" ) +
                        "</nobr>" );
        } else if ( cosetNames.count() ) {
            if ( cosetNames.count() > 1 ) {
                cosetNames[0] = "<b>In front:</b> " + cosetNames[0] + chunkCosetName;
                if ( cosetNames.count() > 2 ) {
                    QStringList::iterator one = cosetNames.begin();
                    one++;
                    cosetNames.insert( one, "<b>Others behind:</b>" );
                    for ( int i = 2 ; i < cosetNames.count() ; i++ )
                        cosetNames[i] = " &nbsp; &nbsp; "
                                      + cosetNames[i] + chunkCosetName;
                } else {
                    cosetNames[1] = "<b>Behind:</b> " + cosetNames[1] + chunkCosetName;
                }
            } else {
                cosetNames[0] += chunkCosetName;
            }
            QToolTip::showText( helpEvent->globalPos(),
            //useMe->tip( QRect( p.x() - 2, p.y() - 2, 5, 5 ),
                        "<nobr>" + cosetNames.join( "</nobr><br>\n<nobr>" ) +
                        "</nobr>" );
        }
    }
    return QWidget::event( event );
}



CDLargeControl::CDLargeControl ( GEVLarge* container, QWidget* parent, const char* name )
    : GEVLargeWidget( container, parent, name )
{
    addProperty( "zoom", "250" );
    addProperty( "line", "3" );
    addProperty( "node", "100" );
    addProperty( "fogl", "15" );
    addProperty( "fogs", "Y" );
    addProperty( "head", "100" );
    addProperty( "labs", "Y" );
    addProperty( "labz", "12" );

    Q3VBoxLayout* mainLayout = new Q3VBoxLayout( this, 0, 6 );

    QTabWidget* tabwidget = new QTabWidget( this );
    mainLayout->addWidget( tabwidget );

    // set up the tab page with the subset list box
    listBox = new SubsetListBox( container, 0, this );
    listBox->setHighlightTypes( QStringList()
        << "img/hightype-sphere-node.jpg:Node color"
        << "img/hightype-sphere-ring.jpg:Ring around node"
        << "img/hightype-sphere-square.jpg:Square around node" );
    if ( container ) container->addLargeWidget( listBox, TRUE );
    tabwidget->addTab( listBox, "&Subsets" );

    // set up the tab page with all the 3D controls
    ModelViewControl* mvc = new ModelViewControl( container, this );
    if ( container ) container->addLargeWidget( mvc, TRUE );
    labelCheckBox = new QCheckBox( "Show labels of this size:", mvc );
    labelCheckBox->setChecked( readProperty( "labs" ) == "Y" );
    connect( labelCheckBox, SIGNAL(clicked()), this, SLOT(labelStatusChanged()) );
    labelSlider = new QSlider( 10, 40, 1, readProperty( "labz" ).toInt(),
                               Qt::Horizontal, mvc );
    labelSlider->setTracking( FALSE );
    connect( labelSlider, SIGNAL(valueChanged(int)), this, SLOT(labelSizeChanged(int)) );
    QLabel* arrowhead_label = new QLabel( "Arrowhead placement:", mvc );
    headSlider = new QSlider( 10, 100, 10, readProperty( "head" ).toInt(),
                              Qt::Horizontal, mvc );
    headSlider->setTracking( FALSE );
    connect( headSlider, SIGNAL(valueChanged(int)), this, SLOT(headPosChanged(int)) );
    tabwidget->addTab( mvc, "&View" );

    // set up the blank tab that will contain the CD-specific controls
    gTable = new GeneratorControl( container, 0, this );
    if ( container ) container->addLargeWidget( gTable, TRUE );
    tabwidget->addTab( gTable, "&Diagram" );

    // set up the Reset and Help buttons on the bottom
    Q3HBoxLayout* buttonLayout = new Q3HBoxLayout( mainLayout );

    QPushButton* helpButton = new QPushButton( "Help", this ); 
    buttonLayout->addWidget( helpButton );
    connect( helpButton, SIGNAL(clicked()), this, SLOT(showHelp()) );

    QPushButton* resetButton = new QPushButton( "Reset", this ); 
    buttonLayout->addWidget( resetButton );
    connect( resetButton, SIGNAL(clicked()), this, SIGNAL(reset()) );

    GEWhatsThis::setup( helpButton, "largeWindow-helpButton" );
    GEWhatsThis::setup( resetButton, "largeWindow-resetButton" );
    GEWhatsThis::setup( arrowhead_label, "cd-headPosSlider" );
    GEWhatsThis::setup( headSlider, "cd-headPosSlider" );
    GEWhatsThis::setup( labelSlider, "cd-labelSizeControls" );
    GEWhatsThis::setup( labelCheckBox, "cd-labelSizeControls" );
}

void CDLargeControl::showHelp ()
{
    GEHelpWindow::showHelp( "rf-um-cd-options.html" );
}

void CDLargeControl::set ( QString key, QString oldvalue, QString value, QString desc )
{
    if ( key == "head" ) {
        headSlider->setValue( value.toInt() );
    } else if ( key == "labs" ) {
        labelCheckBox->setChecked( value == "Y" );
    } else if ( key == "labz" ) {
        labelSlider->setValue( value.toInt() );
    }
    GEVLargeWidget::set( key, oldvalue, value, desc );
}

void CDLargeControl::groupFileChanged ( GroupFile* gf )
{
    GEVLargeWidget::groupFileChanged( gf );
    // extend
}

void CDLargeControl::headPosChanged( int toThis )
{
    change( "head", QString::number( toThis ), "change arrowhead position" );
}

void CDLargeControl::labelSizeChanged ( int toThis )
{
    change( "labz", QString::number( toThis ), "change label size" );
}

void CDLargeControl::labelStatusChanged ()
{
    bool on = labelCheckBox->isChecked();
    change( "labs", on ? "Y" : "N", on ? "turn labels on" : "turn labels off" );
}


CDLarge::CDLarge ( QWidget* parent, const char* name )
    : GEVLarge( parent, name )
{
    addLargeWidget( view = new CDLargeView( this, splitter ), TRUE );
    addLargeWidget( new CDLargeControl( this, splitter ), FALSE );
}

QSize CDLarge::sizeHint () const
{
    return QSize( 750, 400 );
}

QPixmap CDLarge::currentPixmap ()
{
    ensurePolished();
    return view->currentPixmap();
}

QPoint CDLarge::elementLocationPercent ( GElt e )
{
    return view->elementLocationPercent( e );
}


CDVisualizer::CDVisualizer ( GroupFile* gf )
    : GEVisualizer( gf )
{
    processChange( "indx", "0", QString::null ); // be sure I have a non-null index
}

QString CDVisualizer::typeName ()
{
    return "CayleyDiagram";
}

void CDVisualizer::createLarge ()
{
    large = new CDLarge();
}

void CDVisualizer::createSmall ()
{
    smallv = new CDSmall();
}

QPixmap CDVisualizer::quickPixmap ( int w, int h, int index )
{
    return CDLargeView::quickPixmap( w, h, groupfile, index );
}

int CDVisualizer::numIndices ()
{
    return groupfile->cays.count() + 1;
}

QStringList CDVisualizer::orderedKeys ()
{
    QStringList keys = GEVisualizer::orderedKeys();
    // we need to do the node-location properties last so that a diagram exists;
    // otherwise we're trying to modify the locations of nonexistant nodes;
    // same goes for arrow arcings
    int pos = keys.count() - 1;
    for ( int bigpos = keys.count() - 1 ; bigpos >= 0 ; bigpos-- ) {
        if ( ( keys[bigpos][0] == 'N' ) || ( keys[bigpos][0] == 'A' ) ) {
            bool ok;
            keys[bigpos].mid( 1 ).toUInt( &ok );
            if ( ok ) {
                if ( bigpos < pos ) { 
                    QString tmp = keys[bigpos];
                    keys[bigpos] = keys[pos];
                    keys[pos] = tmp;
                }       
                pos--;  
            }       
        }       
    }
    return keys;
}


CDSmall::CDSmall ()
{
}

Q3PtrList<QAction> CDSmall::actionsToExport ()
{
    return Q3PtrList<QAction>(); // change me later
}


/*
CDTipFlagger::CDTipFlagger ( QWidget* onThis, CDLargeView* forThis )
    : QToolTip( onThis ), forThis( forThis )
{
}

void CDTipFlagger::tip ( const QRect& rect, const QString& text )
{
    QToolTip::tip( rect, text ); // just publicizes parent's method; like "using"
}

void CDTipFlagger::maybeTip ( const QPoint& p )
{
    forThis->considerTooltip( p, this );
}
*/


