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


/*
 *  This is the main widget that sits in the center of the main window of
 *  Group Explorer.  It lists all the groups that are currently in memory,
 *  and it allows the user to rearrange all sorts of viewing options and
 *  double click groups to open their group info windows.  This latter
 *  thing is achieved by emitting a signal which MainWindow is listening to.
 */


#ifndef GROUPTABLE_H
#define GROUPTABLE_H


#include <q3table.h>
#include <qstringlist.h>
#include <q3valuelist.h>
//Added by qt3to4:
#include <QEvent>
#include <gepreferences.h>
#include <qtundo.h>
#include <qmap.h>
#include "groupfile.h"


class GroupTable : public Q3Table
{
    Q_OBJECT
    
public:
    GroupTable ( int numRows, int numCols, QWidget * parent = 0,
                 const char * name = 0 );
    
    static GroupTable* firstTable;
    static GroupFile* findGroup ( QString code, bool warning = TRUE );
    static int numGroups ();
    static GroupFile* getGroup ( uint i );
    int findGroupRow ( QString code );
    GroupFile* groupAtRow ( int row );
    
    void loadGroups ( QStringList paths );
    void fillTable ();
    
    void setPreferences ( GEPreferences *prefs );
    void setUndoStack ( QtUndoStack* stack );
    void validatePreferences ();
    void applyPreferences ( GEPreferences *p );
    void applyPreferences ();
    void saveRowsInPreferences ();
    void saveColumnsInPreferences ();
    void setCustomValue ( QString colname, QString groupcode, QString value );
    
    virtual void showColumn ( int col );
    virtual void hideColumn ( int col );
    virtual void removeColumn ( int col );
    virtual void appendColumn ( QString name );
    virtual void appendColumn ( QString name, QStringList values, int width = -1 );

    QStringList getRowOrder ();
    void setRowOrder ( const QStringList& order );
    QStringList getBuiltInColumnNames ();
    void sortColumn ( int column, bool ascending );
    void sortColumn ( int column );
    void updateColumnsAndSorting ();
    void swapColumns ( int col1, int col2 );
    void setColumnName( int column, QString name );
    
    bool eventFilter ( QObject* o, QEvent* e );

    QString selectionAsHTML ();
    
signals:
    void progressChanged ( uint );
    void messageChanged ( QString );
    void enterNewPhase ( uint, uint );
    void finishPhase ();
    void hideSplashScreen ();
    void reshowSplashScreen ();
    
    void requestedGroupInfo ( QString code );

public slots:
    void rowsRearranged ( int, int from, int to );
    void columnsRearranged ( int, int from, int to );
    void columnSizeChanged( int section, int oldsize, int newsize );
    void doubleClickedRow ( int row, int col );
    void columnHeaderClicked ( int column );
    void headerMenu ( int id );
    void contextMenuRequested ( int row, int col, const QPoint& pos );
    void explain ();
    void postEditCommand ( int row, int col );

private:
    uint numColumns;
    QStringList tableColumns;
    GEPreferences *lastPrefs;
    bool wayJustSorted;
    int colJustSorted;
    QtUndoStack *undoStack;
    Q3ValueList<GroupFile> groups;
    int popupRow, popupCol;
    QMap<QString,bool> colNameToFilled;
    QMap<QString,int> colNameToPos;
    QMap<QString,int> colNameToStdPos;
    QStringList colPosToName;
    bool isAtStartup;
    uint numColsComputed;
    uint numColsToCompute;

    void fillColumn ( uint col );
    void hideAndShowColumns ();
    int rowOriginalPosition ( int row );
    void updateColumnMaps ();

};


class GTCmdEditCell : public QtCommand
{
    Q_OBJECT
    
public:
    GTCmdEditCell ( GroupTable* table, GEPreferences* prefs, int row, int col );
    virtual void redo ();
    virtual void undo ();
    
private:
    GroupTable* table;
    QString colname;
    QString groupcode;
    QString oldvalue;
    QString newvalue;
};


class GTCmdSortColumn : public QtCommand
{
    Q_OBJECT
    
public:
    GTCmdSortColumn ( GroupTable *table, int column, bool ascending,
                      int colJustSorted, bool wayJustSorted );
    virtual void redo ();
    virtual void undo ();

private:
    GroupTable *table;
    QStringList oldRowOrder;
    int sortByThisColumn;
    bool ascending;
    int colJustSorted;
    bool wayJustSorted;
};


class GTCmdReorderRows : public QtCommand
{
    Q_OBJECT
    
public:
    GTCmdReorderRows ( GroupTable *table,
                       int draggedFrom, int draggedTo,
                       int colJustSorted, bool wayJustSorted );
    virtual void redo ();
    virtual void undo ();
    
private:
    GroupTable *table;
    QStringList oldRowOrder;
    int colJustSorted;
    bool wayJustSorted;
    int draggedThisRow;
    int intoThisRow;
    bool firstRedo;
    
    void pushRow ( int from, int to );
};


class GTCmdReorderColumns : public QtCommand
{
    Q_OBJECT
    
public:
    GTCmdReorderColumns ( GroupTable *table, int draggedFrom, int draggedTo );
    virtual void redo ();
    virtual void undo ();
    
private:
    GroupTable *table;
    QStringList oldColumnOrder;
    int draggedThisColumn;
    int intoThisColumn;
    bool firstRedo;
    
    void pushColumn ( int from, int to );
};


class GTCmdHideShowColumn : public QtCommand
{
    Q_OBJECT
    
public:
    GTCmdHideShowColumn ( GroupTable* table, int column, bool hide );
    virtual void redo ();
    virtual void undo ();
    
private:
    GroupTable* table;
    int column;
    bool hide;
};


class GTCmdDeleteColumn : public QtCommand
{
    Q_OBJECT
    
public:
    GTCmdDeleteColumn ( GroupTable* table, int column );
    virtual void redo ();
    virtual void undo ();
    
private:
    GroupTable* table;
    QString colname;
    int column;
    QStringList values;
};


class GTCmdRenameColumn : public QtCommand
{
    Q_OBJECT
    
public:
    GTCmdRenameColumn ( GroupTable* table, int column, QString text );
    virtual void redo ();
    virtual void undo ();
    
private:
    GroupTable* table;
    int column;
    QString newname;
    QString oldname;
};


class GTCmdCreateColumn : public QtCommand
{
    Q_OBJECT
    
public:
    GTCmdCreateColumn ( GroupTable* table, QString colname );
    virtual void redo ();
    virtual void undo ();
    
private:
    GroupTable* table;
    QString colname;
};


class GTCmdApplyPreferences : public QtCommand
{
    Q_OBJECT
    
public:
    GTCmdApplyPreferences ( GroupTable* table,
                            GEPreferences* oldPrefs, GEPreferences* newPrefs );
    virtual void redo ();
    virtual void undo ();
    
private:
    GroupTable *table;
    GEPreferences *oldPrefCopy;
    GEPreferences *newPrefCopy;
    
};


class GTCmdResizeColumn : public QtCommand
{
    Q_OBJECT
    
public:
    GTCmdResizeColumn ( GroupTable* table, int column, int oldsize, int newsize );
    virtual void redo ();
    virtual void undo ();
    
protected:
    virtual bool mergeMeWith ( QtCommand* c );
    
private:
    GroupTable* table;
    int column;
    int oldsize;
    int newsize;
    bool firstRedo;
    
};


#endif // GROUPTABLE_H
