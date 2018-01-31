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


#include "geltgrid.h"


GEltGrid::GEltGrid ()
{
    dims.fill( 0, 1 );
}

GEltGrid::GEltGrid ( GEltList dimensions )
{
    redimension( dimensions );
}

GEltGrid::GEltGrid ( const GEltGrid& grid )
{
    dims = grid.dims.copy();
    data = grid.data.copy();
}

GEltGrid::GEltGrid ( GEltList dimensions, GEltList contents )
{
    dims = dimensions.copy();
    data = contents.copy();
    // yes, we trust you
}

GEltList GEltGrid::dimensions ()
{
    return dims.copy();
}

uint GEltGrid::numDimensions ()
{
    return dims.size();
}

uint GEltGrid::dimension ( uint i )
{
    return ( i < dims.size() ) ? dims[i] : 0;
}

void GEltGrid::redimension ( GEltList dimensions )
{
    dims = dimensions.copy();
    if ( !dims.size() ) {
        data.resize( 0 );
    } else {
        uint product = 1;
        for ( uint i = 0 ; i < dims.size() ; i++ ) product *= dims[i];
        data.resize( product );
    }
}

uint zero = 0; // this is useless to touch
GElt& GEltGrid::lookup ( GEltList index )
{
    if ( !index.size() && ( dims.size() == 1 ) && ( dims[0] == 1 ) ) {
        return data[0]; // special case of a scalar disguised as a GEltGrid
    }
    if ( index.size() != dims.size() ) {
        qWarning( "Bad index: " + elt_list_to_string( index ) +
                  " into grid of size " + elt_list_to_string( dims ) );
        return zero; // this is useless to touch
    }
    uint trueIndex = 0;
    for ( uint i = 0 ; i < index.size() ; i++ ) {
        if ( i ) trueIndex *= dims[i];
        trueIndex += index[i];
    }
    if ( trueIndex >= data.size() ) {
        qWarning( QString( "Cannot index grid of dims %1 by index %2" )
                  .arg( elt_list_to_string( dims ) ).arg( elt_list_to_string( index ) ) );
        return zero; // this is useless to touch
    }
    return data[trueIndex];
}

GElt& GEltGrid::operator[] ( GEltList index )
{
    return lookup( index );
}

uint GEltGrid::flatsize ()
{
    if ( !dims.size() ) return 0;
    uint product = 1;
    for ( uint i = 0 ; i < dims.size() ; i++ ) product *= dims[i];
    return product;
}

GElt& GEltGrid::flatlookup ( uint index )
{
    return data[index];
}

GEltList GEltGrid::flatidx ( uint index )
{
    uint product = flatsize();
    GEltList result( dims.size() );
    for ( uint i = 0 ; i < dims.size() ; i++ ) {
        product /= dims[i];
        result[i] = index / product;
        index -= result[i] * product;
    }
    return result;
}

GEltList GEltGrid::elementToPositionMap ()
{
    // assuming elements 0,...,n-1 are in data with data.size() == n, this will tell you
    // where each element is sitting in that array; use only if it's appropriate
    return perm_inverse( data );
}

int GEltGrid::contains ( GElt e )
{
    return data.contains( e );
}

GEltList GEltGrid::flatcopy ()
{
    return data.copy();
}

void GEltGrid::append ( GEltGrid item )
{
    if ( item.dims.size() + 1 != dims.size() ) {
        qWarning( "Cannot append grid of shape " + elt_list_to_string( item.dims ) +
                  " to grid of shape " + elt_list_to_string( dims ) );
        return;
    }
    for ( uint i = 0 ; i < item.dims.size() ; i++ ) {
        if ( item.dims[i] != dims[i + 1] ) {
            qWarning( "Cannot append grid of shape " + elt_list_to_string( item.dims ) +
                      " to grid of shape " + elt_list_to_string( dims ) );
            return;
        }
    }
    dims[0]++;
    redimension( dims );
    for ( uint i = 1 ; i <= item.data.size() ; i++ )
        data[data.size() - i] = item.data[item.data.size() - i];
}

void GEltGrid::concat ( GEltGrid grid )
{
    if ( grid.dims.size() != dims.size() ) {
        qWarning( "Cannot concat grid of shape " + elt_list_to_string( grid.dims ) +
                  " to grid of shape " + elt_list_to_string( dims ) );
        return;
    }
    for ( uint i = 1 ; i < grid.dims.size() ; i++ ) {
        if ( grid.dims[i] != dims[i] ) {
            qWarning( "Cannot concat grid of shape " + elt_list_to_string( grid.dims ) +
                      " to grid of shape " + elt_list_to_string( dims ) );
            return;
        }
    }
    dims[0] += grid.dims[0];
    redimension( dims );
    for ( uint i = 1 ; i <= grid.data.size() ; i++ )
        data[data.size() - i] = grid.data[grid.data.size() - i];
}

QString toStringRec ( GEltGrid& grid, GEltList indexSoFar )
{
    if ( indexSoFar.size() > grid.numDimensions() )
        return "[ERROR--startdim too big]";
    if ( indexSoFar.size() == grid.numDimensions() )
        return QString::number( grid[indexSoFar] );
    GEltList newindex( indexSoFar.size() + 1 );
    for ( uint i = 0 ; i < indexSoFar.size() ; i++ ) newindex[i] = indexSoFar[i];
    uint thisdim = indexSoFar.size();
    QString result = "[";
    for ( newindex[thisdim] = 0 ;
          newindex[thisdim] < grid.dimension( thisdim ) ;
          newindex[thisdim]++ )
        result += " " + toStringRec( grid, newindex );
    return result + " ]";
}
QString GEltGrid::toString ()
{
    return //"dims: " + elt_list_to_string( dims ) + "; "
           //"data: " + elt_list_to_string( data ) + "; "
           //"real: " +
           toStringRec( *this, GEltList() );
}


