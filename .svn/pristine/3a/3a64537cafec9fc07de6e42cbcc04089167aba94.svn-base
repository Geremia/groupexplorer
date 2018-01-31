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


#ifndef SUBSETCOMPUTATIONS_H
#define SUBSETCOMPUTATIONS_H


#include "sympanewidgets.h"


/*
class MyComputation : public SubsetComputation
{
public:
    MyComputation ( SubsetListBox* lb, ... );
    virtual bool isSensible ();
    virtual QString description ();
public slots:
    virtual void performComputation ();
protected:
    ...
};
*/


class ConjClassesComputation : public SubsetComputation
{
public:
    ConjClassesComputation ( SubsetListBox* lb );
    virtual bool isSensible ();
    virtual QString description ();
public slots:
    virtual void performComputation ();
};


class OrdClassesComputation : public SubsetComputation
{
public:
    OrdClassesComputation ( SubsetListBox* lb );
    virtual bool isSensible ();
    virtual QString description ();
public slots:
    virtual void performComputation ();
};


class NormalizerComputation : public SubsetComputation
{
public:
    NormalizerComputation ( SubsetListBox* lb, GroupSubset* argument );
    virtual bool isSensible ();
    virtual QString description ();
public slots:
    virtual void performComputation ();
protected:
    GroupSubset* argument;
};


class ClosureComputation : public SubsetComputation
{
public:
    ClosureComputation ( SubsetListBox* lb, GroupSubset* argument );
    virtual bool isSensible ();
    virtual QString description ();
public slots:
    virtual void performComputation ();
protected:
    GroupSubset* argument;
};


class LeftCosetsComputation : public SubsetComputation
{
public:
    LeftCosetsComputation ( SubsetListBox* lb, GroupSubset* argument );
    virtual bool isSensible ();
    virtual QString description ();
public slots:
    virtual void performComputation ();
protected:
    GroupSubset* argument;
};


class RightCosetsComputation : public SubsetComputation
{
public:
    RightCosetsComputation ( SubsetListBox* lb, GroupSubset* argument );
    virtual bool isSensible ();
    virtual QString description ();
public slots:
    virtual void performComputation ();
protected:
    GroupSubset* argument;
};


class IntersectionComputation : public SubsetComputation
{
public:
    IntersectionComputation ( SubsetListBox* lb,
                              GroupSubset* argument1, GroupSubset* argument2 );
    virtual bool isSensible ();
    virtual QString description ();
public slots:
    virtual void performComputation ();
protected:
    GroupSubset* argument1;
    GroupSubset* argument2;
};


class UnionComputation : public SubsetComputation
{
public:
    UnionComputation ( SubsetListBox* lb,
                              GroupSubset* argument1, GroupSubset* argument2 );
    virtual bool isSensible ();
    virtual QString description ();
public slots:
    virtual void performComputation ();
protected:
    GroupSubset* argument1;
    GroupSubset* argument2;
};


class ElementwiseProductComputation : public SubsetComputation
{
public:
    ElementwiseProductComputation ( SubsetListBox* lb,
                              GroupSubset* argument1, GroupSubset* argument2 );
    virtual bool isSensible ();
    virtual QString description ();
public slots:
    virtual void performComputation ();
protected:
    GroupSubset* argument1;
    GroupSubset* argument2;
};


class MorphismPushComputation : public SubsetComputation
{
public:
    MorphismPushComputation ( GECanvasMorphism* throughThis, GroupSubset* pushMe );
    virtual bool isSensible ();
    virtual QString description ();
public slots:
    virtual void performComputation ();
protected:
    GECanvasMorphism* f;
    GroupSubset* S;
};


class MorphismPullComputation : public SubsetComputation
{
public:
    MorphismPullComputation ( GECanvasMorphism* throughThis, GroupSubset* pullMe );
    virtual bool isSensible ();
    virtual QString description ();
public slots:
    virtual void performComputation ();
protected:
    GECanvasMorphism* f;
    GroupSubset* S;
};


#endif // SUBSETCOMPUTATIONS_H


