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


#ifndef SUBSETEDITOR_H
#define SUBSETEDITOR_H


#include "groupfile.h"
#include <qdialog.h>
#include <q3listbox.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QLabel>


class QLabel;
class QToolButton;
class Q3GridLayout;
class Q3VBoxLayout;
class QSpacerItem;


//#include "sympanewidgets.h"
class GroupSubset;


// mirroring this class after the one on pages 217-218 of Blanchette & Summerfield
class GEltListBox : public Q3ListBox
{
    Q_OBJECT

public:
    GEltListBox ( GroupFile* groupfile, QWidget* parent, const char *name = 0 );

    void setElements ( GEltList elts );
    void addElement ( GElt e );
    void removeElement ( GElt e );
    GElt getElement ( uint index );
    GEltList getElements ();

protected:
    void contentsMousePressEvent ( QMouseEvent *e );
    void contentsMouseMoveEvent ( QMouseEvent* e );
    void contentsDragEnterEvent ( QDragEnterEvent *e );
    void contentsDropEvent ( QDropEvent *e );

private:
    void startDrag ();

    GroupFile* groupfile;
    QPoint dragPos;
};


class SubsetEditor : public QDialog
{
    Q_OBJECT

public:
    SubsetEditor ( GroupSubset* subset, QWidget* parent, const char *name = 0,
        bool modal = FALSE, Qt::WFlags fl = 0 );

    GEltList getElements ();

private:
    GEltListBox* inside;
};


#endif // SUBSETEDITOR_H
