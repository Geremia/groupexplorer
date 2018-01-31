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


#ifndef GEPREFERENCES_H
#define GEPREFERENCES_H


#include "mathtext.h"
#include <mypreferences.h>


class GEPreferences : public MyPreferences {
    
public:
    GEPreferences ( const QString& filename,
                    const QString& format,
                    const QString& version,
                    const QStringList& colNames );

    GEPreferences* createCopy ();
    void debugGroup ();
    
    void setColumnVisible ( const QString& key, const bool value );
    bool getColumnVisible ( const QString& key, const bool def = FALSE );
    void setColumnPosition ( const QString& key, const int value );
    int getColumnPosition ( const QString& key, const int def = -1 );
    void setColumnWidth ( const QString& key, const int value );
    int getColumnWidth ( const QString& key, const int def = -1 );
    
    void setDefaultColumnVisible ( const QString& key, const bool def );
    void setDefaultColumnPosition ( const QString& key, const uint def );
    
    QStringList getColumnsMentioned ();

    void setGroupNotes ( const QString& code, const QString& notes );
    QString getGroupNotes ( const QString& code,
                            const QString& def = QString::null );
    
    void setGroupRep ( const QString& code, const uint repNum,
                       const MathTextList reps );
    MathTextList getGroupRep ( const QString& code, const uint repNum );
    uint numGroupReps ( const QString& code );
    void removeGroupRep ( const QString& code, const uint repNum );
    void insertGroupRep ( const QString& code, const uint beforeThis,
                          MathTextList rep );
    uint getCurrentGroupRep ( const QString& code );
    void setCurrentGroupRep ( const QString& code, const uint repNum );
    
    void createColumn ( const QString& colname );
    void deleteColumn ( const QString& colname );
    void renameColumn ( const QString& oldname, const QString& newname );
    void setColumnValue ( const QString& colname, const QString& groupcode,
                          const QString& value );
    QString getColumnValue ( const QString& colname, const QString& groupcode,
                             const QString& def = QString::null );
    QStringList getUserDefinedColumns ();
    
private:
    GEPreferences ();
    
};


#endif // GEPREFERENCES_H


