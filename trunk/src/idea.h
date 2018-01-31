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


#ifndef IDEA_H
#define IDEA_H


#include <qstring.h>
#include <qstringlist.h>


template<class X> class IdeaFactory;

template<class X> class Idea {
public:
    Idea ();
    Idea ( X* subject, IdeaFactory<X>* factory );
    
    X* subject;
    IdeaFactory<X>* factory;
    
    bool applies;
    QString computedValue;
    QString longDescription;
    QString internalData; // a place to put information for internal purposes
};

template<class X> class IdeaFactory {
public:
    virtual ~IdeaFactory ();
    
    QString title;
    QString question;
    QStringList topics;
    
    virtual Idea<X> consider ( X & x );
    
    // Idea Factory Topics as static variables:
    static QString IFTAbelianGroups;
    static QString IFTAlternatingGroups;
    static QString IFTCenterOfAGroup;
    static QString IFTCentralizerOfASubgroup;
    static QString IFTClassEquation;
    static QString IFTCommutatorSubgroup;
    static QString IFTConjugacy;
    static QString IFTCyclicGroups;
    static QString IFTDihedralGroups;
    static QString IFTDirectProduct;
    static QString IFTFrobeniusGroups;
    static QString IFTGroupsOfSymmetry;
    static QString IFTIsomorphismTheorems;
    static QString IFTIsomorphismTheorem1;
    static QString IFTIsomorphismTheorem2;
    static QString IFTIsomorphismTheorem3;
    static QString IFTNormalizerOfASubgroup;
    static QString IFTNormalSubgroups;
    static QString IFTOrderOfAnElement;
    static QString IFTOrderOfASubgroup;
    static QString IFTPlatonicSolids;
    static QString IFTQuaternions;
    static QString IFTSemidirectProduct;
    static QString IFTSimpleGroups;
    static QString IFTSolvableGroups;
    static QString IFTSubgroups;
    static QString IFTSylowTheorems;
    static QString IFTSylowTheorem1;
    static QString IFTSylowTheorem2;
    static QString IFTSylowTheorem3;
    static QString IFTSymmetricGroups;
};


template<class X> Idea<X>::Idea () {
    subject = 0;
    factory = 0;
    applies = 0;
    computedValue = "";//"Error: blank idea";
    longDescription = "";//"Error: blank idea";
}

template<class X> Idea<X>::Idea ( X* subject, IdeaFactory<X>* factory )
    : subject( subject ), factory( factory )
{
    applies = 0;
    computedValue = "";//"Error: blank idea";
    longDescription = "";//"Error: blank idea";
}


//////////////////////////////////////////////////////////////////////////////////
//
// NOTE THAT TEMPLATE CLASSES MUST HAVE THE IMPLEMENTATION IN THE SAME FILE:
//


template<class X> IdeaFactory<X>::~IdeaFactory () { }

template<class X> Idea<X> IdeaFactory<X>::consider ( X & x ) {
    // I can't make this pure virtual, because QValueList wants to be able to
    // instantiate one.  So I make this dummy function with error messages.
    return Idea<X>( &x, this );
}


template<class X> QString
        IdeaFactory<X>::IFTAbelianGroups = "abelian groups";
template<class X> QString
        IdeaFactory<X>::IFTAlternatingGroups = "alternating groups";
template<class X> QString
        IdeaFactory<X>::IFTCenterOfAGroup = "center of a group";
template<class X> QString
        IdeaFactory<X>::IFTCentralizerOfASubgroup = "centralizer of a subgroup";
template<class X> QString
        IdeaFactory<X>::IFTClassEquation = "class equation";
template<class X> QString
        IdeaFactory<X>::IFTCommutatorSubgroup = "commutator subgroup";
template<class X> QString
        IdeaFactory<X>::IFTConjugacy = "conjugacy";
template<class X> QString
        IdeaFactory<X>::IFTCyclicGroups = "cyclic groups";
template<class X> QString
        IdeaFactory<X>::IFTDihedralGroups = "dihedral groups";
template<class X> QString
        IdeaFactory<X>::IFTDirectProduct = "direct product";
template<class X> QString
        IdeaFactory<X>::IFTFrobeniusGroups = "frobenius groups";
template<class X> QString 
        IdeaFactory<X>::IFTGroupsOfSymmetry = "groups of symmetry";
template<class X> QString
        IdeaFactory<X>::IFTIsomorphismTheorems = "Isomorphism Theorems";
template<class X> QString
        IdeaFactory<X>::IFTIsomorphismTheorem1 = "First Isomorphism Theorem";
template<class X> QString
        IdeaFactory<X>::IFTIsomorphismTheorem2 = "Second Isomorphism Theorem";
template<class X> QString
        IdeaFactory<X>::IFTIsomorphismTheorem3 = "Third Isomorphism Theorem";
template<class X> QString
        IdeaFactory<X>::IFTNormalizerOfASubgroup = "normalizer of a subgroup";
template<class X> QString
        IdeaFactory<X>::IFTNormalSubgroups = "normal subgroups";
template<class X> QString
        IdeaFactory<X>::IFTOrderOfAnElement = "order of an element";
template<class X> QString
        IdeaFactory<X>::IFTOrderOfASubgroup = "order of a subgroup";
template<class X> QString 
        IdeaFactory<X>::IFTPlatonicSolids = "platonic solids";
template<class X> QString 
        IdeaFactory<X>::IFTQuaternions = "quaternions";
template<class X> QString 
        IdeaFactory<X>::IFTSemidirectProduct = "semidirect product";
template<class X> QString 
        IdeaFactory<X>::IFTSimpleGroups = "simple groups";
template<class X> QString 
        IdeaFactory<X>::IFTSolvableGroups = "solvable groups";
template<class X> QString 
        IdeaFactory<X>::IFTSubgroups = "subgroups";
template<class X> QString 
        IdeaFactory<X>::IFTSylowTheorems = "Sylow Theorems";
template<class X> QString 
        IdeaFactory<X>::IFTSylowTheorem1 = "First Sylow Theorem";
template<class X> QString 
        IdeaFactory<X>::IFTSylowTheorem2 = "Second Sylow Theorem";
template<class X> QString 
        IdeaFactory<X>::IFTSylowTheorem3 = "Third Sylow Theorem";
template<class X> QString 
        IdeaFactory<X>::IFTSymmetricGroups = "symmetric groups";


#endif // IDEA_H
