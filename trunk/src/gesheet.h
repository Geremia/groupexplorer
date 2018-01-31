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


#ifndef GESHEET_H
#define GESHEET_H


#include <q3canvas.h>
#include <qdom.h>
#include <QUndoStack>
#include <QUndoCommand>
//Added by qt3to4:
#include <QContextMenuEvent>
#include <Q3PointArray>
#include <QPixmap>
#include <QMouseEvent>
#include <Q3PtrList>
#include <QKeyEvent>
#include <Q3MemArray>
#include <sympanewidgets.h>


class CanvasLayoutGrid : public Q3CanvasRectangle
{
public:
    CanvasLayoutGrid ();
    ~CanvasLayoutGrid ();
    void resize ( int rows, int cols );
    void insert ( int row, int col, Q3CanvasItem* item );
    void erase ( int row, int col );
    void setMargins ( int h, int v );
    void setPadding ( int h, int v );
    QSize size ();
    void placeOnCanvas ( Q3Canvas* canvas, int x, int y, int z );
    static int RTTI;
    virtual int rtti () const;
private:
    Q3MemArray<Q3CanvasItem*> gridEntries;
    int hpadding;
    int vpadding;
    int hmargin;
    int vmargin;
    int numrows;
    int numcols;
};


class GECanvasText : public Q3CanvasText
{
public:
    GECanvasText ( Q3Canvas* canvas );
    void appendXMLTo ( QDomNode n );
    void makeFromXML ( QDomNode n );
protected:
    virtual void draw ( QPainter& painter );
};


class GECanvasRect : public Q3CanvasRectangle
{
public:
    GECanvasRect ( Q3Canvas* canvas );
    void appendXMLTo ( QDomNode n );
    void makeFromXML ( QDomNode n );
protected:
    virtual void draw ( QPainter& painter );
};


class GECanvasLine : public Q3CanvasLine
{
public:
    GECanvasLine ( Q3Canvas* canvas );

    void setFromItem ( Q3CanvasItem* item );
    void setToItem ( Q3CanvasItem* item );
    Q3CanvasItem* getFromItem ();
    Q3CanvasItem* getToItem ();

    void appendXMLTo ( QDomNode n, Q3CanvasItemList everyone );
    void makeFromXML ( QDomNode n, Q3CanvasItemList allsofar );

    virtual Q3PointArray areaPoints () const;

protected:
    static const int margin;
    void recomputeLocalVectors ( QPoint& dir, QPoint& perp ) const;
    void prepareToDraw ();
    virtual void draw ( QPainter& painter );

    Q3CanvasItem* from;
    Q3CanvasItem* to;
};


class GECanvasMorphism : public GECanvasLine
{
public:
    GECanvasMorphism ( Q3Canvas* canvas );
    ~GECanvasMorphism ();

    static GEltList tryEmbedding ( GroupFile* G1, GroupFile* G2 );
    static GEltList tryEmbedding ( GroupFile* G1, GroupFile* G2, GEltList H2 );

    GroupMorphism* getMorphism ();

    void setFromSmall ( GEVSmall* smallv );
    void setToSmall ( GEVSmall* smallv );
    void push ( GroupSubset* S );
    void pull ( GroupSubset* S );

    void appendXMLTo ( QDomNode n, Q3CanvasItemList everyone );
    void makeFromXML ( QDomNode n, Q3CanvasItemList allsofar );

    static int RTTI;
    virtual int rtti () const;

    virtual Q3PointArray areaPoints () const;

    GEltList domgens;
    GEltList codgens;
    GEltList buildMap ( GEltList dgens, GEltList cgens );

    bool showName;
    bool showDomCod;
    bool showEpiMon;
    bool showGens;
    bool manyLines;
    void updateLabel();

protected:
    void drawArrow ( QPainter& painter, QPoint from, QPoint to );
    virtual void draw ( QPainter& painter );
    GEVSmall* fromSmall () const;
    GEVSmall* toSmall () const;
    GroupMorphism morphism;
    Q3SimpleRichText* label;

    static GEltList tryEmbedding ( GroupFile* G1, GroupFile* G2, GEltList H2,
                                   GEltList domgens, GEltList somecodgens,
                                   GECanvasMorphism* m );
};


class GECanvasCornerHighlight : public Q3CanvasRectangle
{
public:
    GECanvasCornerHighlight ( Q3Canvas* canvas );
    void surroundThis ( Q3CanvasItem* item );
    Q3CanvasItem* surrounded ();
protected:
    virtual void draw ( QPainter& painter );
private:
    Q3CanvasItem* last;
};


class GESheet : public Q3Canvas
{
    Q_OBJECT

public:
    GESheet ( int w, int h );

    static int searchItem ( Q3CanvasItem* item, QString searchText );
    static bool canResizeItem ( Q3CanvasItem* item, int& w, int& h );
    static void resizeItem ( Q3CanvasItem* item, int w, int h );
    static bool isLineItem ( Q3CanvasItem* item );

    static Q3CanvasItem* XMLToItem ( QDomNode n,
                                    Q3CanvasItemList items = Q3CanvasItemList() );
    static void appendXMLTo ( QDomNode n, Q3CanvasItem* item,
                              Q3CanvasItemList items = Q3CanvasItemList() );
    static Q3CanvasItem* XMLStringToItem ( QString xml/*,
                                          QCanvasItemList items = QCanvasItemList()*/ );
    static QString itemToXMLString ( Q3CanvasItem* item );
    static Q3CanvasItemList XMLStringToItems ( QString xml/*,
                                              QCanvasItemList items = QCanvasItemList()*/
                                              );
    static QString itemsToXMLString ( Q3CanvasItemList items, int* missing = 0 );

    static QStringList groupsInSheet ( QString filename );
    static QStringList sheetsScanned ();
    static bool searchSheet ( QString filename, QString search4me,
                              bool wholeWord, bool caseSensitive );

    virtual void resize ( int w, int h );
    Q3CanvasItemList selectedItems ();
    void setSelection ( Q3CanvasItemList items );
    void selectAll ( bool on = TRUE );
    Q3CanvasItem* firstItemUnder ( QPoint canvasPoint );
    void mouseOver ( QPoint p );

    QString save ( QString filename ); // returns null for success, error msg otherwise
    QString saveToString ();
    QString load ( QString filename ); // returns null for success, error msg otherwise
    QString loadFromString ( QString fromMe );
    QPixmap drawToPixmap ();

    static QString formatMorphismName ( QString name );
    bool morphismNameUsed ( QString name );
    QString suggestMorphismName ();

    void noticeNewSmall ( GEVSmall* smallv );

signals:
    void registerNewSmall ( GEVSmall* smallv );

private:
    Q3CanvasRectangle* bkgd;
    GECanvasCornerHighlight* corn;

    static QMap<QString,QStringList> sheetToGroups;
    static QStringList scanned;
    static QStringList scanForGroups ( QString filename );
};


class GEShCmdParameterized : public QUndoCommand, public QObject
{
public:
    GEShCmdParameterized ( QUndoStack* undoStack );
    void setParameters ( Q3CanvasItemList items );
    Q3CanvasItem* parameter ( int i );
    virtual bool setup (); // called after parameters are set, before redo() ever called
protected:
    Q3CanvasItemList parameters;
private:
    QUndoStack* undoStack;
};


class GESheetView : public Q3CanvasView
{
    Q_OBJECT

public:
    GESheetView ( GESheet* s, QWidget* parent = 0, const char* name = 0, Qt::WFlags f = 0 );
    void clear ();

    void setSheet( GESheet* s );
    GESheet* sheet ();

    double zoom ();
    void selectItems ( Q3CanvasItemList items );
    Q3CanvasItemList selectedItems ();
    void copySelection ();
    Q3CanvasItemList paste ();
    void placeNewItem ( Q3CanvasItem* item );

    void getParametersFor ( GEShCmdParameterized* cmd, int numParams );

signals:
    void movedSelection ( int fromX, int fromY, int toX, int toY );
    void selectThese ( Q3CanvasItemList items );
    void changeTextItem ( Q3CanvasText* textItem, QString newtext );
    void maybeChangedVisualizer ();
    void selectionChanged ( Q3CanvasItemList items );
    void sheetContextMenu ( QPoint here, Q3PtrList<QAction> extraActions );
    void resizedItem ( Q3CanvasItem* item, int* origDims );
    void addedParameter ( Q3CanvasItem* param, int paramNum, int outOf );
    void cancelledParameters ();
    void editMorphism ( GECanvasMorphism* m );

public slots:
    void zoomIn ();
    void zoomOut ();
    void updateCanvas ();

protected:
    void contentsMouseDoubleClickEvent ( QMouseEvent* e );
    void contentsMousePressEvent ( QMouseEvent* e );
    void contentsMouseMoveEvent ( QMouseEvent* e );
    void contentsMouseReleaseEvent ( QMouseEvent* e );
    void contentsContextMenuEvent ( QContextMenuEvent* e );
    void keyPressEvent ( QKeyEvent* e );

private:
    QPoint movingStart;
    QPoint lastPos;
    double zoomLevel;
    bool moving;
    int resizeH;
    int resizeV;
    Qt::CursorShape cursor;
    int origDims[4];
    int numParamsTotal;
    int numParamsLeft;
    Q3CanvasItemList parameters;
    GEShCmdParameterized* pending;

    void changeCursor ( Qt::CursorShape newcursor );
    void resizingCodes ( QPoint mouse, int& horizontal, int& vertical, int* origDims = 0 );
};


#endif // GESHEET_H


