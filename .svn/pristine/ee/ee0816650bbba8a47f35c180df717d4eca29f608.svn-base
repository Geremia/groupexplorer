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
/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/


int NameSchemeForm::exec( MathText name, MathTextList origrep, MathTextList newrep )
{
    setCaption( "Edit naming scheme for " + name.text );
    TopLabel->setText( "Below are all the naming schemes for " + name.HTML + ".\n"
                       "Edit the Custom representation column by double-clicking "
                       "on the entries you wish to change." );
    Table->setNumCols( 2 );
    Table->setNumRows( origrep.size() );
    Table->setReadOnly( FALSE );
    Table->horizontalHeader()->setLabel( 0, "Default representation" );
    Table->setColumnReadOnly( 0, TRUE );
    Table->horizontalHeader()->setLabel( 1, "Custom representation" );
    Table->setColumnReadOnly( 1, FALSE );
    for ( int i = 0 ; i < origrep.size() ; i++ ) {
        Table->setText( i, 0, origrep[i].text );
        Table->setText( i, 1, newrep[i].text );
    }
    Table->adjustColumn( 0 );
    Table->adjustColumn( 1 );
    Table->setLeftMargin( 0 );
    resize( sizeHint() );
    return QDialog::exec();
}


int NameSchemeForm::exec( MathText name, MathTextList origrep, QStringList newrep )
{
    MathTextList tmp;
    for ( int i = 0 ; i < newrep.size() ; i++ )
        tmp.append( MathText( newrep[i], "<mtext>" + newrep[i] + "</mtext>" ) );
    return exec( name, origrep, tmp );
}


MathTextList NameSchemeForm::getRepresentation()
{
    MathTextList result;
    for ( int i = 0 ; i < Table->numRows() ; i++ ) {
        result.append( MathText( Table->text( i, 1 ),
                                 "<mtext>" + Table->text( i, 1 ) + "</mtext>" ) );
    }
    return result;
}


