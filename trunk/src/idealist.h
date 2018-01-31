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


#ifndef IDEALIST_H
#define IDEALIST_H


#include "idea.h"
#include "groupfile.h"
#include "gevisualizer.h"
#include <q3ptrlist.h>


//
// First, the classes that keep lists of ideas:
//

class GroupIdeas {
public:
    static Q3PtrList<IdeaFactory<GroupFile> >* listAll();
};

/*
class VisualizerIdeas {
public:
    static QPtrList<IdeaFactory<GEVisualizer> >* listAll();
};
*/


//
// Now, the idea classes themselves:
//

class AbelianGIF : public IdeaFactory<GroupFile> {
public:
    AbelianGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
};

/*
class AlternatingGIF : public IdeaFactory<GroupFile> {
public:
    AlternatingGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
};
*/

class ClassEquationGIF : public IdeaFactory<GroupFile> {
public:
    ClassEquationGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
};

class CyclicGIF : public IdeaFactory<GroupFile> {
public:
    CyclicGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
};

/*
class DihedralGIF : public IdeaFactory<GroupFile> {
public:
    DihedralGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
};

class DirectProductGIF : public IdeaFactory<GroupFile> {
public:
    DirectProductGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
};

class FrobeniusGIF : public IdeaFactory<GroupFile> {
public:
    FrobeniusGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
};
*/

class OrderClassesGIF : public IdeaFactory<GroupFile> {
public:
    OrderClassesGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
};

/*
class PlatonicSolidGIF : public IdeaFactory<GroupFile> {
public:
    PlatonicSolidGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
};

class QuaternionGIF : public IdeaFactory<GroupFile> {
public:
    QuaternionGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
};

class SemidirectProductGIF : public IdeaFactory<GroupFile> {
public:
    SemidirectProductGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
};
*/

class SolvableGIF : public IdeaFactory<GroupFile> {
public:
    SolvableGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
    QStringList findDecompositionFor ( GroupFile* gf );
};

class SubgroupsGIF : public IdeaFactory<GroupFile> {
public:
    SubgroupsGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
    static QString tableRow ( GroupFile& G, GEltListList subgroups, int i, int& error );
    static GroupFile* findSubgroupInLibrary ( GroupFile* G, GEltList subgroup,
                                              GEltList* partialMap = 0 );
    static GroupFile* findGroupInLibrary ( GroupFile* G, GEltList* isomorphism = 0 );
    static void computeQuotient ( Group& G, GEltList normalSgr,
                                  GroupFile* putQuotientHere, GEltList* quotientMap = 0 );
};

/*
class SymmetricGIF : public IdeaFactory<GroupFile> {
public:
    SymmetricGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
};
*/


/*
class LatticeOfSubgroupsGIF : public IdeaFactory<GroupFile> {
public:
    LatticeOfSubgroupsGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
};

class SubgroupEmbeddingGIF : public IdeaFactory<GroupFile> {
public:
    SubgroupEmbeddingGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
    static GroupFile* findSubgroupInLibrary ( GroupFile* G, GEltList subgroup );
};

class FirstIsoTheoremGIF : public IdeaFactory<GroupFile> {
public:
    FirstIsoTheoremGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
};
*/

class ZnmGIF : public IdeaFactory<GroupFile> {
public:
    ZnmGIF ();
    Idea<GroupFile> consider ( GroupFile & x );
};


#endif // IDEALIST_H


