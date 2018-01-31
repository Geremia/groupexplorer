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


#ifndef SYMPANEWIDGETS_H
#define SYMPANEWIDGETS_H


#include "groupfile.h"
#include "utilpieces.h"
#include "gevisualizer.h"
#include <qstringlist.h>
#include <qtundo.h>
#include <qbitarray.h>
#include <q3ptrlist.h>
//Added by qt3to4:
#include <QContextMenuEvent>
#include <Q3ValueList>
#include <Q3MemArray>


class GroupMorphism {
    
public:
    GroupMorphism ( QString name, GroupFile* domain, GroupFile* codomain );
    GroupMorphism ();
    // defaults to x |-> e
    
    void setMap ( GElt fromThis, GElt toThis );
    void setMap ( GEltList fullMap );
    GElt lookup ( GElt g );
    GEltList getMap ();
    /*
    QString encodeMap ();
    void decodeMap ( QString code );
    */
    
    void setName ( QString name );
    QString getName ();
    QString getTextName ();
    void setDomain ( GroupFile* gf );
    GroupFile* getDomain ();
    void setCodomain ( GroupFile* gf );
    GroupFile* getCodomain ();
    QString getDomainName ();
    QString getCodomainName ();

    GEltList kernel ();
    GEltList image ();
    GEltList pushSubset ( GEltList s );
    GEltList pullSubset ( GEltList s );
    
private:
    GEltList map;
    QString symbol;
    GroupFile* domain;
    GroupFile* codomain;
    
};


class GroupSubset
{
    
public:
    enum SubsetType { Subgroup, UserDefined, EquivalenceClass, /*Image,*/ None };
    static SubsetType nextSubsetType ( SubsetType type );
    static SubsetType prevSubsetType ( SubsetType type );
    static QString subsetTypeName ( SubsetType type );
    static QString subsetTypeDescription ( SubsetType type );
    
    GroupSubset ();
    GroupSubset ( GroupFile* groupfile, QString symbol, SubsetType type,
                  GEltList elements, QString description );
    /*
    GroupSubset ( GroupFile* groupfile, GroupMorphism* morph,
                  GroupSubset* source, bool forward );
    */
    
    void setSymbol ( QString sym );
    QString getName ();
    QString getTextName ();
    void setDescription ( QString description );
    QString getDescription ();
    GroupFile* getGroup();
    void setType ( SubsetType t );
    SubsetType getType ();
    QString getShortExpression ();
    
    /*
    void markAsImage ( GroupMorphism* f, GroupSubset* src, bool fwd );
    // above function describes this subset as being the image of src under the
    // morphism f (or the inverse image if !fwd)
    bool isAnImage ();
    bool isAnInverseImage ();
    GEltList imageSource ();
    GroupMorphism* imageMap ();
    */
    
    void markAsCoset ( bool leftCoset, uint subgroupNumber );
    bool isCoset ();
    char cosetType (); // L or R
    uint subgroupNumberForCoset ();
    
    void setWhatsThisHTML ( QString text );
    QString getWhatsThisHTML ();
    QString getElementListHTML ();
    
    QString getRichListBoxItemHTML ();
    
    GEltList elements ();
    GEltList intersectionWith ( GroupSubset& s );
    GEltList unionWith ( GroupSubset& s );
    
private:
    GroupFile* gf;
    QString symbol;
    SubsetType type;
    GEltList elts;
    QString desc;
    QString html;
    /*
    bool imageIsForward;
    GroupMorphism* morphism;
    GEltList sourceSet;
    */
    char coset;
    uint subgroupOfThisCoset;
    
    void updateWhatsThisHTML ();
    
};

GroupSubset::SubsetType& operator++ ( GroupSubset::SubsetType& t );
GroupSubset::SubsetType& operator-- ( GroupSubset::SubsetType& t );


/*
class HighlightingData
{
    
public:
    HighlightingData ( GroupFile* gf = 0,
                       QStringList typeDescriptions = QStringList() );
    
    void setGroupFile ( GroupFile* gf );
    GroupFile* getGroupFile ();
    void setHighlightingTypes ( QStringList descriptions );
    QStringList getHighlightingTypes ();
    uint numHighlightingTypes ();
    QString highlightingType ( uint num );
    
    void clearAllHighlightings ();
    void clearHighlightingBy ( uint typeNum );
    void highlight ( GEltList elements, uint typeNum, QColor usingThis );
    void highlight ( GEltListList partition, uint typeNum );
    
    bool highlightingTypeUsed ( uint typeNum );
    bool isElementHighlightedBy ( GElt elt, uint typeNum );
    QColor elementHighlighting ( GElt elt, uint typeNum );

    QString debugRep ();
    
private:
    GroupFile* groupfile;
    QStringList typeDescs;
    // these two arrays are of size groupfile->group.order * typeDescs.count(),
    // and are indexed by [groupfile->group.order*DescIndex+ElementIndex].
    QBitArray highlightPresent;
    QMemArray<QColor> highlightColor;
    
    uint groupSize ();
    
};
*/


class SubsetListBoxItem : public RichListBoxItem
{
public:
    SubsetListBoxItem ( GroupSubset& gsub, Q3ListBox* listbox = 0 );
    SubsetListBoxItem ( GroupSubset& gsub, Q3ListBox* listbox, Q3ListBoxItem* after );
};


class SubsetListBox : public GEVLargeWidget
{
    Q_OBJECT
    
public:
    SubsetListBox ( GEVLarge* container = 0, GroupFile* gf = 0,
                    QWidget* parent = 0, const char* name = 0 );
    
    void reset ();
    void setGroupFile ( GroupFile* gf );
    void setHighlightTypes ( QStringList types );
    
    virtual void contextMenuEvent ( QContextMenuEvent* e );
    
    // generic routines for add/remove/get/set subsets:
    // (more tailored ones are below)
    uint nextSubsetNum ( GroupSubset::SubsetType type );
    uint addSubset ( GroupSubset S );
    void setSubset ( GroupSubset S, uint num );
    uint addSubset ( GEltList elts, GroupSubset::SubsetType type );
    void setSubset ( GEltList elts, GroupSubset::SubsetType type, uint num );
    void removeSubset ( GroupSubset::SubsetType type, uint num );
    GroupSubset getSubset ( GroupSubset::SubsetType type, uint num );
    uint getNumSubsets ( GroupSubset::SubsetType type );
    // convenience functions for partition pieces and [inverse] images:
    /*
    uint addImageSubset ( GroupMorphism* f, GroupSubset* src, bool fwd ); // ret idx
    void setImageSubset ( uint num, GroupMorphism* f, GroupSubset* src, bool fwd );
    */
    uint addPartition ( Q3ValueList<GroupSubset> pieces );
    uint partitionIndexFor ( GroupSubset& S );
    void removePartition ( uint num );
    uint getNumPartitions ();
    uint getNumPartitionPieces ( uint partnum );
    GroupSubset& getPartitionPiece ( uint partnum, uint piecenum );
    GEltListList getPartition ( uint partnum );

    // these functions execute appropriate change() calls to cause our set() function
    // to call functions like setSubset() or addPartition() or whatever.
    // we have these so that we can follow the correct visualizer design paradigm,
    // which requires we use the change() function to make changes to our OWN settings.
    void evokeChangeUDSubset ( GEltList newelements, uint index );
    void evokeRemoveUDSubset ( uint index );
    void evokeAddUDSubset ( GEltList elts );
    void evokeAddUDSubset ();
    void evokeRemovePartition ( uint index );
    void evokeAddConjClsPartition ();
    void evokeAddOrdClsPartition ();
    void evokeAddCosetsPartition ( bool left, uint subgroup );
    
    void addConjClsPartition ();
    void addOrdClsPartition ();
    void addCosetsPartition ( uint subgroup, bool leftCosets );
    
    /*
    QString encode ();
    void decode ( QString code );
    static void decodeHighlighting ( QString code, HighlightingData* intoThis );
    */
    virtual void set ( QString key, QString oldvalue, QString value, QString desc );
    virtual void updateContent ( QStringList changedKeys );

    static uint subsetNumber ( QString name );
    static QString subsetName ( char c, uint number );
    static QString subsetTextName ( char c, uint number );

signals:
    //void changed ( QString description );
    void getAllMorphismsToMe ( Q3PtrList<GECanvasMorphism>& list );
    void getAllMorphismsFromMe ( Q3PtrList<GECanvasMorphism>& list );

public slots:
    virtual void groupFileChanged ( GroupFile* gf );
    void doubleClickedItem ( Q3ListBoxItem* item );
    void suggestComputedSubset ( GEltList elts, QString computationDescription );

private:
    GroupFile* groupfile;
    QMap<GroupSubset::SubsetType, Q3ValueList<GroupSubset> > subsets;
    Q3MemArray<uint> firstIndexOfEachPartition;
    QMap<GroupSubset::SubsetType, RichListBoxItem*> headers;
    QStringList highlightTypes;
    /*
    QMemArray<GroupSubset::SubsetType> highlightSubsetType;
    QMemArray<int> highlightSubsetIndex;
    QValueList<QColor> highlightColor;
    */
    /*
    uint silence;
    bool dirty; // have we run encode() since last change?  dirty == no
    QString lastEncoding; // if dirty is false, this is still valid
    */
    Q3ListBox* lb;
    
    /*
    void change ( QString description );
    void increaseSilence ();
    void decreaseSilence ();
    */
    GroupSubset createSubset ( GEltList elts, GroupSubset::SubsetType type,
                               uint num );
    void makeNoneMarker ( GroupSubset::SubsetType type );
    int findUserDefinedSubset ( uint number );
    void getItemInformation ( Q3ListBoxItem* item, GroupSubset*& subset,
                              int& itemIndex, int& partitionIndex );
    void editSubsetContents ( uint userDefinedSubsetIndex );
    QString partitionName ( Q3ValueList<GroupSubset> pieces,
                            int startIndex = 0, int stopIndex = -1 );
    QString partitionName ( uint partIndex );
    QString partitionTextName ( Q3ValueList<GroupSubset> pieces,
                                int startIndex = 0, int stopIndex = -1 );
    QString partitionTextName ( uint partIndex );

};


class SubsetComputation : public QObject
{
    Q_OBJECT
    
public:
    SubsetComputation ( SubsetListBox* lb );
    
    virtual void addToMenu ( QMenu* m, int id = -1 );
    virtual bool isSensible ();
    virtual QString description ();

public slots:
    virtual void performComputation ();
    
protected:
    SubsetListBox* listBox;
    
};


#endif // SYMPANEWIDGETS_H

