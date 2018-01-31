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


#ifndef MODELVIEWWIDGETS_H
#define MODELVIEWWIDGETS_H


#include "gevisualizer.h"
#include <qstringlist.h>


class QSlider;
class QCheckBox;


class ModelViewControl : public GEVLargeWidget
{
    Q_OBJECT
    
public:
    ModelViewControl ( GEVLarge* container = 0,
                       QWidget* parent = 0, const char* name = 0 );
    
    virtual void set ( QString key, QString oldvalue, QString value, QString desc );

public slots:
    virtual void zoomChanged ( int zoom );
    virtual void thicknessChanged ( int zoom );
    virtual void radiusChanged ( int zoom );
    virtual void fogLevelChanged ( int zoom );
    virtual void fogStatusChanged ();

private:
    QSlider* zoomSlider;
    QSlider* thickSlider;
    QSlider* radiusSlider;
    QSlider* fogSlider;
    QCheckBox* fogCheckBox;

};


#endif // MODELVIEWWIDGETS_H

