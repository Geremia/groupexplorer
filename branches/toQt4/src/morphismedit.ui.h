//Added by qt3to4:
#include <QLabel>
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


void MorphismEditDialog::setMorphism( GECanvasMorphism* m )
{
    // copy the data I need:
    clearPairs();
    morphism = m;
    for ( uint i = 0 ; i < m->domgens.size() ; i++ )
        addPair( m->domgens[i], m->codgens[i] );
    ShowNameCheckBox->setChecked( m->showName );
    ShowDomCodCheckBox->setChecked( m->showDomCod );
    ShowDomCodCheckBox->setEnabled( m->showName );
    ShowEpiMonCheckBox->setChecked( m->showEpiMon );
    ShowGensCheckBox->setChecked( m->showGens );
    ManyLinesCheckBox->setChecked( m->manyLines );
    ManyLinesCheckBox->setEnabled(
            ( ( GEVSmall* )( m->getFromItem() ) )->hasElementLocations() &&
            ( ( GEVSmall* )( m->getToItem() ) )->hasElementLocations() );
    // configure lists to appropriately reflect data from morphism:
    updateDomainList();
    // configure labels to use the names of the morphism, its domain and codomain:
    InstructionsLabel->setText( QString(
            "Edit the homomorphism %1 : %2 -> %3 below.  "
            "Select an element from the domain and its image from the range, "
            "then click \"Map\" to connect them via %4." )
            .arg( m->getMorphism()->getName() )
            .arg( m->getMorphism()->getDomainName() )
            .arg( m->getMorphism()->getCodomainName() )
            .arg( m->getMorphism()->getName() ) );
    updateRemovablesLabel();
    // and now make us look pretty:
    assertWidgetSize( MapListFrame );
}


int MorphismEditDialog::exec( GECanvasMorphism * m )
{
    setMorphism( m );
    return QDialog::exec();
}


void MorphismEditDialog::HelpButton_clicked()
{
    GEHelpWindow::showHelp( "rf-um-morphedit.html" );
}


void MorphismEditDialog::updateRemovablesLabel()
{
    if ( domgens.size() == 0 ) {
        RemovablesTitleLabel->setText( QString(
                "No pairs have been placed in the morphism %1 yet.  "
                "(In this case the zero morphism is assumed.)" )
                .arg( morphism->getMorphism()->getName() ) );
    } else {
        RemovablesTitleLabel->setText( QString(
                "Here are the current pairs that have been placed in %1." )
                .arg( morphism->getMorphism()->getName() ) );
    }
}


void MorphismEditDialog::updateDomainList()
{
    DomainListBox->setEnabled( TRUE );
    DomainListBox->clear();
    GroupFile* dgf = morphism->getMorphism()->getDomain();
    GEltList domsgr = dgf->group.subgroupGenBy( domgens );
    for ( uint i = 0 ; i < dgf->group.order ; i++ ) {
        if ( !domsgr.contains( i ) ) {
            RichListBoxItem* rlbi = new RichListBoxItem(
                    dgf->getRepresentation( i ).HTML, DomainListBox );
            rlbi->setTag( i );
        }
    }
    if ( DomainListBox->count() ) {
        DomainListBox->setCurrentItem( 0 );
    } else {
        DomainListBox->setEnabled( FALSE );
        new RichListBoxItem( "<i>none left</i>", DomainListBox );
    }
    updateCodomain();
    OkButton->setEnabled( !DomainListBox->isEnabled() || !domgens.size() );
}


void MorphismEditDialog::addPair( GElt a, GElt b )
{
    // put the new widget in the stack of pairs:
    Q3HBox* container = new Q3HBox( MapListFrame );
    QLabel* lab = new QLabel( QString( "<b>%1 ( %2 ) = %3</b>" )
                              .arg( morphism->getMorphism()->getName() )
                              .arg( morphism->getMorphism()->getDomain()
                                    ->getRepresentation( a ).HTML )
                              .arg( morphism->getMorphism()->getCodomain()
                                    ->getRepresentation( b ).HTML ), container );
    lab->setAlignment( Qt::AlignCenter );
    QPushButton* but = new QPushButton( "Remove", container );
    connect( but, SIGNAL(clicked()), mapper, SLOT(map()) );
    mapper->setMapping( but, domgens.size() );
    MapListFrame->layout()->addWidget( container );
    pairWidgets.append( container );
    container->show();
    assertWidgetSize( MapListFrame );
    // keep the data up-to-date:
    domgens.resize( domgens.size() + 1 );
    domgens[domgens.size() - 1] = a;
    codgens.resize( codgens.size() + 1 );
    codgens[codgens.size() - 1] = b;
    // and update to match the data:
    updateDomainList();
    updateRemovablesLabel();
}


void MorphismEditDialog::init ()
{
    mapper = new QSignalMapper( this );
    connect( mapper, SIGNAL(mapped(int)), this, SLOT(removePair(int)) );
}


void MorphismEditDialog::removePair( int index )
{
    // erase this indexed element from both domgens and codgens:
    for ( uint j = index + 1 ; j < domgens.size() ; j++ ) {
        domgens[j-1] = domgens[j];
        codgens[j-1] = codgens[j];
    }
    domgens.resize( domgens.size() - 1 );
    codgens.resize( codgens.size() - 1 );
    // delete pair representation in gui if it exists:
    if ( uint( index ) < pairWidgets.count() )
        pairWidgets.at( index )->deleteLater();
    // and update to match the data:
    updateDomainList();
    updateRemovablesLabel();
}


void MorphismEditDialog::clearPairs()
{
    while ( domgens.size() ) removePair( 0 );
}


void MorphismEditDialog::SeeFullMapButton_clicked()
{
    GroupFile* dgf = morphism->getMorphism()->getDomain();
    GroupFile* cgf = morphism->getMorphism()->getCodomain();
    GEltList gendomain = dgf->group.subgroupGenBy( domgens );
    QString HTML;
    GEltList map = morphism->buildMap( domgens, codgens );
    if ( map.size() == 0 ) {
        HTML = "<font color=#FF0000>ERROR</font><br>\n"
               "This should not happen.  Please report this bug.";
    } else {
        QString left;
        QString right;
        for ( uint i = 0 ; i < dgf->group.order ; i++ ) {
            if ( gendomain.contains( i ) ) {
                left += QString( "%1 ( %2 ) = %3<br>\n" )
                        .arg( morphism->getMorphism()->getName() )
                        .arg( dgf->getRepresentation( i ).HTML )
                        .arg( cgf->getRepresentation( map[i] ).HTML );
            } else {
                right += QString( "%1 ( %2 ) = ?<br>\n" )
                         .arg( morphism->getMorphism()->getName() )
                         .arg( dgf->getRepresentation( i ).HTML );
            }
        }
        if ( !right.length() ) right = "<i>none; all have been assigned</i>";
        HTML = "<table border=1>\n"
               "<tr><td><b><nobr>Determined elements</nobr><br>\n"
               "           <nobr>of the domain</nobr></b></td>\n"
               "    <td><b><nobr>Undetermined elements</nobr><br>\n"
               "           <nobr>of the domain</nobr></b></td></tr>\n"
               "<tr><td>\n"
               + left +
               "</td><td>\n"
               + right +
               "</td></tr>\n"
               "</table>";
    }
    QMessageBox::information( this, "Homomorphism information", HTML,
                              QMessageBox::Ok );
}


bool divides ( uint a, uint b ) { return a && ( ( ( b / a ) * a ) == b ); }
bool MorphismEditDialog::anyValidImage( uint index )
{
//    QString indent = "                                                         ";
//    indent = indent.left( index * 4 );
//    qDebug( indent + QString( "anyValidImage %1[0..%2]" )
//            .arg( elt_list_to_string( codgenscopy ) ).arg( index ) );
    Group* dg = &( morphism->getMorphism()->getDomain()->group );
    Group* cg = &( morphism->getMorphism()->getCodomain()->group );
    uint ord = dg->orderOf( domgenscopy[index] );
    GEltList tmpdom( index + 1 );
    GEltList tmpcod( index + 1 );
    if ( index == domgenscopy.size() - 1 ) {
        for ( uint i = 0 ; i < tmpdom.size() ; i++ ) {
            tmpdom[i] = domgenscopy[i];
            tmpcod[i] = codgenscopy[i];
        }
    }
    for ( uint i = 0 ; i < cg->order ; i++ ) {
        if ( !divides( cg->orderOf( i ), ord ) ) continue; // faster
//        qDebug( QString( indent + "letting tmpcod[%1] = %2" )
//                .arg( index ).arg( i ) );
        if ( index == domgenscopy.size() - 1 ) {
            tmpcod[index] = i;
            GEltList test = morphism->buildMap( tmpdom, tmpcod );
            if ( test.size() > 0 ) {
//                qDebug( indent + "all good via full morphism " +
//                        elt_list_to_string( test ) + "; returning TRUE" );
                return TRUE;
            }
        } else {
            codgenscopy[index] = i;
            if ( anyValidImage( index + 1 ) ) {
//                qDebug( indent + "all good after recurring; returning TRUE" ); 
                return TRUE;
            }
        }
    }
//    qDebug( "failed to find any valid image @ " + QString::number( index ) );
    return FALSE;
}
GEltList MorphismEditDialog::validImages( uint index )
{
//    QString indent = "                                                         ";
//    indent = indent.left( index * 4 );
//    qDebug( indent + QString( "validImages %1[0..%2]" )
//            .arg( elt_list_to_string( codgenscopy ) ).arg( index ) );
    GEltList result;
    Group* dg = &( morphism->getMorphism()->getDomain()->group );
    Group* cg = &( morphism->getMorphism()->getCodomain()->group );
    uint ord = dg->orderOf( domgenscopy[index] );
    GEltList tmpdom( index + 1 );
    GEltList tmpcod( index + 1 );
    if ( index == domgenscopy.size() - 1 ) {
        for ( uint i = 0 ; i < tmpdom.size() ; i++ ) {
            tmpdom[i] = domgenscopy[i];
            tmpcod[i] = codgenscopy[i];
        }
    }
    for ( uint i = 0 ; i < cg->order ; i++ ) {
        if ( !divides( cg->orderOf( i ), ord ) ) continue; // faster
//        qDebug( QString( indent + "letting tmpcod[%1] = %2" )
//                .arg( index ).arg( i ) );
        if ( index == domgenscopy.size() - 1 ) {
            tmpcod[index] = i;
            GEltList test = morphism->buildMap( tmpdom, tmpcod );
            if ( test.size() > 0 ) {
//                qDebug( indent + "all good via full morphism " +
//                        elt_list_to_string( test ) + "; appending " +
//                        QString::number( i ) );
                result.resize( result.size() + 1 );
                result[result.size() - 1] = i;
            }
        } else {
            codgenscopy[index] = i;
            if ( anyValidImage( index + 1 ) ) {
//                qDebug( indent + "all good after recurring; appending " + 
//                        QString::number( i ) );
                result.resize( result.size() + 1 );
                result[result.size() - 1] = i;
            }
        }
    }
//    qDebug( indent + "found these valid images @ " + 
//            QString::number( index ) + ": " +
//            elt_list_to_string( result ) );
    return result;
}
void MorphismEditDialog::updateCodomain()
{
    CodomainListBox->setEnabled( TRUE );
    CodomainListBox->clear();
    if ( DomainListBox->isEnabled() && DomainListBox->selectedItem() ) {
        Group* dg = &( morphism->getMorphism()->getDomain()->group );
        domgenscopy = domgens.copy();
        domgenscopy.resize( domgens.size() + 1 );
        domgenscopy[domgens.size()] = 
            ( ( RichListBoxItem* )( DomainListBox->selectedItem() ) )->getTag();
        domgenscopy = dg->completeGens( domgenscopy, iota( dg->order ) );
        codgenscopy.fill( 0, domgenscopy.size() );
        for ( uint i = 0 ; i < codgens.size() ; i++ ) codgenscopy[i] = codgens[i];
//        qDebug( "domgens = " + elt_list_to_string( domgens ) );
//        qDebug( "codgens = " + elt_list_to_string( codgens ) );
//        qDebug( "domgenscopy = " + elt_list_to_string( domgenscopy ) );
//        qDebug( "codgenscopy = " + elt_list_to_string( codgenscopy ) );
        GEltList valids = validImages( domgens.size() );
        GroupFile* cgf = morphism->getMorphism()->getCodomain();
        if ( valids.size() ) {
            for ( uint i = 0 ; i < valids.size() ; i++ ) {
                RichListBoxItem* rlbi = new RichListBoxItem(
                        cgf->getRepresentation( valids[i] ).HTML, CodomainListBox );
                rlbi->setTag( valids[i] );
            }
        } else {
            QMessageBox::warning( this, "Homomorphism error",
                                  "The current homomorphism is invalid.\n"
                                  "Group Explorer should not let this happen.\n"
                                  "It may result from a corrupt sheet file\n"
                                  "(have you been editing them by hand? shame!)\n"
                                  "or perhaps this is a bug.  Please report it.\n"
                                  "\n"
                                  "To fix the current homomorphism,\n"
                                  "simply remove some pairs from it.",
                                  QMessageBox::Ok, Qt::NoButton );
        }
    }
    if ( CodomainListBox->count() ) {
        CodomainListBox->setCurrentItem( 0 );
    } else {
        CodomainListBox->setEnabled( FALSE );
    }
    MapButton->setEnabled( CodomainListBox->isEnabled() );
}


void MorphismEditDialog::MapButton_clicked()
{
    if ( DomainListBox->selectedItem() ) {
        GElt domelt = ( ( RichListBoxItem* )
                        ( DomainListBox->selectedItem() ) )->getTag();
        if ( CodomainListBox->selectedItem() ) {
            GElt codelt = ( ( RichListBoxItem* )
                            ( CodomainListBox->selectedItem() ) )->getTag();
            addPair( domelt, codelt );
            return;
        }
    }
    // this shouldn't happen, but:
    QMessageBox::warning( this, "Error",
                          "You must select an element of the domain "
                          "and an element of the range before clicking "
                          "\"Map.\"", QMessageBox::Ok, Qt::NoButton );
}


GEltList MorphismEditDialog::getFullMap()
{
    return morphism->buildMap( domgens, codgens );
}


GEltList MorphismEditDialog::getDomGens()
{
    return domgens.copy();
}


GEltList MorphismEditDialog::getCodGens()
{
    return codgens.copy();
}


void MorphismEditDialog::ShowNameCheckBox_clicked()
{
    ShowDomCodCheckBox->setEnabled( ShowNameCheckBox->isChecked() );
}


bool MorphismEditDialog::getShowName()
{
    return ShowNameCheckBox->isChecked();
}


bool MorphismEditDialog::getShowDomCod()
{
    return ShowNameCheckBox->isChecked() && ShowDomCodCheckBox->isChecked();
}


bool MorphismEditDialog::getShowEpiMon()
{
    return ShowEpiMonCheckBox->isChecked();
}


bool MorphismEditDialog::getShowGens()
{
    return ShowGensCheckBox->isChecked();
}


bool MorphismEditDialog::getManyLines()
{
    return ManyLinesCheckBox->isChecked() && ManyLinesCheckBox->isEnabled();
}
