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


#include "subsetcomputations.h"
#include "gesheet.h"


ConjClassesComputation::ConjClassesComputation ( SubsetListBox* lb )
    : SubsetComputation( lb )
{
}

bool ConjClassesComputation::isSensible ()
{
    for ( uint i = 0 ; i < listBox->getNumPartitions() ; i++ ) {
        if ( listBox->getPartitionPiece( i, 0 ).getName().startsWith( "CC_" ) )//"<i>CC" ) )
            return FALSE;
    }
    return TRUE;
}

QString ConjClassesComputation::description ()
{
    return "all conjugacy classes CC_i";//<i>CC<sub>i</sub></i>";
}

void ConjClassesComputation::performComputation ()
{
    listBox->evokeAddConjClsPartition();
}


OrdClassesComputation::OrdClassesComputation ( SubsetListBox* lb )
    : SubsetComputation( lb )
{
}

bool OrdClassesComputation::isSensible ()
{
    for ( uint i = 0 ; i < listBox->getNumPartitions() ; i++ ) {
        if ( listBox->getPartitionPiece( i, 0 ).getName().startsWith( "OC_" ) )//"<i>OC" ) )
            return FALSE;
    }
    return TRUE;
}

QString OrdClassesComputation::description ()
{
    return "all order classes OC_i";//<i>OC<sub>i</sub></i>";
}

void OrdClassesComputation::performComputation ()
{
    listBox->evokeAddOrdClsPartition();
}


NormalizerComputation::NormalizerComputation ( SubsetListBox* lb, GroupSubset* argument )
    : SubsetComputation( lb ), argument( argument )
{
}

bool NormalizerComputation::isSensible ()
{
    return argument && ( argument->getType() == GroupSubset::Subgroup );
}

QString NormalizerComputation::description ()
{
    return "the normalizer of " + argument->getTextName() +
        ", Norm(" + argument->getTextName() + ")";
}

void NormalizerComputation::performComputation ()
{
    // for starters, the normalizer must contain the subgroup itself (which we'll call H):
    GEltList H = argument->elements();
    GEltList result = H.copy();
    GroupFile* gf = argument->getGroup();
    // now let's see what else goes in (obviously skipping the identity):
    for ( GElt h = 1 ; h < gf->group.order ; h++ ) {
        if ( !result.contains( h ) ) {
            // if h not in result yet, let's see if it should be.
            // it should be added iff every hgh^{-1} is in H, for g in H:
            bool allWereIn = TRUE;
            for ( uint i = 0 ; i < H.size() ; i++ ) {
                if ( !H.contains( gf->group.conj( H[i], h ) ) ) {
                    allWereIn = FALSE;
                    break;
                }
            }
            // it passed the test, so add it:
            if ( allWereIn ) {
                result.resize( result.size() + 1 );
                result[result.size() - 1] = h;
            }
        }
    }
    listBox->suggestComputedSubset( result,
        "It is the normalizer of " + argument->getName() +
        ", Norm(" + argument->getName() + ").\n"
        "This means that it is the largest subgroup of " +
        gf->getPrimaryName().HTML +
        " in which " + argument->getName() + " is normal." );
}


ClosureComputation::ClosureComputation ( SubsetListBox* lb, GroupSubset* argument )
    : SubsetComputation( lb ), argument( argument )
{
}

bool ClosureComputation::isSensible ()
{
    return argument && ( argument->getType() != GroupSubset::Subgroup );
}

QString ClosureComputation::description ()
{
    return "the closure of " + argument->getTextName() +
        ", <" + argument->getTextName() + ">";
}

void ClosureComputation::performComputation ()
{
    listBox->suggestComputedSubset(
        argument->getGroup()->group.subgroupGenBy( argument->elements() ),
        "It is the closure of " + argument->getName() +
        ", &lt;" + argument->getName() + "&gt;.\n"
        "This means that it is the smallest subgroup of " +
        argument->getGroup()->getPrimaryName().HTML +
        " which contains " + argument->getName() + "." );
}


LeftCosetsComputation::LeftCosetsComputation ( SubsetListBox* lb, GroupSubset* argument )
    : SubsetComputation( lb ), argument( argument )
{
}

bool LeftCosetsComputation::isSensible ()
{
    // it must be a subgroup
    if ( !argument || ( argument->getType() != GroupSubset::Subgroup ) )
        return FALSE;
    // and it must not already have its left cosets computed
    for ( uint i = 0 ; i < listBox->getNumPartitions() ; i++ ) {
        if ( listBox->getPartitionPiece( i, 0 ).getName()
             .endsWith( argument->getName() ) )
            return FALSE;
    }
    return TRUE;
}

QString LeftCosetsComputation::description ()
{
    return "all left cosets g" + argument->getTextName()
        + " of " + argument->getTextName();
}

void LeftCosetsComputation::performComputation ()
{
    listBox->evokeAddCosetsPartition(
        TRUE, SubsetListBox::subsetNumber( argument->getName() ) );
    /*
    QValueList<GroupSubset> partition;
    GEltListList ell = argument->getGroup()->group.cosetsOf( argument->elements(), TRUE );
    for ( uint i = 0 ; i < ell.count() ; i++ ) {
        partition.append( GroupSubset( argument->getGroup(),
                                       argument->getGroup()->getRepresentation(
                                           ell[i][0] ).HTML + argument->getName(),
                                       GroupSubset::EquivalenceClass, ell[i],
                                       "a left coset of " + argument->getName() ) );
    }
    listBox->addPartition( partition );
    */
}


RightCosetsComputation::RightCosetsComputation ( SubsetListBox* lb,
                                                 GroupSubset* argument )
    : SubsetComputation( lb ), argument( argument )
{
}

bool RightCosetsComputation::isSensible ()
{
    // it must be a subgroup
    if ( !argument || ( argument->getType() != GroupSubset::Subgroup ) )
        return FALSE;
    // and it must not already have its right cosets computed
    for ( uint i = 0 ; i < listBox->getNumPartitions() ; i++ ) {
        if ( listBox->getPartitionPiece( i, 0 ).getName()
             .startsWith( argument->getName() ) )
            return FALSE;
    }
    return TRUE;
}

QString RightCosetsComputation::description ()
{
    return "all right cosets " + argument->getTextName()
        + "g of " + argument->getTextName();
}

void RightCosetsComputation::performComputation ()
{
    listBox->evokeAddCosetsPartition(
        FALSE, SubsetListBox::subsetNumber( argument->getName() ) );
    /*
    QValueList<GroupSubset> partition;
    GEltListList ell = argument->getGroup()->group.cosetsOf( argument->elements(),
                                                             FALSE );
    for ( uint i = 0 ; i < ell.count() ; i++ ) {
        partition.append( GroupSubset( argument->getGroup(),
                                       argument->getName() +
                                           argument->getGroup()->getRepresentation(
                                               ell[i][0] ).HTML,
                                       GroupSubset::EquivalenceClass, ell[i],
                                       "a right coset of " + argument->getName() ) );
    }
    listBox->addPartition( partition );
    */
}


IntersectionComputation::IntersectionComputation (
        SubsetListBox* lb, GroupSubset* argument1, GroupSubset* argument2 )
    : SubsetComputation( lb ), argument1( argument1 ), argument2( argument2 )
{
}

bool IntersectionComputation::isSensible ()
{
    return TRUE;
}

QString IntersectionComputation::description ()
{
    return "the intersection of " + argument1->getTextName()
         + " with " + argument2->getTextName();
}

void IntersectionComputation::performComputation ()
{
    GEltList H1 = argument1->elements();
    GEltList H2 = argument2->elements();
    GEltList result;
    // for every element in H1, if it is in H2, add it to our (initially empty) result:
    for ( uint i = 0 ; i < H1.size() ; i++ ) {
        if ( H2.contains( H1[i] ) ) {
            result.resize( result.size() + 1 );
            result[result.size() - 1] = H1[i];
        }
    }
    listBox->suggestComputedSubset( result,
        "It is the intersection of " + argument1->getName() +
        " with " + argument2->getName() + "." );
}


UnionComputation::UnionComputation (
        SubsetListBox* lb, GroupSubset* argument1, GroupSubset* argument2 )
    : SubsetComputation( lb ), argument1( argument1 ), argument2( argument2 )
{
}

bool UnionComputation::isSensible ()
{
    return TRUE;
}

QString UnionComputation::description ()
{
    return "the union of " + argument1->getTextName() + " with " + argument2->getTextName();
}

void UnionComputation::performComputation ()
{
    GEltList result = argument1->elements().copy();
    GEltList H2 = argument2->elements();
    // for every element in H2, if it is not already in the result, add it:
    for ( uint i = 0 ; i < H2.size() ; i++ ) {
        if ( !result.contains( H2[i] ) ) {
            result.resize( result.size() + 1 );
            result[result.size() - 1] = H2[i];
        }
    }
    listBox->suggestComputedSubset( result,
        "It is the union of " + argument1->getName() +
        " with " + argument2->getName() + "." );
}


ElementwiseProductComputation::ElementwiseProductComputation (
        SubsetListBox* lb, GroupSubset* argument1, GroupSubset* argument2 )
    : SubsetComputation( lb ), argument1( argument1 ), argument2( argument2 )
{
}

bool ElementwiseProductComputation::isSensible ()
{
    return TRUE;
}

QString ElementwiseProductComputation::description ()
{
    return "the elementwise product of " + argument1->getTextName() +
           " with " + argument2->getTextName();
}

void ElementwiseProductComputation::performComputation ()
{
    GEltList H1 = argument1->elements();
    GEltList H2 = argument2->elements();
    GEltList result;
    // create the set { ab | a in H1, b in H2 } (without repetitions, of course):
    for ( uint i = 0 ; i < H1.size() ; i++ ) {
        for ( uint j = 0 ; j < H2.size() ; j++ ) {
            GElt newelt = argument1->getGroup()->group.op( H1[i], H2[j] );
            if ( !result.contains( newelt ) ) {
                result.resize( result.size() + 1 );
                result[result.size() - 1] = newelt;
            }
        }
    }
    listBox->suggestComputedSubset( result,
        "It is the elementwise product of " + argument1->getName() +
        " with " + argument2->getName() + ".\n"
        "This means that it is the set of all elements <i>ab</i> in " +
        argument1->getGroup()->getPrimaryName().HTML +
        ", with <i>a</i> from " + argument1->getName() +
        " and <i>b</i> from " + argument2->getName() + ".  "
        "Note that the elementwise product operation is not necessarily commutative."  );
}


MorphismPushComputation::MorphismPushComputation (
        GECanvasMorphism* throughThis, GroupSubset* pushMe )
    : SubsetComputation( 0 ), f( throughThis ), S( pushMe )
{
}

bool MorphismPushComputation::isSensible ()
{
    return TRUE;
}

QString MorphismPushComputation::description ()
{
    return QString( "push %1 through %2, giving %3[%4]" )
           .arg( S->getTextName() ).arg( f->getMorphism()->getTextName() )
           .arg( f->getMorphism()->getTextName() ).arg( S->getTextName() );
}

void MorphismPushComputation::performComputation ()
{
    f->push( S );
}


MorphismPullComputation::MorphismPullComputation (
        GECanvasMorphism* throughThis, GroupSubset* pullMe )
    : SubsetComputation( 0 ), f( throughThis ), S( pullMe )
{
}

bool MorphismPullComputation::isSensible ()
{
    return TRUE;
}

QString MorphismPullComputation::description ()
{
    return QString( "pull %1 back through %2, giving %3<sup>-1</sup>[%4]" )
           .arg( S->getTextName() ).arg( f->getMorphism()->getTextName() )
           .arg( f->getMorphism()->getTextName() ).arg( S->getTextName() );
}

void MorphismPullComputation::performComputation ()
{
    f->pull( S );
}


