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


#include "idealist.h"
#include "myutils.h"
#include "gevisualizerlist.h"
#include <q3stylesheet.h>
//Added by qt3to4:
#include <Q3PtrList>
#include <Q3MemArray>
#include <grouptable.h>
#include <gesheet.h>


//
// First, the implementations of the classes that keep lists of ideas:
//

Q3PtrList<IdeaFactory<GroupFile> >* GroupIdeas::listAll () {
    // The next two lines only get called once ever, because local static variables
    // only get initialized once.  Note that ideas will exist for the lifetime of
    // the GE application, and the OS will clean up that memory when GE exits.
    static Q3PtrList<IdeaFactory<GroupFile> >* ideas =
            new Q3PtrList<IdeaFactory<GroupFile> >();
    static bool firstTime = TRUE;
    if ( firstTime ) {
        // why I do this, I dunno, because it never gets deleted, but...:
        ideas->setAutoDelete( TRUE );
        // now append all existing group ideas (modify this as you make more!):
        ideas->append( new AbelianGIF() );
////        ideas->append( new AlternatingGIF() );
        ideas->append( new ClassEquationGIF() );
        ideas->append( new CyclicGIF() );
////        ideas->append( new DihedralGIF() );
////        ideas->append( new DirectProductGIF() );
////        ideas->append( new FrobeniusGIF() );
        ideas->append( new SubgroupsGIF() );
        ideas->append( new OrderClassesGIF() );
////        ideas->append( new PlatonicSolidGIF() );
////        ideas->append( new QuaternionGIF() );
////        ideas->append( new SemidirectProductGIF() );
////        ideas->append( new SimpleGIF() );
        ideas->append( new SolvableGIF() );
////        ideas->append( new SymmetricGIF() );
        ideas->append( new ZnmGIF() );

        /*
        ideas->append( new LatticeOfSubgroupsGIF() );
        ideas->append( new SubgroupEmbeddingGIF() );
        ideas->append( new FirstIsoTheoremGIF() );
        */

        firstTime = FALSE;
    }
    return ideas;
}


/* ****************** THESE THREE IDEAS HAVE BEEN MOVED UP INTO THE GIFS ***********
QPtrList<IdeaFactory<GEVisualizer> >* VisualizerIdeas::listAll () {
    // this is like the above function
    static QPtrList<IdeaFactory<GEVisualizer> >* ideas =
            new QPtrList<IdeaFactory<GEVisualizer> >();
    static bool firstTime = TRUE;
    if ( firstTime ) {
        ideas->setAutoDelete( TRUE );
        ideas->append( new LatticeOfSubgroupsVIF() );
        ideas->append( new SubgroupEmbeddingVIF() );
        ideas->append( new FirstIsoTheoremVIF() );
        firstTime = FALSE;
    }
    return ideas;
}
*/


//
// Now, the implementations of the idea classes themselves:
//

AbelianGIF::AbelianGIF () {
    title = "Abelian group";
    question = "Is this group abelian?";
    topics << IFTAbelianGroups;
}

Idea<GroupFile> AbelianGIF::consider ( GroupFile & x ) {
    Idea<GroupFile> result( &x, this );
    GElt counterex1 = 0, counterex2 = 0;
    result.applies = TRUE;
    for ( uint i = 0 ; i < x.group.order ; i++ )
        for ( uint j = i + 1 ; j < x.group.order ; j++ )
            if ( x.group.op( i, j ) != x.group.op( j, i ) ) {
                result.applies = FALSE;
                counterex1 = i;
                counterex2 = j;
                break;
            }
    if ( result.applies ) {
        result.computedValue = "yes";
        result.longDescription =
                QString( "<p>%1 is <a href=\"rf-groupterms.html#abelian\">abelian</a>; "
                         "every pair of elements commutes.</p>\n" )
                .arg( x.getPrimaryName().HTML );
    } else {
        result.computedValue = "no";
        result.longDescription =
                QString( "<p>%1 is not <a href=\"rf-groupterms.html#abelian\">"
                         "abelian</a>.  We can find two elements that do not commute:  "
                         "%2 <b><sup>.</sup></b> %3 is %4, "
                         "but %5 <b><sup>.</sup></b> %6 is %7.</p>" )
                .arg( x.getPrimaryName().HTML )
                .arg( x.getRepresentation( counterex1 ).HTML )
                .arg( x.getRepresentation( counterex2 ).HTML )
                .arg( x.getRepresentation( x.group.op(counterex1,counterex2) ).HTML )
                .arg( x.getRepresentation( counterex2 ).HTML )
                .arg( x.getRepresentation( counterex1 ).HTML )
                .arg( x.getRepresentation( 
                        x.group.op(counterex2,counterex1) ).HTML );
    }
    result.longDescription = "<qt type=detail>" + result.longDescription + "</qt>";
    // but now we will say that this concept should be shown for every group,
    // by setting its applies value to true, even though its computed value may be N:
    result.applies = TRUE;
    return result;
}


/*
AlternatingGIF::AlternatingGIF () {
    title = "Alternating group";
    question = "Is this an alternating group?";
    topics << IFTAlternatingGroups;
}
Idea<GroupFile> AlternatingGIF::consider ( GroupFile & x ) {
    Idea<GroupFile> result( &x, this );
    return result;
}
*/


ClassEquationGIF::ClassEquationGIF () {
    title = "Class equation";
    question = "What is this group's class equation?";
    topics << IFTClassEquation << IFTConjugacy;
}

Idea<GroupFile> ClassEquationGIF::consider ( GroupFile & x ) {
    Idea<GroupFile> result( &x, this );
    GEltListList ell = x.group.conjClasses();
    result.applies = TRUE;

    if ( ( ell.size() == (int)x.group.order ) && ( x.group.order > 5 ) ) {
        result.computedValue = QString( "1 + 1 + ... (%1 times) ... + 1 = %2" )
                               .arg( x.group.order ).arg( x.group.order );
    } else {
        result.computedValue = "";
        for ( int i = 0 ; i < ell.size() ; i++ ) {
            if ( i )
                result.computedValue += " + ";
            result.computedValue += QString( "%1" ).arg( ell[i].size() );
        }
        result.computedValue += QString( " = %1" ).arg( x.group.order );
    }

    result.longDescription = QString(
            "<p>The <a href=\"rf-groupterms.html#classequation\">class equation</a> "
            "for %1 is %2.</p>\n" )
            .arg( x.getPrimaryName().HTML )
            .arg( result.computedValue );
    if ( ( ell.size() == (int)x.group.order ) && ( x.group.order > 5 ) ) {
        result.longDescription += QString(
                "<p>Each of the %1 elements in the group is in a conjugacy "
                "class of its own, because the group is abelian.</p>" )
                .arg( x.group.order );
    } else {
        result.longDescription +=
                "<p>The <a href=\"rf-groupterms.html#conjclasses\">conjugacy classes</a> "
                "used in the class equation are listed here:</p>\n"
                "<ul>\n";
        for ( int i = 0 ; i < ell.size() ; i++ ) {
            result.longDescription += "  <li>";
            for ( uint j = 0 ; j < ell[i].size() ; j++ ) {
                if ( j ) result.longDescription += ", ";
                result.longDescription += 
                        x.getRepresentation( ell[i][j] ).HTML;
            }
            result.longDescription += "</li>\n";
        }
        result.longDescription += "</ul>\n";
    }

    result.longDescription += 
        "<p>To see a visual representation of this class equation, "
        "click one of the following links.<br>\n";
    QStringList vnames = GEVisualizers::allVisualizerNames();
    for ( int i = 0 ; i < vnames.count() ; i++ )
        if ( vnames[i] != "SymmetryObject" )
            result.longDescription +=
                QString( "<a href=\"do createClassEquation %1 %2\">"
                         "Show me by %3</a><br>\n" )
                .arg( x.codeName() ).arg( vnames[i] )
                .arg( GEVisualizers::printableName( vnames[i] ) );
    result.longDescription += "</p>\n";

    if ( ell.size() > 5 ) {
        result.longDescription += QString(
            "<p><font color=\"#FF0000\"><b>WARNING:</b></font> Clicking one of the "
            "links above will create a sheet with %1 visualizers in it.  Sheets with "
            "that many three-dimensional visualizers (like Cayley diagrams) can slow "
            "down your computer a <i>lot</i> (<a href=\"rf-um-graphicshardware.html\">"
            "read why here</a>).  Be prepared to wait if you generate such sheets.</p>"
            ).arg( ell.size() );
    }

    result.longDescription = "<qt type=detail>" + result.longDescription + "</qt>";
    return result;
}


CyclicGIF::CyclicGIF () {
    title = "Cyclic group";
    question = "Is this group cyclic?";
    topics << IFTCyclicGroups;
}

Idea<GroupFile> CyclicGIF::consider ( GroupFile & x ) {
    Idea<GroupFile> result( &x, this );
    GElt example = 0;
    result.applies = TRUE;
    bool isCyclic = FALSE;
    for ( uint i = 0 ; i < x.group.order ; i++ )
        if ( x.group.orderOf( i ) == x.group.order ) {
            isCyclic = TRUE;
            example = i;
            break;
        }
    if ( isCyclic ) {
        result.computedValue = "yes";
        result.longDescription =
                QString( "<p>%1 is <a href=\"rf-groupterms.html#cyclic\">cyclic</a>.  "
                         "An element that generates the whole group is %2.</p>" )
                .arg( x.getPrimaryName().HTML )
                .arg( x.getRepresentation( example ).HTML );
    } else {
        result.computedValue = "no";
        result.longDescription +=
                QString( "<p>%1 is not <a href=\"rf-groupterms.html#cyclic\">cyclic</a>; "
                         "no element generates the whole group.</p>" )
                .arg( x.getPrimaryName().HTML );
    }

    result.longDescription = "<qt type=detail>" + result.longDescription + "</qt>";
    return result;
}


/*
DihedralGIF::DihedralGIF () {
    title = "Dihedral group";
    question = "Is this a dihedral group?";
    topics << IFTDihedralGroups;
}
Idea<GroupFile> DihedralGIF::consider ( GroupFile & x ) {
    Idea<GroupFile> result( &x, this );
    return result;
}


DirectProductGIF::DirectProductGIF () {
    title = "Direct product";
    question = "Is this group a direct product of other groups?";
    topics << IFTDirectProduct;
}
Idea<GroupFile> DirectProductGIF::consider ( GroupFile & x ) {
    Idea<GroupFile> result( &x, this );
    return result;
}


FrobeniusGIF::FrobeniusGIF () {
    title = "Frobenius group";
    question = "Is this a Frobenius group?";
    topics << IFTFrobeniusGroups;
}
Idea<GroupFile> FrobeniusGIF::consider ( GroupFile & x ) {
    Idea<GroupFile> result( &x, this );
    return result;
}
*/


OrderClassesGIF::OrderClassesGIF () {
    title = "Order classes";
    question = "How is this group partitioned into order classes?";
    topics << IFTOrderOfAnElement;
}

Idea<GroupFile> OrderClassesGIF::consider ( GroupFile & x ) {
    Idea<GroupFile> result( &x, this );
    GEltListList ell = x.group.orderClasses();
    result.applies = TRUE;
    result.computedValue = QString( "%1 order classes" ).arg( ell.size() );
    if ( ell.size() > 1 ) {
        result.longDescription =
                QString( "<p>In %1, there are %2 <a href=\""
                         "rf-groupterms.html#orderclasses\">order classes</a>.  "
                         "Each is listed here:</p>\n" )
                .arg( x.getPrimaryName().HTML ).arg( ell.size() );
        result.longDescription += "<ul>\n";
        for ( int i = 0 ; i < ell.size() ; i++ ) {
            result.longDescription += QString( "  <li>Elements of order %1: " )
                                      .arg( x.group.orderOf( ell[i][0] ) );
            for ( uint j = 0 ; j < ell[i].size() ; j++ ) {
                if ( j ) result.longDescription += ", ";
                result.longDescription += 
                        x.getRepresentation( ell[i][j] ).HTML;
            }
            result.longDescription += "</li>\n";
        }
        result.longDescription += "</ul>\n";
    } else {
        result.longDescription +=
                QString( "<p>In %1, there is just one <a href=\""
                         "rf-groupterms.html#orderclasses\">order class</a> "
                         "containing all the elements of the group.</p>\n" )
                .arg( x.getPrimaryName().HTML );
    }

    result.longDescription = "<qt type=detail>" + result.longDescription + "</qt>";
    return result;
}


/*
PlatonicSolidGIF::PlatonicSolidGIF () {
    title = "Platonic solids";
    question = "Is this the group of symmetries of one of the Platonic solids?";
    topics << IFTPlatonicSolids;
}
Idea<GroupFile> PlatonicSolidGIF::consider ( GroupFile & x ) {
    Idea<GroupFile> result( &x, this );
    return result;
}


QuaternionGIF::QuaternionGIF () {
    title = "Quaternion group";
    question = "Is this a quaternion group?";
    topics << IFTQuaternions;
}
Idea<GroupFile> QuaternionGIF::consider ( GroupFile & x ) {
    Idea<GroupFile> result( &x, this );
    return result;
}


SemidirectProductGIF::SemidirectProductGIF () {
    title = "Semidirect product";
    question = "Is this group a semidirect product of other groups?";
    topics << IFTSemidirectProduct;
}
Idea<GroupFile> SemidirectProductGIF::consider ( GroupFile & x ) {
    Idea<GroupFile> result( &x, this );
    return result;
}


SimpleGIF::SimpleGIF () {
    title = "Simple group";
    question = "Is this group simple?";
    topics << IFTSimpleGroups;
}
Idea<GroupFile> SimpleGIF::consider ( GroupFile & x ) {
    Idea<GroupFile> result( &x, this );
    return result;
}
*/

/*
class SolvableIdea : public Idea<GroupFile>
{
public:
    SolvableIdea ( GroupFile* subject, IdeaFactory<GroupFile>* factory )
        : Idea<GroupFile>( subject, factory ) { }

    QPtrList<GroupFile> decomposition;

    static QStringList findDecompositionFor ( GroupFile* gf, SolvableGIF* factory )
    {
        for ( uint i = 0 ; i < gf->ideas.count() ; i++ )
            if ( gf->ideas[i].factory == factory )
                return QStringList::split( " ", gf->ideas[i].internalData );
        gf->ideas << factory->consider( *gf );
        return QStringList::split( " ", gf->ideas[gf->ideas.count() - 1].internalData );
    }
};
*/

SolvableGIF::SolvableGIF () {
    title = "Solvable group";
    question = "Is this group solvable?";
    topics << IFTSolvableGroups << IFTSimpleGroups << IFTSubgroups;
}
QStringList SolvableGIF::findDecompositionFor ( GroupFile* gf )
{
    for ( int i = 0 ; i < gf->ideas.count() ; i++ )
        if ( gf->ideas[i].factory == this )
            return QStringList::split( " ", gf->ideas[i].internalData );
    gf->ideas << consider( *gf );
    return QStringList::split( " ", gf->ideas[gf->ideas.count() - 1].internalData );
}
Idea<GroupFile> SolvableGIF::consider ( GroupFile & x ) {
    Idea<GroupFile> result( &x, this );
    result.applies = TRUE;
    result.computedValue = QString::null;
    result.longDescription = QString::null;
    result.internalData = QString::null;
    // OK, if I'm abelian, then I'm solvable.  Let's check that right away.
    bool abelianSoFar = TRUE;
    for ( GElt g = 0 ; abelianSoFar && ( g < x.group.order ) ; g++ )
        for ( GElt h = g + 1 ; abelianSoFar && ( h < x.group.order ) ; h++ )
            if ( x.group.op( g, h ) != x.group.op( h, g ) ) abelianSoFar = FALSE;
    if ( abelianSoFar ) {
        result.computedValue = "yes";
        result.longDescription =
            QString( "<p>%1 is <a href=\"rf-groupterms.html#solvable\">solvable</a> "
                     "because it is <a href=\"rf-groupterms.html#abelian\">"
                     "abelian</a>.</p>" ).arg( x.getPrimaryName().HTML );
        result.internalData = x.codeName();
        return result;
    }
    // let's look through every darn normal subgroup we've got:
    for ( int i = 0 ; i < x.group.subgroups().count() ; i++ ) {
        if ( x.group.isNormal( i ) ) {
            // compute the quotient group:
            GroupFile dummy;
            SubgroupsGIF::computeQuotient( x.group, x.group.subgroups()[i], &dummy );
            // is the quotient group abelian?
            bool abelianSoFar = TRUE;
            for ( GElt g = 0 ; abelianSoFar && ( g < dummy.group.order ) ; g++ )
                for ( GElt h = g + 1 ; abelianSoFar && ( h < dummy.group.order ) ; h++ )
                    if ( dummy.group.op( g, h ) != dummy.group.op( h, g ) )
                        abelianSoFar = FALSE;
            if ( !abelianSoFar ) // because it is not abelian,
                continue;        // this quotient can't help us; keep looking...
            // since this quotient is abelian, let's hope we can find it in the library:
            GroupFile* QG = SubgroupsGIF::findGroupInLibrary( &dummy );
            if ( !QG ) {
                // we cannot, so we write a warning message, but keep looking in case we
                // can find a DIFFERENT solvable decomposition that IS in the libarary:
                result.computedValue = "unable to determine";
                result.longDescription =
                    QString( "<p>Group Explorer is currently unable to determine "
                             "whether %1 is a <a href=\"rf-groupterms.html#solvable\">"
                             "solvable</a> group because it does not "
                             "have access to all the groups it needs.  For example, "
                             "the <a href=\"rf-groupterms.html#quotient\">quotient</a> "
                             "of %2 by the <a href=\"rf-groupterms.html#normalsubgroup\">"
                             "normal subgroup</a> <i>H</i><sub>%3</sub> is "
                             "<a href=\"rf-groupterms.html#abelian\">abelian</a>, but "
                             "Group Explorer cannot compute further because it is not "
                             "<a href=\"rf-groupterms.html#isomorphic\">isomorphic</a> "
                             "to any group in the library currently loaded.</p>\n"
                             "<p>You will need to more groups loaded (see "
                             "<a href=\"rf-um-options-dialog.html\">options window</a> "
                             "for starters) to make this computation possible.</p>" )
                    .arg( x.getPrimaryName().HTML )
                    .arg( x.getPrimaryName().HTML )
                    .arg( i + 1 );
                continue; // keep looking
            }
            // since we found an abelian quotient group, let's hope we can find the
            // normal subgroup also in the group library:
            GroupFile* NS = SubgroupsGIF::findSubgroupInLibrary(
                &x, x.group.subgroups()[i] );
            if ( !NS ) {
                // we cannot, so we write a warning message, but keep looking in case we
                // can find a DIFFERENT solvable decomposition that IS in the libarary:
                result.computedValue = "unable to determine";
                result.longDescription =
                    QString( "<p>Group Explorer is currently unable to determine "
                             "whether %1 is a <a href=\"rf-groupterms.html#solvable\">"
                             "solvable</a> group because it does not "
                             "have access to all the groups it needs.  For example, "
                             "the <a href=\"rf-groupterms.html#normal\">normal "
                             "subgroup</a> <i>H</i><sub>%2</sub> yields an "
                             "<a href=\"rf-groupterms.html#abelian\">abelian</a> "
                             "<a href=\"rf-groupterms.html#quotient\">quotient group</a>,"
                             " but it is not "
                             "<a href=\"rf-groupterms.html#isomorphic\">isomorphic</a> "
                             "to any group in the library currently loaded.</p>\n"
                             "<p>You will need to more groups loaded (see "
                             "<a href=\"rf-um-options-dialog.html\">options window</a> "
                             "for starters) to make this computation possible.</p>" )
                    .arg( x.getPrimaryName().HTML )
                    .arg( i + 1 );
                continue; // keep looking
            }
            // since we have an abelian quotient group that's in the library, and its
            // normal subgroup is too, then we're solvable iff the normal subgroup is:
            QStringList decomp = findDecompositionFor( NS );
            if ( decomp.count() == 0 ) {
                // it is not solvable, so we are not either
                result.computedValue = "no";
                result.longDescription =
                    QString( "<p>%1 is not <a href=\"rf-groupterms.html#solvable\">"
                             "solvable</a>.  There is no sequence of groups that is its "
                             "<a href=\"rf-groupterms.html#solvable\">solvable "
                             "decomposition</a>.</p>" ).arg( x.getPrimaryName().HTML );
                return result;
            }
            // it is solvable, so we are, too
            result.computedValue = "yes";
            QString decompDescrip =
                QString( "The <a href=\"rf-groupterms.html#quotient\">quotient</a> of "
                         "<a href=\"do openGroup %1\">%2</a> by its "
                         "<a href=\"rf-groupterms.html#normalsubgroup\">normal "
                         "subgroup</a> <i>H</i><sub>%3</sub> "
                         "(<a href=\"rf-groupterms.html#isomorphic\">isomorphic</a> to "
                         "<a href=\"do openGroup %4\">%5</a>) gives "
                         "<a href=\"do openGroup %6\">%7</a>.<br>\n" )
                .arg( x.codeName() ).arg( x.getPrimaryName().HTML ).arg( i + 1 )
                .arg( NS->codeName() ).arg( NS->getPrimaryName().HTML )
                .arg( QG->codeName() ).arg( QG->getPrimaryName().HTML );
            QString shortDecompDescrip =
                QString( "<a href=\"do openGroup %1\">%2</a>" )
                .arg( x.codeName() ).arg( x.getPrimaryName().HTML );
            QString sheetParams =
                QString( "%1 %2 %3" )
                .arg( x.codeName() ).arg( i + 1 ).arg( QG->codeName() );
            for ( int j = 0 ; j < decomp.count() ; j += 4 ) {
                GroupFile* tmp = GroupTable::findGroup( decomp[j] );
                if ( j + 4 >= decomp.count() ) {
                    decompDescrip +=
                        QString( "The group <a href=\"do openGroup %1\">%2</a> is "
                                 "<a href=\"rf-groupterms.html#abelian\">abelian</a>." )
                        .arg( decomp[j] ).arg( tmp->getPrimaryName().HTML );
                    sheetParams = decomp[j] + " " + sheetParams;
                } else {
                    decompDescrip +=
                        QString( "The <a href=\"rf-groupterms.html#quotient\">"
                                 "quotient</a> of "
                                 "<a href=\"do openGroup %1\">%2</a> by its "
                                 "<a href=\"rf-groupterms.html#normalsubgroup\">"
                                 "normal subgroup</a> <i>H</i><sub>%3</sub> "
                                 "(<a href=\"rf-groupterms.html#isomorphic\">"
                                 "isomorphic</a> to "
                                 "<a href=\"do openGroup %4\">%5</a>) gives "
                                 "<a href=\"do openGroup %6\">%7</a>.<br>\n" )
                        .arg( decomp[j] ).arg( tmp->getPrimaryName().HTML )
                        .arg( decomp[j + 1] )
                        .arg( decomp[j + 2] )
                        .arg( GroupTable::findGroup( decomp[j + 2] )
                              ->getPrimaryName().HTML )
                        .arg( decomp[j + 3] )
                        .arg( GroupTable::findGroup( decomp[j + 3] )
                              ->getPrimaryName().HTML );
                    sheetParams = QString( "%1 %2 %3 %4" )
                                  .arg( decomp[j] ).arg( decomp[j + 1] )
                                  .arg( decomp[j + 3] ).arg( sheetParams );
                }
                shortDecompDescrip =
                    QString( "<a href=\"do openGroup %1\">%2</a> &lt; %3" )
                    .arg( decomp[j] ).arg( tmp->getPrimaryName().HTML )
                    .arg( shortDecompDescrip );
            }
            QString diagramLinks;
            QStringList vnames = GEVisualizers::allVisualizerNames();
            for ( int j = 0 ; j < vnames.count() ; j++ ) {
                if ( vnames[j] != "SymmetryObject" ) {
                    if ( diagramLinks.length() ) diagramLinks += " or ";
                    diagramLinks += "by <a href=\"do createSolvableDecomposition " +
                                    vnames[j] + " " + sheetParams + "\">" +
                                    GEVisualizers::printableName( vnames[j] ) + "</a>";
                }
            }
            result.longDescription =
                QString( "<p>%1 is a <a href=\"rf-groupterms.html#solvable\">solvable</a>"
                         " group by the following solvable decomposition:</p>\n"
                         "<blockquote>%2</blockquote>\n"
                         "<p>In summary, <a href=\"do openGroup Z_1|Nathan-_Carter-_"
                         "(ncarter@bentley.edu)\"><i><b>Z</b></i><sub>1</sub></a> "
                         "&lt; %3.</p>"
                         "<p>You can see a diagram of all the groups in the solvable "
                         "decomposition, including quotient maps, %4.</p>" )
                .arg( x.getPrimaryName().HTML )
                .arg( decompDescrip ).arg( shortDecompDescrip ).arg( diagramLinks );
            result.internalData = QString( "%1 %2 %3 %4 %5" )
                                  .arg( x.codeName() ).arg( i + 1 )
                                  .arg( NS->codeName() ).arg( QG->codeName() )
                                  .arg( decomp.join( " " ) );
            return result;
        }
    }
    // OK we fell out of the loop with no result, which either means we never found a
    // normal subgroup w/abelian quotient AT ALL, or the ones we found weren't in the
    // group library.  If the latter is the case, we've got a warning message already;
    // otherwise, we make one now:
    if ( result.computedValue.isNull() ) {
        result.computedValue = "no";
        result.longDescription =
            QString( "<p>%1 is not a <a href=\"rf-groupterms.html#solvable\">solvable</a> "
                     "group.  In fact, it does not even have a "
                     "<a href=\"rf-groupterms.html#normalsubgroup\">normal subgroup</a> "
                     "that can be used to form an <a href=\"rf-groupterms.html#abelian\">"
                     "abelian</a> <a href=\"rf-groupterms.html#quotient\">quotient</a> "
                     "group.</p>" )
            .arg( x.getPrimaryName().HTML );
    }
    return result;
}


SubgroupsGIF::SubgroupsGIF () {
    title = "Subgroups";
    question = "What subgroups does this group have?";
    topics << IFTSubgroups;
}

GroupFile* SubgroupsGIF::findSubgroupInLibrary ( GroupFile* G, GEltList subgroup,
                                                 GEltList* partialMap )
{
    int num = GroupTable::numGroups();
    for ( int i = 0 ; i < num ; i++ ) {
        GroupFile* tmp = GroupTable::getGroup( i );
        if ( tmp->group.order == subgroup.size() ) {
            GEltList map = GECanvasMorphism::tryEmbedding( tmp, G, subgroup );
            if ( map.size() ) {
                if ( partialMap ) {
                    partialMap->fill( tmp->group.order, tmp->group.order );
                    for ( uint j = 0 ; j < map.size() ; j++ ) (*partialMap)[map[j]] = j;
                }
                return tmp;
            }
        }
    }
    return 0;
}
GroupFile* SubgroupsGIF::findGroupInLibrary ( GroupFile* G, GEltList* isomorphism )
{
    return findSubgroupInLibrary( G, iota( G->group.order ), isomorphism );
}

void SubgroupsGIF::computeQuotient (
    Group& G, GEltList normalSgr, GroupFile* putQuotientHere, GEltList* quotientMap )
{
    putQuotientHere->group.setSize( G.order / normalSgr.size() );
    GEltList quotientTable( putQuotientHere->group.order * putQuotientHere->group.order );
    GEltList cosetIndex( G.order );
    GEltListList cosets = G.cosetsOf( normalSgr, TRUE );
    for ( int i = 0 ; i < cosets.count() ; i++ )
        for ( uint j = 0 ; j < cosets[i].size() ; j++ )
            cosetIndex[cosets[i][j]] = i;
    for ( uint i = 0 ; i < putQuotientHere->group.order ; i++ )
        for ( uint j = 0 ; j < putQuotientHere->group.order ; j++ )
            quotientTable[i*putQuotientHere->group.order+j] =
                cosetIndex[G.op( cosetIndex.find( i ), cosetIndex.find( j ) )];
    putQuotientHere->group.setMTable( quotientTable );
    putQuotientHere->generatorlists.append( putQuotientHere->group.findGens() );
    if ( quotientMap ) *quotientMap = cosetIndex;
}

QString SubgroupsGIF::tableRow ( GroupFile& G, GEltListList subgroups, int i, int& error )
{
    GEltList sgr;
    QString comment;
    // first find out if the whole group is a p-group
    bool isPGroup = FALSE;
    uint p = 0;
    if ( G.group.order > 1 ) {
        for ( p = 2 ; G.group.order % p ; p++ ) { } // find the smalest p | order gp
        uint powers;
        for ( powers = 1 ; powers < G.group.order ; powers *= p ) { }
        isPGroup = ( powers == G.group.order );
    }
    if ( i == 0 ) {
        sgr = iota( 1 );
        comment = "the trivial subgroup";
    } else if ( i > subgroups.count() ) {
        sgr = G.group.elements();
        if ( isPGroup )
            comment = QString( "the whole group, a <a href=\"rf-groupterms.html#psubgroup\">"
                               "%1-group</a>" ).arg( p );
        else
            comment = "the whole group";
    } else {
        sgr = subgroups[i-1];
        p = G.group.isPSubgroup( i-1 );
        if ( p ) {
            // it MAY be a Sylow p-subgroup, unless the whole group is a p-group already
            bool isSylow = !isPGroup;
            // look for counterexample among the proper subgroups now:
            for ( int j = G.group.subgroups().count() - 1 ; isSylow && ( j > i-1 ) ; j-- ) {
                if ( ( G.group.isPSubgroup( j ) == p ) &&
                     elt_subset( G.group.subgroups()[i-1], G.group.subgroups()[j] ) )
                    isSylow = FALSE;
            }
            if ( isSylow ) {
                comment = QString( "a <a href=\"rf-groupterms.html#sylowpsubgroup\">"
                                   "Sylow %1-subgroup</a>" ).arg( p );
            } else {
                comment = QString( "a <a href=\"rf-groupterms.html#psubgroup\">"
                                   "%1-subgroup</a>" ).arg( p );
            }
        }
    }
    int order = sgr.size();
    int index = G.group.order / order;

    QString SES;
    error = -1;
    QStringList vnames = GEVisualizers::allVisualizerNames();
    GroupFile* SG = findSubgroupInLibrary( &G, sgr );
    if ( !SG ) error = 0;
    QString normal;
    if ( ( index == 1 ) || ( order == 1 ) || G.group.isNormal( i - 1 ) ) {
        normal = "yes";
        if ( SG ) {
            GroupFile dummy;
            computeQuotient( G.group, sgr, &dummy );
            GroupFile* QG = findGroupInLibrary( &dummy );
            if ( QG ) {
                for ( int i = 0 ; i < vnames.count() ; i++ ) {
                    if ( vnames[i] != "SymmetryObject" ) {
                        if ( SES.length() ) SES += " or ";
                        SES += QString( "by <a href=\"do createSES %1 %2 %3\">%4</a>" )
                               .arg( G.codeName() )
                               .arg( elt_set_to_code( sgr ) )
                               .arg( vnames[i] )
                               .arg( GEVisualizers::printableName( vnames[i] ) );
                    }
                }
                SES = "See the <a href=\"rf-groupterms.html#ses\">short exact "
                      "sequence</a> exhibiting the <a href=\""
                      "rf-groupterms.html#quotient\">quotient group</a> " + SES + ".";
                if ( G.group.order > 15 ) {
                    SES += "  <font color=\"#FF0000\"><b>Warning:</b></font> Clicking "
                           "the Cayley diagram link in the previous sentence will create "
                           "a sheet with five Cayley diagrams in it, the largest of "
                           "which will be somewhat complex.  This may be time consuming "
                           "on your computer (<a href=\"rf-um-graphicshardware.html\">"
                           "read why here</a>).";
                }
            } else {
                error = 1;
                SES = "I cannot show you the quotient group right now ("
                      "<a href=\"#starstar\">see below**</a>).";
            }
        }
    } else {
        normal = "no";
    }
    QString embedding;
    if ( SG ) {
        for ( int i = 0 ; i < vnames.count() ; i++ ) {
            if ( vnames[i] != "SymmetryObject" ) {
                if ( embedding.length() ) embedding += " or ";
                embedding += QString( "by <a href=\"do createEmbedding %1 %2 %3 %4\">"
                                      "%5</a>" )
                             .arg( SG->codeName() ).arg( G.codeName() )
                             .arg( elt_set_to_code( sgr ) ).arg( vnames[i] )
                             .arg( GEVisualizers::printableName( vnames[i] ) );
            }
        }
        embedding = "It is <a href=\"rf-groupterms.html#isomorphic\">isomorphic</a> to "
                    + SG->getPrimaryName().HTML + " and you can see the embedding "
                    + embedding + ".";
    } else {
        embedding = "I cannot show you the embedding right now ("
                    "<a href=\"#star\">see below*</a>).";
    }

    QString elements = "{ ";
    for ( uint j = 0 ; j < sgr.size() ; j++ ) {
        if ( j ) elements += ", ";
        elements += G.getRepresentation( sgr[j] ).HTML;
    }
    elements += " }";

    return QString( "  <tr>\n"
                    "    <td>%1</td>\n"
                    "    <td><i>H</i><sub>%2</sub></td>\n"
                    "    <td>%3</td>\n"
                    "    <td>%4</td>\n"
                    "    <td>%5</td>\n"
                    "  </tr>\n"
                    "  <tr><td colspan=5><i>H</i><sub>%6</sub>%7 is %8.<br>\n"
                    "      <ul>%9</ul></td></tr>\n" )
           .arg( SG ? ( "<img src=\"CayleyDiagram/" + SG->codeName() + "/0/50/50\">" )
                    : QString( "none*" ) )
           .arg( i ).arg( order ).arg( index ).arg( normal ).arg( i )
           .arg( comment.length() ? ( ", " + comment + "," ) : QString( "" ) )
           .arg( elements )
           .arg( "<li>" + embedding + "</li>\n<li>" + SES + "</li>" );
}

Idea<GroupFile> SubgroupsGIF::consider ( GroupFile & x ) {
    //qDebug( "considering subgroups of %s", x.getPrimaryName().text.latin1() );
    Idea<GroupFile> result( &x, this );
    result.applies = TRUE;
    GEltListList ell = x.group.subgroups();
    result.computedValue =
        QString::number( ( x.group.order == 1 ) ? 1 : ( ell.count() + 2 ) );
    result.longDescription =
        "<p>All <a href=\"rf-groupterms.html#subgroup\">subgroups</a> of " +
        x.getPrimaryName().HTML + " are listed in the following table.  "
        "Also, a link which exhibits the embedding for each subgroup is also provided, "
        "or in some cases a reason why it is not possible right now.  Subgroup "
        "<a href=\"rf-groupterms.html#ordersub\">order</a> and "
        "<a href=\"rf-groupterms.html#indexsub\">index</a> are also given.</p>\n"
        "<p>The column entitled \"normal\" reports whether the subgroup is "
        "<a href=\"rf-groupterms.html#normalsubgroup\">normal</a> in " +
        x.getPrimaryName().HTML + ", and if so, tries to provide a link to "
        "showing that normality by means of a sheet showing a short exact "
        "sequence (SES) which exhibits the <a href=\"rf-groupterms.html#IT-1\">First "
        "Isomorphism Theorem</a> applied to the subgroup.</p>\n"
        "<p>The subgroups can also be shown arranged in a "
        "<a href=\"rf-groupterms.html#subgrouplattice\">lattice</a>, "
        "each shown as highlighted portions of the whole group, "
        "connected by the identity (inclusion) homomorphism.  You may see that lattice ";
    QString lattice;
    QStringList vnames = GEVisualizers::allVisualizerNames();
    for ( int j = 0 ; j < vnames.count() ; j++ ) {
        if ( ( vnames[j] != "SymmetryObject" ) /* doesn't make sense */
          /*&& ( vnames[j] != "CayleyDiagram" ) still too slow, but warning below */  ) {
            if ( lattice.length() ) lattice += " or ";
            lattice += "by <a href=\"do createSubgroupLattice " + x.codeName() + " " +
                       vnames[j] + "\">" + GEVisualizers::printableName( vnames[j] ) +
                       "</a>";
        }
    }
    result.longDescription += lattice + ".";
    result.longDescription += "</p>\n";
    if ( ell.count() > 5 ) {
        result.longDescription += QString(
            "<p><font color=\"#FF0000\"><b>WARNING:</b></font> Clicking one of the "
            "links above will create a sheet with %1 visualizers in it.  Sheets with "
            "that many three-dimensional visualizers (like Cayley diagrams) can slow "
            "down your computer a <i>lot</i> (<a href=\"rf-um-graphicshardware.html\">"
            "read why here</a>).  Be prepared to wait if you generate such sheets."
            ).arg( ell.count() );
    }
    bool existsANormalSubgroup = FALSE;
    QString table =
        "<table border=2>\n"
        "  <tr><td><u>Image</u></td><td><u>Name</u></td><td><u>Order</u></td>"
        "<td><u>Index</u></td><td><u>Normal</u></td></tr>\n";
    int error;
    bool errors[2] = { FALSE, FALSE };
    table += tableRow( x, ell, 0, error = -1 );
    if ( ( error > -1 ) && ( error < 2 ) ) errors[error] = TRUE;
    for ( int i = 0 ; i < ell.count() ; i++ ) {
        if ( !existsANormalSubgroup && x.group.isNormal( i ) )
            existsANormalSubgroup = TRUE;
        table += tableRow( x, ell, i + 1, error = -1 );
        if ( ( error > -1 ) && ( error < 2 ) ) errors[error] = TRUE;
    }
    if ( ell.count() ) {
        table += tableRow( x, ell, ell.count() + 1, error = -1 );
        if ( ( error > -1 ) && ( error < 2 ) ) errors[error] = TRUE;
    }
    table += "</table>\n";
    if ( existsANormalSubgroup ) {
        result.longDescription += "<p>At least one of the subgroups on the list below "
                                  " is <a href=\"rf-groupterms.html#normalsubgroup\">"
                                  "normal</a>.  For this reason, " +
                                  x.getPrimaryName().HTML + " is not a "
                                  "<a href=\"rf-groupterms.html#simple\">simple</a> "
                                  "group.</p>\n";
    } else {
        result.longDescription += "<p>None of the subgroups on the list below "
                                  " is <a href=\"rf-groupterms.html#normalsubgroup\">"
                                  "normal</a>.  For this reason, " +
                                  x.getPrimaryName().HTML + " is a "
                                  "<a href=\"rf-groupterms.html#simple\">simple</a> "
                                  "group.</p>\n";
    }
    result.longDescription += table;
    if ( errors[0] )
        result.longDescription += "<p><a name=\"star\">*</a> No group <a href=\""
                                  "rf-groupterms.html#isomorphic\">isomorphic</a> "
                                  "to this subgroup appears in "
                                  "the group library.  This makes it impossible to draw "
                                  "embedding diagrams, or examples of the <a href=\""
                                  "do openHelp rf-groupterms.html#IT-1\">First "
                                  "Isomorphism Theorem</a>.  You can choose what groups "
                                  "to include in your group library in "
                                  "<a href=\"rf-um-options-dialog.html\">the options "
                                  "window</a>.</p>\n";
    if ( errors[1] )
        result.longDescription += "<p><a name=\"starstar\">**</a> No group <a href=\""
                                  "rf-groupterms.html#isomorphic\">isomorphic</a> "
                                  "to the quotient of " +
                                  x.getPrimaryName().HTML + " by the normal subgroup "
                                  "appears in the group library.  This makes it "
                                  "impossible to illustrate the quotient group.  "
                                  "You can choose what groups "
                                  "to include in your group library in "
                                  "<a href=\"rf-um-options-dialog.html\">the options "
                                  "window</a>.</p>\n";
    return result;
}


/*
SymmetricGIF::SymmetricGIF () {
    title = "Symmetric group";
    question = "Is this a symmetric group?";
    topics << IFTSymmetricGroups;
}
Idea<GroupFile> SymmetricGIF::consider ( GroupFile & x ) {
    Idea<GroupFile> result( &x, this );
    return result;
}
*/


ZnmGIF::ZnmGIF ()
{
    title = "Z_nm groups";
    question = "Is <i><b>Z</b><sub>nm</sub></i> isomorphic to "
               "<i><b>Z</b><sub>n</sub></i> x <i><b>Z</b><sub>m</sub></i>?";
    topics << IFTCyclicGroups << IFTDirectProduct;
}

Idea<GroupFile> ZnmGIF::consider ( GroupFile & x ) {
    Idea<GroupFile> result( &x, this );
    // Does this computation even apply?  The group must be a Z_{mn} for m,n rel'y prime.
    result.applies = FALSE;
    GElt g = 0;
    uint ordg = 0;
    uint i = 0;
    if ( x.group.order >= 6 ) {
        for ( i = 0 ; i < x.group.order ; i++ ) {
            if ( ordg < x.group.orderOf( i ) ) {
                g = i;
                ordg = x.group.orderOf( i );
            }
        }
        if ( ordg == x.group.order ) {
            result.applies = TRUE;
            for ( i = 2 ; i < ordg ; i++ )
                if ( !( ordg % i ) && ( gcd( i, ordg / i ) == 1 ) ) break;
            if ( i == ordg ) i = 0;
        }
    }
    if ( !result.applies ) {
        result.computedValue = "N/A";
        result.longDescription =
            "<p>" + x.getPrimaryName().HTML + " is not a group of the form "
            "<i><b>Z</b><sub>n</sub></i> x <i><b>Z</b><sub>m</sub></i>, and hence "
            "computations about that type of group do not apply here.</p>";
        return result;
    }
    if ( i ) {
        result.computedValue = QString( "= Z_%1 x Z_%2" )
                               .arg( i ).arg( ordg / i );
        result.longDescription = QString(
            "<p>A group of the form <i><b>Z</b><sub>mn</sub></i> is isomorphic to the "
            "product group <nobr><i><b>Z</b><sub>m</sub></i> x "
            "<i><b>Z</b><sub>n</sub></i></nobr> "
            "just when <i>m</i> and <i>n</i> are relatively prime.  In this case, "
            "because %1 and %2 are relatively prime, "
            "<nobr><i><b>Z</b></i><sub>%3</sub></i> x "
            "<i><b>Z</b></i><sub>%4</sub></i></nobr> is isomorphic to "
            "<i><b>Z</b></i><sub>%5</sub></i>.</p>\n"
            "<p><a href=\"do showZnmIsomorphism %6 %7 %8\">Click here</a> to see an "
            "illustration of why this is true.</p>" )
            .arg( i ).arg( ordg / i ).arg( i ).arg( ordg / i ).arg( ordg )
            .arg( x.codeName() ).arg( i ).arg( ordg / i );
    } else {
        result.computedValue = "= no Z_m x Z_n";
        Q3MemArray<uint> factors; // just the factors <= sqrt(group order)
        for ( uint j = 2 ; j * j <= ordg ; j++ ) {
            if ( !( ordg % j ) ) {
                factors.resize( factors.size() + 1 );
                factors[factors.size() - 1] = j;
            }
        }
        result.longDescription = QString(
            "<p>A group of the form <i><b>Z</b><sub>mn</sub></i> is isomorphic to the "
            "product group <nobr><i><b>Z</b><sub>m</sub></i> x "
            "<i><b>Z</b><sub>n</sub></i></nobr> "
            "just when <i>m</i> and <i>n</i> are relatively prime.  In this case, "
            "<i>mn</i> = %1, which gives %2 %3 for <i>m</i> and <i>n</i>.</p>\n" )
            .arg( ordg )
            .arg( factors.size() ? QString::number( factors.size() ) : QString( "no" ) )
            .arg( ( factors.size() == 1 ) ? "possibility" : "possibilities" );
        if ( !factors.size() ) {
            result.longDescription +=
                "<p>Thus there is not even a product group "
                "<nobr><i><b>Z</b><sub>m</sub></i> x <i><b>Z</b><sub>n</sub></i></nobr> "
                "to speak of being isomorphic to.</p>";
        } else {
            if ( factors.size() == 1 ) {
                result.longDescription += "<p>This is that possibility:</p>\n";
            } else {
                result.longDescription += "<p>Here is each possibility:</p>\n";
            }
            result.longDescription += "<ul>\n";
            for ( uint j = 0 ; j < factors.size() ; j++ ) {
                result.longDescription += QString(
                    "  <li>When <i>m</i> = %1 and <i>n</i> = %2, %3 divides both "
                    "<i>m</i> and <i>n</i>, so they are not relatively prime.<br>\n"
                    "<a href=\"do showNoZnmIsomorphism %4 %5 %6\">Click here</a> "
                    "to see an illustration of why <i><b>Z</b><sub>%7</sub></i> is not "
                    "isomorphic to <nobr><i><b>Z</b><sub>%8</sub></i> x "
                    "<i><b>Z</b><sub>%9</sub></i></nobr>.</li>\n" )
                    .arg( factors[j] ).arg( ordg / factors[j] )
                    .arg( gcd( factors[j], ordg / factors[j] ) )
                    .arg( x.codeName() ).arg( factors[j] ).arg( ordg / factors[j] )
                    .arg( ordg ).arg( factors[j] ).arg( ordg / factors[j] );
            }
            result.longDescription += "</ul>\n";
        }
    }
    return result;
}


