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

/*
 *  The GroupFile represents all the information loaded from a group
 *  definition file, including the mathematical group itself (as an
 *  instance of the Group class) and all the associated data, like
 *  the author's name, the various definitions and representations,
 *  etc.).
 *
 *  This class provides methods for loading and saving groups to/from
 *  files, and thus contains routines relating to the group file format.
 *
 *  The data in a group's GroupFile object, loaded from the group definition file,
 *  plus the data on the group loaded from the user's config file, together
 *  form all the data the user has on the group.
 *
 */


#ifndef GROUPFILE_H
#define GROUPFILE_H


#include "group.h"
#include "mathtext.h"
#include "idea.h"
#include "glscenenode.h"
#include <q3valuelist.h>
#include <qfileinfo.h>
#include <qpixmap.h>
//Added by qt3to4:
#include <Q3MemArray>
#include <Q3PtrList>


class GEPreferences;


class SymmetryObject : public GLSceneNode
{

public:
    SymmetryObject ();
    SymmetryObject ( QString name );
    
    SymmetryObject* copy ();

    QString getName ();
    void appendXMLTo ( QDomNode n );
    bool makeFromXML ( QDomNode n );
    void changeBaseRadius ( double newRadius );

private:
    QString name;

    GLSceneNode* fixedNodes;
    GLSceneNode* movingNodes;
    Q3PtrList<GLSceneNode> nodes;
    double lastRadius;

    QDomNode loadedFrom;

    GEltList operElts;
    Q3MemArray<double> operDegs;
    Q3MemArray<double> operAxesX;
    Q3MemArray<double> operAxesY;
    Q3MemArray<double> operAxesZ;

    void setupInside ();

};


class CustomCayley
{

public:
    QString getName ();
    GEltList getArrows ();
    void getNodePosition ( GElt g, GLdouble& x, GLdouble& y, GLdouble& z );

    void appendXMLTo ( QDomNode n );
    bool makeFromXML ( QDomNode n );

private:
    QString name;
    GEltList arrows;
    Q3MemArray<GLdouble> xs;
    Q3MemArray<GLdouble> ys;
    Q3MemArray<GLdouble> zs;

};


class GroupFile {
    
public:
    QFileInfo fileInfo;
    QString phrasename;
    QString author;
    QString notes;
    Q3ValueList<MathText> names;
    Q3ValueList<MathTextList> reps;
    Q3ValueList<MathText> defs;
    int currentRep;
    Group group;
    GEltListList generatorlists;
    Q3ValueList<Idea<GroupFile> > ideas;
    Q3ValueList<SymmetryObject> symms;
    Q3ValueList<CustomCayley> cays;
    
    GroupFile ();
    GroupFile ( const GroupFile& gf );
    GroupFile ( QString fname );
    
    void appendXMLTo ( QDomNode n );
    bool makeFromXML ( QDomNode n );
    QString toString ();
    bool load ( QString fname );
    bool load ();
    bool save ( QString fname );
    bool save ();
    
    MathText getPrimaryName ();
    MathText getOtherNames ();
    MathText getPrimaryDefinition ();
    MathText getRepresentation ( GElt e );

    QString debugSubset ( GEltList lst ); // text representation of subset
    QString debugSubsets ( GEltListList lsts ); // text representation of subset array
    
    QString groupInfoHTML ( GEPreferences* preferences = 0 );
    
    static QString codeName ( QString primaryName, QString author );
    static QString decodeAuthor ( QString codeName );
    static QString decodePrimaryName ( QString codeName );
    
    QString codeName ();
    
};


#endif // GROUPFILE_H
