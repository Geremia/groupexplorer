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


#ifndef CYCLEGRAPH_H
#define CYCLEGRAPH_H


#include "gevisualizer.h"
#include "sympanewidgets.h"
#include <gevlarge.h>
//Added by qt3to4:
#include <QPixmap>
#include <Q3PointArray>
#include <Q3PtrList>


class CGLargeView : public GEVLargeWidget
{
    Q_OBJECT
    
public:
    CGLargeView ( GEVLarge* container = 0, QWidget* parent = 0, const char* name = 0 );
    
    virtual void set ( QString key, QString oldvalue, QString value, QString desc );
    virtual Q3PtrList<QAction> actionsToExport ();
    virtual void updateContent ( QStringList changedKeys );
    
    static QPixmap quickPixmap ( int w, int h, GroupFile* gf );
    virtual QPixmap currentPixmap ();
    QPoint elementLocationPercent ( GElt e );
    
public slots:
    virtual void zoomIn ();
    virtual void zoomOut ();
    virtual void copyImage ();
    virtual void groupFileChanged ( GroupFile* gf );
    //virtual void graphStartDrag( QPoint p );
    virtual void dumpAsyCode ();
    
private:
    ScrollableLabel* graph;
    QPixmap p; // p is for making the graph offscreen and remembering it
    QAction* zia;
    QAction* zoa;

    GroupFile* computedForThis; // used to tell if recomputation needed (different gfile)
    GEltListList myCycles; // stores part of computation for later speedy recall
    Q3PointArray myEltPoints; // stores part of computation for later speedy recall
    QSize mySize; // stores part of computation for later speedy recall
    Q3PointArray myArcPoints; // stores part of computation for later speedy recall
    
    static uint maximumEfficientSize;

    static void computeEverything (
        Group& g, GEltList permutedElements, double distanceRequested, uint granularity,
        GEltListList& cycles,
        Q3PointArray& eltpositions, Q3PointArray& arcpositions, QSize& size );
    void saveEverything ( GroupFile* gf );
    void updatePixmapP ();
};


class CGLargeControl : public GEVLargeWidget
{
    Q_OBJECT
    
public:
    CGLargeControl ( GEVLarge* container = 0, QWidget* parent = 0, const char* name = 0 );
    
public slots:
    virtual void showHelp ();
    virtual void groupFileChanged ( GroupFile* gf );
    
private:
    SubsetListBox* listBox;
};


class CGLarge : public GEVLarge
{
public:
    CGLarge ( QWidget* parent = 0, const char* name = 0 );
    virtual QSize sizeHint () const;
    virtual QPixmap currentPixmap ();
    virtual QPoint elementLocationPercent ( GElt e );
private:
    CGLargeView* view;
};


class CGVisualizer : public GEVisualizer
{
    Q_OBJECT
    
public:
    CGVisualizer ( GroupFile* gf = 0 );
    
    virtual QString typeName ();
    virtual QPixmap quickPixmap ( int w = -1, int h = -1, int index = -1 );
    
protected:
    virtual void createLarge ();
    virtual void createSmall ();
    
};


class CGSmall : public GEVSmall
{
    Q_OBJECT

public:
    CGSmall ();

    /* I'll eventually need these, but not yet
    virtual void set ( QString key, QString oldvalue, QString value, QString desc );
    virtual void updateContent ( QStringList changedKeys );
    */

    virtual Q3PtrList<QAction> actionsToExport ();
};


#endif // CYCLEGRAPH_H

