//Added by qt3to4:
#include <Q3ValueList>
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


void PreferencesDialog::niceShow ()
{
    show();
    assertWidgetSize( FilesTab );
    setActiveWindow();
    raise();
}


void PreferencesDialog::init () {
    for ( uint i = 0 ; i < NumPGs ; i++ ) {
        prefGroupDirty[i] = prefGroupEmittable[i] = FALSE;
    }
    oldPrefs = 0;
}

void PreferencesDialog::destroy ()
{
    delete oldPrefs; // I own this because it is a copy of prefs from elsewhere
}

void PreferencesDialog::ApplyButton_clicked () {
    applyPreferences();
}

void PreferencesDialog::readPreferences ( GEPreferences* p ) {
    // here we set up our widgets to reflect the contents of p.
    // there are some parts of p we ignore, because they are not specified via
    // our widgets; stuff like positions and sizes of windows.
    
    // the lists of search paths for the first page:
    p->setGroup( "MainWindow" );
    GroupPathsList->clear();
    GroupPathsList->insertStringList( p->getStringList( "groupsearchpaths" ) );
    SheetPathsList->clear();
    SheetPathsList->insertStringList( p->getStringList( "sheetsearchpaths" ) );
    
    // the user info and maxgroupsize for second page:
    UserNameEdit->setText( p->getString( "username" ) );
    UserEmailEdit->setText( p->getString( "useremail", "" ) );
    BytesSpinBox->setValue( p->getNumber( "maximumGroupFileSize" ) / 1000 );
    int ord = p->getNumber( "maximumGroupOrder" );
    OrderCheckBox->setChecked( ord > 0 );
    OrderSpinBox->setEnabled( ord > 0 );
    if ( ord > 0 ) OrderSpinBox->setValue( ord );
    prefGroupDirty[PGOther] = FALSE; // because the above changes will mark it dirty
    
    // then we save p for later, in case we ever need to reread:
    lastPrefs = p;
}


void PreferencesDialog::rereadPreferences () {
    readPreferences( lastPrefs );
}


void PreferencesDialog::writePreferences ( GEPreferences * p ) {
    // here we save the contents of our widgets to p.
    // there are some parts of p we ignore, because they are not specified via
    // our widgets; stuff like positions and sizes of windows.
    
    for ( uint i = 0 ; i < NumPGs ; i++ ) {
        if ( prefGroupDirty[i] ) {
            switch ( i ) {
            case PGFiles:
                { // bracketing these so I can declare variables, like this:
                    QStringList paths;
                    p->setGroup( "MainWindow" );
                    for ( uint i = 0 ; i < GroupPathsList->count() ; i++ )
                        paths << GroupPathsList->text(i);
                    p->setStringList( "groupsearchpaths", paths );
                    paths.clear();
                    for ( uint i = 0 ; i < SheetPathsList->count() ; i++ )
                        paths << SheetPathsList->text(i);
                    p->setStringList( "sheetsearchpaths", paths );
                    break;
                }
            case PGOther:
                p->setGroup( "MainWindow" );
                p->setString( "username", UserNameEdit->text() );
                p->setString( "useremail", UserEmailEdit->text() );
                p->setNumber( "maximumGroupFileSize", BytesSpinBox->value() * 1000 );
                p->setNumber( "maximumGroupOrder", OrderCheckBox->isChecked() ?
                              OrderSpinBox->value() : -1 );
                break;
            }
            prefGroupDirty[i] = FALSE;
            prefGroupEmittable[i] = TRUE;
        }
    }
}


void PreferencesDialog::rewritePreferences () {
    writePreferences( lastPrefs );
}


void PreferencesDialog::CancelButton_clicked () {
    hide();
    rereadPreferences();
}


void PreferencesDialog::OKButton_clicked () {
    applyPreferences();
    hide();
}

GEPreferences* PreferencesDialog::oldPreferences ()
{
    return oldPrefs;
}

void PreferencesDialog::applyPreferences () {
    // first store the current preferences in oldPrefs before changing them:
    if ( oldPrefs ) {
        *oldPrefs = *lastPrefs; // just overwrite in memory
    } else {
        oldPrefs = lastPrefs->createCopy(); // get my first copy
    }
    // OK now we can finish, using prefGroup{Dirty,Emittable}[]:
    rewritePreferences();
    Q3ValueList<uint> changes;
    for ( uint i = 0 ; i < NumPGs ; i++ ) {
        if ( prefGroupEmittable[i] ) {
            changes << i;
            prefGroupEmittable[i] = FALSE;
        }
    }
    if ( changes.size() ) emit preferencesChanged( changes );
}


void PreferencesDialog::AddGroupPathButton_clicked()
{
    QString dir = Q3FileDialog::getExistingDirectory( QString::null, this );
    if ( !dir.isEmpty() ) {
        GroupPathsList->insertItem( QFileInfo( dir ).absFilePath() );
        prefGroupDirty[PGFiles] = TRUE;
    }
}


void PreferencesDialog::AddSheetPathButton_clicked()
{
    QString dir = Q3FileDialog::getExistingDirectory( QString::null, this );
    if ( !dir.isEmpty() ) {
        SheetPathsList->insertItem( QFileInfo( dir ).absFilePath() );
        prefGroupDirty[PGFiles] = TRUE;
    }
}


void PreferencesDialog::DelGroupPathButton_clicked()
{
    if ( GroupPathsList->currentItem() > -1 ) {
        GroupPathsList->removeItem( GroupPathsList->currentItem() );
        prefGroupDirty[PGFiles] = TRUE;
    }
}


void PreferencesDialog::DelSheetPathButton_clicked()
{
    if ( SheetPathsList->currentItem() > -1 ) {
        SheetPathsList->removeItem( SheetPathsList->currentItem() );
        prefGroupDirty[PGFiles] = TRUE;
    }
}


void PreferencesDialog::SheetPathsList_highlighted( int i )
{
    DelSheetPathButton->setEnabled( i > -1 );
}


void PreferencesDialog::GroupPathsList_highlighted( int i )
{
    DelGroupPathButton->setEnabled( i > -1 );
}


void PreferencesDialog::markOtherDataDirty()
{
    prefGroupDirty[PGOther] = TRUE;
}
