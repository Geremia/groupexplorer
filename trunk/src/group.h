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
 *  The Group is the fundamental class of the Group Explorer project.
 *  It is a computer representation of an abstract mathematical
 *  object.  Though mathematical groups can be infinite, Group
 *  Explorer groups are always finite.
 *
 *  Objects of this class contain no information about the group,
 *  rather only its mathematical pattern.  A different class,
 *  GroupFile, will have a Group object as a field, and its other
 *  fields will contain all the information about that group loaded
 *  from a group definition file.
 */

#ifndef GROUP_H
#define GROUP_H


#include <q3memarray.h>
#include <q3valuelist.h>
#include <qdom.h>


typedef uint GElt;
typedef Q3MemArray<GElt> GEltList;
typedef Q3ValueList<GEltList> GEltListList;

GEltList iota ( uint upTo );
GEltList perm_inverse ( GEltList perm );

QString elt_list_to_string ( GEltList L );
QString elt_list_list_to_string ( GEltListList LL );
bool elt_subset ( GEltList S1, GEltList S2 );
bool elt_sets_are_equal ( GEltList S1, GEltList S2 );
bool elt_lists_are_equal ( GEltList L1, GEltList L2 );

QString elt_list_to_code ( GEltList L );
GEltList code_to_elt_list ( QString code );
QString elt_set_to_code ( GEltList S );
GEltList code_to_elt_set ( QString code );
QString elt_set_list_to_code ( GEltListList SL );
GEltListList code_to_elt_set_list ( QString code );

uint gcd ( uint a, uint b );


class Group {
    
public:
    uint order;
    GEltList mtable;
    void setSize ( int sz );
    
    Group ();
    Group ( int sz );
    Group ( Group& g );
    void setMTable ( GEltList newtable );
    
    void appendXMLTo ( QDomNode n );
    bool makeFromXML ( QDomNode n );
    
    GElt op ( GElt a, GElt b );
    GElt inv ( GElt a );
    GElt conj ( GElt a, GElt by );
    GElt pow ( GElt a, uint p );
    
    uint orderOf ( GElt a );
    GEltList orbitOf ( GElt a, bool includeIdentity = TRUE );
    GEltList orderClassOf ( GElt a );
    GEltList conjClassOf ( GElt a );
    GEltListList orderClasses ();
    GEltListList conjClasses ();
    GEltListList cosetsOf ( GEltList subgroup, bool leftCosets );
    bool isNormal ( GEltList subgroup );
    bool isNormal ( uint index );
    uint isPSubgroup ( GEltList subgroup ); // returns 0 for no, or p for yes
    uint isPSubgroup ( uint index ); // returns 0 for no, or p for yes
    GEltList subgroupGenBy ( GEltList gens );
    GEltList elements ();

    void generateStructuredSubgroup ( GEltList generators,
                                      GEltList& result, GEltList& dimensions,
                                      bool useRightMult = true );
    
    GEltListList subgroups (); // returns only proper nontrivial subgroups
    GEltList neededGens ( GEltList gens ); // remove unneeded ones
    GEltList findGens ( GEltList sgr ); // pick a minimal list of gens for subgroup
    GEltList findGens (); // pick a minimal list of gens for the group itself
    GEltList completeGens ( GEltList soFar, GEltList goal ); // findGens for rest of group
    
private:
    GEltList invs; // keep the results of inv & ord computations - speed
    GEltList ords;
    Q3MemArray<bool> isInTheOrbitOf; // entry[i*n+j] == is i in the orbit of j?
    GEltListList allSubgroups; // used by subgroups()
    GEltList normalOnes; // keep results of isNormal computations - speed
    GEltList pSubgroups; // keep results of isPSubgroup computations - speed
    
    void computeIsInTheOrbitOfMatrix ();
    void findSubgroupsAbove ( GEltList* s );
    GEltList expandByOneGenerator ( GEltList subgroup, GElt g, bool useRightMult = true );
    GElt findNextGen ( GEltList goal, GEltList goalgens, GEltList gendsofar,
                       GEltList gens, bool restriction );
    
};


#endif // GROUP_H
