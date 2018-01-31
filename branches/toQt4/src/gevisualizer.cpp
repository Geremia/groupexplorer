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


#include "gevisualizer.h"
#include "gevisualizerlist.h"
#include "gesheet.h"
#include "gevlarge.h"
#include "geml.h"
#include "groupfile.h"
#include "grouptable.h"
#include <qimage.h>
#include <q3url.h>
#include <qtimer.h>
//Added by qt3to4:
#include <Q3PtrList>
#include <QPixmap>


GEVLargeWidget::GEVLargeWidget ( GEVLarge* container, QWidget* parent, const char* name )
    : QWidget( parent, name ), groupfile( 0 ), container( container )
{
    elementNames.setAutoDelete( TRUE );
    addProperty( "indx", "0" );
}

Q3PtrList<QAction> GEVLargeWidget::actionsToExport ()
{
    return Q3PtrList<QAction>(); // empty by default
}

QString GEVLargeWidget::readProperty ( QString key )
{
    QString lookup = mycopy[key];
    if ( lookup.isNull() ) return getDefault( key );
    return lookup;
}

void GEVLargeWidget::writeProperty ( QString key, QString value )
{
    if ( value.isNull() ) {
        mycopy.remove( key );
    } else {
        mycopy[key] = value;
    }
}

void GEVLargeWidget::change ( QString key, QString value, QString desc )
{
    processChange( key, value, desc );
    emit changed( key, value, desc );
}

void GEVLargeWidget::set ( QString key, QString oldvalue, QString value, QString )
{
#define DEBUG_GEVLW_SET 0
    // descendants should call this guy even if they also handle the key!
    QString start = readProperty( "indx" ) + " ";
    if ( key != "indx" ) {
        // save this option under this indx, so later it can be retrieved if needed
        // (but only save it if I'm in charge of it)
        if ( keys().contains( key ) && !optionsToForget.contains( key ) ) {
            optionsMemory[start + key] = value;
#if DEBUG_GEVLW_SET
            qDebug( "%s", QString( "%1::set( \"%2\", \"%3\", \"%4\", ... ) saves "
                                   "optionsMemory[\"%5\"] = \"%6\"" )
                          .arg( className() ).arg( key ).arg( oldvalue ).arg( value )
                          .arg( start + key ).arg( value ).latin1() );
#endif
        }
    } else if ( oldvalue != value ) {
        // this is the "later" spoken of in the previous comment.
        blockSignals( TRUE );
        QStringList k = optionsMemory.keys();
        for ( int i = 0 ; i < k.count() ; i++ ) {
            if ( k[i].startsWith( start ) ) {
                QString rest = k[i].mid( start.length() );
                if ( !optionsToForget.contains( rest ) ) {
                    change( rest, optionsMemory[k[i]], QString::null );
#if DEBUG_GEVLW_SET
                    qDebug( "%s", QString( "%1::set( \"%2\", \"%3\", \"%4\", ... ) "
                                           "retrieves \"%5\" = \"%6\"" )
                                  .arg( className() ).arg( key ).arg( oldvalue )
                                  .arg( value ).arg( rest ).arg( optionsMemory[k[i]] )
                                  .latin1() );
#endif
                }
            }
        }
        k = keys();
        for ( int i = 0 ; i < k.count() ; i++ ) {
            if ( ( k[i] != "indx" ) && !optionsToForget.contains( k[i] ) ) {
                change( k[i], getDefault( k[i] ), QString::null );
#if DEBUG_GEVLW_SET
                qDebug( "%s", QString( "%1::set( \"%2\", \"%3\", \"%4\", ... ) retrieves "
                                       "default \"%5\" = \"%6\"" )
                              .arg( className() ).arg( key ).arg( oldvalue ).arg( value )
                              .arg( k[i] ).arg( getDefault( k[i] ) ).latin1() );
#endif
            }
        }
        blockSignals( FALSE );
    }
    // now continue processing as usual
    if ( key.startsWith( "hig" ) ) {
        bool ok;
        int idx = key.mid( 3 ).toInt( &ok );
        if ( ok ) {
            while ( highlightCodes.count() <= idx ) highlightCodes << "";
            highlightCodes[idx] = value;
            //qDebug( "GEVLargeWidget::set( \""+key+"\", \""+value+"\", \"...\" ) "
            //        "puts highlightCodes["+QString::number(idx)+"] = "+value );
        }
    }
}

bool GEVLargeWidget::updateHighlighting ( QStringList changedKeys )
{
    bool needToChangeHighlighting = FALSE;
    for ( int i = 0 ; i < changedKeys.count() ; i++ ) {
        if ( changedKeys[i].startsWith( "hig" ) ) {
            bool ok;
            changedKeys[i].mid( 3 ).toUInt( &ok );
            if ( ok ) {
                needToChangeHighlighting = TRUE;
                break;
            }
        }
    }
    if ( needToChangeHighlighting ) {
        uint o = groupfile->group.order;
        numParts.fill( 0, highlightCodes.size() );
        highValues.fill( 0, highlightCodes.size() * o );
        // highValues[o * highlightIndex + elementIndex] is how we index this array
        for ( int i = 0 ; i < highlightCodes.size() ; i++ ) {
            if ( highlightCodes[i].isEmpty() ) {
                // do nothing -- fill() calls above did it
            } else if ( highlightCodes[i].contains( "," ) ) {
                GEltListList tmp = code_to_elt_set_list( highlightCodes[i] );
                numParts[i] = tmp.count();
                for ( int j = 0 ; j < tmp.count() ; j++ )
                    for ( uint k = 0 ; k < tmp[j].size() ; k++ )
                        highValues[o * i + tmp[j][k]] = j;
            } else {
                GEltList tmp = code_to_elt_set( highlightCodes[i] );
                for ( uint j = 0 ; j < tmp.size() ; j++ )
                    highValues[o * i + tmp[j]] = 1;
            }
        }
    }
    return needToChangeHighlighting;
}

bool GEVLargeWidget::highlightingTypeUsed ( uint htype )
{
    if ( htype >= numParts.size() ) return FALSE;
    if ( numParts[htype] ) return TRUE;
    for ( uint i = 0 ; i < groupfile->group.order ; i++ )
        if ( highValues[groupfile->group.order * htype + i] ) return TRUE;
    return FALSE;
}

bool GEVLargeWidget::isElementHighlightedBy ( GElt elt, uint htype )
{
#if 0
    qDebug( QString( "GEVLargeWidget::isElementHighlightedBy( %1, %2 ) "
                     "with numParts.size() == %3 and "
                     "highValues[%4 * %5 + %6] == %7" )
            .arg( elt ).arg( htype ).arg( numParts.size() )
            .arg( groupfile->group.order ).arg( htype ).arg( elt )
            .arg( htype >= numParts.size() ?
                  "can't compute" :
                  QString( "%1" )
                  .arg( highValues[groupfile->group.order * htype + elt] ) ) );
#endif
    if ( htype >= numParts.size() ) return FALSE;
    return ( numParts[htype] || highValues[groupfile->group.order * htype + elt] );
}

QColor GEVLargeWidget::elementHighlighting ( GElt elt, uint htype )
{
    if ( numParts[htype] ) {
        return QColor( highValues[groupfile->group.order * htype + elt]
                       * 360 / numParts[htype], 100, 255, QColor::Hsv );
    } else {
        return QColor( htype * 360 / highlightCodes.size(), 100, 255, QColor::Hsv );
    }
}

void GEVLargeWidget::updateContent ( QStringList )
{
    // does nothing by default--just override me
}

void GEVLargeWidget::callUpdateContent ()
{
    if ( changedKeys.count() ) updateContent( changedKeys );
    changedKeys.clear();
}

QStringList GEVLargeWidget::keys ()
{
    return propertyKeys;
}

QString GEVLargeWidget::getDefault ( QString key )
{
    int index = propertyKeys.findIndex( key );
    return ( index > -1 ) ? propertyDefaults[index] : QString::null;
}

void GEVLargeWidget::addProperty ( QString key, QString defaultValue, bool remember )
{
    propertyKeys << key;
    propertyDefaults << defaultValue;
    if ( !remember ) optionsToForget << key;
}

void GEVLargeWidget::processChange ( QString key, QString value, QString desc )
{
    QString oldval = readProperty( key );
    if ( oldval != value ) {
        writeProperty( key, value );
        set( key, oldval, readProperty( key ), desc );
        if ( !changedKeys.contains( key ) ) changedKeys << key;
        QTimer::singleShot( 0, this, SLOT(callUpdateContent()) );
    }
}

void GEVLargeWidget::groupFileChanged ( GroupFile* gf )
{
    groupfile = gf;
    elementNames.clear();
    if ( !changedKeys.contains( "groupfile" ) ) changedKeys << "groupfile";
    QTimer::singleShot( 0, this, SLOT(callUpdateContent()) );
}

Q3SimpleRichText* GEVLargeWidget::elementText ( GElt e )
{
    static QFont f = QFont();
    static bool firstTime = TRUE;
    if ( firstTime ) {
        f.setPixelSize( 15 );
        firstTime = FALSE;
    }
    // if they haven't been rendered yet, render them
    if ( elementNames.isEmpty() && groupfile )
        for ( GElt g = 0 ; g < groupfile->group.order ; g++ )
            elementNames.append( new Q3SimpleRichText(
                groupfile->getRepresentation( g ).HTML, f ) );
    return ( e < elementNames.count() ) ? elementNames.at( e ) : 0;
}

QSize GEVLargeWidget::largestElementText ()
{
    int padding = 0;
#ifdef Q_WS_WIN
    padding = 5;
#endif
    QSize result( 0, 0 );
    if ( groupfile )
        for ( GElt g = 0 ; g < groupfile->group.order ; g++ )
            result = result.expandedTo(
                QSize( elementText( g )->width() + padding, elementText( g )->height() ) );
    // In the transition to Qt4, these answers seem to get reported too small.
    // I suspect something is wrong with Q3SimpleRichText (or my use of it? or its interface
    // to the new paint system in Qt4?).
    // So for now I just scale this result back as a workaround, and expect a real fix when
    // all the Q3* aspects of GE are finally removed.
    return QSize( result.width() * 4 / 3, result.height() * 4 / 3 );
}

void GEVLargeWidget::drawElementText ( GElt e, QPainter* paint, QPoint point,
                                       Qt::AlignmentFlag align )
{
    Q3SimpleRichText* srt = elementText( e );
    QPoint topleft = point;
    switch ( align & Qt::AlignHorizontal_Mask )
    {
        case Qt::AlignRight: topleft.setX( topleft.x() - srt->width/*Used*/() ); break;
        case Qt::AlignHCenter: topleft.setX( topleft.x() - srt->width/*Used*/() / 2 );
    }
    switch ( align & Qt::AlignVertical_Mask )
    {
        case Qt::AlignBottom: topleft.setY( topleft.y() - srt->height() ); break;
        case Qt::AlignVCenter: topleft.setY( topleft.y() - srt->height() / 2 );
    }
    srt->draw( paint, topleft.x(), topleft.y(),
               QRect( topleft.x(), topleft.y(), 2 * srt->width(), srt->height() ),
               defaultColorGroup, &seeThroughBrush );
}


GEVSmall::GEVSmall ()
    : Q3CanvasRectangle( 0 ), visualizer( 0 ), groupfile( 0 ),
      lastWidth( -1 ), lastHeight( -1 ), waiting( 0 )
{
    // originally one should have done setUndoStack() here, but I 98% gave that up...
    // I have commented out (but not deleted) all that functionality...it's all in this
    // file; just search for pushUndoCommand through this .cpp file.
    setSelected( FALSE );
    setSize( 200, 200 ); // reasonable default
    lastUpdate.start();
}

void GEVSmall::setSheet ( GESheet* sheet )
{
    setCanvas( sheet );
}

void GEVSmall::setCanvas ( Q3Canvas* c )
{
    Q3CanvasRectangle::setCanvas( c );
    if ( c->inherits( "GESheet" ) ) ( ( GESheet* )c )->noticeNewSmall( this );
}

void GEVSmall::appendXMLTo ( QDomNode n )
{
    QDomElement e = n.ownerDocument().createElement( "visualizer" );
    append_string_node_to( "type", visualizer->typeName(), e );
    append_string_node_to( "group", groupfile->codeName(), e );
    append_string_node_to( "code", visualizer->encode(), e );
    append_string_node_to( "x", QString::number( x() ), e );
    append_string_node_to( "y", QString::number( y() ), e );
    append_string_node_to( "z", QString::number( z() ), e );
    append_string_node_to( "w", QString::number( width() ), e );
    append_string_node_to( "h", QString::number( height() ), e );
    n.appendChild( e );
}

void GEVSmall::appendXMLMention ( QDomNode n )
{
    append_string_node_to( "mention", groupfile->codeName(), n );
}

GroupFile* GEVSmall::getGroupFile ()
{
    return groupfile;
}

void GEVSmall::invokeSuggest ( GEltList elts, QString computationDescription )
{
    visualizer->invokeSuggest( elts, computationDescription );
}

bool GEVSmall::hasElementLocations ()
{
    return TRUE; // subclasses may override -- e.g. OSSmall
}

QPoint GEVSmall::elementLocationOnCanvas ( GElt e )
{
    QPoint percent = visualizer->elementLocationPercent( e );
    if ( ( percent.x() > 0 ) && ( percent.y() > 0 ) ) {
        return QPoint( int( x() + width() * percent.x() / 100 ),
                       int( y() + height() * percent.y() / 100 ) );
    }
    return percent;
}

GEVisualizer* GEVSmall::makeFromXML ( QDomNode n )
{
    int x = -1;
    int y = -1;
    int z = -1;
    int w = -1;
    int h = -1;
    QString groupCode;
    QString vizCode;
    QString vizType;
    for ( QDomNode ch = n.firstChild() ; !ch.isNull() ; ch = ch.nextSibling() ) {
        QString tmp;
        if ( ch.nodeName() == "x" ) {
            if ( try_get_string( tmp, ch, "x", "\\d+" ) ) x = tmp.toInt();
        } else if ( ch.nodeName() == "y" ) {
            if ( try_get_string( tmp, ch, "y", "\\d+" ) ) y = tmp.toInt();
        } else if ( ch.nodeName() == "z" ) {
            if ( try_get_string( tmp, ch, "z", "\\d+" ) ) z = tmp.toInt();
        } else if ( ch.nodeName() == "w" ) {
            if ( try_get_string( tmp, ch, "w", "\\d+" ) ) w = tmp.toInt();
        } else if ( ch.nodeName() == "h" ) {
            if ( try_get_string( tmp, ch, "h", "\\d+" ) ) h = tmp.toInt();
        } else if ( ch.nodeName() == "type" ) {
            if ( try_get_string( tmp, ch, "type", ".+" ) ) vizType = tmp;
        } else if ( ch.nodeName() == "group" ) {
            if ( try_get_string( tmp, ch, "group", ".+" ) ) groupCode = tmp;
        } else if ( ch.nodeName() == "code" ) {
            if ( try_get_string( tmp, ch, "code", ".*" ) ) vizCode = tmp;
        }
    }
    GEVisualizer* result = GEVisualizers::getNewVisualizer( vizType );
    result->setGroup( GroupTable::findGroup( groupCode ) );
    result->decode( vizCode );
    GEVSmall* smallv = result->getSmall();
    smallv->setX( x );
    smallv->setY( y );
    smallv->setZ( z );
    smallv->setSize( w, h );
    return result;
}

QStringList GEVSmall::keys ()
{
    return propertyKeys;
}

QString GEVSmall::getDefault ( QString key )
{
    int index = propertyKeys.findIndex( key );
    return ( index > -1 ) ? propertyDefaults[index] : QString::null;
}

void GEVSmall::addProperty ( QString key, QString defaultValue )
{
    propertyKeys << key;
    propertyDefaults << defaultValue;
}

/*
void GEVSmall::pushUndoCommand ( QtCommand* cmd )
{
    undoStack->push( cmd );
}

void GEVSmall::setUndoStack ( QtUndoStack* s )
{
    undoStack = s;
}
*/

int GEVSmall::RTTI = 142536;
int GEVSmall::rtti () const
{
    return RTTI;
}

QString GEVSmall::readProperty ( QString key )
{
    QString lookup = mycopy[key];
    if ( lookup.isNull() ) return getDefault( key );
    return lookup;
}

void GEVSmall::writeProperty ( QString key, QString value )
{
    if ( value.isNull() ) {
        mycopy.remove( key );
    } else {
        mycopy[key] = value;
    }
}
    
void GEVSmall::set ( QString, QString, QString, QString )
{
    // defaults to nothing; subclasses should override this
}

void GEVSmall::updateContent ( QStringList )
{
    if ( !canvas() ) return;
    canvas()->setChanged( boundingRect() );
    canvas()->update();
}

void GEVSmall::groupFileChanged ( GroupFile* gf )
{
    groupfile = gf;
    if ( !changedKeys.contains( "groupfile" ) ) changedKeys << "groupfile";
    QTimer::singleShot( 0, this, SLOT(callUpdateContent()) );
}

void GEVSmall::processChange ( QString key, QString value, QString desc )
{
    QString oldval = readProperty( key );
    if ( oldval != value ) {
        writeProperty( key, value );
        set( key, oldval, readProperty( key ), desc );
        if ( !changedKeys.contains( key ) ) changedKeys << key;
        QTimer::singleShot( 0, this, SLOT(callUpdateContent()) );
    }
}

void GEVSmall::getAllMorphismsToMe ( Q3PtrList<GECanvasMorphism>& list )
{
    Q3CanvasItemList everything = canvas()->allItems();
    Q3CanvasItemList::Iterator it;
    for ( it = everything.begin() ; it != everything.end() ; ++it ) {
        if ( (*it)->rtti() == GECanvasMorphism::RTTI ) {
            GECanvasMorphism* cm = ( GECanvasMorphism* )(*it);
            if ( cm->getToItem() == this ) list.append( cm );
        }
    }
}

void GEVSmall::getAllMorphismsFromMe ( Q3PtrList<GECanvasMorphism>& list )
{
    Q3CanvasItemList everything = canvas()->allItems();
    Q3CanvasItemList::Iterator it;
    for ( it = everything.begin() ; it != everything.end() ; ++it ) {
        if ( (*it)->rtti() == GECanvasMorphism::RTTI ) {
            GECanvasMorphism* cm = ( GECanvasMorphism* )(*it);
            if ( cm->getFromItem() == this ) list.append( cm );
        }
    }
}

void GEVSmall::callUpdateContent ()
{
    updateContent( changedKeys );
    changedKeys.clear();
}

void GEVSmall::change ( QString key, QString value, QString desc )
{
    processChange( key, value, desc );
    emit changed( key, value, desc );
}

void GEVSmall::draw ( QPainter& painter )
{
    if ( ( lastWidth != width() ) || ( lastHeight != height() ) ||
         ( lastUpdate.elapsed() > 1000 ) ) {
        if ( !lastPixmap.width() || !lastPixmap.height() ||
             ( lastUpdate.elapsed() > 1000 ) ) {
            QPixmap newPixmap = visualizer->getLarge()->currentPixmap();
            if ( newPixmap.isNull() ) {
                // the large wasn't ready fast enough; try again in 1/4 of a second,
                // or 1/2 a second, or 1 second, etc.
                painter.setPen( Qt::black );
                painter.setBrush( Qt::gray );
                painter.drawRect( int( x() ), int( y() ), width(), height() );
                QTimer::singleShot( 250 * ++waiting, this, SLOT(callUpdateContent()) );
                return;
            }
            waiting = 0;
            lastPixmap = newPixmap;
        }
        QMatrix scaling;
        if ( lastPixmap.width() && lastPixmap.height() )
            scaling.scale( double( width() ) / double( lastPixmap.width() ),
                           double( height() ) / double( lastPixmap.height() ) );
        lastPixmap = lastPixmap.xForm( scaling );
        lastWidth = width();
        lastHeight = height();
        lastUpdate.restart();
    }
    painter.drawPixmap( int( x() ), int( y() ), lastPixmap );
    if ( isSelected() ) {
        QRect r = boundingRect();
        painter.setPen( Qt::magenta );
        painter.setBrush( Qt::NoBrush );
        painter.drawRect( r.left(), r.top(), r.width(), r.height() );
    }
}

Q3PtrList<QAction> GEVSmall::actionsToExport ()
{
    return Q3PtrList<QAction>(); // empty by default
}


GEVisualizer::GEVisualizer ( GroupFile* gf )
    : groupfile( gf ), large( 0 ), smallv( 0 ), recursionDepth( 0 )
{
    reset( QString::null );
}

GEVisualizer::~GEVisualizer ()
{
    if ( large ) delete large;
    if ( smallv ) delete smallv;
}

void GEVisualizer::setGroup ( GroupFile* gf )
{
    groupfile = gf;
    emit groupFileChanged( gf );
}

GroupFile* GEVisualizer::getGroup ()
{
    return groupfile;
}

QString GEVisualizer::typeName ()
{
    qWarning( "GEVisualizer::typeName() not implemented; subclasses must override" );
    return "untyped";
}

QPixmap GEVisualizer::quickPixmap ( int w, int h, int )
{
    qWarning( "GEVisualizer::quickPixmap() should be overridden in subclasses; "
              "right now returning a plain gray pixmap" );
    // subclasses will probably want to override this to call a static method
    // SomeGEVLargeWidgetSubclass::quickPixmap(w,h,groupfile) that they define.
    if ( w < 0 ) w = 50;
    if ( h < 0 ) h = 50;
    QImage result( w, h, 32 );
    result.fill( 0x505050 );
    return QPixmap( result );
}

QPoint GEVisualizer::elementLocationPercent ( GElt e )
{
    return getLarge() ? large->elementLocationPercent( e ) : QPoint( 0, 0 );
}

void GEVisualizer::invokeSuggest ( GEltList elts, QString computationDescription )
{
    emit suggestComputedSubset( elts, computationDescription );
}

int GEVisualizer::numIndices ()
{
    // -1 means indexing does not apply to this visualizer type
    // 0 means the visualizer type cannot apply to the given group
    // >0 means the visualizer type applies in that many ways
    return -1; // the simplest visualizers are one-way only (MT, CG)
}

void GEVisualizer::createLarge ()
{
    // descendants should fill this in with large = new MyGEVLargeSubclass(...);
}

GEVLarge* GEVisualizer::getLarge ()
{
    if ( !large ) {
        // we must store our current state, because the large may try to change it
        // during creation, or as the group file is set
        QString remember = encode();
        createLarge();
        if ( large ) {
            large->setTypeName( GEVisualizers::printableName( typeName() ) );
            connect( this, SIGNAL(changed(QString,QString,QString)),
                     large, SLOT(processChange(QString,QString,QString)) );
            connect( this, SIGNAL(groupFileChanged(GroupFile*)),
                     large, SLOT(vizGroupFileChanged(GroupFile*)) );
            connect( large, SIGNAL(childChanged(QString,QString,QString)),
                     this, SLOT(processChange(QString,QString,QString)) );
            connect( large, SIGNAL(getAllMorphismsToMe(Q3PtrList<GECanvasMorphism>&)),
                     this, SIGNAL(getAllMorphismsToMe(Q3PtrList<GECanvasMorphism>&)) );
            connect( large, SIGNAL(getAllMorphismsFromMe(Q3PtrList<GECanvasMorphism>&)),
                     this, SIGNAL(getAllMorphismsFromMe(Q3PtrList<GECanvasMorphism>&)) );
            connect( this, SIGNAL(suggestComputedSubset(GEltList,QString)),
                     large, SIGNAL(suggestComputedSubset(GEltList,QString)) );
            connect( large, SIGNAL(reset()), this, SLOT(reset()) );
            connect( large, SIGNAL(destroyed()), this, SLOT(largeViewDestroyed()) );
            large->vizGroupFileChanged( groupfile );
            //QStringList keys = orderedKeys();
            //for ( uint i = 0 ; i < keys.count() ; i++ )
            //    large->processChange( keys[i], properties[keys[i]], QString::null );
            decode( remember );
        }
    }
    large->visualizer = this;
    return large;
}

void GEVisualizer::createSmall ()
{
    // descendants should fill this in with small = new MyGEVSmallSubclass(...);
}

GEVSmall* GEVisualizer::getSmall ()
{
    if ( !smallv ) {
        // we must store our current state, because the large may try to change it
        // during creation, or as the group file is set
        QString remember = encode();
        //qDebug( " * * create small, remembering: %s", remember.latin1() );
        createSmall();
        if ( smallv ) {
            connect( this, SIGNAL(changed(QString,QString,QString)),
                     smallv, SLOT(processChange(QString,QString,QString)) );
            connect( this, SIGNAL(groupFileChanged(GroupFile*)),
                     smallv, SLOT(groupFileChanged(GroupFile*)) );
            connect( smallv, SIGNAL(changed(QString,QString,QString)),
                     this, SLOT(processChange(QString,QString,QString)) );
            connect( this, SIGNAL(getAllMorphismsFromMe(Q3PtrList<GECanvasMorphism>&)),
                     smallv, SLOT(getAllMorphismsFromMe(Q3PtrList<GECanvasMorphism>&)) );
            connect( this, SIGNAL(getAllMorphismsToMe(Q3PtrList<GECanvasMorphism>&)),
                     smallv, SLOT(getAllMorphismsToMe(Q3PtrList<GECanvasMorphism>&)) );
            connect( smallv, SIGNAL(reset()), this, SLOT(reset()) );
            connect( smallv, SIGNAL(destroyed()), this, SLOT(smallViewDestroyed()) );
            //qDebug( " * * about to do small->groupFileChanged( %s )",
            //        groupfile->getPrimaryName().text.latin1() );
            smallv->groupFileChanged( groupfile );
            //QStringList keys = orderedKeys();
            //for ( uint i = 0 ; i < keys.count() ; i++ )
            //    smallv->processChange( keys[i], properties[keys[i]], QString::null );
            //qDebug( " * * about to decode: %s", remember.latin1() );
            decode( remember );
        }
    }
    smallv->visualizer = this;
    return smallv;
}

void GEVisualizer::largeViewDestroyed ()
{
    // this means I should delete myself, too, but if I do it without this next line,
    // the destructor will try to delete large, which gives memory access errors:
    large = 0;
    deleteLater();
}

void GEVisualizer::smallViewDestroyed ()
{
    // this means I should delete myself, too, but if I do it without this next line,
    // the destructor will try to delete small, which gives memory access errors:
    smallv = 0;
    deleteLater();
}

QStringList GEVisualizer::orderedKeys ()
{
    QStringList keys = properties.keys();
    // we must move indx to first in the list, so that it won't erase the others
    // (see how GEVLargeWidget handles set(), for instance)
    int pos = keys.findIndex( "indx" );
    if ( pos > 0 ) {
        keys[pos] = keys[0];
        keys[0] = "indx";
    }
    return keys;
}

QString GEVisualizer::encode ()
{
    QString result;
    QStringList keys = orderedKeys();
    for ( int i = 0 ; i < keys.count() ; i++ ) {
        if ( i ) result += " ";
        QString value = properties[keys[i]];
        Q3Url::encode( value );
        result += keys[i] + "=" + value;
    }
    return result;
}

void GEVisualizer::decode ( QString code, QString desc )
{
    reset( QString::null );
    QStringList pieces = QStringList::split( " ", code );
    for ( int i = 0 ; i < pieces.count() ; i++ ) {
        uint j = pieces[i].find( "=" );
        QString key = pieces[i].left( j );
        QString value = pieces[i].mid( j + 1 );
        Q3Url::decode( value );
        processChange( key, value, desc );
        // default value for desc parameter is empty string;
        // this will not be recorded in undo stacks,
        // but neither will it pass the isNull() test,
        // so the program will prompt the user before doing any
        // ridiculous calculations/manipulations.
    }
}

QString GEVisualizer::readProperty ( QString key )
{
    return properties[key];
}

void GEVisualizer::reset ( QString desc )
{
    //qDebug( QString( "%1::reset( \"%2\"%3 )" )
    //        .arg( className() ).arg( desc ).arg( desc.isNull() ? " [NULL]" : "" ) );
    if ( !desc.isNull() ) {
        // if this has a description, we need to post it to the undo stack, which
        // will actually recall this function with a null desc, so we return if we
        // successfully post to the stack (rather than do the work twice).
        if ( large && ( sender() == large ) ) {
            large->pushUndoCommand( new GEVCommand( this, desc ) );
            return;
        /*
        } else if ( smallv && ( sender() == smallv ) ) {
            smallv->pushUndoCommand( new GEVCommand( this, desc ) );
            return;
        */
        }
    }
    // Ah, a null description means that this function is being called from a
    // QtCommand object, or from the constructor, and should therefore be obeyed:
    QStringList keys = orderedKeys();
    for ( int i = 0 ; i < keys.count() ; i++ )
        processChange( keys[i], QString::null, QString::null );
}

void GEVisualizer::processChange ( QString key, QString value, QString desc )
{
    QString oldvalue = properties[key];
    if ( oldvalue != value ) {
        properties[key] = value;
        recursionDepth++;
        emit changed( key, value, desc );
        recursionDepth--;
        // Now check to see if I'm the outermost call (not a recursive one), and if
        // this change was one that has a description (and thus was user-initiated).
        // If both those are true then we need to make a good effort to put it onto a
        // QtUndoStack, either that of the small or large:
        if ( !recursionDepth && !desc.isNull() ) {
            if ( large && ( sender() == large ) ) {
                large->pushUndoCommand(
                        new GEVCommand( this, desc, key, oldvalue, value ) );
            /*
            } else if ( smallv && ( sender() == smallv ) ) {
                smallv->pushUndoCommand(
                        new GEVCommand( this, desc, key, oldvalue, value ) );
            */
            }
        }
#ifdef QT_DEBUG
        qDebug( "%s", QString( "[QT_DEBUG] %1::encode() == %2" )
                      .arg( className() ).arg( encode() ).latin1() );
#endif
#if 0
        QString tmp = className();
        tmp += " proc'd \"";
        tmp += desc;
        tmp += "\"; ";
        tmp += key;
        tmp += " went from \"";
        tmp += oldvalue;
        tmp += "\" to \"";
        tmp += value;
        tmp += "\"";//; code:\n\t\"";
        //tmp += encode();
        //tmp += "\"";
        qDebug( "%s", tmp.latin1() );
    } else {
        qDebug( QString( "%1 proc'd \"%2\"; %3 didn't change" )
                .arg( className() ).arg( desc ).arg( key ) );
#endif
    }
}


GEVCommand::GEVCommand ( GEVisualizer* viz, QString description,
                         QString key, QString oldvalue, QString newvalue )
    : firstRedo( TRUE ), isResetCommand( FALSE ),
      viz( viz ), key( key ), oldvalue( oldvalue ), newvalue( newvalue )
{
    setDescription( description );
}

GEVCommand::GEVCommand ( GEVisualizer* viz, QString description )
    : isResetCommand( TRUE ), viz( viz )
{
    setDescription( description );
    oldvalue = viz->encode();
}

void GEVCommand::redo ()
{
    if ( isResetCommand ) {
        viz->reset( QString::null );
    } else {
        if ( firstRedo ) {
            firstRedo = FALSE;
        } else {
            viz->processChange( key, newvalue, QString::null );
        }
    }
}

void GEVCommand::undo ()
{
    if ( isResetCommand ) {
        viz->decode( oldvalue, QString::null );
    } else {
        viz->processChange( key, oldvalue, QString::null );
    }
}

