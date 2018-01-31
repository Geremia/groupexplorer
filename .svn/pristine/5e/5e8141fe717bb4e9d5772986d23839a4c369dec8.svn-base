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


#include "geml.h"
#include <qregexp.h>
#include <QTextStream>


//
// here are some helper functions I wrote to make DOM processing easier
//

bool try_get_string ( QString& result, QDomNode n, QString tagname, QString pattern ) {
    if ( !tagname.isEmpty() && ( n.nodeName() != tagname ) ) {
        qWarning( QString( "Got %1 tag when expected %2 tag" )
                  .arg( n.nodeName() ).arg( tagname ).toLatin1().constData() );
        return 0;
    }
    result = n.firstChild().isNull() ? QString( "" ) : n.firstChild().toText().nodeValue();
    if ( QRegExp( pattern ).search( result ) == -1 ) {
        qWarning( QString( "Expected text of form %1, got text %2" )
                  .arg( pattern ).arg( result ).toLatin1().constData() );
        return 0;
    }
    return 1;
}

bool try_get_attrib ( QString& result, QDomNode n, QString attrname, QString pattern ) {
    if ( !n.isElement() ) {
        qWarning( QString( "Requested attribute %1 inside %2 tag (non-element)" )
                  .arg( attrname ).arg( n.nodeName() ).toLatin1().constData() );
        return 0;
    }
    QDomElement elt = n.toElement();
    if ( !elt.hasAttribute( attrname ) ) {
        qWarning( QString( "Attribute %1 requested; not present in %2 element" )
                  .arg( attrname ).arg( n.nodeName() ).toLatin1().constData() );
        return 0;
    }
    QString val = elt.attribute( attrname );
    if ( QRegExp( pattern ).search( val ) == -1 ) {
        qWarning( QString( "Expected attribute value of form %1, got value %2" )
                  .arg( pattern ).arg( val ).toLatin1().constData() );
        return 0;
    }
    result = val;
    return 1;
}

QDomNode append_string_node_to ( QString tagname, QString content, QDomNode n ) {
    // creates new last child of n, <tagname>content</tagname>, and returns it
    return n.appendChild ( n.ownerDocument().createElement( tagname ) )
            .appendChild ( n.ownerDocument().createTextNode( content ) )
            .parentNode();
}

QString node_children_to_string ( QDomNode n ) {
    QString answer = "";
    QTextStream ts( &answer, QIODevice::WriteOnly );
    QDomNodeList l = n.childNodes();
    for ( uint i = 0 ; i < l.length(); i++ )
        l.item(i).save( ts, 4 );
    return answer;
}


