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


#include "gesheet.h"
#include "gesheetwindow.h"
#include "gevisualizer.h"
#include <qtundo.h>


class GEPreferences;


class GEShCmdInsertText : public QtCommand
{
    Q_OBJECT
    
public:
    GEShCmdInsertText ( GESheetWindow* window );
    virtual void redo ();
    virtual void undo ();
    
private:
    GESheetWindow* window;
    GECanvasText* textBox;
    
};


class GEShCmdInsertRect : public QtCommand
{
    Q_OBJECT
    
public:
    GEShCmdInsertRect ( GESheetWindow* window );
    virtual void redo ();
    virtual void undo ();
    
private:
    GESheetWindow* window;
    GECanvasRect* box;
    
};


class GEShCmdInsertVisualizer : public QtCommand
{
    Q_OBJECT
    
public:
    GEShCmdInsertVisualizer ( GESheetWindow* window, GEVSmall* visualizer );
    virtual void redo ();
    virtual void undo ();
    
private:
    GESheetWindow* window;
    GEVSmall* visualizer;
    
};


class GEShCmdZoomIn : public QtCommand
{
    Q_OBJECT
    
public:
    GEShCmdZoomIn ( GESheetWindow* window );
    virtual void redo ();
    virtual void undo ();
    
private:
    GESheetWindow* window;
    
};


class GEShCmdZoomOut : public QtCommand
{
    Q_OBJECT
    
public:
    GEShCmdZoomOut ( GESheetWindow* window );
    virtual void redo ();
    virtual void undo ();
    
private:
    GESheetWindow* window;
    
};


class GEShCmdMovedSelection : public QtCommand
{
    Q_OBJECT
    
public:
    GEShCmdMovedSelection ( GESheetWindow* window, 
                            int fromX, int fromY, int toX, int toY );
    virtual void redo ();
    virtual void undo ();
    
private:
    GESheetWindow* window;
    bool firstTime;
    int fromX;
    int fromY;
    int toX;
    int toY;
    
};


class GEShCmdSelectItems : public QtCommand
{
    Q_OBJECT
    
public:
    GEShCmdSelectItems ( GESheetView* sheetview, Q3CanvasItemList items );
    virtual void redo ();
    virtual void undo ();
    
private:
    GESheetView* sheetview;
    Q3CanvasItemList newsel;
    Q3CanvasItemList oldsel;
    
};


class GEShCmdChangeText : public QtCommand
{
    Q_OBJECT

public:
    GEShCmdChangeText ( Q3CanvasText* textitem, QString newtext );
    virtual void redo ();
    virtual void undo ();

private:
    Q3CanvasText* textitem;
    QString oldtext;
    QString newtext;

};


class GEShCmdResizeText : public QtCommand
{
    Q_OBJECT

public:
    GEShCmdResizeText ( Q3CanvasText* textitem, bool increase );
    virtual void redo ();
    virtual void undo ();

private:
    Q3CanvasText* textitem;
    bool increase;

};


class GEShCmdCutItems : public QtCommand
{
    Q_OBJECT

public:
    GEShCmdCutItems ( GESheetView* sheetview );
    virtual void redo ();
    virtual void undo ();
    uint numberOfConnections (); // how many EXTRA things did you have to cut?

private:
    Q3CanvasItemList items;
    Q3CanvasItemList cnxns; // connections to these items, which we must also hide
    GESheetView* sheetview;

};


class GEShCmdPasteItems : public QtCommand
{
    Q_OBJECT

public:
    GEShCmdPasteItems ( Q3CanvasItemList items );
    virtual void redo ();
    virtual void undo ();

private:
    Q3CanvasItemList items;

};


class GEShCmdResizedItem : public QtCommand
{
    Q_OBJECT

public:
    GEShCmdResizedItem ( Q3CanvasItem* item, int* origDims );
    virtual void redo ();
    virtual void undo ();

private:
    bool firstTime;
    Q3CanvasItem* item;
    int origD[4];
    int newD[4];

};


class GEShCmdPushSelectionZ : public QtCommand
{
    Q_OBJECT

public:
    GEShCmdPushSelectionZ ( GESheetView* sheetview, bool backwards );
    virtual void redo ();
    virtual void undo ();

private:
    Q3CanvasItemList items;
    bool backwards;

};


class GEShCmdConnectWithLine : public GEShCmdParameterized
{
    Q_OBJECT

public:
    GEShCmdConnectWithLine ( QtUndoStack* undoStack );
    virtual bool setup ();
    virtual void redo ();
    virtual void undo ();

private:
    GECanvasLine* line;
};


class GEShCmdConnectWithMorphism : public GEShCmdParameterized
{
    Q_OBJECT

public:
    GEShCmdConnectWithMorphism ( QtUndoStack* undoStack, QString morphName );
    virtual bool setup ();
    virtual void redo ();
    virtual void undo ();

private:
    GECanvasMorphism* morphism;
    QString tmp;
};


class GEShCmdChangeMorphism : public QtCommand
{
    Q_OBJECT

public:
    GEShCmdChangeMorphism ( GECanvasMorphism* morphism,
                            GEltList domgens, GEltList codgens,
                            bool name, bool domcod, bool epimon, bool gens, bool many );
    virtual void redo ();
    virtual void undo ();

private:
    GECanvasMorphism* morphism;
    GEltList newdomgens;
    GEltList newcodgens;
    bool newname;
    bool newdomcod;
    bool newepimon;
    bool newgens;
    bool newmany;
    GEltList olddomgens;
    GEltList oldcodgens;
    bool oldname;
    bool olddomcod;
    bool oldepimon;
    bool oldgens;
    bool oldmany;
};


