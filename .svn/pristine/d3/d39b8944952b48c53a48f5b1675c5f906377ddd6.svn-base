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


#include "group.h"
#include <qstringlist.h>
#include <qregexp.h>
//Added by qt3to4:
#include <Q3MemArray>


GEltList iota ( uint upTo )
{
    GEltList result( upTo );
    for ( uint i = 0 ; i < upTo ; i++ ) result[i] = i;
    return result;
}

GEltList perm_inverse ( GEltList perm )
{
	GEltList result( perm.size() );
	for ( uint i = 0 ; i < perm.size() ; i++ ) result[perm[i]] = i;
	return result;
}


QString elt_list_to_string ( GEltList L )
{
    QString answer;
    for ( uint i = 0 ; i < L.count() ; i++ ) answer += QString( " %1" ).arg( L[i] );
    return "[" + answer + " ]";
}

QString elt_list_list_to_string ( GEltListList LL )
{
    QString result;
    for ( int i = 0 ; i < LL.count() ; i++ ) result += " " + elt_list_to_string( LL[i] );
    return "{" + result + " }";
}

bool elt_subset ( GEltList S1, GEltList S2 )
{
    if ( S1.size() > S2.size() ) return FALSE;
    for ( uint i = 0 ; i < S1.size() ; i++ ) if ( !S2.contains( S1[i] ) ) return FALSE;
    return TRUE;
}

bool elt_sets_are_equal ( GEltList S1, GEltList S2 )
{
    if ( S1.size() != S2.size() ) return FALSE;
    for ( uint i = 0 ; i < S1.size() ; i++ ) if ( !S2.contains( S1[i] ) ) return FALSE;
    return TRUE;
}

bool elt_lists_are_equal ( GEltList L1, GEltList L2 )
{
    if ( L1.size() != L2.size() ) return FALSE;
    for ( uint i = 0 ; i < L1.size() ; i++ ) if ( L1[i] != L2[i] ) return FALSE;
    return TRUE;
}

QString elt_list_to_code ( GEltList L )
{
    QString result;
    for ( uint i = 0 ; i < L.size() ; i++ ) {
        if ( i ) result += "-";
        result += QString::number( L[i] );
    }
    return result;
}

GEltList code_to_elt_list ( QString code )
{
    QStringList codeBits = code.split( "-" );
    GEltList result( codeBits.count() );
    for ( int i = 0 ; i < codeBits.count() ; i++ ) result[i] = codeBits[i].toUInt();
    return result;
}

QString elt_set_to_code ( GEltList S )
{
    QString result;
    GElt max = 0;
    for ( uint i = 0 ; i < S.size() ; i++ ) if ( max < S[i] ) max = S[i];
    for ( uint i = 0 ; i <= max ; i++ ) result += ( S.contains( i ) ? '1' : '0' );
    return result;
}

GEltList code_to_elt_set ( QString code )
{
    GEltList result;
    for ( int i = 0 ; i < code.length() ; i++ ) if ( code[i] == '1' ) {
        result.resize( result.size() + 1 );
        result[result.size() - 1] = i;
    }
    return result;
}

QString elt_set_list_to_code ( GEltListList LL )
{
    QString result;
    for ( int i = 0 ; i < LL.count() ; i++ ) {
        if ( i ) result += ",";
        result += elt_set_to_code( LL[i] );
    }
    return result;
}

GEltListList code_to_elt_set_list ( QString code )
{
    QStringList codes = code.split( "," );
    GEltListList result;
    for ( int i = 0 ; i < codes.count() ; i++ )
        result.append( code_to_elt_set( codes[i] ) );
    return result;
}

uint gcd ( uint a, uint b )
{
    if ( b < a ) qSwap( a, b );
    uint r = b % a;
    return r ? gcd( r, a ) : a;
}


Group::Group () {
    setSize( 1 );
}

Group::Group ( int sz ) {
    setSize( sz );
}
        
Group::Group ( Group& g ) {
    // I think C++ defaults to this, so I don't have to do it, but I'm not sure...
    order = g.order;
    mtable = g.mtable.copy();
    invs = g.invs;
    ords = g.ords;
    isInTheOrbitOf = g.isInTheOrbitOf.copy();
}


void Group::setMTable ( GEltList newtable )
{
    mtable = newtable.copy();
    invs.fill( 0, order );
    ords.fill( 0, order );
    computeIsInTheOrbitOfMatrix();
}

/*
 *  As you will notice from the implementation of the following function,
 *  mtable reads across the rows of the multiplication table, and contains
 *  the result of multiplying the left row header by the top column header
 *  (in that order).  That is, the entry at row i col j is rowheader i
 *  times colheader j, where times is the group op, and order matters.
 *  Thus indexing mtable should be done by row*order+col.
 */
void Group::setSize ( int sz ) {
    order = sz;
    mtable.resize( order*order );
    for ( uint i = 0 ; i < order ; i++ )
        for ( uint j = 0 ; j < order ; j++ )
            mtable[i*order+j] = (i + j) % order;
    invs.fill( 0, order );
    ords.fill( 0, order );
    computeIsInTheOrbitOfMatrix();
}

void Group::appendXMLTo ( QDomNode n ) {
    QDomNode tag = n.appendChild ( n.ownerDocument().createElement ( "multtable" ) );
    for ( uint i = 0 ; i < order ; i++ ) {
        QString rowtext ( "" );
        for ( uint j = 0; j < order ; j++ ) {
            rowtext = QString ( "%1 %2" ).arg( rowtext ).arg( op(i,j) );
        }
        tag.appendChild ( n.ownerDocument().createElement ( "row" ) )
                .appendChild ( n.ownerDocument().createTextNode ( rowtext ) );
    }
}

bool Group::makeFromXML ( QDomNode n ) {
    // make sure it's the right type of node
    if ( n.nodeName() != "multtable" ) return 0;
    // loop through its children
    QDomNodeList list = n.childNodes();
    uint row = 0;
    Group tmp;
    for ( int i = 0 ; i < list.count() ; i++ ) {
        if ( list.item(i).isElement() ) {
            // make sure each element is a nonempty row node
            if ( list.item(i).nodeName() != "row" ) return 0;
            QDomText rowtxt = list.item(i).firstChild().toText();
            if ( rowtxt.isNull() ) return 0;
            // get its array of numbers and be sure it's the right length
            QStringList nums = rowtxt.nodeValue().split( QRegExp( "\\s+" ),
                                                         QString::SkipEmptyParts );
            if ( i == 0 )
                tmp.setSize( nums.size() );
            else if ( (int)( tmp.order ) != nums.size() )
                return 0;
            // grab each number (ensuring it is a number) and put it in our mtable
            for ( uint j = 0 ; j < tmp.order ; j++ ) {
                bool ok;
                tmp.mtable[row*tmp.order+j] = nums[j].toUInt(&ok);
                if ( !ok ) return 0;
            }
        }
        row++;
    }
    // make sure I got the right number of rows
    if ( row != tmp.order ) return 0;
    // now let's just check that we read a valid multiplication table
    for ( uint i = 0 ; i < tmp.order ; i++ ) {
        // is 0 the identity?  I require that it be so:
        if ( ( tmp.op( 0, i ) != (GElt)i ) || ( tmp.op( i, 0 ) != (GElt)i ) )
            return 0;
        bool leftinv = false;
        bool rightinv = false;
        for ( uint j = 0 ; j < tmp.order ; j++ ) {
            // is every element left- and right-invertible?  I require that it be so:
            if ( tmp.op( i, j ) == 0 ) rightinv = true;
            if ( tmp.op( j, i ) == 0 ) leftinv = true;
            for ( uint k = 0 ; k < tmp.order ; k++ )
                // is the operation associative?  I require that it be so:
                if ( tmp.op( i, tmp.op( j, k ) ) != tmp.op( tmp.op( i, j ), k ) )
                    return 0;
        }
        if ( !leftinv || !rightinv )
            return 0;
    }
    // everything's ok, so I keep what I parsed
    order = tmp.order;
    setMTable( tmp.mtable );
    computeIsInTheOrbitOfMatrix();
    return 1;
}

    
GElt Group::op ( GElt a, GElt b ) {
    return mtable[a*order+b];
}

GElt Group::inv ( GElt a ) {
    // stores its answer in invs[a] for faster computation next time.
    // if invs[a] is not zero (or a is zero) we can give answer fast.
    if ( !a ) return 0;
    if ( invs[a] ) return invs[a];
    for ( uint i = 0 ; i < order ; i++ ) {
        if (mtable[i*order+a] == 0) return invs[a] = i;
    }
    qFatal("Group operation has no inverse");
    return 0;
}

GElt Group::conj ( GElt a, GElt by ) {
    return op(by,op(a,inv(by)));
}

GElt Group::pow ( GElt a, uint p )
{
    GElt result = 0;
    for ( uint i = 0 ; i < p ; i++ ) result = op( a, result );
    return result;
}


uint Group::orderOf ( GElt a ) {
    // stores its answer in ords[a] for faster computation next time.
    // if ords[a] is not zero we can five answer immediately.
    if ( ords[a] ) return ords[a];
    uint ans = 1;
    for ( uint lcv = a ; lcv != 0 ; lcv = op(lcv,a) ) {
        ans++;
    }
    return ords[a] = ans;
}

GEltList Group::orbitOf ( GElt a, bool includeIdentity )
{
    GEltList result( orderOf( a ) + ( includeIdentity ? 0 : -1 ) );
    GElt tmp = includeIdentity ? 0 : a;
    for ( uint i = 0 ; i < result.size() ; i++ ) {
        result[i] = tmp;
        tmp = op( tmp, a );
    }
    return result;
}

GEltList Group::orderClassOf ( GElt a ) {
    // we will put our answers in ans, so make it large enough:
    GEltList ans ( order );
    uint orda = orderOf(a), last = 0;
    // find everything with the same order, and append them to ans:
    for ( uint i = 0 ; i < order ; i++ ) {
        if ( orderOf(i) == orda ) ans[last++] = i;
    }
    // trim ans down to just the initial segment we've used:
    ans.resize(last);
    return ans;
}

GEltList Group::conjClassOf ( GElt a ) {
    // initialize our answer to be the singleton set {a}
    GEltList ans ( 1 );
    ans[0] = a;
    // add every conjugate i*a*i^{-1} to ans, but only 1 of each:
    for ( uint i = 0 ; i < order ; i++ ) {
        uint c = conj(a,i);
        if ( !ans.contains( c ) ) {
            ans.resize( ans.size() + 1 );
            ans[ans.size() - 1] = c;
        }
    }
    return ans;
}

GEltListList Group::orderClasses () {
    GEltListList ans;
    for ( uint i = 1 ; i <= order ; i++ ) {
        if ( order % i == 0 ) {
            bool foundOne = 0;
            for ( uint j = 0 ; j < order ; j++ ) {
                if ( orderOf(j) == i ) {
                    if ( !foundOne ) {
                        ans.append( GEltList() );
                        foundOne = 1;
                    }
                    ans.last().resize( ans.last().size() + 1 );
                    ans.last()[ans.last().size() - 1] = j;
                }
            }
        }
    }
    return ans;
}

GEltListList Group::conjClasses () {
    GEltListList ans;
    bool* flags = new bool[order];
    for ( uint i = 0 ; i < order ; i++ ) flags[i] = 0;
    for ( uint i = 0 ; i < order ; i++ ) {
        if ( !flags[i] ) {
            GEltList tmp = conjClassOf( i );
            for ( uint j = 0 ; j < tmp.size() ; j++ ) {
                flags[tmp[j]] = 1;
            }
            ans.append( tmp );
        }
    }
    delete flags;
    // but these things should be sorted into ascending order of size:
    GEltList tmp;
    bool inOrder = 0;
    while ( !inOrder ) {
        inOrder = 1;
        for ( int i = 1 ; i < ans.size() ; i++ ) {
            if ( ans[i-1].size() > ans[i].size() ) {
                inOrder = 0;
                tmp = ans[i-1];
                ans[i-1] = ans[i];
                ans[i] = tmp;
            }
        }
    }
    return ans;
}

GEltListList Group::cosetsOf ( GEltList subgroup, bool leftCosets )
{
	GEltListList ans;
	GEltList used;
	used.fill( 0, order );
        for ( uint i = 0 ; i < order ; i++ ) {
		if ( !used[i] ) {
			GEltList tmp( subgroup.size() );
                        for ( uint j = 0 ; j < subgroup.size() ; j++ ) {
				tmp[j] = leftCosets ? op( i, subgroup[j] ) : op( subgroup[j], i );
				used[tmp[j]] = 1;
			}
			ans.append( tmp );
		}
	}
	return ans;
}

bool Group::isNormal ( GEltList subgroup )
{
    GEltListList sgrs = subgroups();
    for ( int i = 0 ; i < sgrs.count() ; i++ )
        if ( elt_sets_are_equal( subgroup, sgrs[i] ) ) return isNormal( i );
    return FALSE; // what you askin' for?  it ain't even a subgroup!
}

bool Group::isNormal ( uint index )
{
    // if normalOnes array of stored values isn't big enough to mention this, stretch it:
    if ( index >= normalOnes.size() ) {
        uint old = normalOnes.size();
        normalOnes.resize( index + 1 );
        for ( uint i = old ; i <= index ; i++ ) normalOnes[i] = 2;
    }
    // if normalOnes array has never computed this before (2 == never before), compute it:
    if ( normalOnes[index] == 2 ) {
        GEltList sgr = subgroups()[index];
        for ( uint i = 0 ; ( i < order ) && ( normalOnes[index] == 2 ) ; i++ )
            for ( uint j = 0 ; ( j < sgr.size() ) && ( normalOnes[index] == 2 ) ; j++ )
                if ( !sgr.contains( conj( sgr[j], i ) ) )
                    normalOnes[index] = 0; // not normal
        if ( normalOnes[index] == 2 ) normalOnes[index] = 1; // passed all requirements
    }
    // return the result of the previous computation, whether just now or long ago:
    return ( normalOnes[index] == 1 );
}

uint Group::isPSubgroup ( GEltList subgroup )
{
    GEltListList sgrs = subgroups();
    for ( int i = 0 ; i < sgrs.count() ; i++ )
        if ( elt_sets_are_equal( subgroup, sgrs[i] ) ) return isPSubgroup( i );
    return 0; // what you askin' for?  it ain't even a subgroup!
}

uint Group::isPSubgroup ( uint index )
{
    // if pSubgroups array of stored values isn't big enough to mention this, stretch it:
    if ( index >= pSubgroups.size() ) {
        uint old = pSubgroups.size();
        pSubgroups.resize( index + 1 );
        for ( uint i = old ; i <= index ; i++ ) pSubgroups[i] = 1;
    }
    // if pSubgroups array has never computed this before (1 == never before), compute it:
    if ( pSubgroups[index] == 1 ) {
        GEltList sgr = subgroups()[index];
        if ( sgr.size() < 2 ) return 0;
        // find the smallest number which divides the order of the sgr (it'll be prime):
        // (by Cauchy's theorem, sgr contains an elt with this order, so we can use this)
        uint somePrime;
        for ( somePrime = 2 ; sgr.size() % somePrime ; somePrime++ ) { }
        // now let's assume that the sgr is a somePrime-subgroup, & look for a counterex:
        pSubgroups[index] = somePrime;
        // check to see if order of every elt in sgr is a power of this prime (skip 0):
        for ( uint i = 0 ; i < sgr.size() ; i++ ) if ( sgr[i] ) {
            uint powers;
            uint order = orderOf( sgr[i] );
            for ( powers = 1 ; powers < order ; powers *= somePrime ) { }
            if ( powers != order ) { // counterexample found; quit w/ans==0
                pSubgroups[index] = 0;
                break;
            }
        }
    }
    // return the result of the previous computation, whether just now or long ago:
    return pSubgroups[index];
}

GEltList Group::subgroupGenBy ( GEltList gens ) {
    GEltList ans = gens.copy();
    if ( !ans.count() ) { // right off the bat, sgr gen'd by no gens is { identity }
        ans.resize( 1 );
        ans[0] = 0;
        return ans;
    }
    // otherwise, we have gens to use, so let's use 'em:
    for ( uint i = 0 ; i < ans.count() ; i++ ) {
        for ( uint j = 0 ; j < gens.count() ; j++ ) {
            GElt tryme = op( ans[i], gens[j] );
            if ( !ans.contains( tryme ) ) {
                ans.resize( ans.count() + 1 );
                ans[ans.count() - 1] = tryme;
            }
        }
    }
    return ans;
}

GEltList Group::elements ()
{
    return iota( order );
}

GEltList Group::expandByOneGenerator ( GEltList subgroup, GElt g, bool useRightMult )
{
    /*
    qDebug( QString( "%3::expandByOneGenerator( %1, %2 )" )
            .arg( elt_list_to_string( subgroup ) ).arg( g )
            .arg( elt_list_to_string( elements() ) ) );
    */
    // This routine takes a subgroup and a generator g, and returns the smallest subgroup
    // containing both.  But it does so in an orderly fashion, so that the result (if the
    // subgroup was a1,...,an) looks like:
    //  a1 ... an ga1 ... gan gga1 ... ggan ....... g^ka1 ... g^kan
    // where g^k+1a1 is the first (in the sense of smallest k) element in the given sgr.
    // If this is not possible (i.e. if the above technique falls short of generating all
    // of the smallest subgroup desired) then a new, unused element is picked as an
    // "arbitrary" starting point for continuing.  I.e.
    //  a1 ... an ....... g^ka1 ... g^kan ta1 ... tan gta1 ... gtan (and so on).

    // first we figure out what our goal is:
    GEltList all = subgroup.copy();
    all.resize( all.size() + 1 );
    all[all.size() - 1] = g; 
    GEltList goal = subgroupGenBy( all );
    // now we shoot for that goal:
    GEltList answer = subgroup.copy();
    int sz = subgroup.size();
    while ( answer.count() < goal.count() ) {
        int asz = answer.size();
        if ( answer.contains( useRightMult ? op( g, answer[asz - sz] )
                                           : op( answer[asz - sz], g ) ) ) { 
            // we need to pick an element t as documented above
            GElt t; 
            for ( t = 0 ; t < goal.size() ; t++ ) 
                if ( !answer.contains( goal[t] ) ) break;
            answer.resize( answer.size() + sz ); 
            for ( int i = 0 ; i < sz ; i++ )
                answer[asz + i] = useRightMult ? op( goal[t], answer[i] )
                                               : op( answer[i], goal[t] );
            /*      
            qDebug( QString( "\tbecause %1*%2=%3 was done, found %4: now answer is %5" )
                    .arg( g ).arg( answer[asz - sz] ).arg( op( g, answer[asz - sz] ) )
                    .arg( goal[t] ).arg( elt_list_to_string( answer ) ) );
            */
        } else {
            // we can append a new g*answer[start],...,g*answer[stop] set
            answer.resize( answer.size() + sz ); 
            for ( int i = 0 ; i < sz ; i++ )
                answer[asz + i] = useRightMult ? op( g, answer[asz - sz + i] )
                                               : op( answer[asz - sz + i], g );
            /*      
            qDebug( QString( "\tbecause %1*%2=%3 was not done, cont'd: now answer is %4" )
                    .arg( g ).arg( answer[asz - sz] ).arg( op( g, answer[asz - sz] ) )
                    .arg( elt_list_to_string( answer ) ) );
            */
        }       
    }
    return answer; 
}
void Group::generateStructuredSubgroup ( GEltList generators,
                                         GEltList& result, GEltList& dimensions,
                                         bool useRightMult )
{
    result = iota( 1 );
    int previousSize = 1;
    dimensions.fill( 0, generators.size() );
    for ( uint i = 0 ; i < generators.size() ; i++ ) {
        result = expandByOneGenerator( result, generators[i], useRightMult );
        dimensions[dimensions.size() - 1 - i] = result.size() / previousSize;
        previousSize = result.size();
    }
}

void Group::computeIsInTheOrbitOfMatrix ()
{
    isInTheOrbitOf.fill( FALSE, order*order );
    if ( order ) {
        isInTheOrbitOf[0] = TRUE;
        for ( GElt g = 1 ; g < order ; g++ )
            for ( GElt o = g ; o ; o = op( g, o ) )
                isInTheOrbitOf[o*order+g] = TRUE;
    }
}

void Group::findSubgroupsAbove ( GEltList* s ) {
    // if too big, there can be no proper subgroups above it, so we're done:
    if ( s->size() > order / 4 ) return;
    // make a bool array to store what elements we've already checked:
    Q3MemArray<bool> checked;
    checked.fill( FALSE, order );
    for ( uint i = 0 ; i < s->size() ; i++ ) checked[(*s)[i]] = TRUE;
    // now we'll need to build new subgroups above s, so let s be a set of generators
    // followed by a blank slot that we'll fill in with different elements, to use in
    // generating subgroups larger than s:
    GEltList newGenerators = s->copy();
    newGenerators.resize( newGenerators.size() + 1 );
    // now check each g (that needs to be checked) to see if we get a new subgroup:
    for ( GElt g = 1 ; g < order ; g++ ) {
        //if ( s->size() == 1 ) qDebug( "generator %d of %d", g, order );
        if ( !checked[g] ) {
            newGenerators[newGenerators.size() - 1] = g;
            GEltList maybeNewSubgroup = subgroupGenBy( newGenerators );
            // if this is not the whole group, then check to see if it's new:
            if ( maybeNewSubgroup.size() < order ) {
                // find the index of the first already-built subgroup at least as
                // big as this one (so we can check for equality):
                int i = 0;
                while ( ( i < allSubgroups.count() ) &&
                        ( allSubgroups[i].size() < maybeNewSubgroup.size() ) ) i++;
                // now compare this subgroup to everyone of the same size:
                bool foundBefore = FALSE;
                int j = i;
                while ( ( j < allSubgroups.count() ) &&
                        ( allSubgroups[j].size() == maybeNewSubgroup.size() ) ) {
                    if ( elt_sets_are_equal( maybeNewSubgroup, allSubgroups[j++] ) )
                    {
                        foundBefore = TRUE;
                        break;
                    }
                }
                if ( !foundBefore ) {
                    // it's new, so save it:
                    if ( j < allSubgroups.count() ) {
                        allSubgroups.insert( allSubgroups.at( j ),
                                             maybeNewSubgroup.copy() );
                    } else {
                        allSubgroups.append( maybeNewSubgroup.copy() );
                    }
                    // also, let's now recur up above this subgroup:
                    findSubgroupsAbove( &maybeNewSubgroup );
                }
            }
            // for every o in <g> such that <o>==<g>, mark all elements in the
            // sets o<s> and <o>s checked:
            // (we know to mark g checked, and we know to mark gh checked for h
            // in <s> because <g+s>==<gh+s>.  furthermore, we know to mark oh
            // checked for h in <s> because <o>==<g> and so <oh>==<gh> and so
            // <oh+s>==<gh+s>.)
            // (we verify <o>==<g> by verifying gcd(n,order(g))==1 for o=g^n)
            int n = 1;
            for ( GElt o = g ; o ; o = op( o, g ) ) {
                if ( gcd( n, orderOf( g ) ) == 1 ) {
                    for ( uint j = 0 ; j < s->size() ; j++ ) {
                        checked[op( (*s)[j], o )] = TRUE;
                        checked[op( o, (*s)[j] )] = TRUE;
                    }
                }
                n++;
            }
        }
    }
}

GEltListList Group::subgroups () {
    if ( !allSubgroups.count() ) {
        GEltList s;
        s.fill( 0, 1 );
        findSubgroupsAbove( &s );
//if ( order == 30 ) qDebug( "%s", elt_list_list_to_string( allSubgroups ).latin1() );
    }
    return allSubgroups;
}

GElt Group::findNextGen ( GEltList goal, GEltList goalgens,
                          GEltList gendsofar, GEltList gens, bool restriction )
{
    GEltList g = gens.copy();
    g.resize( g.size() + 1 );
    GElt largest = 0;
    uint largestsize = gendsofar.size();
    for ( uint i = 0 ; ( i < goalgens.size() )
                    && ( largestsize < goal.size() ) ; i++ ) {
        if ( !gendsofar.contains( goalgens[i] ) ) {
            // We have a candidate, goalgens[i], for our next generator.
            // First, check to see if it could replace an old gen (if so, ignore it).
            // We only do this first check if restriction is true.
            if ( restriction ) {
                bool candidateOK = TRUE;
                GEltList c = g.copy();
                c.resize( c.size() - 1 );
                for ( uint j = 0 ; j < c.size() ; j++ ) {
                    if ( j ) c[j - 1] = g[j - 1];
                    c[j] = goalgens[i];
                    if ( elt_subset( gendsofar, subgroupGenBy( c ) ) ) {
                        candidateOK = FALSE;
                        break;
                    }
                }
                if ( !candidateOK ) continue;
            }
            // Second, if it generates the most new stuff so far, record it.
            g[g.size() - 1] = goalgens[i];
            GEltList tmp = subgroupGenBy( g );
            if ( tmp.size() > largestsize ) {
                largestsize = tmp.size();
                largest = goalgens[i];
            }
        }
    }
    return largest;
}
GEltList Group::completeGens ( GEltList soFar, GEltList goalgens )
{
    // I don't know if this algorithm always succeeds, but here's the idea:
    //   - Start with the subgroup < soFar >
    //   - Grab an element from goalgens that generates the most (has largest orbit)
    //     and yet which could not supplant an earlier generator (i.e. given 3 in Z_15,
    //     we should not choose 1 because that could replace the 3 and generate all)
    //   - Grab another element that generates the most more (when combined w/1st),
    //     with the same restriction
    //   - Continue in this manner until subgroup < goalgens > is generated
    // Conclude that you have chosen the minimal number of generators.
    // Does this always work?  Good question!  I should try to prove it one day.
    GEltList result = soFar.copy();
    GEltList generatedSoFar = subgroupGenBy( result );
    GEltList goal = subgroupGenBy( goalgens );
    //GEltList tmp;
    //GElt largest = 0;
    //uint largestsize = 0;
    while ( generatedSoFar.size() < goal.size() ) {
        /*
        result.resize( result.size() + 1 );
        for ( uint i = 0 ; ( i < goalgens.size() )
                        && ( largestsize < goal.size() ) ; i++ ) {
            if ( !generatedSoFar.contains( goalgens[i] ) ) {
                // We have a candidate, goalgens[i], for our next generator.
                // First, check to see if it could replace an old gen (if so, ignore it).
                GEltList rescopy = result.copy();
                for ( uint j = 0 ; j < rescopy.size() ; j++ ) {
                    if ( j ) rescopy[j - 1] = result[j];
                    rescopy[j] = goalgens[i];
                    if ( elt_subset( generatedSoFar, subgroupGenBy( rescopy ) ) )
                        continue; // this guy is no good--it's too potent
                }
                // Second, if it generates the most new stuff so far, record it.
                result[result.size() - 1] = goalgens[i];
                tmp = subgroupGenBy( result );
                if ( tmp.size() > largestsize ) {
                    largestsize = tmp.size();
                    largest = goalgens[i];
                }
            }
        }
        result[result.size() - 1] = largest;
        generatedSoFar = tmp.copy();
        */
        // first try with the restriction on:
        GElt next = findNextGen( goal, goalgens, generatedSoFar, result, TRUE );
        // if that fails, drop the restriction:
        if ( !next ) next = findNextGen( goal, goalgens, generatedSoFar, result, FALSE );
        result.resize( result.size() + 1 );
        result[result.size() - 1] = next;
        generatedSoFar = subgroupGenBy( result );
    }
    return result;
}

GEltList Group::neededGens ( GEltList gens )
{
    return completeGens( GEltList(), gens );
}

GEltList Group::findGens ( GEltList sgr ) {
    return neededGens( sgr );
}

GEltList Group::findGens () {
    return neededGens( elements() );
}


