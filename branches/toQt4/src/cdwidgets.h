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


#ifndef CDWIDGETS_H
#define CDWIDGETS_H


#include "gevisualizer.h"
#include "glscenenodes.h"
#include "geltgrid.h"
#include <q3table.h>
//Added by qt3to4:
#include <QLabel>
#include <Q3PtrList>
#include <Q3MemArray>


class GroupFile;
class Q3ListBox;
class QPushButton;
class QComboBox;
class QSlider;
class QCheckBox;
class QLabel;


/*
class PushyTable : public Q3Table // so called because it gives a large sizeHint
{
public:
    PushyTable ( QWidget* parent = 0 );
    virtual QSize minimumSizeHint () const;
};
*/


class GeneratorControl : public GEVLargeWidget
{
    Q_OBJECT
    
public:
    GeneratorControl ( GEVLarge* container = 0, GroupFile* gf = 0,
                       QWidget* parent = 0, const char* name = 0 );
    
    void reset ();
    virtual void set ( QString key, QString oldvalue, QString value, QString desc );
    //virtual void updateContent ( QStringList changedKeys );

    void updateTableCell ( int row, int col );
    void setGenerators ( GEltList gens );
    void setAxes ( Q3MemArray<uint> ax );
    void setOrder ( GEltList ordr );
    void setArrows ( GEltList arrs );
    void fixAnyGeneratorProblems ( uint lastGoodGenIndex );

    static void niceDiagram ( GroupFile* groupfile, GEltList& gens,
                              GEltGrid& grid, GEltList& axes, GEltList& ordr );

public slots:
    virtual void groupFileChanged ( GroupFile* gf );
    virtual void contextMenuRequested ( int row, int col, const QPoint& pos );
    virtual void rowsRearranged ( int section, int from, int to );
    virtual void addButtonClicked ();
    virtual void removeButtonClicked ();
    virtual void enableDisableAddRemoveButtons ();
    virtual void changeChunk ( int toThis );
    //virtual void labelSizeChanged ( int toThis );
    //virtual void labelStatusChanged ();
    //virtual void headPosChanged ( int toThis );
    virtual void changeMult ( int toThis );
    virtual void diagramChanged ( int toThis );

private:
    Q3Table* table;
    Q3ListBox* arrows;
    QPushButton* addButton;
    QPushButton* removeButton;
    QComboBox* chunk;
    QLabel* chunkLabel;
    QComboBox* mult;
    QComboBox* diagram;
    //QCheckBox* labelCheckBox;
    //QSlider* labelSlider;
    //QSlider* headSlider;

    QStringList axisNames;
    QStringList axisPictures;
    GEltList generators;
    GEltGrid orbitGrid;
    Q3MemArray<uint> axes;
    GEltList orderperm;

    void gTableChanged ( QString reason = QString::null );
    void arrowsChanged ( QString reason = QString::null );
    void chunkChanged ( QString reason = QString::null );

    void resetDiagram ();
    void resetGenerators ();
    void resetArrows ();
    void checkChunkOrder ();

    GEltList getArrows ();
    
};


class CDAxisWrapper : public GLSceneNode//GLWireCube
{
public:
    CDAxisWrapper ( GLSceneNode* parent = 0 );
    virtual void placeMe ( uint axis, uint myIndex, uint outOf );
    virtual void preRender ();
    virtual void renderMe ();
    void chunkThis ( bool on = TRUE );

    static QStringList getAllAxisNames ();
    static QStringList getAllAxisPictures ();
private:
    void axesUsed ( bool& x, bool& y, bool& z );
    bool chunk;
    uint axistype;
};


class CDNode : public GLSceneNode
{
public:
    CDNode ( GLSceneNode* diagram = 0, bool simple = FALSE );

    virtual void preRender ();
    virtual void renderMe ();
    virtual void postRender ();

    void placeActualNode ();
    void setRadius ( double r );
    void setLabel ( QString l );
    void setSize ( uint s );
    QString getLabel ();

    bool containsSphere ( GLSceneNode* sphere );
    bool containedByChunk ( GLSceneNode* chunk );

    void setAbsolutePosition ( double x, double y, double z );
    void clearAbsolutePosition ();
    void getAbsolutePositionAndRadius ( double& x, double& y, double& z, double& r,
                                        GLSceneNode* relativeTo = 0 );
    void getDiagramPositionAndRadius ( double& x, double& y, double& z, double& r );

    void setHighlighting ( uint htype, QColor color );
    void clearHighlighting ( uint htype );

private:
    double radius;
    double myx, myy, myz;
    QString label;
    GLSphere* actualNode;
    QFont font;
    bool useLabels;
    GLSceneNode* diagram;
    bool highlight[3];
    QColor highcolor[3];
};


class CDArrow : public GLSceneNode
{
public:
    CDArrow ( CDNode* from, CDNode* to, GLSceneNode* diagram/*, bool simple = FALSE*/ );
    void setArcing ( GLdouble amount,
                     GLdouble centerX, GLdouble centerY, GLdouble centerZ );
    void setArcing ( GLdouble amount );
    GLdouble getArcing ();
    void setHeadPosition ( GLdouble percent );
    GLdouble getHeadPosition ();
    void setOrder ( uint o );
    uint getOrder ();

    double myInvasions ( Q3PtrList<CDNode> nodes, double arcLevel );
    double myInvasions ( Q3PtrList<CDNode> nodes ); // calls above func w/current arcAmount
    double bestHigherArcing ( Q3PtrList<CDNode> nodes, double thanThis );
    double bestHigherArcing ( Q3PtrList<CDNode> nodes ); // call above func w/cur arcAmount
    void decideArcing ( Q3PtrList<CDNode> nodes );
    void decideArcing ();
    void arrowPath ( double percent, GLdouble& x, GLdouble& y, GLdouble& z,
                     double arcLevel );

    virtual void renderMe ();

private:
    CDNode* fromNode;
    CDNode* toNode;
    GLSceneNode* diagram;
    uint order;
    GLdouble arcAmount;
    GLdouble arcCenterX, arcCenterY, arcCenterZ;
    GLdouble arcVX, arcVY, arcVZ;
    GLdouble headPos;
    //bool simple;

    GLdouble fr, fx, fy, fz;
    GLdouble tr, tx, ty, tz;
    GLdouble ux, uy, uz;
    void setupInternalValues (); // sets f*,t*,u* according to fromNode, toNode positions
    void myArrowPath ( double percent, GLdouble& x, GLdouble& y, GLdouble& z );
    void myArrowPath ( double percent, GLdouble& x, GLdouble& y, GLdouble& z,
                       double arcLevel ); // calls above func w/current arcAmount
};


#endif // CDWIDGETS_H


