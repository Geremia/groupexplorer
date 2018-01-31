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


#include "geml.h"
#include "idealist.h"
#include "gehelpwindow.h"
#include "gevisualizerlist.h"
#include "gepreferences.h"
#include "glscenenodes.h"
#include "gesheet.h"
#include "mainwindow.h"
#include <qregexp.h>
#include <q3stylesheet.h>
#include <qfile.h>
#include <qdom.h>
//Added by qt3to4:
#include <Q3TextStream>
#include <Q3ValueList>


const QString doubleRE = "(?:\\d+\\.?|\\d*\\.\\d+)";

bool try_get_point ( double& x, double& y, double& z, QDomNode n )
{
    if ( n.isNull() ) {
        qWarning( "Missing required point tag" );
        return FALSE;
    }
    if ( n.nodeName() != "point" ) {
        qWarning( QString( "Got %1 tag when expected point tag" ).arg( n.nodeName() )
                  .toLatin1().constData() );
        return FALSE;
    }
    QString tmp;
    if ( !try_get_attrib( tmp, n, "x", doubleRE ) ) return FALSE;
    x = tmp.toDouble();
    if ( !try_get_attrib( tmp, n, "y", doubleRE ) ) return FALSE;
    y = tmp.toDouble();
    if ( !try_get_attrib( tmp, n, "z", doubleRE ) ) return FALSE;
    z = tmp.toDouble();
    return TRUE;
}

bool try_get_color ( GLSceneNode* sn, QDomNode dn, bool useMaterial = TRUE )
{
    QString tmp, tmp2;
    if ( !try_get_attrib( tmp, dn, "color", "\\#[0-9a-fA-F]{6}" ) ) return FALSE;
    tmp2 = "1.0";
    if ( dn.toElement().hasAttribute( "alpha" ) )
        if ( !try_get_attrib( tmp2, dn, "alpha", doubleRE ) ) return FALSE;
    if ( useMaterial ) {
        sn->setDiffuse( double( tmp.mid( 1, 2 ).toInt( 0, 16 ) ) / 255.0,
                        double( tmp.mid( 3, 2 ).toInt( 0, 16 ) ) / 255.0,
                        double( tmp.mid( 5, 2 ).toInt( 0, 16 ) ) / 255.0,
                        tmp2.toDouble() );
    } else {
        sn->setColor( double( tmp.mid( 1, 2 ).toInt( 0, 16 ) ) / 255.0,
                      double( tmp.mid( 3, 2 ).toInt( 0, 16 ) ) / 255.0,
                      double( tmp.mid( 5, 2 ).toInt( 0, 16 ) ) / 255.0,
                      tmp2.toDouble() );
    }
    return TRUE;
}

bool try_get_fixed ( GLSceneNode* sn, QDomNode dn, GLSceneNode* f, GLSceneNode* m )
{
    QString tmp;
    tmp = "Y";
    if ( dn.toElement().hasAttribute( "fixed" ) )
        if ( !try_get_attrib( tmp, dn, "fixed", "[ynYN]" ) ) return FALSE;
    if ( tmp.upper() == "N" ) m->addChild( sn ); else f->addChild( sn );
    return TRUE;
}

//
// SymmetryObject class implementation
//

SymmetryObject::SymmetryObject ()
    : name( "unnamed" ), lastRadius( 1.0 )
{
    setupInside();
}

SymmetryObject::SymmetryObject ( QString name )
    : name( name ), lastRadius( 1.0 )
{
    setupInside();
}

SymmetryObject* SymmetryObject::copy ()
{
    SymmetryObject* result = new SymmetryObject();
    result->makeFromXML( loadedFrom );
    return result;
}

void SymmetryObject::setupInside ()
{
    addChild( fixedNodes = new GLSceneNode() );
    addChild( movingNodes = new GLSceneNode() );
    fixedNodes->name = "FixedNodesContainer";
    movingNodes->name = "MovingNodesContainer";
}

QString SymmetryObject::getName ()
{
    return name;
}

void SymmetryObject::changeBaseRadius ( double newRadius )
{
    double factor = newRadius / lastRadius;
    for ( uint i = 0 ; i < nodes.count() ; i++ )
        nodes.at( i )->scale( factor, factor, factor );
    lastRadius = newRadius;
}

void SymmetryObject::appendXMLTo ( QDomNode /*n*/ )
{
    // Here is what these look like:
    // <symmetryobject name="...">
    //   <operation element="uint" degrees="double">
    //     <point x="double" y="double" z="double">
    //   </operation>
    //   <sphere radius="double" fixed="YN" color="#hhhhhh">
    //     <point x="double" y="double" z="double">
    //   </sphere>
    //   ...
    //   <path fixed="YN" color="#hhhhhh">
    //     <point x="double" y="double" z="double">
    //     ...
    //   </path>
    //   ...
    //   <polygon fixed="YN" color="#hhhhhh">
    //     <point x="double" y="double" z="double">
    //     ...
    //   </polygon>
    //   ...
    //   <label fixed="YN" color="#hhhhhh" text="...">
    //     <point x="double" y="double" z="double">
    //   </label>
    //   ...
    // </symmetryobject>
    // The reason this has not yet been implemented is that I would need to read back
    // all this information from the GL scene, which I'm not about to do.  This doesn't
    // really matter right now because these things don't get altered ever, but if they
    // do one day, you should bother saving them carefully.  Of course, that day, you'd
    // probably have a better internal representation...or maybe not...
}

bool SymmetryObject::makeFromXML ( QDomNode n )
{
    loadedFrom = n.cloneNode( true ); // remember for making copies later

    fixedNodes->children.clear();
    movingNodes->children.clear();
    nodes.clear();
    lastRadius = 1.0;
    operElts.resize( 0 );
    operDegs.resize( 0 );
    operAxesX.resize( 0 );
    operAxesY.resize( 0 );
    operAxesZ.resize( 0 );

    if ( !try_get_attrib( name, n, "name" ) ) {
        qWarning( "Symmetry Object name required" );
        return FALSE;
    }

    double x;
    double y;
    double z;
    QString tmp;

    for ( QDomNode ch = n.firstChild() ; !ch.isNull() ; ch = ch.nextSibling() ) {
        if ( ch.isElement() ) {
            QDomElement e = ch.toElement();
            if ( ch.nodeName() == "operation" ) {
                // fetch group element represented by this operation
                if ( !try_get_attrib( tmp, ch, "element", "\\d+" ) ) return FALSE;
                operElts.resize( operElts.size() + 1 );
                operElts[operElts.size() - 1] = tmp.toUInt();
                // fetch degrees of rotation
                if ( !try_get_attrib( tmp, ch, "degrees", doubleRE ) ) return FALSE;
                operDegs.resize( operDegs.size() + 1 );
                operDegs[operDegs.size() - 1] = tmp.toDouble();
                // fetch axis of rotation
                operAxesX.resize( operAxesX.size() + 1 );
                operAxesY.resize( operAxesY.size() + 1 );
                operAxesZ.resize( operAxesZ.size() + 1 );
                if ( !try_get_point( operAxesX[operAxesX.size() - 1],
                                     operAxesY[operAxesY.size() - 1],
                                     operAxesZ[operAxesZ.size() - 1], ch.firstChild() ) )
                    return FALSE;
            } else if ( ch.nodeName() == "sphere" ) {
                GLSceneNode* holder = new GLSceneNode();
                if ( !try_get_fixed( holder, ch, fixedNodes, movingNodes ) ) return FALSE;
                if ( !try_get_color( holder, ch ) ) return FALSE;
                // fetch point at center of sphere
                if ( !try_get_point( x, y, z, ch.firstChild() ) ) return FALSE;
                holder->translate( x, y, z );
                // fetch radius
                GLSphere* s = new GLSphere( 30, 12 );
                holder->addChild( s );
                if ( !try_get_attrib( tmp, ch, "radius", doubleRE ) ) return FALSE;
                x = tmp.toDouble();
                s->scale( x, x, x );
                nodes.append( s );
            } else if ( ch.nodeName() == "path" ) {
                GLSceneNode* p = new GLSceneNode();
                p->name = "Path";
                if ( !try_get_fixed( p, ch, fixedNodes, movingNodes ) ) return FALSE;
                if ( !try_get_color( p, ch, FALSE ) ) return FALSE;
                // fetch points, creating line segments from each conecutive pair of them
                double x[2], y[2], z[2];
                uint count = 0;
                for ( QDomNode chch = ch.firstChild() ; !chch.isNull() ;
                      chch = chch.nextSibling() ) {
                    if ( !try_get_point(
                        x[count % 2], y[count % 2], z[count % 2], chch ) ) return FALSE;
                    if ( count ) {
                        GLSceneNode* l = new GLLine(
                            x[(count - 1) % 2], y[(count - 1) % 2], z[(count - 1) % 2],
                            x[count % 2], y[count % 2], z[count % 2] );
                        p->addChild( l );
                    }
                    count++;
                }
            } else if ( ch.nodeName() == "polygon" ) {
                GLSceneNode* p = new GLSceneNode();
                p->name = "Polygon";
                if ( !try_get_fixed( p, ch, fixedNodes, movingNodes ) ) return FALSE;
                if ( !try_get_color( p, ch ) ) return FALSE;
                // fetch points, creating triangles from each consecutive triple of points
                double x[3], y[3], z[3];
                uint count = 0;
                for ( QDomNode chch = ch.firstChild() ; !chch.isNull() ;
                      chch = chch.nextSibling() ) {
                    if ( !try_get_point(
                        x[count % 3], y[count % 3], z[count % 3], chch ) ) return FALSE;
                    if ( count > 1 ) {
                        GLSceneNode* t = new GLTriangle(
                            x[(count - 2) % 3], y[(count - 2) % 3], z[(count - 2) % 3],
                            x[(count - 1) % 3], y[(count - 1) % 3], z[(count - 1) % 3],
                            x[count % 3], y[count % 3], z[count % 3] );
                        p->addChild( t );
                    }
                    count++;
                }
            } else if ( ch.nodeName() == "label" ) {
                // fetch text content (message)
                if ( !try_get_attrib( tmp, ch, "text", "." ) ) return FALSE;
                GLSceneNode* l = new GLTextLabel( tmp, QFont() );
                l->name = "TextLabel";
                if ( !try_get_fixed( l, ch, fixedNodes, movingNodes ) ) return FALSE;
                if ( !try_get_color( l, ch, FALSE ) ) return FALSE;
                // fetch point where text originates
                if ( !try_get_point( x, y, z, ch.firstChild() ) ) return FALSE;
                l->translate( x, y, z );
            } else {
                qWarning( QString( "Unexpected tag inside symmetry object: %1" )
                          .arg( ch.nodeName() ).toLatin1().constData() );
                return FALSE;
            }
        }
    }
    return TRUE;
}


//
// CustomCayley class implementation
//

QString CustomCayley::getName ()
{
    return name;
}

GEltList CustomCayley::getArrows ()
{
    return arrows;
}

void CustomCayley::getNodePosition ( GElt g, GLdouble& x, GLdouble& y, GLdouble& z )
{
    if ( g < xs.size() ) {
        x = xs[g];
        y = ys[g];
        z = zs[g];
    } else {
        x = y = z = 0.0;
    }
}

void CustomCayley::appendXMLTo ( QDomNode /*n*/ )
{
    // Here is what these look like:
    // <cayleydiagram>
    //   <name>Name here</name>
    //   <arrow>#</arrow>
    //   ...
    //   <elementlocation x="#.#" y="#.#" z="#.#"/>
    //   ...
    // </cayleydiagram>
    // The reason this has not yet been implemented is that they don't get altered ever,
    // but if they do one day, you should bother saving them carefully.  That's easy to
    // do, because we store everything we read in member vars.
}

bool CustomCayley::makeFromXML ( QDomNode n )
{
    name = QString::null;
    arrows.resize( 0 );
    xs.resize( 0 );
    ys.resize( 0 );
    zs.resize( 0 );

    for ( QDomNode ch = n.firstChild() ; !ch.isNull() ; ch = ch.nextSibling() ) {
        if ( ch.isElement() ) {
            QDomElement e = ch.toElement();
            if ( ch.nodeName() == "name" ) {
                if ( !name.isEmpty() ) {
                    qWarning( "Redefinition of name not allowed" );
                    return FALSE;
                }
                if ( !try_get_string( name, ch, "", "." ) ) return FALSE;
            } else if ( ch.nodeName() == "arrow" ) {
                if ( xs.size() ) {
                    qWarning( "Arrows must precede elementlocations" );
                    return FALSE;
                }
                if ( name.isEmpty() ) {
                    qWarning( "Arrows must follow Cayley diagram name" );
                    return FALSE;
                }
                QString tmp;
                if ( !try_get_string( tmp, ch, "", "[0-9]+" ) ) return FALSE;
                arrows.resize( arrows.size() + 1 );
                arrows[arrows.size() - 1] = tmp.toUInt();
            } else if ( ch.nodeName() == "point" ) {
                if ( name.isEmpty() ) {
                    qWarning( "Points must follow Cayley diagram name" );
                    return FALSE;
                }
                xs.resize( xs.size() + 1 );
                ys.resize( ys.size() + 1 );
                zs.resize( zs.size() + 1 );
                if ( !try_get_point(
                        xs[xs.size() - 1], ys[ys.size() - 1], zs[zs.size() - 1], ch ) )
                    return FALSE;
            }
        }
    }
    return TRUE;
}


//
// GroupFile class implementation
//

GroupFile::GroupFile ()
    : currentRep( 0 )
{
    // everybody's default constructor is fine; nothing needed here
}

GroupFile::GroupFile ( const GroupFile& gf )
{
    // just a simple member-by-member copy
    fileInfo = gf.fileInfo;
    phrasename = gf.phrasename;
    author = gf.author;
    notes = gf.notes;
    names = gf.names;
    defs = gf.defs;
    reps = gf.reps;
    group = gf.group;
    currentRep = gf.currentRep;
    generatorlists = gf.generatorlists;
    cays = gf.cays;
    symms = gf.symms;
}

GroupFile::GroupFile ( QString fname ) {
    load( fname );
}


bool GroupFile::makeFromXML ( QDomNode n ) {
    // clear us out first
    phrasename = author = notes = "";
    group.setSize(0);
    defs.clear();
    reps.clear();
    names.clear();
    for ( QDomNode ch = n.firstChild() ; !ch.isNull () ; ch = ch.nextSibling() ) {
        if ( ch.isElement() ) {
            /*
             *  Perhaps this processing should also check for elements with
             *  extraneous attributes, and complain about those?
             */
            if ( ch.nodeName() == "phrase" ) {
                if ( !phrasename.isEmpty() ) {
                    qWarning( "Redefinition of phrase not allowed" );
                    return FALSE;
                }
                if ( !try_get_string( phrasename, ch, "", "." ) ) return FALSE;
            } else if ( ch.nodeName() == "author" ) {
                if ( !author.isEmpty() ) {
                    qWarning( "Redefinition of author not allowed" );
                    return FALSE;
                }
                if ( !try_get_string( author, ch, "", "." ) ) return FALSE;
            } else if ( ch.nodeName() == "notes" ) {
                if ( !notes.isEmpty() ) {
                    qWarning( "Redefinition of notes not allowed" );
                    return FALSE;
                }
                if ( !try_get_string( notes, ch, "", "." ) ) return FALSE;
            } else if ( ch.nodeName() == "multtable" ) {
                if ( group.order ) {
                    qWarning( "Redefinition of multtable not allowed" );
                    return FALSE;
                }
                if ( !group.makeFromXML( ch ) ) return FALSE;
            } else if ( ch.nodeName() == "definition" ) {
                MathText* mt = new MathText();
                defs.append( *mt );
                if ( !defs.last().makeFromXML( ch ) ) return FALSE;
            } else if ( ch.nodeName() == "representation" ) {
                if ( !group.order ) {
                    qWarning( "Must define multtable before any representations" );
                    return FALSE;
                }
                reps.append ( MathTextList() );
                if ( !reps.last().makeFromXML( ch, "representation", "element" ) )
                    return FALSE;
                if ( reps.last().size() != (int)group.order ) {
                    qWarning( QString( 
                                "Representation with %1 elements (should be %2)" )
                              .arg( reps.last().size() ).arg( group.order )
                              .toLatin1().constData() );
                    return FALSE;
                }
            } else if ( ch.nodeName() == "name" ) {
                MathText* mt = new MathText();
                names.append ( *mt );
                if ( !names.last().makeFromXML( ch ) ) return FALSE;
            } else if ( ch.nodeName() == "generators" ) {
                QString tmp;
                if ( !try_get_attrib( tmp, ch, "list",
                                      "\\s*\\d+(?:\\s+\\d+)*\\s*" ) ) {
                    qWarning( "Generators list attribute required" );
                    return FALSE;
                }
                QStringList genlist = tmp.split( " " );
                GEltList newlist( genlist.size() );
                for ( int i = 0 ; i < genlist.size() ; i++ )
                    newlist[i] = genlist[i].toUInt(); // guaranteed OK by above RE
                generatorlists.append( newlist );
            } else if ( ch.nodeName() == "cayleydiagram" ) {
                CustomCayley* cc = new CustomCayley();
                cays.append( *cc );
                if ( !cays.last().makeFromXML( ch ) ) return FALSE;
            } else if ( ch.nodeName() == "symmetryobject" ) {
                SymmetryObject* so = new SymmetryObject();
                symms.append( *so );
                if ( !symms.last().makeFromXML( ch ) ) return FALSE;
            } else {
                qWarning( QString( "Invalid element: %1 (ignoring it)" )
                          .arg( ch.nodeName() ).toLatin1().constData() );
            }
        }
    }
    if ( !generatorlists.size() ) {
        // we can create these
        generatorlists.append( group.findGens() );
    }
    if ( !names.size() ) {
        qWarning( "Must define at least one name for the group" );
        return FALSE;
    }
    if ( !group.order ) {
        qWarning( "Must provide a multtable for the group" );
        return FALSE;
    }
    currentRep = MainWindow::original() ?
                 MainWindow::original()->preferences->getCurrentGroupRep( codeName() ) :
                 0;
    return TRUE;
}

void GroupFile::appendXMLTo ( QDomNode n ) {
    append_string_node_to( "phrase", phrasename, n );
    append_string_node_to( "author", author, n );
    append_string_node_to( "notes", notes, n );
    group.appendXMLTo ( n );
    for ( int i = 0 ; i < generatorlists.size() ; i++ ) {
        QDomElement e = n.ownerDocument().createElement( "generators" );
        QString tmp;
        for ( uint j = 0 ; j < generatorlists[i].count() ; j++ )
            tmp += QString( " %1" ).arg( generatorlists[i][j] );
        e.setAttribute( "list", tmp.mid( 1 ) );
        n.appendChild( e );
    }
    for ( Q3ValueList<MathText>::iterator i = names.begin() ; i != names.end() ; i++ )
        (*i).appendXMLTo ( n, "name" );
    Q3ValueList<MathTextList>::iterator i;
    for ( Q3ValueList<MathText>::iterator i = defs.begin() ; i != defs.end() ; i++ )
        (*i).appendXMLTo ( n, "definition" );
    for ( i = reps.begin() ; i != reps.end() ; i++ )
        (*i).appendXMLTo( n, "representation", "element" );
    // cayley diagrams to come
    for ( int i = 0 ; i < symms.count() ; i++ )
        symms[i].appendXMLTo( n );
}

QString GroupFile::toString () {
    QDomDocument doc ( "groupexplorerml" );
    QDomElement root = doc.createElement ( "group" );
    doc.appendChild( root );
    appendXMLTo( root );
    return fileInfo.absFilePath() + "\n" + doc.toString(4);
}    

bool GroupFile::load ( QString fname ) {
    // try to open the file
    QFile groupFile ( fname );
    if ( !groupFile.open( QIODevice::ReadOnly ) ) {
        qWarning( QString ( "Could not open file %1" ).arg( fname ).toLatin1().constData() );
        return FALSE;
    }
    // successfully opened, so read everything and close
    QString xml = groupFile.readAll();
    groupFile.close();
    // try to build an XML document internally from the input
    QDomDocument doc;
    if ( !doc.setContent( xml ) ) {
        qWarning( QString( "The data in %1 is not valid XML" ).arg( fname )
                  .toLatin1().constData() );
        return FALSE;
    }
    if ( doc.documentElement().nodeName() != "group" ) {
        qWarning( QString( 
                    "Main document element in group file %1 must be a group tag" )
                  .arg( fname ).toLatin1().constData() );
        return FALSE;
    }
    // now check to see if what we got is parse-able:
    if ( !makeFromXML( doc.documentElement() ) ) {
        qWarning( QString( "Could not parse the XML input from file %1" )
                  .arg( fname ).toLatin1().constData() );
        return FALSE;
    }
    // it parsed OK, so we keep this file's info and return success:
    fileInfo = QFileInfo( groupFile );
    return TRUE;
}

bool GroupFile::load () {
    return load( fileInfo.filePath() );
}

bool GroupFile::save ( QString fname ) {
    // try to open the specified output file for writing
    QFile groupFile( fname );
    if ( !groupFile.open( QIODevice::WriteOnly ) ) {
        qWarning( QString( "Cannot open file %1 for writing" ).arg( fname )
                  .toLatin1().constData() );
        return FALSE;
    }
    // we'll build an XML document internally, and then use its toString() function
    QDomDocument doc( "groupexplorerml" );
    QDomElement root = doc.createElement( "groupexplorerml" );
    doc.appendChild( root );
    appendXMLTo( root );
    Q3TextStream( &groupFile ) << doc.toString( 4 );
    groupFile.close();
    return TRUE;
}

bool GroupFile::save () {
    return save( fileInfo.filePath() );
}


MathText GroupFile::getPrimaryName () {
    return names.count() ? names[0] : MathText();
}

MathText GroupFile::getOtherNames () {
    MathText answer;
    if ( names.count() ) {
        for ( int j = 1 ; j < names.count() ; j++ ) {
            if ( j > 1 ) {
                answer.text += ", ";
                answer.mathML += "<mo>,</mo>";
            }
            answer.text += names[j].text;
            answer.mathML += names[j].mathML;
        }
    } else {
        answer = MathText( "none" );
    }
    answer.mathML = "<mrow>" + answer.mathML + "</mrow>";
    return answer;
}

MathText GroupFile::getPrimaryDefinition () {
    return defs.count() ? defs[0] : MathText( "none" );
}

MathText GroupFile::getRepresentation ( GElt e ) {
    if ( e < group.order ) {
        if ( (int)currentRep < reps.count() ) return reps[currentRep][e];
        MainWindow* mw = MainWindow::original();
        if ( mw ) {
            uint remainder = currentRep - reps.count();
            QString code = codeName();
            if ( remainder < mw->preferences->numGroupReps( code ) )
                return mw->preferences->getGroupRep( code, remainder )[e];
        }
    }
    return MathText( QString( "[Error--no #%1 rep for %2]" ).arg( currentRep ).arg( e ) );
}

QString GroupFile::debugSubset ( GEltList lst )
{
    QString result = "{ ";
    for ( uint i = 0 ; i < lst.size() ; i++ ) {
        if ( i ) result += ", ";
        result += getRepresentation( lst[i] ).text;
    }
    return result + " }";
}

QString GroupFile::debugSubsets ( GEltListList lsts )
{
    QString result = "{ ";
    for ( int i = 0 ; i < lsts.count() ; i++ ) {
        if ( i ) result += ", ";
        result += debugSubset( lsts[i] );
    }
    return result + " }";
}

QString GroupFile::codeName ( QString primaryName, QString author ) {
    QString codedPriName = primaryName;
    QString codedAuthor = author;
    codedPriName.replace( "-", "--" ).replace( "|", "-|" ).replace( " ", "-_" );
    codedAuthor.replace( "-", "--" ).replace( "|", "-|" ).replace( " ", "-_" );
    return codedPriName + "|" + codedAuthor;
}

QString GroupFile::decodeAuthor ( QString codeName ) {
    int i = -1;
    do {
        i = codeName.find( '|', i + 1 );
    } while ( ( i < 0 ) || ( codeName[i-1] == '-' ) );
    if ( i >= 0 ) {
        return codeName.mid( i + 1 ).replace( "-|", "|" )
                .replace( "-_", " " ).replace( "--", "-" );
    } else {
        qWarning( QString( "GroupFile::decodeAuthor: cannot decode \"%1\"" )
                  .arg( codeName ).toLatin1().constData() );
        return "";
    }
}

QString GroupFile::decodePrimaryName ( QString codeName ) {
    int i = -1;
    do {
        i = codeName.find( '|', i + 1 );
    } while ( ( i < 0 ) || ( codeName[i-1] == '-' ) );
    if ( i >= 0 ) {
        return codeName.left( i ).replace( "-|", "|" )
                .replace( "-_", " " ).replace( "--", "-" );
    } else {
        qWarning( QString( "GroupFile::decodePrimaryName: cannot decode \"%1\"" )
                  .arg( codeName ).toLatin1().constData() );
        return "";
    }
}


QString GroupFile::codeName () {
    return GroupFile::codeName( getPrimaryName().text, author );
}


QString GroupFile::groupInfoHTML ( GEPreferences* preferences )
{
    QString trow = "    <tr><td align=right>%1</td><td>%2</td></tr>\n";
    QString trow3 = "    <tr><td align=right>%1</td><td>%2</td><td>%3</td></tr>\n";
    QString code = codeName();
    QString info;
    info = QString( "<title>Information for group %1</title>\n"
                    "<center><h1>%2%3</h1></center>\n" )
           .arg( getPrimaryName().text )
           .arg( getPrimaryName().HTML )
           .arg( phrasename.isEmpty() ? QString( "" ) :
                 ( " - " + Q3StyleSheet::escape( phrasename ) ) );
    //// CONTENTS
    info += "<center>"
            "Contents: "
            "<a href=\"#facts\">Facts</a> | "
            "<a href=\"#views\">Views</a> | "
            "<a href=\"#description\">Description</a> | "
            "<a href=\"#properties\">Computed properties</a> | "
            "<a href=\"#relatedsheets\">Related sheets</a> | "
            "<a href=\"#filedata\">File data</a> | "
            "<a href=\"#otherdefs\">Other definitions</a> | "
            "<a href=\"#generators\">Generators</a> | "
            "<a href=\"#namingschemes\">Naming schemes</a> | "
            "<a href=\"#notes\">Notes</a>"
            //"<a href=\"#relatedconcepts\">Related concepts</a> | "
            "</center>\n"
            "<hr>\n";
    //// GROUP FACTS SECTION
    info += "<a name=\"facts\"><h2>Basic facts</h2></a>\n";
    info += "<blockquote>\n"
            "  <table border=1>\n";
    if ( defs.size() )
        info += QString( trow )
                .arg( "Definition" )
                .arg( getPrimaryDefinition().HTML );
    info += QString( trow )
            .arg( "Order" )
            .arg( group.order );
    if ( names.size() > 1 ) {
        info += QString( trow )
                .arg( "Other names" )
                .arg( getOtherNames().HTML );
    }
    info += "  </table>\n"
            "</blockquote>\n";
    //// VIEWS SECTION
    info += "<a name=\"views\"><h2>Views</h2></a>\n";
    info += "<blockquote>\n";
    info += "  <p>Click any view to open a copy for exploration.</p>\n";
    QStringList vnames = GEVisualizers::allVisualizerNames();
    info += "  <table border=0 cellspacing=10>\n";
    info += "    <tr>\n";
    for ( int i = 0 ; i < vnames.count() ; i++ )
        info += "      <td><h3>" + GEVisualizers::printableName( vnames[i] ) +
                "</h3></td>\n";
    info += "    </tr><tr>\n";
    for ( int i = 0 ; i < vnames.count() ; i++ )
        info += "      <td><a href=\"do openHelp " + GEVisualizers::helpURL( vnames[i] ) +
                "\">Help on this</a></td>\n";
    info += "    </tr><tr>\n";
    for ( int i = 0 ; i < vnames.count() ; i++ ) {
        int indices = GEVisualizers::numIndices( vnames[i], this );
        info += "      <td align=\"center\">\n";
        if ( indices == -1 ) {
            info += "        <a href=\"do showVisualizer " + vnames[i] + " " + code
                    + "\"><img src=\"" + vnames[i] + "/" + code +
                    "/0/100/100\"></a>\n";
        } else if ( indices == 0 ) {
            info += "        <i>none</i>\n";
        } else {
            for ( int j = 0 ; j < indices ; j++ ) {
                QString caption = "";
                if ( vnames[i] == "CayleyDiagram" ) {
                    if ( j < cays.count() )
                        caption = cays[j].getName();
                } else if ( vnames[i] == "SymmetryObject" ) {
                    if ( ( j >= 0 ) && ( j < (int)symms.count() ) )
                        caption = symms[j].getName();
                }
                if ( j ) info += "        <br><br>\n";
                info += "        <a href=\"do showVisualizer " + vnames[i] + " " + code
                        + " indx=" + QString::number( j ) + "\"><img "
                        "src=\"" + vnames[i] + "/" + code + "/" + QString::number( j )
                        + "/100/100\"><br>" + caption + "</a>\n";
            }
        }
        info += "      </td>\n";
    }
    info += "    </tr>\n";
    info += "  </table>\n";
    info += "  <p><a href=\"do createAllVisualizers " + code + "\">"
            "Show me all the views connected together in a sheet.</p>\n";
    info += "</blockquote>\n";
    //// DESCRIPTION SECTION
    info += "<a name=\"description\"><h2>Description</h2></a>\n";
    info += "<blockquote>\n";
    if ( QRegExp( "\\S" ).search( notes ) > -1 ) {
        info += "  <p>" + notes + "</p>\n";
    } else {
        info += "  <i>none</i>\n";
    }
    info += "</blockquote>\n";
    //// PROPERTIES SECTION
    info += "<a name=\"properties\"><h2>Computed properties</h2></a>\n";
    info += "<blockquote>\n";
    if ( ideas.size() ) {
        info += "  <table border=1>\n";
        for ( int i = 0 ; i < ideas.size() ; i++ ) if ( ideas[i].applies ) {
            QString title = ideas[i].factory->title;
            info += QString( trow3 )
                    .arg( title )
                    .arg( ideas[i].computedValue )
                    .arg( /*ideas[i].applies ?*/
                          /*
                          QString( "<a href=\"#relatedconcepts_%1\">"
                                   "tell me more...</a>" )
                          .arg( title.replace( " ", "_" ) )
                          */
                          "<a href=\"do openGroupComputation " + code + " " +
                              title + "\">tell me more</a>"
                          /*: ""*/
                          );
        }
        info += "  </table>\n";
    } else {
        info += "  <i>none</i>\n";
    }
    info += "</blockquote>\n";
    //// RELATED SHEETS SECTION
    info += "<a name=\"relatedsheets\"><h2>Related sheets</h2></a>\n";
    info += "<blockquote>\n";
    QStringList keys = GESheet::sheetsScanned();
    bool someSheetsRelate = FALSE;
    for ( int i = 0 ; i < keys.count() ; i++ ) {
        if ( GESheet::groupsInSheet( keys[i] ).contains( code ) ) {
            info += "  <p><a href=\"do openSheet " + keys[i] + "\">" +
                    keys[i] + "</a></p>\n";
            someSheetsRelate = TRUE;
        }
    }
    if ( !someSheetsRelate ) info += "  <i>none</i>\n";
    info += "</blockquote>\n";
    //// FILE DATA SECTION
    info += "<a name=\"filedata\"><h2>File data</h2></a>\n";
    info += "<blockquote>\n"
            "  <table border=1>\n";
    info += QString( trow )
            .arg( "Filename" )
            .arg( Q3StyleSheet::escape( fileInfo.fileName() ) );
    info += QString( trow )
            .arg( "Path" )
            .arg( Q3StyleSheet::escape( fileInfo.dirPath() ) );
    info += QString( trow )
            .arg( "Author" )
            .arg( Q3StyleSheet::escape( author ) );
    info += QString( trow )
            .arg( "Last modified" )
            .arg( Q3StyleSheet::escape( fileInfo.lastModified().toString() ) );
    info += "  </table>\n"
            "</blockquote>\n";
    //// OTHER DEFINITIONS SECTION
    info += "<a name=\"otherdefs\"><h2>Other definitions</h2></a>\n";
    info += "<blockquote>\n";
    if ( defs.size() <= 1 ) {
        info += "  <i>none</i>\n";
    } else {
        for ( int i = 1 ; i < defs.size() ; i++ ) {
            info += QString( "  <p>%1</p>\n" )
                    .arg( defs[i].HTML );
        }
    }
    info += "</blockquote>\n";
    //// GENERATORS SECTION
    info += "<a name=\"generators\"><h2>Generators</h2></a>\n";
    info += "<blockquote>\n";
    if ( !generatorlists.size() ) {
        info += "  <i>none</i>\n";
    } else if ( !generatorlists[0].count() ) {
        info += "  <p>There are no generators in this group, "
                "only the identity element.</p>\n";
    } else {
        for ( int i = 0 ; i < generatorlists.size() ; i++ ) {
            if ( generatorlists[i].count() > 1 )
                info += "  <p>The elements ";
            else
                info += "  <p>The element ";
            QStringList genstrs;
            for ( uint j = 0 ; j < generatorlists[i].count() ; j++ )
                genstrs << getRepresentation( generatorlists[i][j] ).HTML;
            info += naturalLanguageJoin( genstrs );
            if ( generatorlists[i].count() > 1 )
                info += " generate the group.</p>\n";
            else
                info += " generates the group.</p>\n";
        }
    }
    info += "</blockquote>\n";
    //// NAMING SCHEMES SECTION
    info += "<a name=\"namingschemes\"><h2>Naming schemes</h2></a>\n";
    info += "<blockquote>\n";
    info += "  <h3>Default element names</h3>\n";
    info += "  <blockquote>\n"
            "    <p>";
    for ( GElt g = 0 ; g < group.order; g++ ) {
        if ( g ) info += ", ";
        info += getRepresentation( g ).HTML;
    }
    if ( currentRep < reps.count() ) {
        info += "<br>This representation was loaded from the group file.";
    } else {
        info += "<br>This representation is user-defined; see below.";
    }
    info += "</p>\n"
            "  </blockquote>\n";
    info += "  <h3>Loaded naming schemes</h3>\n";
    info += "  <ol>\n";
    for ( int j = 0 ; j < reps.size() ; j++ ) {
        info += "    <li>";
        if ( currentRep != j ) {
            for ( uint i = 0 ; i < group.order; i++ ) {
                if ( i ) info += ",<br>";
                info += getRepresentation( i ).HTML + " = " + reps[j][i].HTML;
            }
            info += "<br>\n";
            info += QString( "      <p><a href=\"do chooseRep %1 %2\">Click here</a>"
                             "         to make this the default representation.</p>\n" )
                    .arg( code ).arg( j );
        }
        info += "</li>\n";
    }
    info += "  </ol>\n";
    info += "  <a name=\"user-defined-representations\">"
            "<h3>User-defined naming schemes</h3></a>\n";
    uint n = preferences->numGroupReps( code );
    if ( n ) {
        info += "  <ol>\n";
        for ( uint j = 0 ; j < n ; j++ ) {
            info += "    <a name=\"user-defined-representation-"
                    + QString::number( j ) + "\"><li>";
            if ( (uint)currentRep == reps.count() + j ) {
                info += "The default naming scheme shown above<br>"
                        "If you wish to edit or remove this representation, "
                        "first make a different representation the default.";
            } else {
                MathTextList rep = preferences->getGroupRep( code, j );
                for ( uint i = 0 ; i < group.order ; i++ ) {
                    if ( i ) info += ",<br>";
                    info += reps[0][i].HTML + " = " + rep[i].HTML;
                }
                info += "\n";
                info += QString( "      <p><a href=\"do chooseRep %1 %2\">Click here</a>"
                                 "         to make this the default "
                                 "         representation.</p>\n" )
                        .arg( code ).arg( j + reps.count() );
                info += QString( "  <p><a href=\"do editRep %1 %2\">Click here</a>"
                                 "     to edit this representation.</p>\n" )
                        .arg( code ).arg( j );
                info += QString( "  <p><a href=\"do removeRep %1 %2\">Click here</a>"
                                 "     to remove this representation.</p>\n" )
                        .arg( code ).arg( j );
            }
            info += "</li></a>\n";
        }
        info += "  </ol>\n";
    } else {
        info += "  <i>none</i>\n";
    }
    info += QString( "  <p><a href=\"do newRep %1\">Click here</a>"
                     "     to add a new representation for this group.</p>\n" )
            .arg( code );
    info += "</blockquote>\n";
    //// NOTES SECTION
    info += "<a name=\"notes\"><h2>Notes</h2></a>\n";
    if ( preferences ) {
        preferences->setGroup( "GroupNotes" );
        QString usernotes = preferences->getString( code, QString::null );
        info += "<blockquote>\n";
        if ( usernotes.isNull() ) {
            info += "  <p><i>none</i></p>\n";
        } else {
            info += "  <p>" + usernotes + "</p>\n";
        }
        info += "  <p><a href=\"do editNotes " + code
              + "\">Click to edit notes</a></p>\n";
        info += "</blockquote>\n";
    } else {
        info += "<blockquote><i>none</i></blockquote>\n";
    }
    //// RELATED CONCEPTS SECTION
    /*
    info += "<a name=\"relatedconcepts\"><h2>Related concepts</h2></a>\n";
    info += "<blockquote>\n";
    bool someIdeasApplied = FALSE;
    for ( uint i = 0 ; i < ideas.size() ; i++ ) {
        //if ( ideas[i].applies ) {
            QString title = ideas[i].factory->title;
            info += QString(
                    "  <a name=\"relatedconcepts_%1\"><h3>%2</h3></a>\n"
                    "  %3\n" ) // longDesc is a <p>...</p>
                    .arg( title.replace( " ", "_" ) )
                    .arg( QStyleSheet::escape( ideas[i].factory->question ) )
                    .arg( ideas[i].longDescription ); // longDesc is a <p>...</p>
            someIdeasApplied = TRUE;
        //}
    }
    if ( !someIdeasApplied ) info += "  <i>none</i>\n";
    info += "</blockquote>\n";
    */
    info += "<hr>\n"
            "<center>"
            "Contents: "
            "<a href=\"#facts\">Facts</a> | "
            "<a href=\"#views\">Views</a> | "
            "<a href=\"#description\">Description</a> | "
            "<a href=\"#properties\">Computed properties</a> | "
            "<a href=\"#relatedsheets\">Related sheets</a> | "
            "<a href=\"#filedata\">File data</a> | "
            "<a href=\"#otherdefs\">Other definitions</a> | "
            "<a href=\"#generators\">Generators</a> | "
            "<a href=\"#namingschemes\">Naming schemes</a> | "
            "<a href=\"#notes\">Notes</a>"
            //"<a href=\"#relatedconcepts\">Related concepts</a> | "
            "</center>\n";
    return info;
}
