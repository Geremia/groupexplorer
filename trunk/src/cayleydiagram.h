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


#ifndef CAYLEYDIAGRAM_H
#define CAYLEYDIAGRAM_H


#include "gevisualizer.h"
#include "geltgrid.h"
#include "cdwidgets.h"
#include <glmodelview.h>
#include <gevlarge.h>
#include <qtooltip.h>
//Added by qt3to4:
#include <QPixmap>
#include <Q3PtrList>
#include <Q3MemArray>


class SubsetListBox;
class GeneratorControl;
class CDTipFlagger; // see end of this file
class QSlider;
class QCheckBox;


class CDLargeView : public GEVLargeWidget
{
    Q_OBJECT
    
public:
    CDLargeView ( GEVLarge* container = 0, QWidget* parent = 0, const char* name = 0 );
    
    virtual void set ( QString key, QString oldvalue, QString value, QString desc );
    virtual void updateContent ( QStringList changedKeys );
    virtual Q3PtrList<QAction> actionsToExport ();

    void placeCamera ();
    void reformDiagram ();
    void disassembleOrbitGridLayout ( GLSceneNode* startHere = 0 );
    void arrangeNodes ();
    void arrangeArrows ();
    //void considerTooltip ( const QPoint& p, CDTipFlagger* useMe );
    
    static QPixmap quickPixmap ( int w, int h, GroupFile* gf, int index );
    virtual QPixmap currentPixmap ();

    QPoint elementLocationPercent ( GElt e );
    
    static GEltGrid generateOrbitGrid ( GroupFile* G, GEltList gens );

public slots:
    virtual void groupFileChanged ( GroupFile* gf );
    //virtual void objectStartDrag ();
    virtual void copyImage ();
    virtual void dumpAsyCode ();
    virtual void startEditDrag ( QPoint orig );
    virtual void duringEditDrag ( QPoint prev, QPoint now );
    virtual void finishEditDrag ( QPoint orig, QPoint now );
    virtual void reset ();

protected:
    // for intercepting tooltip events and handling them based on widget region
    bool event( QEvent *event );

private:
    GLModelView* viewer;
    Q3PtrList<CDNode> nodes;
    Q3PtrList<CDArrow> arrows;
    GLSceneNode* diagram;
    GLSceneNode* nodesholder;
    GLSceneNode* arrowsholder;
    GEltGrid orbitGrid;
    QPixmap currentP;
    bool usedDims[3];
    GLSceneNode* dragging;
    double dragX, dragY, dragZ, screenZ;
    double screenArcDragX, screenArcDragY;
    bool needToReform;
    bool needToUpdateFog;
    double diagramRadius;
    QString chunkCosetName;

    CDArrow* getArrow ( GElt generator, GElt sourceNode );

    static GLModelView* quickviewer;
    static GLSceneNode* quickdiagram;
    static GLSceneNode* quicknodesholder;
    static GLSceneNode* quickarrowsholder;
    static void calibrateFog ( GLModelView* mv, QString level  );

    static void layOutOrbitGrid ( GEltGrid grid, Q3MemArray<uint> axes,
                                  GEltList orderperm, Q3PtrList<CDNode> nodes,
                                  GLSceneNode* insideThis,
                                  GEltList index = GEltList(), uint depth = 0 );
    
};


class CDLargeControl : public GEVLargeWidget
{
    Q_OBJECT

public:
    CDLargeControl ( GEVLarge* container = 0, QWidget* parent = 0, const char* name = 0 );

    virtual void set ( QString key, QString oldvalue, QString value, QString desc );

public slots:
    virtual void showHelp ();
    virtual void groupFileChanged ( GroupFile* gf );
    virtual void headPosChanged ( int toThis );
    virtual void labelStatusChanged ();
    virtual void labelSizeChanged ( int toThis );

private:
    SubsetListBox* listBox;
    GeneratorControl* gTable;
    QSlider* headSlider;
    QSlider* labelSlider;
    QCheckBox* labelCheckBox;

};


class CDLarge : public GEVLarge
{
public:
    CDLarge ( QWidget* parent = 0, const char* name = 0 );
    virtual QSize sizeHint () const;
    virtual QPixmap currentPixmap ();
    QPoint elementLocationPercent ( GElt e );
private:
    CDLargeView* view;
};


class CDVisualizer : public GEVisualizer
{
    Q_OBJECT
    
public:
    CDVisualizer ( GroupFile* gf = 0 );
    
    virtual QString typeName ();
    virtual QPixmap quickPixmap ( int w = -1, int h = -1, int index = -1 );
    virtual int numIndices ();
    virtual QStringList orderedKeys ();
    
protected:
    virtual void createLarge ();
    virtual void createSmall ();
    
};


class CDSmall : public GEVSmall
{
    Q_OBJECT

public:
    CDSmall ();

    // I'll eventually need these, but not yet
    //virtual void set ( QString key, QString oldvalue, QString value, QString desc );
    //virtual void updateContent ( QStringList changedKeys );

    virtual Q3PtrList<QAction> actionsToExport ();
};


/*
class CDTipFlagger : public QToolTip
{
public:
    CDTipFlagger ( QWidget* onThis, CDLargeView* forThis );
    void tip ( const QRect& rect, const QString& text );
protected:
    virtual void maybeTip ( const QPoint& p );
private:
    CDLargeView* forThis;
};
*/


#endif // CAYLEYDIAGRAM_H


