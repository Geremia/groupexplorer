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


#include "sympanewidgets.h"
#include "subsetcomputations.h"
#include "subseteditor.h"
#include "gewhatsthis.h"
#include "gesheet.h"
#include <myutils.h>
#include <textreference.h>
#include <q3url.h>
#include <q3ptrlist.h>
#include <qmessagebox.h>
#include <qlayout.h>
//Added by qt3to4:
#include <QContextMenuEvent>
#include <Q3ValueList>
#include <Q3VBoxLayout>
//Added by me to help porting:
#include <q3mimefactory.h>


GroupSubset::SubsetType& operator++ ( GroupSubset::SubsetType& t )
{
    return t = GroupSubset::nextSubsetType( t );
}

GroupSubset::SubsetType& operator-- ( GroupSubset::SubsetType& t )
{
    return t = GroupSubset::prevSubsetType( t );
}

GroupSubset::SubsetType GroupSubset::nextSubsetType ( GroupSubset::SubsetType type )
{
    return ( type == GroupSubset::None ) ?
            GroupSubset::Subgroup : GroupSubset::SubsetType( type + 1 );
}

GroupSubset::SubsetType GroupSubset::prevSubsetType ( GroupSubset::SubsetType type )
{
    return ( type == GroupSubset::Subgroup ) ?
            GroupSubset::None : GroupSubset::SubsetType( type - 1 );
}

QString GroupSubset::subsetTypeName ( GroupSubset::SubsetType type )
{
    switch ( type ) {
    case Subgroup:          return "subgroup";
    case UserDefined:       return "user-defined";
    case EquivalenceClass:  return "partition";
    //case Image:             return "image";
    default:                return "none";
    }
}

QString GroupSubset::subsetTypeDescription ( GroupSubset::SubsetType type )
{
    switch ( type ) {
    case Subgroup:          return "Subgroups";
    case UserDefined:       return "User-defined subsets";
    case EquivalenceClass:  return "Partitions";
    //case Image:             return "Images and inverse images";
    default:                return "none";
    }
}

GroupSubset::GroupSubset ()
    : gf( 0 ), type( UserDefined ), /*morphism( 0 ),*/ coset( 'N' )
{
    updateWhatsThisHTML();
}

GroupSubset::GroupSubset (
        GroupFile* groupfile, QString symbol, SubsetType type, GEltList elements,
        QString description )
    : gf( groupfile ), symbol( symbol ), type( type ),
    elts( elements.copy() ), desc( description ), /*morphism( 0 ),*/ coset( 'N' )
{
    updateWhatsThisHTML();
}

/*
GroupSubset::GroupSubset ( GroupFile* groupfile, GroupMorphism* morph,
                           GroupSubset* source, bool forward )
    : gf( groupfile ), coset( 'N' )
{
    markAsImage( morph, source, forward );
}
*/

void GroupSubset::updateWhatsThisHTML ()
{
    setWhatsThisHTML(
        "<p><b>Elements of " + symbol + ":</b></p>\n<p>"
        + getElementListHTML() + "</p>\n" );
    /*
    if ( ( type == Image ) && morphism ) {
        QString extra;
        GEltList indom;
        GEltList incod;
        if ( imageIsForward ) {
            indom = sourceSet;
            incod = elts;
        } else {
            indom = elts;
            incod = sourceSet;
        }
        for ( uint i = 0 ; i < incod.size() ; i++ ) {
            for ( uint j = 0 ; j < indom.size() ; j++ ) {
                if ( morphism->lookup( indom[j] ) == incod[i] ) {
                    extra += QString( "%1(%2) = " ).arg( morphism->getName() )
                             .arg( morphism->getDomain()->getRepresentation(
                                     indom[j] ).HTML );
                }
            }
            extra += morphism->getCodomain()->getRepresentation(
                    incod[i] ).HTML + "<br>\n";
        }
        extra = QString( "<p>The map %1 creates %2 as follows.</p>\n<p>%3</p>" )
                .arg( morphism->getName() ).arg( symbol ).arg( extra );
        setWhatsThisHTML( getWhatsThisHTML() + extra );
    }
    */
}

void GroupSubset::setSymbol ( QString sym )
{
    symbol = sym;
}

void GroupSubset::setDescription ( QString description )
{
    desc = description;
}

QString GroupSubset::getDescription ()
{
    return desc;
}

void GroupSubset::setType ( GroupSubset::SubsetType t )
{
    type = t;
}

GroupSubset::SubsetType GroupSubset::getType ()
{
    return type;
}

QString GroupSubset::getShortExpression ()
{
    GEltList tmp;
    QString tmpdesc;
    switch ( type )
    {
        case Subgroup:
            tmp = gf->group.findGens( elts );
            for ( uint i = 0 ; i < tmp.size() ; i++ ) {
                if ( i ) tmpdesc += ", ";
                tmpdesc += gf->getRepresentation( tmp[i] ).HTML;
            }
            if ( !tmp.size() ) tmpdesc += gf->getRepresentation( 0 ).HTML;
            return symbol + " = &lt; " + tmpdesc + " &gt;";
        case UserDefined:
        case EquivalenceClass:
            for ( uint i = 0 ; i < QMIN( elts.size(), (uint)3 ) ; i++ ) {
                if ( i ) tmpdesc += ", ";
                tmpdesc += gf->getRepresentation( elts[i] ).HTML;
            }
            if ( elts.size() > 3 ) tmpdesc += ", ...";
            return symbol + " = { " + tmpdesc + " }";
        default:
            return symbol;
    }
}

/*
void GroupSubset::markAsImage ( GroupMorphism* f, GroupSubset* src, bool fwd )
{
    morphism = f;
    sourceSet = src->elements().copy();
    imageIsForward = fwd;
    elts = fwd ? f->pushSubset( sourceSet ) : f->pullSubset( sourceSet );
    setSymbol( QString( "%1%2[%3]" ).arg( f->getName() )
               .arg( fwd ? "" : "<sup>-1</sup>" ).arg( src->getName() ) );
    setDescription(
            QString( "the %1image of %2 under %3 : %4 -> %5" )
            .arg( fwd ? "" : "inverse " ).arg( src->getName() ).arg( f->getName() )
            .arg( f->getDomainName() ).arg( f->getCodomainName() ) );
    updateWhatsThisHTML();
}

bool GroupSubset::isAnImage ()
{
    return ( type == Image ) && imageIsForward;
}

bool GroupSubset::isAnInverseImage ()
{
    return ( type == Image ) && !imageIsForward;
}

GEltList GroupSubset::imageSource ()
{
    return sourceSet;
}

GroupMorphism* GroupSubset::imageMap ()
{
    return morphism;
}
*/

void GroupSubset::markAsCoset ( bool leftCoset, uint subgroupNumber )
{
    coset = ( leftCoset ? 'L' : 'R' );
    subgroupOfThisCoset = subgroupNumber;
}

bool GroupSubset::isCoset ()
{
    return ( ( coset == 'L' ) || ( coset == 'R' ) );
}

char GroupSubset::cosetType ()
{
    return coset;
}

uint GroupSubset::subgroupNumberForCoset ()
{
    return subgroupOfThisCoset;
}

void GroupSubset::setWhatsThisHTML ( QString text )
{
    html = text;
}

QString GroupSubset::getWhatsThisHTML ()
{
    return html;
}

QString GroupSubset::getElementListHTML ()
{
    if ( !elts.size() ) return "<i>(none)</i>";
    QStringList tmp;
    for ( uint i = 0 ; i < elts.size() ; i++ )
        tmp << ( "<nobr>" + gf->getRepresentation( elts[i] ).HTML + "</nobr>" );
    return naturalLanguageJoin( tmp );
}

QString GroupSubset::getName ()
{
    return symbol;
}

QString GroupSubset::getTextName ()
{
    return symbol.replace( "<i>", "" ).replace( "</i>", "" )
                 .replace( "<sub>", "_" ).replace( "</sub>", "" );
}

QString GroupSubset::getRichListBoxItemHTML ()
{
    return QString( "<nobr>%1 is %2.</nobr>" ).arg( getShortExpression() ).arg( desc );
}

GroupFile* GroupSubset::getGroup()
{
    return gf;
}

GEltList GroupSubset::elements ()
{
    return elts;
}

GEltList GroupSubset::intersectionWith ( GroupSubset& s )
{
    GEltList result( elts.size() );
    for ( uint i = 0 ; i < result.size() ; i++ ) {
        if ( s.elts.contains( result[i] ) ) {
            result[i] = result[result.size() - 1];
            result.resize( result.size() - 1 );
        }
    }
    return result;
}

GEltList GroupSubset::unionWith ( GroupSubset& s )
{
    GEltList result = elts.copy();
    for ( uint i = 0 ; i < s.elts.size() ; i++ ) {
        if ( !result.contains( s.elts[i] ) ) {
            result.resize( result.size() + 1 );
            result[result.size() - 1] = s.elts[i];
        }
    }
    return result;
}


SubsetListBoxItem::SubsetListBoxItem ( GroupSubset& gsub, Q3ListBox* listbox )
    : RichListBoxItem( gsub.getRichListBoxItemHTML(), listbox )
{
    setWhatsThisHTML( gsub.getWhatsThisHTML() );
}

SubsetListBoxItem::SubsetListBoxItem ( GroupSubset& gsub, Q3ListBox* listbox,
                                       Q3ListBoxItem* after )
    : RichListBoxItem( gsub.getRichListBoxItemHTML(), listbox, after )
{
    setWhatsThisHTML( gsub.getWhatsThisHTML() );
}


GroupMorphism::GroupMorphism ( QString name, GroupFile* domain, GroupFile* codomain )
    : symbol( name ), domain( domain ), codomain( codomain )
{
    if ( domain ) map.fill( 0, domain->group.order );
}

GroupMorphism::GroupMorphism ()
    : domain( 0 ), codomain( 0 )
{
}

void GroupMorphism::setName ( QString name )
{
    symbol = name;
}

void GroupMorphism::setDomain ( GroupFile* gf )
{
    domain = gf;
    if ( domain ) map.fill( 0, domain->group.order );
}

void GroupMorphism::setCodomain ( GroupFile* gf )
{
    codomain = gf;
    if ( domain ) map.fill( 0, domain->group.order );
}

void GroupMorphism::setMap ( GElt fromThis, GElt toThis )
{
    map[fromThis] = toThis;
}

void GroupMorphism::setMap ( GEltList fullmap )
{
    map = fullmap.copy();
}

GEltList GroupMorphism::getMap ()
{
    return map.copy();
}

/*
QString GroupMorphism::encodeMap ()
{
    return elt_list_to_code( map );
}

void GroupMorphism::decodeMap ( QString code )
{
    map = code_to_elt_list( code );
}
*/

QString GroupMorphism::getName ()
{
    return symbol;
}

QString GroupMorphism::getTextName ()
{
    return symbol.replace( "<i>", "" ).replace( "</i>", "" )
                 .replace( "<sub>", "_" ).replace( "</sub>", "" );
}

QString GroupMorphism::getDomainName ()
{
    return domain ? domain->getPrimaryName().HTML : QString( "NULL" );
}

QString GroupMorphism::getCodomainName ()
{
    return codomain ? codomain->getPrimaryName().HTML : QString( "NULL" );
}

GroupFile* GroupMorphism::getDomain ()
{
    return domain;
}

GroupFile* GroupMorphism::getCodomain ()
{
    return codomain;
}

GEltList GroupMorphism::kernel ()
{
    GEltList result( map.size() );
    uint next = 0;
    for ( uint i = 0 ; i < map.size() ; i++ )
        if ( !map[i] ) result[next++] = i;
    result.resize( next );
    return result;
}

GEltList GroupMorphism::image ()
{
    GEltList result;
    for ( uint i = 0 ; i < map.size() ; i++ ) {
        if ( !result.contains( map[i] ) ) {
            result.resize( result.size() + 1 );
            result[result.size() - 1] = map[i];
        }
    }
    return result;
}

GEltList GroupMorphism::pushSubset ( GEltList s )
{
    GEltList result;
    for ( uint i = 0 ; i < s.size() ; i++ ) {
        if ( ( s[i] < map.size() ) && 
             ( !result.size() || !result.contains( map[s[i]] ) ) ) {
            result.resize( result.size() + 1 );
            result[result.size() - 1] = map[s[i]];
        }
    }
    return result;
}

GEltList GroupMorphism::pullSubset ( GEltList s )
{
    GEltList result( map.size() );
    uint next = 0;
    for ( uint i = 0 ; i < map.size() ; i++ )
        if ( s.contains( map[i] ) ) result[next++] = i;
    result.resize( next );
    return result;
}

GElt GroupMorphism::lookup ( GElt g )
{
    return ( g < map.size() ) ? map[g] : 0;
}


SubsetListBox::SubsetListBox (
        GEVLarge* container, GroupFile* gf, QWidget* parent, const char* name )
    : GEVLargeWidget( container, parent, name )//, silence( 0 ), dirty( TRUE )
{
    Q3VBoxLayout* lay = new Q3VBoxLayout( this );
    lb = new Q3ListBox( this );
    lay->addWidget( lb );
    connect( lb, SIGNAL(doubleClicked(Q3ListBoxItem*)),
             this, SLOT(doubleClickedItem(Q3ListBoxItem*)) );

    static TextReference ref( resourcesPath() + "/help/whatsthis.data" );
    new GEListBoxWhatsThis( lb, ref.lookup( "SubsetListBox" ) );
    // create all the headers and mark their sections empty to start:
    for ( GroupSubset::SubsetType t = GroupSubset::Subgroup ;
          t < GroupSubset::None ; ++t ) {
        headers[t] = new RichListBoxItem(
            QString( "<nobr><h2><u>%1</u></h2></nobr>" )
            .arg( GroupSubset::subsetTypeDescription( t ) ), lb, (bool)FALSE );
        headers[t]->setWhatsThisHTML(
            ref.lookup( "SubsetListBox-header-" + GroupSubset::subsetTypeName( t ) ) );
        makeNoneMarker( t );
    }

    setGroupFile( gf );
}

void SubsetListBox::reset ()
{
    // erase everything:
    for ( GroupSubset::SubsetType t = GroupSubset::Subgroup ;
          t < GroupSubset::None ; ++t )
        while ( getNumSubsets( t ) ) removeSubset( t, 0 );
    // now fill with what belongs here:
    if ( groupfile ) {
        // throw in the trivial group (unless G=={1})...
        if ( groupfile->group.order > 1 ) {
            GEltList elts( 1 );
            elts[0] = 0;
            addSubset( elts, GroupSubset::Subgroup );
        }
        // ...every nontrivial, proper subgroup...
        for ( int i = 0 ; i < groupfile->group.subgroups().count() ; i++ ) {
            addSubset( groupfile->group.subgroups()[i], GroupSubset::Subgroup );
        }
        // ...and the group itself.
        GEltList elts( groupfile->group.order );
        for ( uint i = 0 ; i < groupfile->group.order ; i++ ) elts[i] = i;
        addSubset( elts, GroupSubset::Subgroup );
    }
}

void SubsetListBox::setGroupFile ( GroupFile* gf )
{
    if ( groupfile != gf ) { // only do all the work if it's needed
        groupfile = gf;
        reset();
    }
}

void SubsetListBox::setHighlightTypes ( QStringList types )
{
    highlightTypes = types;
}

void SubsetListBox::editSubsetContents ( uint userDefinedSubsetIndex )
{
    SubsetEditor se( &subsets[GroupSubset::UserDefined][userDefinedSubsetIndex], this );
    if ( se.exec() == QDialog::Accepted ) {
        evokeChangeUDSubset( se.getElements(), userDefinedSubsetIndex );
        //setSubset( se.getElements(), GroupSubset::UserDefined, userDefinedSubsetIndex );
    }
}

void SubsetListBox::getItemInformation ( Q3ListBoxItem* item, GroupSubset*& subset,
                                         int& itemIndex, int& partitionIndex )
{
    // this routine takes a list box item and assigns:
    //   subset = the subset represented by that item (0 if none)
    //   itemIndex = the index in the subset type (0-based, -1 if subset == 0)
    //   partitionIndex = the index of the item's partition (0-based,
    //     -1 if the item is not an equivalence class)
    subset = 0;
    itemIndex = partitionIndex = -1;
    if ( item ) {
        int clickedIndex = lb->index( item );
        GroupSubset::SubsetType type = GroupSubset::None;
        // find nearest header before clicked item
        for ( type = GroupSubset::EquivalenceClass;//Image ;
              lb->index( headers[type] ) > clickedIndex ;
              --type ) { }
        itemIndex = clickedIndex - lb->index( headers[type] ) - 1;
        // if they clicked a valid item, then remember it:
        if ( ( itemIndex >= 0 ) && ( itemIndex < (int)subsets[type].size() ) ) {
            subset = &subsets[type][itemIndex];
            if ( subset->getType() == GroupSubset::EquivalenceClass )
                partitionIndex = partitionIndexFor( *subset );
        }
    }
}

void SubsetListBox::doubleClickedItem ( Q3ListBoxItem* item )
{
    GroupSubset* clickedOn;
    int itemIndex;
    int partIndex;
    getItemInformation( item, clickedOn, itemIndex, partIndex );
    if ( clickedOn ) {
        if ( clickedOn->getType() == GroupSubset::UserDefined ) {
            editSubsetContents( itemIndex );
        } else {
            Q3WhatsThis::display( clickedOn->getWhatsThisHTML() );
        }
    }
}

/*
 *  Things that belong on the popup menu for the subset list box
 *
 *  Let X stand for the menu item clicked on, G for the group, and P for the partition
 *  to which X belongs, if any.
 *
 *  Popup menu items (in submenus, if applicable)   Conditions for appearing
 *  ---------------------------------------------   ------------------------
 *  Edit list of elements in X                      X is an S_i
 *  Delete X                                        "
 *      (remember to delete all references to it
 *       in markings, etc.)
 *  Delete P                                        X is in P (i.e. P exists)
 *  Create S_next                                   always
 *  Compute >   (submenu)
 *              all conjugacy classes CC_i          not yet done
 *              all order classes OC_i              "
 *              normalizer of X, Norm(X)
 *              closure of X, <X>                   X not a subgroup of G
 *              left cosets g_iX                    not yet done & X is a subgroup of G
 *              right cosets Xg_i                   not yet done & X is a subgroup of G
 *              kernel of f, Ker(f)                 one such menu item for every f:G->G'
 *                                                  as long as computation not yet done
 *              image of f, Im(f)                   one such menu item for every f:G'->G
 *                                                  as long as computation not yet done
 *              intersection (\cap) of X with > (submenu)
 *                      (include in submenu every subset for which the computation
 *                      has not yet been done)
 *              union (\cup) of X with > (submenu, as with intersection)
 *              elementwise product of X with > (submenu, as with previous two,
 *                      except note that this operation may not be commutative)
 *  Morphisms > (submenu)
 *              Push X through f into G'            one such menu item for every f:G->G'
 *                                                  as long as it has not yet been done
 *              Push [X's highlighting] through f into G'
 *                                                  one such menu item for every f:G->G'
 *                                                  as long as it has not yet been done;
 *                                                  note that this implies the previous
 *                                                  if it hasn't yet been done
 *              Pull X back through f into G
 *              Pull [X's highlighting] back through f into G
 *                  (just like previous two, but with inverse images)
 *              Push each P through f into G'
 *              Push [P's highlighting] through f into G'
 *                  (just like first two, but with partitions)
 *              Pull each P back through f into G
 *              Pull [P's highlighting] back through f into G
 *                  (just like previous two, but with inverse images)
 *  Highlight subset by > (submenu, with choices provided by highlightTypes)
 *                                                  always present, with 0-1 items checked
 *  Highlight partition by > (submenu, with choices provided by highlightTypes)
 *                                                  always present, with 0-1 items checked
 *  Clear all highlighting                          always
 */

void SubsetListBox::contextMenuEvent ( QContextMenuEvent* e )
{
    // let's try to figure out which subset they clicked on
    GroupSubset* clickedOn;
    int itemIndex;
    int partIndex;
    getItemInformation( lb->itemAt( e->pos() ), clickedOn, itemIndex, partIndex );
    // and make the menu
    QMenu menu( this );
    QAction* tmp;
    if ( clickedOn && ( clickedOn->getType() == GroupSubset::UserDefined ) ) {
        tmp = new QAction( "Edit list of elements in " + clickedOn->getTextName(), this );
        tmp->setData( 0 );
        menu.addAction( tmp );
        tmp = new QAction( "Delete " + clickedOn->getTextName(), this );
        tmp->setData( 1 );
        menu.addAction( tmp );
        /*
        menu.insertItem( new RichCustomMenuItem(
            "<nobr>Edit list of elements in " + clickedOn->getName() + "</nobr>" ), 0 );
        menu.insertItem( new RichCustomMenuItem(
            "<nobr>Delete " + clickedOn->getName() + "</nobr>" ), 1 );
        */
    }
    if ( partIndex > -1 ) {
        tmp = new QAction( "Delete partition { " + partitionTextName( partIndex )
                         + " }", this );
        tmp->setData( 2 );
        menu.addAction( tmp );
        //menu.insertItem( new RichCustomMenuItem(
        //    "<nobr>Delete partition { " + partitionName( partIndex ) + " }</nobr>" ), 2 );
    }
    tmp = new QAction( "Create " + subsetTextName( 'S',
                                                   nextSubsetNum( GroupSubset::UserDefined ) ),
                       this );
    tmp->setData( 3 );
    menu.addAction( tmp );
    //menu.insertItem( new RichCustomMenuItem( "<nobr>Create " +
    //    subsetName( 'S', nextSubsetNum( GroupSubset::UserDefined ) ) + "</nobr>" ), 3 );
    menu.insertSeparator();
    QMenu computemenu( this );
    int count = 100;
    (new ConjClassesComputation( this ))->addToMenu( &computemenu, count++ );
    (new OrdClassesComputation( this ))->addToMenu( &computemenu, count++ );
    (new NormalizerComputation( this, clickedOn ))->addToMenu( &computemenu, count++ );
    (new ClosureComputation( this, clickedOn ))->addToMenu( &computemenu, count++ );
    (new LeftCosetsComputation( this, clickedOn ))->addToMenu( &computemenu, count++ );
    (new RightCosetsComputation( this, clickedOn ))->addToMenu( &computemenu, count++ );
    QMenu intersectionmenu( this );
    if ( clickedOn ) {
        for ( GroupSubset::SubsetType t = GroupSubset::Subgroup ;
              t < GroupSubset::None ; ++t ) {
            for ( int i = 0 ; i < subsets[t].size() ; i++ ) {
                if ( clickedOn != &subsets[t][i] ) {
                    (new IntersectionComputation( this, clickedOn, &subsets[t][i] ))
                        ->addToMenu( &intersectionmenu, count++ );
                }
            }
        }
    }
    if ( intersectionmenu.count() )
        computemenu.insertItem( "an intersection", &intersectionmenu, 9999 );
    QMenu unionmenu( this );
    if ( clickedOn ) {
        for ( GroupSubset::SubsetType t = GroupSubset::Subgroup ;
              t < GroupSubset::None ; ++t ) {
            for ( int i = 0 ; i < subsets[t].size() ; i++ ) {
                if ( clickedOn != &subsets[t][i] ) {
                    (new UnionComputation( this, clickedOn, &subsets[t][i] ))
                        ->addToMenu( &unionmenu, count++ );
                }
            }
        }
    }
    if ( unionmenu.count() )
        computemenu.insertItem( "a union", &unionmenu, 9999 );
    QMenu elemprodmenu( this );
    if ( clickedOn ) {
        for ( GroupSubset::SubsetType t = GroupSubset::Subgroup ;
              t < GroupSubset::None ; ++t ) {
            for ( int i = 0 ; i < subsets[t].size() ; i++ ) {
                if ( clickedOn != &subsets[t][i] ) {
                    (new ElementwiseProductComputation( this, clickedOn, &subsets[t][i] ))
                        ->addToMenu( &elemprodmenu, count++ );
                }
            }
        }
    }
    if ( elemprodmenu.count() )
        computemenu.insertItem( "an elementwise product", &elemprodmenu, 9999 );
    if ( computemenu.count() )
        menu.insertItem( "Compute", &computemenu, 9999 );
    QMenu morphismmenu( this );
    if ( clickedOn ) {
        Q3PtrList<GECanvasMorphism> connectedToMe;
        emit getAllMorphismsToMe( connectedToMe );
        for ( uint i = 0 ; i < connectedToMe.count() ; i++ )
            ( new MorphismPullComputation( connectedToMe.at( i ), clickedOn ) )
                ->addToMenu( &morphismmenu, count++ );
        connectedToMe.clear();
        emit getAllMorphismsFromMe( connectedToMe );
        for ( uint i = 0 ; i < connectedToMe.count() ; i++ )
            ( new MorphismPushComputation( connectedToMe.at( i ), clickedOn ) )
                ->addToMenu( &morphismmenu, count++ );
    }
    if ( morphismmenu.count() )
        menu.insertItem( "Morphisms", &morphismmenu, 9999 );
    QMenu phigh( this );
    if ( clickedOn ) {
        QMenu* high = menu.addMenu( "Highlight item by" );
        for ( int i = 0 ; i < highlightTypes.count() ; i++ ) {
            int pos = highlightTypes[i].indexOf( ":" );
            tmp = new QAction( highlightTypes[i].mid( pos + 1 ), this );
            tmp->setIcon( QIcon( qPixmapFromMimeSource( highlightTypes[i].left( pos ) ) ) );
            tmp->setData( 300 + i );
            high->addAction( tmp );
            //high.insertItem( new RichCustomMenuItem(
            //    "<nobr>" + highlightTypes[i] + "</nobr>" ), 300 + i );
        }
    }
    if ( partIndex > -1 ) {
        QMenu* phigh = menu.addMenu( "Highlight partition by" );
        for ( int i = 0 ; i < highlightTypes.count() ; i++ ) {
            int pos = highlightTypes[i].indexOf( ":" );
            tmp = new QAction( highlightTypes[i].mid( pos + 1 ), this );
            tmp->setIcon( QIcon( qPixmapFromMimeSource( highlightTypes[i].left( pos ) ) ) );
            tmp->setData( 350 + i );
            phigh->addAction( tmp );
            //phigh.insertItem( new RichCustomMenuItem(
            //    "<nobr>" + highlightTypes[i] + "</nobr>" ), 350 + i );
        }
    }
    menu.insertItem( "Clear all highlighting", 399 );
    // show it to the user, and act on their response:
    QAction* resultAction = menu.exec( e->globalPos() );
    int result = ( resultAction == NULL ) ? -1 : resultAction->data().toInt();
    switch ( result ) {
    case 0:
        editSubsetContents( itemIndex );
        break;
    case 1:
        evokeRemoveUDSubset( itemIndex );
        break;
    case 2:
        evokeRemovePartition( partIndex );
        break;
    case 3:
        evokeAddUDSubset();
        break;
    default:
        if ( ( result >= 100 ) && ( result < 200 ) ) {
            // then it gets handled by the computation class's slot
            // so we remain silent
            break;
        } else if ( ( result >= 300 ) && ( result < 350 ) ) {
            change( QString( "hig%1" ).arg( result - 300 ),
                    elt_set_to_code( clickedOn->elements() ),
                    "highlight " + clickedOn->getName() );
        } else if ( ( result >= 350 ) && ( result < 399 ) ) {
            change( QString( "hig%1" ).arg( result - 350 ),
                    elt_set_list_to_code( getPartition( partIndex ) ),
                    "highlight " + partitionName( partIndex ) );
        } else if ( result == 399 ) {
            for ( int i = 0 ; i < highlightTypes.count() ; i++ )
                change( QString( "hig%1" ).arg( i ), QString::null,
                        "remove highlighting" );
        } // else they hit escape or cancelled
    }
}

void SubsetListBox::set ( QString key, QString/* oldvalue*/,
                          QString value, QString/* desc*/ )
{
    // keys will either be hig# (highlighting), par# (partitions) or sub# (UD subsets).
    // so we grab the number first, and make sure we've actually done so legitimately:
    bool ok;
    uint index = key.mid( 3 ).toUInt( &ok );
    if ( ok ) {
        if ( key.startsWith( "sub" ) ) {
            // this is the code of a user-defined subset.
            // it will be in the form of a bit-vector of elements (0110110=1,2,4,5).
            // if it is empty, that means to remove the subset
            // index is the subscript index on the subset name; so let's find it:
            int num = findUserDefinedSubset( index );
            if ( value.isEmpty() ) {
                removeSubset( GroupSubset::UserDefined, num );
            } else {
                GEltList elements;
                for ( int j = 0 ; j < value.length() ; j++ ) {
                    if ( value[j] == '1' ) {
                        elements.resize( elements.size() + 1 );
                        elements[elements.size() - 1] = j;
                    }
                }
                // is this a new subset or an old one?  check:
                if ( num > -1 ) {
                    // findUserDefinedSubset() (above) gave a valid value; it exists:
                    setSubset( elements, GroupSubset::UserDefined, index );
                } else {
                    // num is an invalid value; subset does not yet exist:
                    addSubset( elements, GroupSubset::UserDefined );
                }
            }
        } else if ( key.startsWith( "par" ) ) {
            // this is the code of a partition.
            // it will either be C (conj classes), O (order classes) or
            // L# (left cosets) or R# (right cosets) with #=the number of the subgroup.
            // if it is null, it means to remove; otherwise, it means to add (because
            // there is no interface for editing partitions).
            if ( value.isEmpty() ) {
                removePartition( index );
            } else if ( value == "C" ) {
                addConjClsPartition();
            } else if ( value == "O" ) { // not zero, but 'oh'
                addOrdClsPartition();
            } else if ( ( value[0] == 'L' ) || ( value[0] == 'R' ) ) {
                addCosetsPartition( value.mid( 1 ).toInt(), value[0] == 'L' );
            }
        }
    }
}

void SubsetListBox::updateContent ( QStringList/* changedKeys*/ )
{
    // probably needs do nothing, due to nature of this widget.
    // only thing that doesn't update automatically herein is highlighting,
    // but that will be handled via signals/slots into the display GEVLargeWidget.
}

void SubsetListBox::groupFileChanged ( GroupFile* gf )
{
    setGroupFile( gf );
}

uint SubsetListBox::getNumSubsets ( GroupSubset::SubsetType type )
{
    return subsets[type].count();
}

uint SubsetListBox::addSubset ( GroupSubset S )
{
    uint num = subsets[S.getType()].count();
    // if there are no subsets of this type, remove the "none" marker before adding:
    if ( !num ) delete headers[S.getType()]->next();
    uint insertAfterThis =
            ( ( S.getType() == GroupSubset::EquivalenceClass/*Image*/ ) ? lb->count() :
              lb->index( headers[GroupSubset::nextSubsetType( S.getType() )] ) ) - 1;
    subsets[S.getType()].append( S );
    new SubsetListBoxItem( S, lb, lb->item( insertAfterThis ) );
    return num;
}

void SubsetListBox::setSubset ( GroupSubset S, uint num )
{
    subsets[S.getType()][num] = S;
    Q3ListBoxItem* old = lb->item( lb->index( headers[S.getType()] ) + 1 + num );
    new SubsetListBoxItem( S, lb, old );
    delete old;
}

uint SubsetListBox::nextSubsetNum ( GroupSubset::SubsetType type )
{
    uint result = subsets[type].count();
    // if the type is user-defined, find the smallest number available for subscript:
    if ( type == GroupSubset::UserDefined ) {
        QBitArray used( subsets[type].count() );
        used.fill( FALSE );
        for ( int i = 0 ; i < subsets[type].count() ; i++ ) {
            int num = subsetNumber( subsets[type][i].getName() );
            if ( num < subsets[type].count() ) used.setBit( num );
        }
        for ( int i = 0 ; i < subsets[type].count() ; i++ ) {
            if ( !used[i] ) {
                result = i;
                break;
            }
        }
    }
    return result;
}

uint SubsetListBox::addSubset ( GEltList elts, GroupSubset::SubsetType type )
{
    return addSubset( createSubset( elts, type, nextSubsetNum( type ) ) );
}

void SubsetListBox::setSubset ( GEltList elts, GroupSubset::SubsetType type,
                                uint num )
{
    setSubset( createSubset( elts, type, num ), num );
}

void SubsetListBox::makeNoneMarker ( GroupSubset::SubsetType type )
{
    RichListBoxItem* tmp =
            new RichListBoxItem( "<i>(none)</i>", lb, headers[type], (bool)FALSE );
    tmp->setWhatsThisHTML(
            QString( "<p>There are currently no subsets "
                     "in the \"%1\" category.</p>" )
            .arg( GroupSubset::subsetTypeName( type ) ) );
}

void SubsetListBox::removeSubset ( GroupSubset::SubsetType type, uint num )
{
    QString name = subsets[type][num].getName();
    subsets[type].remove( subsets[type].at( num ) );
    delete lb->item( lb->index( headers[type] ) + 1 + num );
    if ( !subsets[type].count() ) makeNoneMarker( type );
}

GroupSubset SubsetListBox::getSubset ( GroupSubset::SubsetType type, uint num )
{
    return subsets[type][num];
}

QString SubsetListBox::subsetName ( char c, uint number )
{
    return QString( "<i>%1</i><sub>%2</sub>" ).arg( c ).arg( number );
}

QString SubsetListBox::subsetTextName ( char c, uint number )
{
    return QString( "%1_%2" ).arg( c ).arg( number );
}

uint SubsetListBox::subsetNumber( QString name )
{
    return name.mid( 2 ).toUInt();//name.mid( 13, name.length() - 19 ).toUInt();
}

int SubsetListBox::findUserDefinedSubset ( uint number )
{
    QString lookingFor = subsetName( 'S', number );
    for ( uint i = 0 ; i < getNumSubsets( GroupSubset::UserDefined ) ; i++ )
        if ( getSubset( GroupSubset::UserDefined, i ).getName() == lookingFor )
            return i;
    return -1;
}

GroupSubset SubsetListBox::createSubset (
        GEltList elts, GroupSubset::SubsetType type, uint num )
{
    bool proper = ( elts.size() < groupfile->group.order );
    QString tmp;
    switch ( type ) {
    case GroupSubset::Subgroup:
        if ( elts.size() == 1 )
            tmp = QString( "the trivial subgroup { %1 }" )
                  .arg( groupfile->getRepresentation( 0 ).HTML );
        else if ( proper )
            tmp = QString( "a subgroup of order %1" ).arg( elts.size() );
        else
            tmp = "the group itself";
        return GroupSubset( groupfile, subsetName( 'H', num ),
                            GroupSubset::Subgroup, elts, tmp );
    case GroupSubset::UserDefined:
        if ( !elts.size() || !proper )
            tmp = QString( "the subset of size %1" ).arg( elts.size() );
        else
            tmp = QString( "a subset of size %1" ).arg( elts.size() );
        return GroupSubset( groupfile, subsetName( 'S', num ),
                            GroupSubset::UserDefined, elts, tmp );
    default:
        return GroupSubset( groupfile, "SYMBOL NOT GIVEN YET",
                            type, elts, "DESC NOT GIVEN YET" );
    }
}

/*
uint SubsetListBox::addImageSubset ( GroupMorphism* f, GroupSubset* src, bool fwd )
{
    GEltList dummy;
    GroupSubset S( groupfile, "", GroupSubset::Image, dummy, "" );
    S.markAsImage( f, src, fwd );
    return addSubset( S );
}

void SubsetListBox::setImageSubset ( uint num, GroupMorphism* f, GroupSubset* src,
                                     bool fwd )
{
    GEltList dummy;
    GroupSubset S( groupfile, "", GroupSubset::Image, dummy, "" );
    S.markAsImage( f, src, fwd );
    setSubset( S, num );
}
*/

QString SubsetListBox::partitionName (
    Q3ValueList<GroupSubset> pieces, int startIndex, int stopIndex )
{
    QString result;
    if ( stopIndex == -1 ) stopIndex = pieces.count() - 1;
    if ( stopIndex - startIndex >= 0 ) {
        result = pieces[startIndex].getName();
        if ( stopIndex - startIndex > 0 ) {
            if ( stopIndex - startIndex > 1 ) result += ", ...";
            result += ", " + pieces[stopIndex].getName();
        }
    }
    return result;
}

QString SubsetListBox::partitionName ( uint partIndex )
{
    uint stop = subsets[GroupSubset::EquivalenceClass].count() - 1;
    if ( partIndex + 1 < firstIndexOfEachPartition.size() )
        stop = firstIndexOfEachPartition[partIndex + 1] - 1;
    return partitionName( subsets[GroupSubset::EquivalenceClass],
                          firstIndexOfEachPartition[partIndex], stop );
}

QString SubsetListBox::partitionTextName (
    Q3ValueList<GroupSubset> pieces, int startIndex, int stopIndex )
{
    QString result;
    if ( stopIndex == -1 ) stopIndex = pieces.count() - 1;
    if ( stopIndex - startIndex >= 0 ) {
        result = pieces[startIndex].getTextName();
        if ( stopIndex - startIndex > 0 ) {
            if ( stopIndex - startIndex > 1 ) result += ", ...";
            result += ", " + pieces[stopIndex].getTextName();
        }
    }
    return result;
}

QString SubsetListBox::partitionTextName ( uint partIndex )
{
    uint stop = subsets[GroupSubset::EquivalenceClass].count() - 1;
    if ( partIndex + 1 < firstIndexOfEachPartition.size() )
        stop = firstIndexOfEachPartition[partIndex + 1] - 1;
    return partitionTextName( subsets[GroupSubset::EquivalenceClass],
                              firstIndexOfEachPartition[partIndex], stop );
}

uint SubsetListBox::addPartition ( Q3ValueList<GroupSubset> pieces )
{
    uint num = firstIndexOfEachPartition.size();
    if ( pieces.count() ) {
        firstIndexOfEachPartition.resize( num + 1 );
        firstIndexOfEachPartition[num] =
                subsets[GroupSubset::EquivalenceClass].count();
        for ( int i = 0 ; i < pieces.count() ; i++ ) {
            GroupSubset S = pieces[i];
            S.setType( GroupSubset::EquivalenceClass );
            addSubset( S );
        }
    }
    return num; // valid return value doesn't mean anything was inserted...take note
}

uint SubsetListBox::partitionIndexFor ( GroupSubset& S )
{
    // first find the location of the subset in question; call it i:
    int i;
    for ( i = 0 ; i < subsets[GroupSubset::EquivalenceClass].count() ; i++ )
        if ( subsets[GroupSubset::EquivalenceClass][i].getName() == S.getName() )
            break; // and then i holds the location, if it is small enough
    // if you couldn't find it, return an invalid location:
    if ( i >= subsets[GroupSubset::EquivalenceClass].count() )
        return firstIndexOfEachPartition.size();
    // ok so now go find the partition to which it belongs:
    for ( uint j = 1 ; j < firstIndexOfEachPartition.size() ; j++ )
        if ( firstIndexOfEachPartition[j] > (uint)i ) return j - 1;
    // it must be that S is in the last partition:
    return firstIndexOfEachPartition.size() - 1;
}

void SubsetListBox::removePartition ( uint num )
{
    uint start = firstIndexOfEachPartition[num];
    uint length = getNumPartitionPieces( num );
    for ( uint i = 0 ; i < length ; i++ )
        removeSubset( GroupSubset::EquivalenceClass, start );
    for ( uint i = num + 1 ; i < firstIndexOfEachPartition.size() ; i++ )
        firstIndexOfEachPartition[i - 1] = firstIndexOfEachPartition[i] - length;
    firstIndexOfEachPartition.resize( firstIndexOfEachPartition.size() - 1 );
}

uint SubsetListBox::getNumPartitions ()
{
    return firstIndexOfEachPartition.size();
}

uint SubsetListBox::getNumPartitionPieces ( uint partnum )
{
    uint next = ( partnum < firstIndexOfEachPartition.size() - 1 ) ?
                firstIndexOfEachPartition[partnum + 1] :
                subsets[GroupSubset::EquivalenceClass].size();
    return next - firstIndexOfEachPartition[partnum];
}

GroupSubset& SubsetListBox::getPartitionPiece ( uint partnum, uint piecenum )
{
    return subsets[GroupSubset::EquivalenceClass][firstIndexOfEachPartition[partnum]
                                                  + piecenum];
}

GEltListList SubsetListBox::getPartition ( uint partnum )
{
    GEltListList result;
    for ( uint i = 0 ; i < getNumPartitionPieces( partnum ) ; i++ )
        result.append( getPartitionPiece( partnum, i ).elements().copy() );
    return result;
}

void SubsetListBox::evokeChangeUDSubset ( GEltList newelements, uint index )
{
    GroupSubset S = subsets[GroupSubset::UserDefined][index];
    change( QString( "sub%1" ).arg( subsetNumber( S.getName() ) ),
            elt_set_to_code( newelements ), "change elements of " + S.getName() );
}

void SubsetListBox::evokeRemoveUDSubset ( uint index )
{
    GroupSubset S = subsets[GroupSubset::UserDefined][index];
    change( QString( "sub%1" ).arg( subsetNumber( S.getName() ) ), QString::null,
            "delete " + S.getName() );
}

void SubsetListBox::evokeAddUDSubset ( GEltList elts )
{
    uint num = nextSubsetNum( GroupSubset::UserDefined );
    change( QString( "sub%1" ).arg( num ), elt_set_to_code( elts ),
            "add subset " + subsetName( 'S', num ) );
}

void SubsetListBox::evokeAddUDSubset ()
{
    uint num = nextSubsetNum( GroupSubset::UserDefined );
    change( QString( "sub%1" ).arg( num ), elt_set_to_code( GEltList() ),
            "add subset " + subsetName( 'S', num ) );
}

void SubsetListBox::evokeRemovePartition ( uint index )
{
    change( QString( "par%1" ).arg( index ), QString::null,
            "remove partition " + partitionName( index ) );
}

void SubsetListBox::evokeAddConjClsPartition ()
{
    change( QString( "par%1" ).arg( getNumPartitions() ), "C",
            "add conjugacy class partition" );
}

void SubsetListBox::evokeAddOrdClsPartition ()
{
    change( QString( "par%1" ).arg( getNumPartitions() ), "O",
            "add order class partition" );
}

void SubsetListBox::evokeAddCosetsPartition ( bool left, uint subgroup )
{
    change( QString( "par%1" ).arg( getNumPartitions() ),
            QString( "%1%2" ).arg( left ? "L" : "R" ).arg( subgroup ),
            QString( "add %1 cosets of %2 partition" )
            .arg( left ? "left" : "right" )
            .arg( subsets[GroupSubset::Subgroup][subgroup].getName() ) );
}

void SubsetListBox::suggestComputedSubset (
        GEltList elts, QString computationDescription )
{
    // first, make a list of the things in elts, for communication w/the user:
    QString eltslisted;
    for ( uint i = 0 ; i < elts.size() ; i++ ) {
        if ( i ) eltslisted += ", ";
        eltslisted += groupfile->getRepresentation( elts[i] ).HTML;
    }
    // next, find the list of already-existing subsets that elts is equivalent to:
    Q3PtrList<GroupSubset> lst;
    for ( GroupSubset::SubsetType t = GroupSubset::Subgroup ;
          t < GroupSubset::None ; ++t ) {
        for ( int i = 0 ; i < subsets[t].count() ; i++ ) {
            if ( elt_sets_are_equal( subsets[t][i].elements(), elts ) ) {
                lst.append( &subsets[t][i] );
            }
        }
    }
    // and put that list into a QStringList for I/O w/the user:
    QStringList equivalentSubsets;
    for ( uint i = 0 ; i < lst.count() ; i++ )
        equivalentSubsets << lst.at( i )->getName();
    // now tell the user about the computation result, and whether it's equivalent to
    // any already-existing subsets, and see if they want to save it as a new user-
    // defined subset or not:
    if ( QMessageBox::question(
            this,
            "Computation completed",
            QString( "<p>The subset { %1 } of %2 was computed as follows.</p>\n"
                     "<p>%2</p>\n"
                     "%3"
                     "%4"
                     "<p>Would you like to add this subset "
                     "to the list as %5?</p>" )
            .arg( eltslisted )
            .arg( groupfile->getPrimaryName().HTML )
            .arg( computationDescription )
            .arg( lst.isEmpty() ?
                  QString( "<p><font color='green'>This subset does not appear anywhere "
                           "on the list of subsets yet.</font></p>" ) :
                  QString( "<p><font color='red'>This subset is equivalent to %1."
                           "</font></p>" )
                  .arg( naturalLanguageJoin( equivalentSubsets ) ) )
            .arg( subsetName( 'S', nextSubsetNum( GroupSubset::UserDefined ) ) ),
            "Yes, add a new subset",
            "No, forget it" ) == 0 ) {
        // they said to still add it (clicked button 0) so add it:
        evokeAddUDSubset( elts );
        //addSubset( elts, GroupSubset::UserDefined );
    }
}

void SubsetListBox::addConjClsPartition ()
{
    Q3ValueList<GroupSubset> partition;
    GEltListList ell = groupfile->group.conjClasses();
    for ( int i = 0 ; i < ell.count() ; i++ ) {
        partition.append( GroupSubset( groupfile,
                                       QString( "<i>CC</i><sub>%1</sub>" ).arg( i ),
                                       GroupSubset::EquivalenceClass, ell[i],
                                       QString( "a conjugacy class of size %1" )
                                       .arg( ell[i].size() ) ) );
    }
    addPartition( partition );
}

void SubsetListBox::addOrdClsPartition ()
{
    Q3ValueList<GroupSubset> partition;
    GEltListList ell = groupfile->group.orderClasses();
    for ( int i = 0 ; i < ell.count() ; i++ ) {
        partition.append( GroupSubset( groupfile,
                                       QString( "<i>OC</i><sub>%1</sub>" ).arg( i ),
                                       GroupSubset::EquivalenceClass, ell[i],
                                       QString( "an order class of size %1" )
                                       .arg( ell[i].size() ) ) );
    }
    addPartition( partition );
}

void SubsetListBox::addCosetsPartition ( uint subgroup, bool leftCosets )
{
    Q3ValueList<GroupSubset> partition;
    GEltListList ell = groupfile->group.cosetsOf(
            subsets[GroupSubset::Subgroup][subgroup].elements(), leftCosets );
    QString sname = subsetName( 'H', subgroup );
    for ( int i = 0 ; i < ell.count() ; i++ ) {
        QString ename = groupfile->getRepresentation( ell[i][0] ).HTML;
        partition.append( GroupSubset( groupfile,
                                       QString( leftCosets ? "%1%2" : "%2%1" )
                                       .arg( ename ).arg( sname ),
                                       GroupSubset::EquivalenceClass, ell[i],
                                       QString( "the %1 of %2 by %3" )
                                       .arg( leftCosets ?
                                             "left coset" : "right coset" )
                                       .arg( sname ).arg( ename ) ) );
    }
    addPartition( partition );
}


SubsetComputation::SubsetComputation ( SubsetListBox* lb )
    : QObject( lb ), listBox( lb )
{
}

void SubsetComputation::addToMenu ( QMenu* m, int id )
{
    if ( isSensible() ) {
        QAction* tmp = new QAction( description(), m );
        m->addAction( tmp );
        tmp->setData( id );
        connect( tmp, SIGNAL(triggered()), this, SLOT(performComputation()) );
        //int realid = m->insertItem(
        //    new RichCustomMenuItem( "<nobr>" + description() + "</nobr>" ), id );
        //m->connectItem( realid, this, SLOT(performComputation()) );
    }
}

bool SubsetComputation::isSensible ()
{
    return FALSE; // invisible unless descendants override, which they'll want to
}

QString SubsetComputation::description ()
{
    return "SubsetComputation::description() <font color=red>not implemented</font>;"
            " subclasses must override";
    // be sure when you override this in subclasses not to use the names of any
    // subgroups, but rather their lists of elements, because we don't want the
    // user's editing of subsets to invalidate this description.
}

void SubsetComputation::performComputation ()
{
    qDebug( "SubsetComputation::performComputation() not implemented; "
            "subclasses must override, and if appropriate, "
            "BE SURE TO USE SubsetListBox::suggestComputedSubset(...)" );
}


