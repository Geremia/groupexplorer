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


#ifndef MATHTABLEITEM_H
#define MATHTABLEITEM_H


#include <q3table.h>
#include <qstring.h>
//Added by qt3to4:
#include <QPixmap>
//#include <qtmmlwidget.h>
#include "mathtext.h"


class Q3SimpleRichText;


class GETableItem : public Q3TableItem {
    
public:
    GETableItem ( Q3Table* table, uint origRow, EditType et );
    GETableItem ( Q3Table* table, uint origRow, QString data, EditType et = Never );
    GETableItem ( Q3Table* table, uint origRow, QPixmap data ); // uneditable
    GETableItem ( Q3Table* table, uint origRow, int data ); // uneditable
    GETableItem ( Q3Table* table, uint origRow, MathText data, uint size = 0 );// uneditable
    ~GETableItem ();
    
    uint originalRow ();
    
    void setLink ( QString ref );
    QString getLink ();
    
    QString key () const;

    virtual void paint ( QPainter * p, const QColorGroup & cg, const QRect & cr,
                         bool selected );
    virtual QSize sizeHint () const;
    
// immitating QComboTableItem:
#if !defined(Q_NO_USING_KEYWORD)
    using Q3TableItem::text;
#endif
    QString asHTML ();

    int rtti() const;
    static int RTTI;
    
private:
    uint origRow;
    QString link;
    QString myKey;
    Q3SimpleRichText* srt;
    QString html;
    
};


#endif // MATHTABLEITEM_H


