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


#include "symmetryobject.h"
#include "gehelpwindow.h"
#include "gewhatsthis.h"
#include "modelviewwidgets.h"
#include <qlayout.h>
#include <qpixmap.h>
#include <q3vbox.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qgl.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qimage.h>
#include <q3dragobject.h>
#include <qapplication.h>
#include <qclipboard.h>
//Added by qt3to4:
#include <Q3PtrList>
#include <Q3VBoxLayout>
//Added by me to help porting:
#include <q3mimefactory.h>

#include <QDebug>


GLSelector::GLSelector ( GLSceneNode* parent )
    : GLSceneNode( parent )
{
    children.setAutoDelete( FALSE );
    potentialChildren.setAutoDelete( TRUE );
}

void GLSelector::selectChild ( int index ) {
    children.clear();
    if ( ( index > -1 ) && ( index < int( potentialChildren.count() ) ) )
        addChild( potentialChildren.at( index ) );
}

void GLSelector::addPotentialChild ( GLSceneNode* pch )
{
    potentialChildren.append( pch );
}

GLSceneNode* GLSelector::potentialChild ( int index )
{
    return ( uint( index ) < potentialChildren.count() ) ?
           potentialChildren.at( index ) : 0;
}

void GLSelector::clear ()
{
    potentialChildren.clear();
}


OSLargeView::OSLargeView ( GEVLarge* container, QWidget* parent, const char* name )
    : GEVLargeWidget( container, parent, name )
{
    Q3VBoxLayout* layout = new Q3VBoxLayout( this );

    addProperty( "zoom", "250" );
    addProperty( "line", "3" );
    addProperty( "node", "100" );
    addProperty( "fogl", "15" );
    addProperty( "fogs", "Y" );

    // create and set up OpenGL model view widget
    viewer = new GLModelView( this );
    viewer->setCameraPosition( 2.5, 2.5, 2.5 );
    viewer->setLineWidth( 3.0 );
    viewer->setClearColor( 0.78, 0.94, 0.78 );
    viewer->setUseFog();
    calibrateFog( viewer, readProperty( "fogl" ) );
    viewer->setMinimumSize( 200, 200 );
    layout->addWidget( viewer );
    //connect( viewer, SIGNAL(startDrag(QPoint)), this, SLOT(objectStartDrag()) );

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

    // create holder for all symmetry objects for our groupfile
    allSOs = new GLSelector();
    allSOs->name = "AllSymmetryObjects";
    viewer->addChild( allSOs );

    GEWhatsThis::setup( viewer, "os-view" );
}

/*
void OSLargeView::objectStartDrag ()
{
    QImage i = viewer->renderPixmap().convertToImage();
    qDebug( "drag: saving a copy to /Users/nathancarter/Desktop/GEqDebug.png..." );
    i.save( "/Users/nathancarter/Desktop/GEqDebug.png", "PNG" );
    qDebug( "done saving copy" );
    ( new QImageDrag( i.copy(), this ) )->drag();
}
*/

void OSLargeView::copyImage ()
{
    currentP = viewer->renderPixmap();
    //qDebug( "drag: saving a copy to /Users/nathancarter/Desktop/GEqDebug.png..." );
    //currentP.convertToImage().save( "/Users/nathancarter/Desktop/GEqDebug.png", "PNG" );
    //qDebug( "done saving copy" );
    QApplication::clipboard()->setPixmap( currentP, QClipboard::Clipboard );
}

Q3PtrList<QAction> OSLargeView::actionsToExport ()
{
    Q3PtrList<QAction> result;

    QAction* cpa = new QAction( QIcon( qPixmapFromMimeSource( "editcopy.png" ) ),
                                "Copy", this );
    cpa->setShortcut( QKeySequence( "Ctrl+C" ) );
    connect( cpa, SIGNAL(activated()), this, SLOT(copyImage()) );
    result.append( cpa );

    return result;
}

void OSLargeView::calibrateFog ( GLModelView* mv, QString level )
{
    GLdouble fogStart = mv->distanceFromEye( 0.0, 0.0, 0.0 );
    mv->setFogValues( 0.78, 0.94, 0.78,
                      fogStart - 1.0, fogStart + ( 60 - level.toInt() ) / 10.0 );
}

void OSLargeView::set ( QString key, QString oldvalue, QString value, QString desc )
{
    if ( key == "node" ) {
        ( (SymmetryObject*)( allSOs->potentialChild( readProperty( "indx" ).toInt() ) ) )
            ->changeBaseRadius( value.toInt() / 100.0 );
    } else if ( key == "line" ) {
        viewer->setLineWidth( double( readProperty( "line" ).toInt() ) );
    } else if ( key == "zoom" ) {
        double dist = ( 500 - value.toInt() ) / 100.0;
        viewer->setCameraPosition( dist, dist, dist );
        calibrateFog( viewer, readProperty( "fogl" ) );
    } else if ( key == "fogl" ) {
        calibrateFog( viewer, readProperty( "fogl" ) );
    } else if ( key == "fogs" ) {
        viewer->setUseFog( value == "Y" );
    } else if ( key == "indx" ) {
        allSOs->selectChild( value.toUInt() );
    }
    GEVLargeWidget::set( key, oldvalue, value, desc );
}

void OSLargeView::groupFileChanged ( GroupFile* gf )
{
    GEVLargeWidget::groupFileChanged( gf );
    allSOs->clear();
    for ( int i = 0 ; i < gf->symms.count() ; i++ )
        allSOs->addPotentialChild( gf->symms[i].copy() );
}

void OSLargeView::updateContent ( QStringList )
{
    viewer->updateGL();
}

GLModelView* OSLargeView::quickviewer = 0;
GLSceneNode* OSLargeView::quickholder = 0;
QPixmap OSLargeView::quickPixmap ( int w, int h, GroupFile* gf, int index )
{
    if ( w < 0 ) w = 30;
    if ( h < 0 ) h = 30;
    if ( gf->symms.count() ) {
        if ( !quickviewer ) {
            quickviewer = new GLModelView();
            quickviewer->setCameraPosition( 3.00, 0.5, 0.5 );
            quickviewer->setLineWidth( 1.0 );
            quickviewer->setClearColor( 0.78, 0.94, 0.78 );
            quickviewer->setUseFog();
            calibrateFog( quickviewer, "30" );
            GLLight* light;
            QGLContext* c = ( QGLContext* )( quickviewer->context() );
            quickviewer->addFixedChild( light = new GLLight( 1.0, 1.0, 1.0, 0.0, c ) );
            light->setDiffuse( 1.0, 1.0, 1.0, 1.0 );
            quickholder = new GLSceneNode();
            quickholder->name = "osholder";
            quickholder->children.setAutoDelete( FALSE );
            quickviewer->addChild( quickholder );
        }
        quickholder->children.clear();
        quickholder->addChild( &gf->symms[index] );
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
    } else {
        QPixmap result( w, h );
        QPainter p( &result );
        p.setPen( Qt::NoPen );
        p.setBrush( Qt::white );
        p.drawRect( 0, 0, w, h );
        p.setPen( Qt::black );
        p.drawText( 0, 0, w, h, Qt::AlignCenter, "none" );
        return result;
    }
}

QPixmap OSLargeView::currentPixmap ()
{
    return viewer->renderPixmap();
}


OSLargeControl::OSLargeControl ( GEVLarge* container, QWidget* parent, const char* name )
    : GEVLargeWidget( container, parent, name )
{
    addProperty( "zoom", "250" );
    addProperty( "line", "3" );
    addProperty( "node", "100" );
    addProperty( "fogl", "15" );
    addProperty( "fogs", "Y" );

    Q3VBoxLayout* mainLayout = new Q3VBoxLayout( this, 6, 11 );
    mainLayout->setAutoAdd( TRUE );

    QLabel* obj_label = new QLabel( "View this symmetry object:", this );
    pickSO = new QComboBox( this );
    connect( pickSO, SIGNAL(activated(int)), this, SLOT(indexChanged(int)) );

    ModelViewControl* mvc = new ModelViewControl( container, this );
    if ( container ) container->addLargeWidget( mvc, TRUE );

    Q3HBox* buttonLayout = new Q3HBox( this );
    QPushButton* helpButton = new QPushButton( "Help", buttonLayout ); 
    QPushButton* resetButton = new QPushButton( "Reset", buttonLayout ); 
    connect( helpButton, SIGNAL(clicked()), this, SLOT(showHelp()) );
    connect( resetButton, SIGNAL(clicked()), this, SIGNAL(reset()) );

    GEWhatsThis::setup( obj_label, "os-chooseObject" );
    GEWhatsThis::setup( pickSO, "os-chooseObject" );
    GEWhatsThis::setup( helpButton, "largeWindow-helpButton" );
    GEWhatsThis::setup( resetButton, "largeWindow-resetButton" );
}

void OSLargeControl::showHelp ()
{
    GEHelpWindow::showHelp( "rf-um-os-options.html" );
}

void OSLargeControl::set ( QString key, QString oldvalue, QString value, QString desc )
{
    if ( key == "indx" ) pickSO->setCurrentItem( value.toInt() );
    GEVLargeWidget::set( key, oldvalue, value, desc );
}

void OSLargeControl::groupFileChanged ( GroupFile* gf )
{
    GEVLargeWidget::groupFileChanged( gf );
    pickSO->clear();
    for ( int i = 0 ; i < gf->symms.count() ; i++ )
        pickSO->insertItem( gf->symms[i].getName() );
}

void OSLargeControl::indexChanged ( int index )
{
    change( "indx", QString::number( index ),
            "switch to viewing " + groupfile->symms[index].getName() );
}


OSLarge::OSLarge ( QWidget* parent, const char* name )
    : GEVLarge( parent, name )
{
    addLargeWidget( view = new OSLargeView( this, splitter ), TRUE );
    addLargeWidget( new OSLargeControl( this, splitter ), FALSE );
}

QSize OSLarge::sizeHint () const
{
    return QSize( 600, 400 );
}

QPixmap OSLarge::currentPixmap ()
{
    ensurePolished();
    return view->currentPixmap();
}


OSVisualizer::OSVisualizer ( GroupFile* gf )
    : GEVisualizer( gf )
{
    processChange( "indx", "0", QString::null ); // be sure I have a non-null index
}

QString OSVisualizer::typeName ()
{
    return "SymmetryObject";
}

void OSVisualizer::createLarge ()
{
    large = new OSLarge();
}

void OSVisualizer::createSmall ()
{
    smallv = new OSSmall();
}

QPixmap OSVisualizer::quickPixmap ( int w, int h, int index )
{
    return OSLargeView::quickPixmap( w, h, groupfile, index );
}

int OSVisualizer::numIndices ()
{
    return groupfile->symms.count();
}


OSSmall::OSSmall ()
{
}

Q3PtrList<QAction> OSSmall::actionsToExport ()
{
    return Q3PtrList<QAction>(); // change me later
}

bool OSSmall::hasElementLocations ()
{
    return FALSE;
}


