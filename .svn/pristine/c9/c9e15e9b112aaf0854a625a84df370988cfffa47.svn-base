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


#include "mathtext.h"
#include "geml.h"
#include <q3stylesheet.h>
#include <qdom.h>
#include <qstringlist.h>
#include <qregexp.h>


//
// some global utils
//

QString mathml_element_to_html ( QDomElement e );

QStringList mathml_element_children_to_html (
        QDomElement e, QString pattern = ".*" )
{
    QStringList result;
    QRegExp rx( pattern );
    for ( QDomNode n = e.firstChild() ; !n.isNull() ; n = n.nextSibling() ) {
        if ( n.isElement() ) {
            QDomElement e = n.toElement();
            if ( rx.search( e.tagName() ) > -1 ) {
                result << mathml_element_to_html( e );
            }
        }
    }
    return result;
}

// this routine removes the ampersand from the front of special mathml characters,
// so that they won't get processed as special characters.  this allows me to later
// recognize them in text (using postpare_mathml_chars, below) and replace them with
// sensible HTML alternatives.
QString prepare_mathml_chars ( QString s ) {
    QString result = s;
    result.replace( "&Zopf;", "Zopf;" );
    result.replace( "&ltimes;", "ltimes;" );
    result.replace( "&times;", "times;" );
    // feel free to insert other special mathML characters here, and places like
    // this, if you start to use more of them; note the corresponding routine below
    return result;
}

// this routine finds the strings left behind by prepare_mathml_chars and replaces
// them with acceptable HTML alternatives that will render sensibly by a browser
QString postpare_mathml_chars ( QString s ) {
    QString result = s;
    result.replace( "Zopf;", "<b>Z</b>" );
    result.replace( "ltimes;", " semidirect-product " );
    result.replace( "times;", " x " );
    // feel free to insert other special mathML characters here, and places like
    // this, if you start to use more of them; note the corresponding routine above
    return result;
}

QString mathml_element_to_html ( QDomElement e ) {
    if ( e.tagName() == "mrow" ) {
        QStringList children = mathml_element_children_to_html( e );
        return children.join( "" );
    } else if ( e.tagName() == "mtext" ) {
        QString inside;
        try_get_string( inside, e, e.tagName() );
        return Q3StyleSheet::escape( inside );
    } else if ( ( e.tagName() == "mo" ) || ( e.tagName() == "mn" ) ) {
        QString inside;
        try_get_string( inside, e, e.tagName() );
        return postpare_mathml_chars( Q3StyleSheet::escape(
                inside.stripWhiteSpace() ) );
    } else if ( e.tagName() == "mi" ) {
        QString inside;
        try_get_string( inside, e, "mi" );
        return "<i>"
                + postpare_mathml_chars( Q3StyleSheet::escape(
                        inside.stripWhiteSpace() ) )
                + "</i>";
    } else if ( e.tagName() == "mspace" ) {
        return "&nbsp;"; // this really should parse the attrib width, but...
    } else if ( e.tagName() == "msup" ) {
        QStringList children = mathml_element_children_to_html( e );
        return children[0] + "<sup>" + children[1] + "</sup>";
    } else if ( e.tagName() == "msub" ) {
        QStringList children = mathml_element_children_to_html( e );
        return children[0] + "<sub>" + children[1] + "</sub>";
    } else if ( e.tagName() == "mfenced" ) {
        QStringList children = mathml_element_children_to_html( e, "^(?!mo)" );
        QString open, close, separator;
        open = e.hasAttribute( "open" ) ? e.attribute( "open" ) : QString( "(" );
        close = e.hasAttribute( "close" ) ? e.attribute( "close" ) : QString( ")" );
        separator = e.hasAttribute( "separators" ) ?
                    e.attribute( "separators" ) : QString( "," );
                    // should split above, but...
        return Q3StyleSheet::escape( open )
                + children.join( Q3StyleSheet::escape( separator ) )
                + Q3StyleSheet::escape( close );
    } else {
        qWarning( QString( "Can't handle %1 yet...returning [MML]" )
                  .arg( e.tagName() ).toLatin1().constData() );
        return "[MML]";
    }
}

QString mathml_to_html ( QString mathML ) {
    QDomDocument doc;
    doc.setContent( prepare_mathml_chars( mathML ) );
    QString result = mathml_element_to_html( doc.documentElement() );
    result.replace( "</i><i>", "" ); // just to simplify
    result.simplifyWhiteSpace();
    return result;
}


//
// MathText class implementation
//

MathText::MathText ()
    : text( "" ), mathML( "" ), HTML( "" )
{
}

MathText::MathText ( QString text )
    : text( text )
{
    mathML = "<mtext>" + text + "</mtext>";
    HTML = mathml_to_html( mathML );
}

MathText::MathText ( QString text, QString mathML )
    : text( text ), mathML( mathML )
{
    HTML = mathml_to_html( mathML );
}

void MathText::appendXMLTo ( QDomNode n, QString tagname ) {
    append_string_node_to( tagname, mathML, n )
            .toElement().setAttribute ( "text", Q3StyleSheet::escape( text ) );
}

bool MathText::makeFromXML ( QDomNode n, QString tagname ) {
    if ( !tagname.isEmpty() && ( n.nodeName() != tagname ) ) {
        qWarning( QString( "Expected %1 tag, got %2 tag" )
                  .arg( tagname ).arg( n.nodeName() ).toLatin1().constData() );
        return 0;
    }
    if ( !try_get_attrib( text, n, "text", "." ) ) {
        qWarning( QString( "Expected %1 tag to have text attribute" )
                  .arg( tagname ).toLatin1().constData() );
        return 0;
    }
    mathML = node_children_to_string( n );
    HTML = mathml_to_html( mathML );
    return 1;
}


//
// MathTextList class implementation
//

void MathTextList::appendXMLTo ( QDomNode n, QString outertag, QString innertag ) {
    QDomNode tag = n.appendChild ( n.ownerDocument().createElement ( outertag ) );
    for ( MathTextList::iterator i = begin() ; i != end() ; i++ )
        (*i).appendXMLTo( tag, innertag );
}

bool MathTextList::makeFromXML ( QDomNode n, QString outertag, QString innertag ) {
    clear();
    if ( !outertag.isEmpty() && ( n.nodeName() != outertag ) ) {
        qWarning( QString( "Expected %1 tag, got %2 tag" )
                  .arg( outertag ).arg( n.nodeName() ).toLatin1().constData() );
        return 0;
    }
    for ( QDomNode ch = n.firstChild() ; !ch.isNull () ; ch = ch.nextSibling() ) {
        if ( ch.isElement() ) {
            append( MathText() );
            if ( !last().makeFromXML( ch, innertag ) ) return 0;
        }
    }
    return 1;
}


