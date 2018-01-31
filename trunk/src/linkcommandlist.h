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


#ifndef LINKCOMMANDLIST_H
#define LINKCOMMANDLIST_H


#include "linkcommand.h"
#include "mathtext.h"
#include "groupfile.h"
#include <qwidget.h>
#include <qbitarray.h>


//
// The class that keeps the list of link commands
//

class LinkCommands {
    
public:
    static LinkCommand* getNewCommand ( QStringList call, QWidget* originator );
    
};


//
// The link command classes themselves
//

// openGroup <codeName> [anchorToScrollTo]
class OpenGroupLC : public LinkCommand {
    
public:
    OpenGroupLC ( QStringList parameters, QWidget* originator );
    
    virtual void redo ();
    
};


// openGroupComputation <codeName> <title (may contain spaces)>
class OpenGroupComputationLC : public LinkCommand {

public:
    OpenGroupComputationLC (
        QStringList parameters, QWidget* originator );

    virtual void redo ();

};


// openHelp <docURL>
class OpenHelpLC : public LinkCommand {
    
public:
    OpenHelpLC ( QStringList parameters, QWidget* originator );
    
    virtual void redo ();
    
};


// editNotes <codeName>
class EditGroupNotesLC : public LinkCommand {
    
public:
    EditGroupNotesLC ( QStringList parameters, QWidget* originator );
    
    virtual void redo ();
    virtual void undo ();
    virtual bool belongsOnStack ();
    
private:
    QString oldnotes;
    QString newnotes;
    
};


// editRep <codeName> <userDefinedRepNumber>
class EditNameSchemeLC : public LinkCommand {
    
public:
    EditNameSchemeLC ( QStringList parameters, QWidget* originator );
    
    virtual void redo ();
    virtual void undo ();
    virtual bool belongsOnStack ();
    
private:
    MathTextList oldrep;
    MathTextList newrep;
    uint repNum;
    
};


// newRep <codeName>
class NewNameSchemeLC : public LinkCommand {
    
public:
    NewNameSchemeLC ( QStringList parameters, QWidget* originator );
    
    virtual void redo ();
    virtual void undo ();
    virtual bool belongsOnStack ();
    
private:
    MathTextList newrep;
    uint repNum;
    
};


// removeRep <codeName> <userDefinedRepNumber>
class RemoveNameSchemeLC : public LinkCommand {
    
public:
    RemoveNameSchemeLC ( QStringList parameters, QWidget* originator );
    
    virtual void redo ();
    virtual void undo ();
    virtual bool belongsOnStack ();
    
private:
    MathTextList oldrep;
    uint repNum;
    
};


// chooseRep <codeName> <anyRepNumber>
class ChooseNameSchemeLC : public LinkCommand {

public:
    ChooseNameSchemeLC ( QStringList parameters, QWidget* originator );

    virtual void redo ();
    virtual void undo ();
    virtual bool belongsOnStack ();

private:
    GroupFile* gf;
    uint numUserDefinedReps;
    uint numLoadedReps;
    uint oldRepNum;
    uint newRepNum;

};


// showVisualizer <typeName> <encoding--potentiallyVeryLong>
class ShowVisualizerLC : public LinkCommand {
    
public:
    ShowVisualizerLC ( QStringList parameters, QWidget* originator );
    
    virtual void redo ();
    
};


// openSheet <filename, maybe even with spaces>
class OpenSheetLC : public LinkCommand {

public:
    OpenSheetLC ( QStringList parameters, QWidget* originator );
    virtual void redo ();

};


// createClassEquation <codeName> <typeName>
class CreateCESheetLC : public LinkCommand {

public:
    CreateCESheetLC ( QStringList parameters, QWidget* originator );
    virtual void redo ();

};


// createSubgroupLattice <codeName> <typeName>
class CreateSLSheetLC : public LinkCommand {

public:
    CreateSLSheetLC ( QStringList parameters, QWidget* originator );
    virtual void redo ();

private:
    GEltListList getAllUpwardPaths ( GEltListList subgroups, QBitArray& inclusions,
                                     uint index = 0 );

};


// createEmbedding <sourceCodeName> <destCodeName> [imageSubsetCode] <typeName>
class CreateEmbeddingSheetLC : public LinkCommand {

public:
    CreateEmbeddingSheetLC ( QStringList parameters, QWidget* originator );
    virtual void redo ();

};


// createSES <codeName> <subsetCode> <typeName> <subgroupNumber>
class CreateSESLC : public LinkCommand {

public:
    CreateSESLC ( QStringList parameters, QWidget* originator );
    virtual void redo ();

};


// createAllVisualizers <codeName>
class CreateAllVisualizersLC : public LinkCommand {

public:
    CreateAllVisualizersLC ( QStringList parameters, QWidget* originator );
    virtual void redo ();

};


// createSolvableDecomposition <typeName> <baseGroupCode>
//                             (<nextGroupCode> <sgrIndexOfPrevGrEmbeddedInThisOne>
//                              <quotientGroupCode>)+
class CreateSolvableDecompLC : public LinkCommand {

public:
    CreateSolvableDecompLC ( QStringList parameters, QWidget* originator );
    virtual void redo ();

};


// showZnmIsomorphism <groupCode> <n> <m>
class ShowZnmIsomorphismLC : public LinkCommand {

public:
    ShowZnmIsomorphismLC ( QStringList parameters, QWidget* originator );
    virtual void redo ();

};


// showNoZnmIsomorphism <groupCode> <n> <m>
class ShowNoZnmIsomorphismLC : public LinkCommand {

public:
    ShowNoZnmIsomorphismLC ( QStringList parameters, QWidget* originator );
    virtual void redo ();

};


#endif // LINKCOMMANDLIST_H


