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


#include "linkcommandlist.h"
#include "gehelpwindow.h"
#include "groupfile.h"
#include "grouptable.h"
#include "ge.h"
#include "textinputdialog.h"
#include "nameschemedialog.h"
#include "gevisualizerlist.h"
#include "mainwindow.h"
#include "gesheetwindow.h"
#include "gesheet.h"
#include "idealist.h"
#include <q3stylesheet.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <Q3PtrList>


//
// First, the implementation of the class that keeps the list of link commands:
//

LinkCommand* LinkCommands::getNewCommand ( QStringList call, QWidget* originator )
{
    QString commandIdentifier = call[0]; // record the command name, then chop
    call.remove( call.begin() ); // so that all that remains are the parameters
    if ( commandIdentifier == "openGroup" ) {
        return new OpenGroupLC( call, originator );
    } else if ( commandIdentifier == "openGroupComputation" ) {
        return new OpenGroupComputationLC( call, originator );
    } else if ( commandIdentifier == "openHelp" ) {
        return new OpenHelpLC( call, originator );
    } else if ( commandIdentifier == "editNotes" ) {
        return new EditGroupNotesLC( call, originator );
    } else if ( commandIdentifier == "editRep" ) {
        return new EditNameSchemeLC( call, originator );
    } else if ( commandIdentifier == "newRep" ) {
        return new NewNameSchemeLC( call, originator );
    } else if ( commandIdentifier == "removeRep" ) {
        return new RemoveNameSchemeLC( call, originator );
    } else if ( commandIdentifier == "chooseRep" ) {
        return new ChooseNameSchemeLC( call, originator );
    } else if ( commandIdentifier == "showVisualizer" ) {
        return new ShowVisualizerLC( call, originator );
    } else if ( commandIdentifier == "openSheet" ) {
        return new OpenSheetLC( call, originator );
    } else if ( commandIdentifier == "createClassEquation" ) {
        return new CreateCESheetLC( call, originator );
    } else if ( commandIdentifier == "createSubgroupLattice" ) {
        return new CreateSLSheetLC( call, originator );
    } else if ( commandIdentifier == "createEmbedding" ) {
        return new CreateEmbeddingSheetLC( call, originator );
    } else if ( commandIdentifier == "createSES" ) {
        return new CreateSESLC( call, originator );
    } else if ( commandIdentifier == "createAllVisualizers" ) {
        return new CreateAllVisualizersLC( call, originator );
    } else if ( commandIdentifier == "createSolvableDecomposition" ) {
        return new CreateSolvableDecompLC( call, originator );
    } else if ( commandIdentifier == "showZnmIsomorphism" ) {
        return new ShowZnmIsomorphismLC( call, originator );
    } else if ( commandIdentifier == "showNoZnmIsomorphism" ) {
        return new ShowNoZnmIsomorphismLC( call, originator );
    }
    qWarning( "Error: no such link command: " +
              commandIdentifier + " " + call.join( " " ) );
    return 0; // bad
}


//
// Now, the implementations of the link command classes themselves:
//

// For this class, the default belongsOnStack() implementation
// suffices; it returns FALSE.
// Thus no undo() method need be implemented,
// and no constructor need exist to set a description;
// I only include one to give access to the ancestor's default behavior of
// storing the parameters for later use in redo().
OpenGroupLC::OpenGroupLC ( QStringList parameters, QWidget* originator )
    : LinkCommand ( parameters, originator )
{
}

void OpenGroupLC::redo ()
{
    if ( parameters.size() ) {
        MainWindow::original()->openGroupInfoWindow( parameters[0],
            ( parameters.size() > 1 ) ? parameters[1] : QString::null );
    }
}

OpenGroupComputationLC::OpenGroupComputationLC (
        QStringList parameters, QWidget* originator )
    : LinkCommand ( parameters, originator )
{
}

void OpenGroupComputationLC::redo ()
{
    if ( parameters.count() < 2 ) return;
    GroupFile* gf = GroupTable::findGroup( parameters[0] );
    parameters.remove( parameters.begin() );
    QString ideatitle = parameters.join( " " );
    for ( int i = 0 ; i < gf->ideas.size() ; i++ ) {
        if ( gf->ideas[i].factory->title == ideatitle ) {
            GEHelpWindow* hw = new GEHelpWindow( 0, 0, 0 );
            hw->setText( "<h2>Results of \"" + ideatitle +
                         "\" computation for " + gf->getPrimaryName().HTML + "</h2>\n" +
                         gf->ideas[i].longDescription );
            hw->showAndFocus();
            return;
        }
    }
}


// For this class, the default belongsOnStack() implementation
// suffices; it returns FALSE.
// Thus no undo() method need be implemented,
// and no constructor need exist to set a description;
// I only include one to give access to the ancestor's default behavior of
// storing the parameters for later use in redo().
OpenHelpLC::OpenHelpLC ( QStringList parameters, QWidget* originator )
    : LinkCommand ( parameters, originator )
{
}

void OpenHelpLC::redo ()
{
    if ( parameters.size() ) {
        GEHelpWindow::showHelp( parameters.join( "#" ) );
    }
}


// this class is interesting because it becomes one of two things in its constructor:
// if the user OKs the dialog exec()ed in the constructor, then the object has
// non-null new notes, and so it a) belongs on the stack, and b) does something when
// you call redo().
// but if the user Cancels the dialog exec()ed in the constructor, then the object
// has null new notes, and so it a) does not belong on the stack, and b) does nothing
// when you call redo().  this effectively gets it thrown away by mainwindow.
EditGroupNotesLC::EditGroupNotesLC ( QStringList parameters, QWidget* originator )
    : LinkCommand ( parameters, originator )
{
    oldnotes = QString::null;
    newnotes = QString::null;
    if ( parameters.size() ) {
        oldnotes = MainWindow::original()->preferences->getGroupNotes( parameters[0] );
        TextInputForm dialog;
        if ( dialog.exec( oldnotes,
                          "Edit notes for "
                          + GroupFile::decodePrimaryName( parameters[0] ) ) ) {
            newnotes = dialog.text();
        }
        if ( !newnotes.isNull() ) {
            setDescription( "change notes for "
                            + GroupFile::decodePrimaryName( parameters[0] ) );
        }
    } else {
        qWarning( "Error: edit group notes link with no group code parameter" );
    }
}

void EditGroupNotesLC::redo ()
{
    if ( !newnotes.isNull() && parameters.size() ) {
        MainWindow::original()->preferences->setGroupNotes( parameters[0], newnotes );
        MainWindow::original()->openGroupInfoWindow( parameters[0], "notes" );
    }
}

void EditGroupNotesLC::undo ()
{
    if ( parameters.size() ) {
        MainWindow::original()->preferences->setGroupNotes( parameters[0], oldnotes );
        MainWindow::original()->openGroupInfoWindow( parameters[0], "notes" );
    }
}

bool EditGroupNotesLC::belongsOnStack ()
{
    return !newnotes.isNull();
}


EditNameSchemeLC::EditNameSchemeLC ( QStringList parameters, QWidget* originator )
    : LinkCommand ( parameters, originator )
{
    if ( parameters.size() > 1 ) {
        bool ok;
        repNum = parameters[1].toUInt( &ok );
        if ( ok ) {
            oldrep = MainWindow::original()->preferences->getGroupRep(
                parameters[0], repNum );
            NameSchemeForm dialog;
            GroupFile* gf = GroupTable::findGroup( parameters[0] );
            if ( gf ) {
                if ( dialog.exec( gf->getPrimaryName(), gf->reps[0], oldrep ) ) {
                    newrep = dialog.getRepresentation();
                    setDescription( "change representation for "
                                    + gf->getPrimaryName().text );
                }
            } else {
                qWarning( "Error: edit name scheme link: group not found: "
                          + parameters[0] );
            }
        } else {
            qWarning( "Error: edit name scheme link: "
                      "second parameter must be nonnegative integer"
                      " (not " + parameters[1] + ")" );
        }
    } else {
        qWarning( "Error: edit name scheme link with fewer than 2 parameters: "
                  + parameters.join( " " ) );
    }
}
    
void EditNameSchemeLC::redo ()
{
    if ( newrep.size() ) {
        MainWindow::original()->preferences->setGroupRep( parameters[0], repNum, newrep );
        MainWindow::original()->openGroupInfoWindow(
                parameters[0],
                "user-defined-representation-" + QString::number( repNum ) );
    }
}

void EditNameSchemeLC::undo ()
{
    if ( newrep.size() ) {
        MainWindow::original()->preferences->setGroupRep( parameters[0], repNum, oldrep );
        MainWindow::original()->openGroupInfoWindow(
                parameters[0],
                "user-defined-representation-" + QString::number( repNum ) );
    }
}

bool EditNameSchemeLC::belongsOnStack ()
{
    return !!newrep.size();
}


NewNameSchemeLC::NewNameSchemeLC ( QStringList parameters, QWidget* originator )
    : LinkCommand ( parameters, originator )
{
    if ( parameters.size() ) {
        NameSchemeForm dialog;
        GroupFile* gf = GroupTable::findGroup( parameters[0] );
        QStringList blanks;
        for ( uint i = 0 ; i < gf->group.order ; i++ ) blanks << "change me";
        if ( gf ) {
            if ( dialog.exec( gf->getPrimaryName(), gf->reps[0], blanks ) ) {
                newrep = dialog.getRepresentation();
                setDescription( "add new representation for "
                                + gf->getPrimaryName().text );
            }
        } else {
            qWarning( "Error: new name scheme link: group not found: "
                      + parameters[0] );
        }
    } else {
        qWarning( "Error: new name scheme link with no group code parameter" );
    }
}
    
void NewNameSchemeLC::redo ()
{
    if ( newrep.size() ) {
        repNum = MainWindow::original()->preferences->numGroupReps( parameters[0] );
        MainWindow::original()->preferences->setGroupRep( parameters[0], repNum, newrep );
        MainWindow::original()->openGroupInfoWindow(
                parameters[0],
                "user-defined-representation-" + QString::number( repNum ) );
    }
}

void NewNameSchemeLC::undo ()
{
    if ( newrep.size() ) {
        MainWindow::original()->preferences->removeGroupRep( parameters[0], repNum );
        MainWindow::original()->openGroupInfoWindow(
            parameters[0], "user-defined-representations" );
    }
}

bool NewNameSchemeLC::belongsOnStack ()
{
    return !!newrep.size();
}


RemoveNameSchemeLC::RemoveNameSchemeLC ( QStringList parameters, QWidget* originator )
    : LinkCommand ( parameters, originator ), repNum( 0 )
{
    if ( parameters.size() > 1 ) {
        bool ok;
        repNum = parameters[1].toUInt( &ok );
        if ( ok ) {
            if ( MainWindow::original()->preferences->numGroupReps( parameters[0] )
                    > repNum ) {
                setDescription( "delete representation from "
                                + GroupFile::decodePrimaryName( parameters[0] ) );
            } else {
                qWarning( "Error: remove name scheme link: " + parameters[0]
                          + " has no naming scheme numbered "
                          + QString::number( repNum ) );
            }
        } else {
            qWarning( "Error: edit name scheme link: "
                      "second parameter must be nonnegative integer"
                      " (not " + parameters[1] + ")" );
        }
    } else {
        qWarning( "Error: edit name scheme link with fewer than 2 parameters: "
                  + parameters.join( " " ) );
    }
}
    
void RemoveNameSchemeLC::redo ()
{
    oldrep = MainWindow::original()->preferences->getGroupRep( parameters[0], repNum );
    MainWindow::original()->preferences->removeGroupRep( parameters[0], repNum );
    MainWindow::original()->openGroupInfoWindow(
        parameters[0], "user-defined-representations" );
}

void RemoveNameSchemeLC::undo ()
{
    MainWindow::original()->preferences->insertGroupRep( parameters[0], repNum, oldrep );
    MainWindow::original()->openGroupInfoWindow(
            parameters[0],
            "user-defined-representation-" + QString::number( repNum ) );
}

bool RemoveNameSchemeLC::belongsOnStack ()
{
    return TRUE;
}


ChooseNameSchemeLC::ChooseNameSchemeLC ( QStringList parameters,
                                               QWidget* originator )
    : LinkCommand ( parameters, originator ), newRepNum( 0 )
{
    if ( parameters.size() > 1 ) {
        gf = GroupTable::findGroup( parameters[0] );
        if ( gf ) {
            oldRepNum = gf->currentRep;
            bool ok;
            newRepNum = parameters[1].toUInt( &ok );
            if ( ok ) {
                numUserDefinedReps =
                    MainWindow::original()->preferences->numGroupReps( parameters[0] );
                numLoadedReps = gf->reps.count();
                if ( newRepNum < numUserDefinedReps + numLoadedReps ) {
                    setDescription( "change default representation for " +
                                    GroupFile::decodePrimaryName( parameters[0] ) );
                } else {
                    qWarning( "Error: choose name scheme link with too large an index: "
                              + parameters[1] );
                }
            } else {
                qWarning( "Error: choose name scheme link: "
                          "second parameter must be nonnegative integer"
                          " (not " + parameters[1] + ")" );
            }
        } else {
            qWarning( "Error: choose name scheme link with bad group code: "
                      + parameters[0] );
        }
    } else {
        qWarning( "Error: choose name scheme link command with fewer than 2 parameters: "
                  + parameters.join( " " ) );
    }
}

void ChooseNameSchemeLC::redo ()
{
    if ( gf ) {
        gf->currentRep = newRepNum;
        MainWindow::original()->preferences->setCurrentGroupRep(
            parameters[0], newRepNum );
        MainWindow::original()->openGroupInfoWindow( parameters[0], "namingschemes" );
        QMessageBox::warning(
            0, "Representation change",
            "<p>You have changed the default representation "
            "for the group " + gf->getPrimaryName().HTML + ".  "
            "Note that this will only affect text or images "
            "generated from now on.  Documents, sheets, etc. "
            "already open will remain unchanged.</p>",
            "Ok" );
    }
}

void ChooseNameSchemeLC::undo ()
{
    if ( gf ) {
        gf->currentRep = oldRepNum;
        MainWindow::original()->preferences->setCurrentGroupRep(
            parameters[0], oldRepNum );
        MainWindow::original()->openGroupInfoWindow( parameters[0], "namingschemes" );
        QMessageBox::warning(
            0, "Representation change",
            "<p>You have changed the default representation "
            "for the group " + gf->getPrimaryName().HTML + ".  "
            "Note that this will only affect text or images "
            "generated from now on.  Documents, sheets, etc. "
            "already open will remain unchanged.</p>",
            "Ok" );
    }
}

bool ChooseNameSchemeLC::belongsOnStack ()
{
    return TRUE;
}


ShowVisualizerLC::ShowVisualizerLC ( QStringList parameters, QWidget* originator )
    : LinkCommand ( parameters, originator )
{
}
    
void ShowVisualizerLC::redo ()
{
    //qDebug( "running ShowVisualizerLC with parameters %s",
    //        parameters.join( ", " ).latin1() );
    if ( parameters.size() ) {
        GEVisualizer* v = GEVisualizers::getNewVisualizer( parameters[0] );
        if ( v ) {
            if ( parameters.size() > 1 ) {
                GroupFile* gf = GroupTable::findGroup( parameters[1] );
                if ( gf ) {
                    v->setGroup( gf );
                    if ( parameters.size() > 2 ) {
                        QStringList paramcopy( parameters );
                        // drop the first two and tell the visualizer about the rest:
                        paramcopy.remove( paramcopy.begin() );
                        paramcopy.remove( paramcopy.begin() );
                        v->decode( paramcopy.join( " " ) );
                    }
					v->getLarge()->setIndependent( TRUE ); // delete when closed
                    v->getLarge()->resize( v->getLarge()->sizeHint() );
                    v->getLarge()->show();
                } else
                    qWarning( "showVisualizer link command: could not find group "
                              "with code \"" + parameters[1] + "\"" );
            } else
                qWarning( "showVisualizer link command: group code missing: "
                          + parameters.join( " " ) );
        }
    }
}


OpenSheetLC::OpenSheetLC ( QStringList parameters, QWidget* originator )
    : LinkCommand( parameters, originator )
{
}

void OpenSheetLC::redo ()
{
    MainWindow::original()->openSheet( parameters.join( " " ) );
}


CreateCESheetLC::CreateCESheetLC ( QStringList parameters, QWidget* originator )
    : LinkCommand( parameters, originator )
{
}

void CreateCESheetLC::redo ()
{
    if ( parameters.count() < 2 ) return;
    GroupFile* gf = GroupTable::findGroup( parameters[0] );
    if ( !gf ) return;
    // setup sheet layout:
    GEltListList ell = gf->group.conjClasses();
    CanvasLayoutGrid* grid = new CanvasLayoutGrid();
    grid->resize( 2, ell.count() * 2 + 1 );
    grid->setMargins( 20, 20 );
    grid->setPadding( 10, 10 );
    // put class equation (as numbers and +'s and =) into sheet:
    for ( int i = 0 ; i < ell.count() ; i++ ) {
        GECanvasText* text = new GECanvasText( 0 );
        QFont f = text->font();
        f.setPointSize( 20 );
        text->setFont( f );
        text->setText( QString::number( ell[i].size() ) );
        grid->insert( 0, i * 2, text );
        if ( i < ell.count() - 1 ) {
            text = new GECanvasText( 0 );
            QFont f = text->font();
            f.setPointSize( 20 );
            text->setFont( f );
            text->setText( "+" );
            grid->insert( 0, i * 2 + 1, text );
        } else {
            text = new GECanvasText( 0 );
            QFont f = text->font();
            f.setPointSize( 20 );
            text->setFont( f );
            text->setText( "=" );
            grid->insert( 0, i * 2 + 1, text );
        }
    }
    GECanvasText* text = new GECanvasText( 0 );
    text->setText( QString::number( gf->group.order ) );
    QFont f = text->font();
    f.setPointSize( 20 );
    text->setFont( f );
    grid->insert( 0, ell.count() * 2, text );
    // put illustrative visualizers into sheet:
    for ( int i = 0 ; i < ell.count() ; i++ ) {
        GEVisualizer* viz = GEVisualizers::getNewVisualizer( parameters[1] );
        viz->setGroup( gf );
        if ( parameters[1] == "CayleyDiagram" )
            viz->processChange( "labs", "N", QString::null );
        viz->processChange( "hig0", elt_set_to_code( ell[i] ), QString::null );
        viz->processChange( "par0", "C", QString::null );
        viz->getSmall()->setSize( 100, 100 );
        grid->insert( 1, i * 2, viz->getSmall() );
    }
    GEVisualizer* viz = GEVisualizers::getNewVisualizer( parameters[1] );
    viz->setGroup( gf );
    if ( parameters[1] == "CayleyDiagram" )
        viz->processChange( "labs", "N", QString::null );
    viz->processChange( "hig0", elt_set_list_to_code( ell ), QString::null );
    viz->processChange( "par0", "C", QString::null );
    viz->getSmall()->setSize( 100, 100 );
    grid->insert( 1, ell.count() * 2, viz->getSmall() );
    // now create a window, put the stuff in its sheet, and show it:
    GESheetWindow* win = GESheetWindow::createNewWindow();
    grid->placeOnCanvas( win->sheet(), 0, 0, 0 );
    QSize s = grid->size();
    win->sheet()->resize( s.width(), s.height() );
    win->show();
    delete grid;
}


CreateSLSheetLC::CreateSLSheetLC ( QStringList parameters, QWidget* originator )
    : LinkCommand( parameters, originator )
{
}

GEltListList CreateSLSheetLC::getAllUpwardPaths (
    GEltListList subgroups, QBitArray& inclusions, uint index )
{
    GEltListList result;
    GEltList sizeTiers;
    sizeTiers.fill( 0, subgroups[subgroups.count() - 1].size() );
    for ( int i = 1 ; i < subgroups.count() ; i++ )
        if ( subgroups[i-1].size() != subgroups[i].size() )
            sizeTiers[subgroups[i].size()-1] = sizeTiers[subgroups[i-1].size()-1] + 1;
    QBitArray minimalNode;
    minimalNode.fill( TRUE, subgroups.count() );
    /*
    qDebug( QString( "                                                                 " )
            .left( sizeTiers[subgroups[index].size()-1] * 4 ) + 
            " in " + elt_list_list_to_string( subgroups ) );
    qDebug( QString( "                                                                 " )
            .left( sizeTiers[subgroups[index].size()-1] * 4 ) + 
            "looking above " + elt_list_to_string( subgroups[index] ) );
    */
    for ( int i = index + 1 ; i < subgroups.count() - 1 ; i++ ) if ( minimalNode[i] ) {
        if ( elt_subset( subgroups[index], subgroups[i] ) ) {
            // there is someone above us, subgroups[i]:
            inclusions[index * subgroups.count() + i] = TRUE; // mark index subset i
            /*
            qDebug( QString( "                                                         " )
                    .left( sizeTiers[subgroups[index].size()-1] * 4 ) + 
                    "found " + elt_list_to_string( subgroups[i] ) + "; recurring..." );
            */
            GEltListList aboveThis = getAllUpwardPaths( subgroups, inclusions, i );
            for ( int j = 0 ; j < aboveThis.count() ; j++ )
                for ( uint k = 0 ; k < aboveThis[j].size() ; k++ )
                    if ( (int)(aboveThis[j][k]) < subgroups.count() )
                        minimalNode[aboveThis[j][k]] = FALSE;

            int distToI = sizeTiers[subgroups[i].size() - 1]
                          - sizeTiers[subgroups[index].size() - 1];
            GEltList tmp;
            tmp.fill( subgroups.count(), distToI + aboveThis[0].size() );
            tmp[distToI - 1] = i;
            for ( int j = 0 ; j < aboveThis.count() ; j++ ) {
                for ( uint k = 0 ; k < aboveThis[j].size() ; k++ )
                    tmp[distToI + k] = aboveThis[j][k];
                result.append( tmp.copy() );
            }
            /*
            qDebug( QString( "                                                         " )
                    .left( sizeTiers[subgroups[index].size()-1] * 4 ) + 
                    "done recurring; returning " + elt_list_list_to_string( result ) );
            */
        }
    }
    if ( !result.count() ) {
        inclusions[index * subgroups.count() + subgroups.count() - 1] = TRUE;
            // above line marks index subset full group
        int distToTop = sizeTiers[subgroups[subgroups.count() - 1].size() - 1]
                        - sizeTiers[subgroups[index].size() - 1];
        GEltList blanks;
        blanks.fill( subgroups.count(), distToTop - 1 );
        result.append( blanks );
        /*
        qDebug( QString( "                                                             " )
                .left( sizeTiers[subgroups[index].size()-1] * 4 ) + 
                "found nothing; returning " + elt_list_list_to_string( result ) );
        */
    }
    return result;
}

void CreateSLSheetLC::redo ()
{
    if ( parameters.count() < 2 ) return;
    GroupFile* gf = GroupTable::findGroup( parameters[0] );
    if ( !gf ) return;

    // prepare data structures:
    GEltListList sgrs = gf->group.subgroups();
    GEltList tier( 1 );
    tier[0] = 0;
    sgrs.insert( sgrs.begin(), tier.copy() );
    if ( gf->group.order > 1 ) {
        tier = iota( gf->group.order );
        sgrs.append( tier.copy() );
    }

    GEltList sizePartition( sgrs.count() );
    sizePartition[0] = 0;
    GEltListList tiers;
    tier.fill( 0, 1 );
    uint maxwidth = 0;
    for ( uint i = 1 ; i < sizePartition.size() ; i++ ) {
        sizePartition[i] = sizePartition[i - 1];
        if ( sgrs[i].size() > sgrs[i - 1].size() ) {
            tiers.append( tier.copy() );
            tier.resize( 1 );
            tier[0] = i;
            sizePartition[i]++;
        } else {
            tier.resize( tier.size() + 1 );
            tier[tier.size() - 1] = i;
            if ( tier.size() > maxwidth ) maxwidth = tier.size();
        }
        /*
        qDebug( QString( "sgr %1: %2\t\tpart%3\ttier %4\ttiers%5" )
                .arg( i ).arg( elt_list_to_string( sgrs[i] ) )
                .arg( sizePartition[i] ).arg( elt_list_to_string( tier ) )
                .arg( elt_list_list_to_string( tiers ) ) );
        */
    }
    tiers.append( tier.copy() );

    // organize the tiers:
    QBitArray inclusions;
    inclusions.fill( FALSE, sgrs.count() * sgrs.count() );
    GEltListList ups = getAllUpwardPaths( sgrs, inclusions );
    /*
    qDebug( "In the subgroups" );
    qDebug( elt_list_list_to_string( sgrs ).replace( "{ ", "\t" ).replace( " }", "" )
            .replace( "] [", "]\n\t[" ) );
    qDebug( "I found these paths" );
    for ( uint i = 0 ; i < ups.count() ; i++ ) {
        QString dbg = "";
        for ( uint j = 0 ; j < ups[i].size() ; j++ ) {
            if ( j ) dbg += " --> ";
            dbg += elt_list_to_string( sgrs[ups[i][j]] );
        }
        qDebug( "\t" + dbg );
    }
    */

    // setup sheet layout:
    Q3PtrList<GEVisualizer> sgviz;
    for ( int i = 0 ; i < sgrs.count() ; i++ ) {
        GEVisualizer* viz = GEVisualizers::getNewVisualizer( parameters[1] );
        viz->setGroup( gf );
        if ( parameters[1] == "CayleyDiagram" )
            viz->processChange( "labs", "N", QString::null );
        viz->processChange( "hig0", elt_set_to_code( sgrs[i] ), QString::null );
        viz->getSmall()->setSize( 75, 75 );
        sgviz.append( viz );
    }
    CanvasLayoutGrid* grid = new CanvasLayoutGrid();
    grid->resize( ups[0].size(), ups.count() );
    grid->setPadding( 10, 50 );
    grid->setMargins( 0, 75 );
    QBitArray alreadyPlaced;
    alreadyPlaced.fill( FALSE, sgrs.count() );
    for ( int i = 0 ; i < ups.count() ; i++ ) {
        for ( uint j = 0 ; j < ups[i].size() ; j++ ) {
            if ( ( (int)(ups[i][j]) < sgrs.count() ) && !alreadyPlaced[ups[i][j]] ) {
                grid->insert( ups[i].size() - j - 1, i,
                              sgviz.at( ups[i][j] )->getSmall() );
                alreadyPlaced[ups[i][j]] = TRUE;
            }
        }
    }
    CanvasLayoutGrid* master = new CanvasLayoutGrid();
    master->resize( 4, 1 );
    master->setMargins( 20, 20 );
    master->setPadding( 0, 30 );
    GECanvasText* text = new GECanvasText( 0 );
    QFont f = text->font();
    f.setPointSize( 30 );
    text->setFont( f );
    text->setText( "Lattice of subgroups for " + gf->getPrimaryName().text );
    master->insert( 0, 0, text );
    master->insert( 1, 0, sgviz.at( sgrs.count() - 1 )->getSmall() );
    master->insert( 2, 0, grid );
    master->insert( 3, 0, sgviz.at( 0 )->getSmall() );
    // now create a window, put the stuff in its sheet, and show it:
    GESheetWindow* win = GESheetWindow::createNewWindow();
    master->placeOnCanvas( win->sheet(), 0, 0, 0 );
    QSize s = master->size();
    win->sheet()->resize( s.width(), s.height() );
    uint count = 0;
    for ( int i = 0 ; i < sgrs.count() ; i++ ) {
        for ( int j = 0 ; j < sgrs.count() ; j++ ) {
            if ( inclusions[i * sgrs.count() + j] ) {
                GECanvasMorphism* m = new GECanvasMorphism( win->sheet() );
                m->setFromSmall( sgviz.at( i )->getSmall() );
                m->setToSmall( sgviz.at( j )->getSmall() );
                m->domgens = gf->generatorlists[0].copy();
                m->codgens = gf->generatorlists[0].copy();
                m->getMorphism()->setMap( m->buildMap( m->domgens, m->codgens ) );
                m->getMorphism()->setName(
                    QString( "<i>i</i><sub>%1</sub>" ).arg( count++ ) );
                m->showName = FALSE;
                m->show();
            }
        }
    }
    win->show();
    delete master;
}


CreateEmbeddingSheetLC::CreateEmbeddingSheetLC (
        QStringList parameters, QWidget* originator )
    : LinkCommand( parameters, originator )
{
}

void CreateEmbeddingSheetLC::redo ()
{
    if ( parameters.count() < 3 ) return;
    GroupFile* G1 = GroupTable::findGroup( parameters[0] );
    if ( !G1 ) {
        qWarning( QString( "Could not find %1 in library" ).arg( parameters[0] ) );
        return;
    }
    GroupFile* G2 = GroupTable::findGroup( parameters[1] );
    if ( !G2 ) {
        qWarning( QString( "Could not find %1 in library" ).arg( parameters[1] ) );
        return;
    }

    QString typeName;
    GEltList H2;
    QString title;
    if ( parameters.count() == 4 ) {
        H2 = code_to_elt_set( parameters[2] );
        typeName = parameters[3];
    } else {
        H2 = iota( G2->group.order );
        typeName = parameters[2];
    }
    GEltList map = GECanvasMorphism::tryEmbedding( G1, G2, H2 );
    if ( !map.size() ) {
        QMessageBox::information( 0,
            "Cannot create embedding map", "There is no such embedding.",
            QMessageBox::Ok );
        return;
    }

    CanvasLayoutGrid* master = new CanvasLayoutGrid();
    master->resize( 2, 1 );
    master->setPadding( 0, 30 );
    master->setMargins( 30, 30 );
    GECanvasText* text = new GECanvasText( 0 );
    QFont f = text->font();
    f.setPointSize( 30 );
    text->setFont( f );
    text->setText(
        "Embedding " + G1->getPrimaryName().text + " in " + G2->getPrimaryName().text );
    master->insert( 0, 0, text );

    CanvasLayoutGrid* grid = new CanvasLayoutGrid();
    grid->resize( 1, 2 );
    grid->setPadding( 100, 0 );
    master->insert( 1, 0, grid );

    GEVisualizer* V1 = GEVisualizers::getNewVisualizer( typeName );
    if ( typeName == "CayleyDiagram" )
        V1->processChange( "labs", "N", QString::null );
    if ( typeName == "MultiplicationTable" )
        V1->processChange( "axis", "left", QString::null );
    V1->setGroup( G1 );
    V1->getSmall()->setSize( 150, 150 );
    grid->insert( 0, 0, V1->getSmall() );

    GEVisualizer* V2 = GEVisualizers::getNewVisualizer( typeName );
    if ( typeName == "CayleyDiagram" )
        V2->processChange( "labs", "N", QString::null );
    if ( typeName == "MultiplicationTable" )
        V2->processChange( "axis", "left", QString::null );
    V2->setGroup( G2 );
    GEltList mapImage;
    for ( uint i = 0 ; i < map.size() ; i++ ) {
        if ( !mapImage.contains( map[i] ) ) {
            mapImage.resize( mapImage.size() + 1 );
            mapImage[mapImage.size() - 1] = map[i];
        }
    }
    V2->processChange( "hig0", elt_set_to_code( mapImage ), QString::null );
    int sgrIdx;
    for ( sgrIdx = 0 ; sgrIdx < G2->group.subgroups().count() ; sgrIdx++ )
        if ( elt_sets_are_equal( mapImage, G2->group.subgroups()[sgrIdx] ) ) break;
    V2->processChange( "orgz", QString::number( sgrIdx + 1 ), QString::null );
    V2->getSmall()->setSize( 150, 150 );
    grid->insert( 0, 1, V2->getSmall() );

    GESheetWindow* win = GESheetWindow::createNewWindow();
    master->placeOnCanvas( win->sheet(), 0, 0, 0 );
    QSize s = master->size();
    win->sheet()->resize( s.width(), s.height() );

    GECanvasMorphism* m = new GECanvasMorphism( win->sheet() );
    m->setFromSmall( V1->getSmall() );
    m->setToSmall( V2->getSmall() );
    m->domgens = G1->generatorlists[0].copy();
    m->codgens.resize( m->domgens.size() );
    for ( uint i = 0 ; i < m->domgens.size() ; i++ ) m->codgens[i] = map[m->domgens[i]];
    m->getMorphism()->setMap( map );
    m->getMorphism()->setName( "<i>e</i>" );
    m->showName = FALSE;
    m->manyLines = TRUE;
    m->show();

    win->show();
    delete master;
}


CreateSESLC::CreateSESLC ( QStringList parameters, QWidget* originator )
    : LinkCommand( parameters, originator )
{
}

void CreateSESLC::redo ()
{
    if ( parameters.count() < 3 ) return;
    GroupFile* G = GroupTable::findGroup( parameters[0] );
    if ( !G ) {
        qWarning( QString( "Could not find %1 in library" ).arg( parameters[0] ) );
        return;
    }
    GEltList subset = code_to_elt_set( parameters[1] );
    QString typeName = parameters[2];
    if ( !( ( subset.size() == 1 ) || ( subset.size() == G->group.order ) ||
            G->group.isNormal( subset ) ) ) return; // just be sure it's normal
    GroupFile* source = SubgroupsGIF::findSubgroupInLibrary( G, subset );
    if ( !source ) return;
    GroupFile dummy;
    GEltList quotientMap;
    SubgroupsGIF::computeQuotient( G->group, subset, &dummy, &quotientMap );
    GEltList isomorphism;
    GroupFile* quotient = SubgroupsGIF::findGroupInLibrary( &dummy, &isomorphism );
    for ( uint i = 0 ; i < quotientMap.size() ; i++ )
        quotientMap[i] = isomorphism[quotientMap[i]];
    if ( !quotient ) return;

    CanvasLayoutGrid* master = new CanvasLayoutGrid();
    master->resize( 2, 1 );
    master->setPadding( 0, 30 );
    master->setMargins( 30, 30 );
    GECanvasText* text = new GECanvasText( 0 );
    QFont f = text->font();
    f.setPointSize( 30 );
    text->setFont( f );
    text->setText(
        "Short exact sequence showing " + G->getPrimaryName().text +
        " / " + source->getPrimaryName().text +
        " = " + quotient->getPrimaryName().text );
    master->insert( 0, 0, text );

    CanvasLayoutGrid* grid = new CanvasLayoutGrid();
    grid->resize( 3, 5 );
    grid->setPadding( 100, 0 );
    master->insert( 1, 0, grid );

    GEVisualizer* V[5];
    for ( uint i = 0 ; i < 5 ; i++ ) {
        V[i] = GEVisualizers::getNewVisualizer( typeName );
        if ( typeName == "CayleyDiagram" )
            V[i]->processChange( "labs", "N", QString::null );
        if ( typeName == "MultiplicationTable" )
            V[i]->processChange( "axis", "left", QString::null );
    }
    V[0]->setGroup( GroupTable::findGroup( "Z_1|Nathan-_Carter-_(ncarter@bentley.edu)" ) );
    V[0]->processChange( "hig0", elt_set_to_code( iota( 1 ) ), QString::null );
    V[1]->setGroup( source );
    V[1]->processChange( "hig0", elt_set_to_code( source->group.elements() ),
                         QString::null );
    V[2]->setGroup( G );
    V[2]->processChange( "hig0", elt_set_to_code( subset ), QString::null );
    if ( typeName == "MultiplicationTable" ) {
        uint sgrnum = 0;
        GEltListList sgrs = G->group.subgroups();
        for ( int i = 0 ; i < sgrs.count() ; i++ ) {
            if ( elt_sets_are_equal( sgrs[i], subset ) ) {
                sgrnum = i + 1;
                break;
            }
        }
        if ( sgrnum )
            V[2]->processChange( "orgz", QString::number( sgrnum ), QString::null );
    }
    V[3]->setGroup( quotient );
    V[3]->processChange( "hig0", elt_set_to_code( iota( 1 ) ), QString::null );
    V[4]->setGroup( V[0]->getGroup() );
    V[4]->processChange( "hig0", elt_set_to_code( iota( 1 ) ), QString::null );
    for ( uint i = 0 ; i < 5 ; i++ ) {
        V[i]->getSmall()->setSize(
            50 + 100 * V[i]->getGroup()->group.order / G->group.order,
            50 + 100 * V[i]->getGroup()->group.order / G->group.order );
        grid->insert( 0, i, V[i]->getSmall() );
        text = new GECanvasText( 0 );
        QFont f = text->font();
        f.setPointSize( 20 );
        text->setFont( f );
        text->setText( V[i]->getGroup()->getPrimaryName().text );
        grid->insert( 1, i, text );
    }

    text = new GECanvasText( 0 );
    text->setText( "Im(id) = Ker(e)" );
    grid->insert( 2, 1, text );
    text = new GECanvasText( 0 );
    text->setText( "Im(e) = Ker(q)" );
    grid->insert( 2, 2, text );
    text = new GECanvasText( 0 );
    text->setText( "Im(q) = Ker(z)" );
    grid->insert( 2, 3, text );

    GESheetWindow* win = GESheetWindow::createNewWindow();
    master->placeOnCanvas( win->sheet(), 0, 0, 0 );
    QSize s = master->size();
    win->sheet()->resize( s.width(), s.height() );

    GECanvasMorphism* m = new GECanvasMorphism( win->sheet() );
    m->setFromSmall( V[0]->getSmall() );
    m->setToSmall( V[1]->getSmall() );
    m->getMorphism()->setName( "<i>id</i>" );
    m->manyLines = TRUE;
    m->showName = TRUE;
    m->updateLabel();
    m->show();

    m = new GECanvasMorphism( win->sheet() );
    m->setFromSmall( V[3]->getSmall() );
    m->setToSmall( V[4]->getSmall() );
    m->getMorphism()->setName( "<i>z</i>" );
    m->manyLines = TRUE;
    m->showName = TRUE;
    m->updateLabel();
    m->show();

    m = new GECanvasMorphism( win->sheet() );
    m->setFromSmall( V[2]->getSmall() );
    m->setToSmall( V[3]->getSmall() );
    m->domgens = G->generatorlists[0].copy();
    m->codgens.resize( m->domgens.size() );
    for ( uint i = 0 ; i < m->domgens.size() ; i++ )
        m->codgens[i] = quotientMap[m->domgens[i]];
    m->getMorphism()->setMap( quotientMap );
    m->getMorphism()->setName( "<i>q</i>" );
    m->manyLines = TRUE;
    m->showName = TRUE;
    m->updateLabel();
    m->show();

    m = new GECanvasMorphism( win->sheet() );
    m->setFromSmall( V[1]->getSmall() );
    m->setToSmall( V[2]->getSmall() );
    GEltList map = GECanvasMorphism::tryEmbedding( V[1]->getGroup(), G, subset );
    m->domgens = V[1]->getGroup()->generatorlists[0].copy();
    m->codgens.resize( m->domgens.size() );
    for ( uint i = 0 ; i < m->domgens.size() ; i++ ) m->codgens[i] = map[m->domgens[i]];
    m->getMorphism()->setMap( map );
    m->getMorphism()->setName( "<i>e</i>" );
    m->manyLines = TRUE;
    m->showName = TRUE;
    m->updateLabel();
    m->show();

    win->show();
    delete master;
}


CreateAllVisualizersLC::CreateAllVisualizersLC (
        QStringList parameters, QWidget* originator )
    : LinkCommand( parameters, originator )
{
}

void CreateAllVisualizersLC::redo ()
{
    if ( !parameters.count() ) return;
    GroupFile* G = GroupTable::findGroup( parameters[0] );
    if ( !G ) {
        qWarning( QString( "Could not find %1 in library" ).arg( parameters[0] ) );
        return;
    }

    CanvasLayoutGrid* master = new CanvasLayoutGrid();
    master->resize( 2, 1 );
    master->setPadding( 0, 30 );
    master->setMargins( 30, 30 );
    GECanvasText* text = new GECanvasText( 0 );
    QFont f = text->font();
    f.setPointSize( 30 );
    text->setFont( f );
    text->setText( "All visualizers for " + G->getPrimaryName().text );
    master->insert( 0, 0, text );

    QStringList vnames = GEVisualizers::allVisualizerNames();
    vnames.remove( "SymmetryObject" );

    CanvasLayoutGrid* grid = new CanvasLayoutGrid();
    grid->resize( 2, vnames.count() );
    grid->setPadding( 100, 0 );
    master->insert( 1, 0, grid );

    GEVisualizer** V = new GEVisualizer*[vnames.count()];
    for ( int i = 0 ; i < vnames.count() ; i++ ) {
        V[i] = GEVisualizers::getNewVisualizer( vnames[i] );
        if ( vnames[i] == "CayleyDiagram" )
            V[i]->processChange( "labs", "N", QString::null );
        if ( vnames[i] == "MultiplicationTable" )
            V[i]->processChange( "axis", "left", QString::null );
        V[i]->setGroup( G );
        V[i]->getSmall()->setSize( 150, 150 );
        grid->insert( 0, i, V[i]->getSmall() );
        GECanvasText* text = new GECanvasText( 0 );
        text->setText( GEVisualizers::printableName( vnames[i] ) );
        grid->insert( 1, i, text );
    }

    GESheetWindow* win = GESheetWindow::createNewWindow();
    master->placeOnCanvas( win->sheet(), 0, 0, 0 );
    QSize s = master->size();
    win->sheet()->resize( s.width(), s.height() );

    for ( int i = 1 ; i < vnames.count() ; i++ ) {
        GECanvasMorphism* m = new GECanvasMorphism( win->sheet() );
        m->setFromSmall( V[i-1]->getSmall() );
        m->setToSmall( V[i]->getSmall() );
        GEltList map = iota( G->group.order );
        m->domgens = G->generatorlists[0].copy();
        m->codgens = m->domgens.copy();
        m->getMorphism()->setMap( map );
        m->getMorphism()->setName( QString( "<i>id</i><sub>%1</sub>" ).arg( i ) );
        m->manyLines = TRUE;
        m->showName = TRUE;
        m->updateLabel();
        m->show();
    }

    win->show();
    delete master;
    delete V;
}


CreateSolvableDecompLC::CreateSolvableDecompLC (
        QStringList parameters, QWidget* originator )
    : LinkCommand( parameters, originator )
{
}

void CreateSolvableDecompLC::redo ()
{
    // legal param lengths are 2, 5, 8, 11, ...
    if ( ( parameters.count() < 2 ) || ( ( parameters.count() - 2 ) % 3 ) )
        return;
    GroupFile* baseGroup = GroupTable::findGroup( parameters[1] );
    if ( !baseGroup ) return;
    GroupFile* bigGroup;
    if ( parameters.count() > 2 ) {
        bigGroup = GroupTable::findGroup( parameters[parameters.count() - 3] );
        if ( !bigGroup ) return;
    } else {
        bigGroup = baseGroup;
    }

    CanvasLayoutGrid* master = new CanvasLayoutGrid();
    master->resize( 3, 1 );
    master->setPadding( 0, 30 );
    master->setMargins( 30, 30 );
    GECanvasText* text = new GECanvasText( 0 );
    QFont f = text->font();
    f.setPointSize( 30 );
    text->setFont( f );
    text->setText( "Solvable decomposition for " + bigGroup->getPrimaryName().text );
    master->insert( 0, 0, text );
    text = new GECanvasText( 0 );
    f = text->font();
    f.setPointSize( 14 );
    text->setFont( f );
    text->setText( "The top row is the solvable decomposition.\n"
                   "The bottom row are (abelian) quotient groups." );
    master->insert( 1, 0, text );

    int columns = ( parameters.count() - 2 ) / 3 + 1;
    CanvasLayoutGrid* inner = new CanvasLayoutGrid();
    inner->resize( 2, columns );
    inner->setPadding( 100, 100 );
    GEVisualizer** topRow = new GEVisualizer*[columns];
    GEVisualizer** botRow = new GEVisualizer*[columns];

    for ( int i = 0 ; i < columns ; i++ ) {
        for ( uint j = 0 ; j < (uint)( i ? 2 : 1 ) ; j++ ) {
            GEVisualizer* V = GEVisualizers::getNewVisualizer( parameters[0] );
            GroupFile* gf = 0;
            if ( i ) {
                V->setGroup( gf =
                    GroupTable::findGroup( parameters[2 + ( 2 * j ) + 3 * ( i - 1 )] ) );
                if ( j ) {
                    botRow[i] = V;
                    V->processChange( "hig0", elt_set_to_code( iota( 1 ) ),
                                      QString::null );
                } else {
                    topRow[i] = V;
                    int subgroupIndex = parameters[3 + 3 * ( i - 1 )].toInt() - 1;
                    V->processChange(
                        "hig0", elt_set_to_code( gf->group.subgroups()[subgroupIndex] ),
                        QString::null );
                    if ( parameters[0] == "MultiplicationTable" )
                        V->processChange( "orgz", QString::number( subgroupIndex + 1 ),
                                          QString::null );
                }
            } else {
                V->setGroup( gf = baseGroup );
                topRow[0] = V;
                V->processChange( "hig0", elt_set_to_code( iota( gf->group.order ) ),
                                  QString::null );
            }
            if ( parameters[0] == "CayleyDiagram" )
                V->processChange( "labs", "N", QString::null );
            if ( parameters[0] == "MultiplicationTable" )
                V->processChange( "axis", "left", QString::null );
            V->getSmall()->setSize(
                50 + 150 * gf->group.order / bigGroup->group.order,
                50 + 150 * gf->group.order / bigGroup->group.order );

            text = new GECanvasText( 0 );
            f = text->font();
            f.setPointSize( 14 );
            text->setFont( f );
            text->setText( gf->getPrimaryName().text );

            CanvasLayoutGrid* dyad = new CanvasLayoutGrid();
            dyad->resize( 2, 1 );
            dyad->setPadding( 0, 20 );
            dyad->insert( 0, 0, V->getSmall() );
            dyad->insert( 1, 0, text );
            inner->insert( j, i, dyad );
        }
    }
    master->insert( 2, 0, inner );

    GESheetWindow* win = GESheetWindow::createNewWindow();
    master->placeOnCanvas( win->sheet(), 0, 0, 0 );
    QSize s = master->size();
    win->sheet()->resize( s.width(), s.height() );

    GECanvasMorphism* m;
    for ( int i = 1 ; i < columns ; i++ ) {
        int subgroupIndex = parameters[3 + 3 * ( i - 1 )].toInt() - 1;
        GEltList sgr = topRow[i]->getGroup()->group.subgroups()[subgroupIndex];
        // insert embedding map from top row guy i-1 to guy i
        m = new GECanvasMorphism( win->sheet() );
        m->setFromSmall( topRow[i - 1]->getSmall() );
        m->setToSmall( topRow[i]->getSmall() );
        GEltList map = GECanvasMorphism::tryEmbedding(
            topRow[i - 1]->getGroup(), topRow[i]->getGroup(), sgr );
        m->domgens = topRow[i - 1]->getGroup()->generatorlists[0].copy();
        m->codgens.resize( m->domgens.size() );
        for ( uint k = 0 ; k < m->domgens.size() ; k++ )
            m->codgens[k] = map[m->domgens[k]];
        m->getMorphism()->setMap( map );
        m->getMorphism()->setName( QString( "<i>e</i><sub>%1</sub>" ).arg( i ) );
        m->manyLines = TRUE;
        m->showName = TRUE;
        m->updateLabel();
        m->show();
        // insert quotient map from top row guy i to bot row guy i
        GroupFile dummy;
        GEltList quotientMap;
        SubgroupsGIF::computeQuotient(
            topRow[i]->getGroup()->group, sgr, &dummy, &quotientMap );
        GEltList isomorphism;
        SubgroupsGIF::findGroupInLibrary( &dummy, &isomorphism );
        for ( uint k = 0 ; k < quotientMap.size() ; k++ )
            quotientMap[k] = isomorphism[quotientMap[k]];
        m = new GECanvasMorphism( win->sheet() );
        m->setFromSmall( topRow[i]->getSmall() );
        m->setToSmall( botRow[i]->getSmall() );
        m->domgens = topRow[i]->getGroup()->generatorlists[0].copy();
        m->codgens.resize( m->domgens.size() );
        for ( uint k = 0 ; k < m->domgens.size() ; k++ )
            m->codgens[k] = quotientMap[m->domgens[k]];
        m->getMorphism()->setMap( quotientMap );
        m->getMorphism()->setName( QString( "<i>q</i><sub>%1</sub>" ).arg( i ) );
        m->manyLines = TRUE;
        m->showName = TRUE;
        m->updateLabel();
        m->show();
    }

    win->show();
    delete master;
    delete topRow;
    delete botRow;
}


ShowZnmIsomorphismLC::ShowZnmIsomorphismLC ( QStringList parameters, QWidget* originator )
    : LinkCommand( parameters, originator )
{
}

void ShowZnmIsomorphismLC::redo ()
{
    if ( parameters.count() < 3 ) return;
    GroupFile* G = GroupTable::findGroup( parameters[0] );
    if ( !G ) {
        qWarning( QString( "Could not find %1 in library" ).arg( parameters[0] ) );
        return;
    }
    uint m = parameters[1].toUInt();
    uint n = parameters[2].toUInt();
    if ( ( m < 2 ) || ( n < 2 ) || ( m * n != G->group.order ) ) {
        qWarning( QString( "Invalid m,n for Znm isomorphic link command: %1,%2 (Z_%3)" )
                  .arg( m ).arg( n ).arg( G->group.order ) );
        return;
    }

    GElt gm = 0;
    GElt gn = 0;
    for ( GElt g = 0 ; ( g < G->group.order ) && !( gm && gn ) ; g++ ) {
        if ( !gm && ( G->group.orderOf( g ) == m ) ) gm = g;
        if ( !gn && ( G->group.orderOf( g ) == n ) ) gn = g;
    }
    if ( !( gm && gn ) ) {
        qWarning( QString( "Znm isomorphism link unable to find elements of orders %1,%2 "
                           "in group %3" )
                  .arg( m ).arg( n ).arg( G->getPrimaryName().text ) );
        return;
    }
    GEltList tmp( 2 );
    tmp[0] = gm;
    tmp[1] = gn;
    QString gmgn = elt_list_to_code( tmp );
    tmp.resize( 1 );
    tmp[0] = G->group.op( gm, gn );
    QString gmn = elt_list_to_code( tmp );
    QString iota2 = elt_list_to_code( iota( 2 ) );

    CanvasLayoutGrid* master = new CanvasLayoutGrid();
    master->resize( 3, 1 );
    master->setPadding( 0, 30 );
    master->setMargins( 30, 30 );
    GECanvasText* text = new GECanvasText( 0 );
    QFont f = text->font();
    f.setPointSize( 30 );
    text->setFont( f );
    text->setText( QString( "Z_%1 is isomorphic to Z_%2 x Z_%3" )
                   .arg( m * n ).arg( m ).arg( n ) );
    master->insert( 0, 0, text );

    CanvasLayoutGrid* grid = new CanvasLayoutGrid();
    grid->resize( 2, 3 );
    grid->setPadding( 30, 0 );
    master->insert( 1, 0, grid );

    for ( uint i = 0 ; i < 3 ; i++ ) {
        GEVisualizer* V = GEVisualizers::getNewVisualizer( "CayleyDiagram" );
        V->setGroup( G );
        V->processChange( "labs", "N", QString::null );
        if ( i < 2 ) {
            V->processChange( "gtbl", gmgn + "," + iota2 + "," + iota2, QString::null );
            if ( !i ) {
                V->processChange( "arro", gmgn, QString::null );
            } else {
                V->processChange( "arro", gmn, QString::null );
            }
        }
        V->getSmall()->setSize( 150, 150 );
        grid->insert( 0, i, V->getSmall() );
    }
    text = new GECanvasText( 0 );
    text->setText( QString( "Here is a Cayley diagram of\n"
                            "Z_%1 x Z_%2, arranged as a\n"
                            "rectangle, showing a generator\n"
                            "of order %3 (let's call it a)\n"
                            "and a generator of order %4\n"
                            "(let's call it b)." ).arg( m ).arg( n ).arg( m ).arg( n ) );
    grid->insert( 1, 0, text );
    text = new GECanvasText( 0 );
    text->setText( QString( "This Cayley diagram illustrates\n"
                            "that we could have connected the\n"
                            "diagram to the left using just\n"
                            "one generator, the product ab of\n"
                            "the original two." ) );
    grid->insert( 1, 1, text );
    text = new GECanvasText( 0 );
    text->setText( QString( "If we take the previous diagram\n"
                            "and unravel the order-%1 ab-orbit,\n"
                            "it lays out nicely in a circle,\n"
                            "which is clearly Z_%2.\n" ).arg( m * n ).arg( m * n ) );
    grid->insert( 1, 2, text );

    text = new GECanvasText( 0 );
    f.setPointSize( 18 );
    text->setFont( f );
    text->setText( QString( "Because the diagram on the left end is clearly Z_%1 x Z_%2,\n"
                            "and the diagram on the right end is clearly Z_%3,\n"
                            "the text above is an explanation of the isomorphism." )
                   .arg( m ).arg( n ).arg( m * n ) );
    master->insert( 2, 0, text );

    GESheetWindow* win = GESheetWindow::createNewWindow();
    master->placeOnCanvas( win->sheet(), 0, 0, 0 );
    QSize s = master->size();
    win->sheet()->resize( s.width(), s.height() );

    win->show();
    delete master;
}


ShowNoZnmIsomorphismLC::ShowNoZnmIsomorphismLC (
        QStringList parameters, QWidget* originator )
    : LinkCommand( parameters, originator )
{
}

void ShowNoZnmIsomorphismLC::redo ()
{
    if ( parameters.count() < 3 ) return;
    GroupFile* G = GroupTable::findGroup( parameters[0] );
    if ( !G ) {
        qWarning( QString( "Could not find %1 in library" ).arg( parameters[0] ) );
        return;
    }
    uint m = parameters[1].toUInt();
    uint n = parameters[2].toUInt();
    if ( ( m < 2 ) || ( n < 2 ) || ( m * n != G->group.order ) ) {
        qWarning( QString( "Invalid m,n for Znm nonisomorphic link command: %1,%2 (Z_%3)" )
                  .arg( m ).arg( n ).arg( G->group.order ) );
        return;
    }

    // we assume m <= n
    GroupFile* P = new GroupFile();
    P->group.setSize( m * n );
    GEltList table( m * n * m * n );
    for ( GElt g = 0 ; g < m * n ; g++ ) {
        uint gq = g / n;
        uint gr = g % n;
        for ( GElt h = 0 ; h < m * n ; h++ ) {
            uint hq = h / n;
            uint hr = h % n;
            table[g * m * n + h] = ( ( gq + hq ) % m ) * n + ( ( gr + hr ) % n );
        }
    }
    P->group.setMTable( table );
    P->generatorlists.append( P->group.findGens() );
    GEltList iso;
    P = SubgroupsGIF::findGroupInLibrary( P, &iso );
    if ( !P ) {
        qWarning( "Znm nonisomorphism could not find group like " +
                  elt_list_to_string( table ) );
        return;
    }

    GEltList tmp( 2 ); tmp[0] = iso[n]; tmp[1] = iso[1];
    QString gmgn = elt_list_to_code( tmp );
    tmp.resize( 1 ); tmp[0] = iso[5];
    QString gmn = elt_list_to_code( tmp );
    QString gmnp = elt_list_to_code( P->group.completeGens( tmp, iota( m * n ) ) );
    qDebug( "Completing gens " + elt_list_to_code( tmp ) + " gives gens " +
            elt_list_to_code( P->group.completeGens( tmp, iota( m * n ) ) ) );
    QString iota2 = elt_list_to_code( iota( 2 ) );

    CanvasLayoutGrid* master = new CanvasLayoutGrid();
    master->resize( 3, 1 );
    master->setPadding( 0, 30 );
    master->setMargins( 30, 30 );
    GECanvasText* text = new GECanvasText( 0 );
    QFont f = text->font();
    f.setPointSize( 30 );
    text->setFont( f );
    text->setText( QString( "Z_%1 is not isomorphic to Z_%2 x Z_%3" )
                   .arg( m * n ).arg( m ).arg( n ) );
    master->insert( 0, 0, text );

    CanvasLayoutGrid* grid = new CanvasLayoutGrid();
    grid->resize( 2, 3 );
    grid->setPadding( 30, 0 );
    master->insert( 1, 0, grid );

    for ( uint i = 0 ; i < 3 ; i++ ) {
        GEVisualizer* V = GEVisualizers::getNewVisualizer( "CayleyDiagram" );
        V->setGroup( P );
        V->processChange( "labs", "N", QString::null );
        if ( i < 2 ) {
            V->processChange( "gtbl", gmgn + "," + iota2 + "," + iota2, QString::null );
        } else {
            V->processChange( "gtbl", gmnp + "," + iota2 + "," + iota2, QString::null );
        }
        if ( !i ) {
            V->processChange( "arro", gmgn, QString::null );
        } else {
            V->processChange( "arro", gmn, QString::null );
        }
        V->getSmall()->setSize( 150, 150 );
        grid->insert( 0, i, V->getSmall() );
    }

    text = new GECanvasText( 0 );
    text->setText( QString( "Here is a Cayley diagram of\n"
                            "Z_%1 x Z_%2, arranged as a\n"
                            "rectangle, showing a generator\n"
                            "of order %3 (let's call it a)\n"
                            "and a generator of order %4\n"
                            "(let's call it b)." ).arg( m ).arg( n ).arg( m ).arg( n ) );
    grid->insert( 1, 0, text );
    text = new GECanvasText( 0 );
    text->setText( QString( "This Cayley diagram shows an\n"
                            "attempt to weave all the nodes\n"
                            "in the previous diagram together\n"
                            "using just one order-%1 generator,\n"
                            "ab, the product of the original\n"
                            "two. Did the attempt succeed?\n" ).arg( m * n ) );
    grid->insert( 1, 1, text );
    text = new GECanvasText( 0 );
    text->setText( "Here we have the same diagram as\n"
                   "the previous, now with the arrows\n"
                   "untangled so that it is clear that\n"
                   "it was not one single loop.\n" );
    grid->insert( 1, 2, text );

    text = new GECanvasText( 0 );
    f.setPointSize( 18 );
    text->setFont( f );
    text->setText( QString(
        "If we could have connected all nodes in the diagram with one\n"
        "order-%1 generator, we would have shown that Z_%2 x Z_%3 was\n"
        "really Z_%4.  But any attempt to do so will fail in a manner\n"
        "similar to the example above--more than one loop will form.\n"
        "Double-click any of the images above to experiment further." )
        .arg( m * n ).arg( m ).arg( n ).arg( m * n ) );
    master->insert( 2, 0, text );

    GESheetWindow* win = GESheetWindow::createNewWindow();
    master->placeOnCanvas( win->sheet(), 0, 0, 0 );
    QSize s = master->size();
    win->sheet()->resize( s.width(), s.height() );

    win->show();
    delete master;
}


