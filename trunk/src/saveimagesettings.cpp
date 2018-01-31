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
/****************************************************************************
** Form implementation generated from reading ui file 'src\saveimagesettings.ui'
**
** Created: Wed Oct 19 16:36:25 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "saveimagesettings.h"

#include <qvariant.h>
#include <qfiledialog.h>
#include <qwmatrix.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qslider.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "saveimagesettings.ui.h"
static const unsigned char image0_data[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x16,
    0x08, 0x06, 0x00, 0x00, 0x00, 0xc4, 0xb4, 0x6c, 0x3b, 0x00, 0x00, 0x04,
    0x5e, 0x49, 0x44, 0x41, 0x54, 0x78, 0x9c, 0xa5, 0xd4, 0x4b, 0x6c, 0x54,
    0x55, 0x1c, 0xc7, 0xf1, 0xef, 0x39, 0xf7, 0xcc, 0x9d, 0x47, 0x6f, 0x3b,
    0x7d, 0xd1, 0x56, 0x1a, 0x40, 0xd4, 0x80, 0x08, 0x8a, 0x60, 0x22, 0xa1,
    0x0d, 0x1a, 0x41, 0x13, 0x45, 0x37, 0xc4, 0x85, 0x26, 0x86, 0x85, 0x9a,
    0x68, 0x24, 0xee, 0x5c, 0x74, 0x63, 0x62, 0xdc, 0x69, 0xe2, 0xc6, 0xa5,
    0x89, 0x89, 0x1b, 0x5d, 0x48, 0xdc, 0xb4, 0x2e, 0x44, 0x49, 0x28, 0x22,
    0x84, 0xa2, 0x89, 0xb5, 0x40, 0x67, 0x3a, 0xa5, 0xcf, 0x69, 0xa7, 0x76,
    0x1e, 0x1d, 0x87, 0xb9, 0x9d, 0xdb, 0x99, 0xfb, 0x38, 0xd7, 0x05, 0x6d,
    0x8d, 0x5a, 0x4c, 0xd4, 0x7f, 0xf2, 0xdf, 0x7e, 0xce, 0xef, 0xfc, 0xff,
    0x27, 0x47, 0x00, 0x72, 0x60, 0x60, 0xe0, 0x65, 0xad, 0xf5, 0x99, 0x52,
    0xa9, 0xd4, 0x6f, 0xdb, 0x36, 0xff, 0xb6, 0xaa, 0xd5, 0x2a, 0xa9, 0x54,
    0xea, 0xda, 0xe2, 0xe2, 0xe2, 0x67, 0xc0, 0x59, 0xc0, 0x51, 0x80, 0xa1,
    0xb5, 0x3e, 0x93, 0xb8, 0xff, 0x89, 0xfe, 0x96, 0xdd, 0x49, 0x62, 0x01,
    0xf4, 0xf4, 0xee, 0xa4, 0x30, 0x71, 0x89, 0xa5, 0xaf, 0x3f, 0x66, 0xe0,
    0xed, 0xe3, 0x1c, 0x7c, 0xe1, 0x45, 0xa4, 0x69, 0x11, 0x86, 0x2e, 0x48,
    0x81, 0x57, 0x9d, 0xa5, 0x7a, 0x23, 0xcd, 0xc5, 0xb3, 0x63, 0x7c, 0x3b,
    0x5d, 0x66, 0xe9, 0xf6, 0x2a, 0x95, 0xe2, 0xca, 0x11, 0xc0, 0x04, 0x86,
    0x81, 0xa2, 0x02, 0x8c, 0x52, 0xa9, 0xd4, 0xdf, 0x7d, 0xf0, 0x5e, 0x26,
    0x32, 0x39, 0x12, 0x56, 0x92, 0x96, 0x9a, 0xcd, 0x4f, 0x83, 0x9f, 0xa2,
    0x6f, 0x8d, 0xd1, 0xa8, 0xee, 0xe5, 0xf6, 0xca, 0x04, 0xca, 0x30, 0x08,
    0x55, 0x94, 0x86, 0x57, 0xc3, 0x5d, 0x59, 0xa0, 0x9c, 0xb9, 0x8e, 0x69,
    0xe7, 0x68, 0xcd, 0xe7, 0x59, 0xcb, 0xdb, 0x48, 0x61, 0x00, 0x1c, 0x02,
    0x3a, 0x81, 0x9a, 0x02, 0x0c, 0xdb, 0xb6, 0xe9, 0xd4, 0x12, 0x33, 0x96,
    0x60, 0x47, 0x4f, 0x07, 0xd5, 0x9b, 0x43, 0xcc, 0x8e, 0x5e, 0xe6, 0x99,
    0x3d, 0x3d, 0x74, 0x6e, 0x6b, 0x47, 0x04, 0x0e, 0x81, 0x8c, 0x82, 0xd7,
    0x20, 0x11, 0x4f, 0x12, 0x89, 0xc7, 0xd0, 0x0f, 0xf4, 0xb2, 0xaf, 0xa9,
    0x99, 0xf6, 0xde, 0x0e, 0x82, 0x0b, 0x53, 0xcc, 0x8e, 0x2f, 0x6c, 0x4c,
    0xa6, 0x19, 0x88, 0x2a, 0x40, 0x00, 0x74, 0x24, 0x2d, 0xf6, 0x9b, 0x71,
    0x92, 0x31, 0x9f, 0x6b, 0xe3, 0x97, 0x29, 0xba, 0x9a, 0x9e, 0xee, 0x66,
    0xac, 0x64, 0x0b, 0x7e, 0xc5, 0x26, 0xb6, 0xcd, 0xc4, 0x88, 0x45, 0x58,
    0x9d, 0x9e, 0xa1, 0x98, 0x9e, 0x22, 0xde, 0x6e, 0xa1, 0xc3, 0x08, 0xb5,
    0x40, 0xb2, 0xea, 0x7a, 0x68, 0x1d, 0x6e, 0xc0, 0x0a, 0x30, 0x36, 0xe1,
    0x86, 0xe7, 0x53, 0x77, 0xa1, 0x56, 0xfe, 0x95, 0xe5, 0xf9, 0x49, 0x2c,
    0x09, 0x13, 0x39, 0x87, 0x73, 0x9f, 0x5f, 0x65, 0xb5, 0x54, 0xe1, 0xc8,
    0xa9, 0x3d, 0xdc, 0xd3, 0xdd, 0xc5, 0x87, 0xef, 0x0c, 0xd2, 0xdd, 0x12,
    0x25, 0xd2, 0xde, 0xc2, 0x48, 0x7a, 0x89, 0xd5, 0x5a, 0x83, 0x64, 0x53,
    0x8c, 0x98, 0x21, 0xa8, 0xde, 0x81, 0x25, 0x20, 0xe4, 0xc6, 0x31, 0x11,
    0x03, 0x1c, 0xbb, 0x44, 0x6a, 0x2a, 0x4b, 0xd1, 0xf6, 0x30, 0xa5, 0xa4,
    0x50, 0xae, 0x31, 0xbf, 0x50, 0x62, 0x7a, 0xd1, 0x21, 0x37, 0x9a, 0x63,
    0x66, 0x6a, 0x05, 0xcf, 0xa9, 0x73, 0xf2, 0xf4, 0x51, 0x6a, 0x9e, 0x4b,
    0x77, 0x8b, 0xc1, 0xeb, 0x6f, 0x1c, 0xe3, 0xf8, 0x63, 0xf7, 0xa1, 0xd8,
    0x4c, 0xbc, 0x19, 0x1b, 0x80, 0xd1, 0x4b, 0x83, 0x5c, 0x19, 0x3e, 0xc7,
    0xce, 0x87, 0xfa, 0x08, 0x35, 0x54, 0x7c, 0xcd, 0xab, 0xa7, 0x1e, 0xe1,
    0xe9, 0x13, 0xfb, 0x98, 0x4f, 0x2d, 0xb2, 0xfd, 0x70, 0x37, 0xa5, 0x45,
    0xcd, 0x53, 0xc7, 0x76, 0xd0, 0xf9, 0xa0, 0xc5, 0x4b, 0xaf, 0xf5, 0x61,
    0xb6, 0x25, 0xe9, 0xea, 0x68, 0xe6, 0xcb, 0xcc, 0x39, 0xbc, 0x80, 0xad,
    0xe1, 0x42, 0xe6, 0x2a, 0x76, 0x7e, 0x8e, 0x15, 0x27, 0x8b, 0xa8, 0x97,
    0xd1, 0x48, 0xf0, 0x1b, 0x6c, 0x3f, 0xb4, 0x87, 0x9e, 0xbe, 0xa3, 0x04,
    0xae, 0x8d, 0x51, 0x9f, 0x26, 0xde, 0xb7, 0x8b, 0xf6, 0xfd, 0x87, 0xe9,
    0x3a, 0xba, 0x0f, 0x77, 0x25, 0xcd, 0xdc, 0xd9, 0xef, 0x48, 0x65, 0x4a,
    0x34, 0xa4, 0xd8, 0x1a, 0x76, 0x6b, 0xb7, 0x09, 0x5d, 0x9b, 0x72, 0xde,
    0x26, 0x1e, 0x80, 0x42, 0x73, 0x33, 0xbd, 0x8c, 0x67, 0xd7, 0xa9, 0xd7,
    0x2b, 0x44, 0x63, 0x16, 0x2a, 0xd1, 0x49, 0xe0, 0x2c, 0x51, 0x2b, 0x56,
    0x31, 0xdd, 0x29, 0xdc, 0xe9, 0x2c, 0xe3, 0x3f, 0x2e, 0x33, 0x53, 0xb5,
    0x41, 0xdc, 0x65, 0x14, 0xa1, 0xd6, 0xf8, 0xee, 0x1a, 0xba, 0xe1, 0x82,
    0x06, 0x53, 0x4a, 0x46, 0xc7, 0x4b, 0x64, 0x86, 0x6f, 0xd2, 0xbe, 0xb7,
    0x8d, 0x82, 0x93, 0xe0, 0xfb, 0x0b, 0x33, 0x14, 0xa7, 0x6f, 0xf1, 0xd6,
    0xc9, 0x7e, 0x9c, 0x85, 0x39, 0xd2, 0xe7, 0xaf, 0x33, 0x36, 0x5d, 0xa0,
    0xe2, 0xfa, 0x18, 0xe2, 0xcf, 0x89, 0x37, 0x97, 0x67, 0x28, 0x85, 0x94,
    0x06, 0x52, 0x4a, 0x5c, 0x09, 0x48, 0x49, 0xb6, 0xd6, 0x60, 0x74, 0x68,
    0x9c, 0xb0, 0xb8, 0x8a, 0x8a, 0x1a, 0x0c, 0x0f, 0x5e, 0xc2, 0xf7, 0x04,
    0xd9, 0xf3, 0xa3, 0xe4, 0x47, 0x52, 0xd8, 0xcb, 0x36, 0xe2, 0x0f, 0x62,
    0x6b, 0x58, 0x87, 0x1a, 0xad, 0x43, 0x84, 0x52, 0x48, 0x24, 0x96, 0x54,
    0x54, 0xb4, 0xc6, 0x56, 0x06, 0xb3, 0xd7, 0xf3, 0xd4, 0x72, 0x05, 0x4e,
    0x9f, 0x78, 0x14, 0x5d, 0xf2, 0xf9, 0xe5, 0x72, 0x0a, 0xd9, 0x1c, 0x47,
    0x98, 0x51, 0x9c, 0x20, 0xa0, 0x11, 0x86, 0x08, 0xee, 0x92, 0x58, 0x22,
    0x51, 0xb1, 0x26, 0x22, 0x66, 0x1c, 0x53, 0x49, 0x7c, 0xa0, 0x0e, 0x0c,
    0xdd, 0xc8, 0x92, 0xe8, 0x6a, 0xa1, 0x6b, 0x57, 0x1b, 0x3a, 0xa6, 0xd9,
    0xbb, 0x3b, 0xc9, 0xce, 0xde, 0x4e, 0x7e, 0x9b, 0xb3, 0xc9, 0x4c, 0xe4,
    0x99, 0xaf, 0x38, 0xac, 0x69, 0x81, 0x10, 0xea, 0x2e, 0x70, 0xc4, 0xc4,
    0x6c, 0x6a, 0x27, 0x6a, 0x75, 0x00, 0x9a, 0x40, 0x6b, 0xda, 0x94, 0xe2,
    0x4a, 0xa1, 0xc8, 0x07, 0x9f, 0x5c, 0x00, 0x65, 0xf2, 0xec, 0xfb, 0xaf,
    0xf0, 0xf8, 0xf3, 0x0f, 0x53, 0x2b, 0x38, 0x0c, 0x5f, 0x9c, 0xe4, 0x87,
    0xa9, 0x1c, 0x65, 0xcf, 0x63, 0xcd, 0xf7, 0xd1, 0xa1, 0xbf, 0xf5, 0xf2,
    0x90, 0x06, 0x2a, 0xd1, 0x8a, 0xc0, 0x47, 0x97, 0xb2, 0x44, 0xa5, 0x26,
    0x40, 0x63, 0x29, 0x93, 0x6f, 0xe6, 0x8a, 0xbc, 0xfb, 0xde, 0x57, 0x3c,
    0xf7, 0xe4, 0x01, 0xe6, 0xc7, 0x97, 0x18, 0x19, 0xcb, 0x92, 0xb3, 0x1d,
    0x5c, 0x09, 0x8e, 0x30, 0xc0, 0x00, 0xe1, 0xf1, 0x37, 0x38, 0xac, 0x56,
    0xab, 0x44, 0xac, 0x5d, 0x58, 0x5d, 0x49, 0xd0, 0x2e, 0xf5, 0x85, 0x34,
    0xbe, 0xbb, 0x8a, 0x87, 0xc4, 0x47, 0x63, 0x2a, 0x93, 0x2f, 0x46, 0x26,
    0x19, 0x1a, 0x99, 0xc4, 0xc2, 0x24, 0x88, 0x99, 0x28, 0x05, 0x52, 0x18,
    0x58, 0x31, 0x49, 0xab, 0x32, 0x50, 0x04, 0x94, 0x9d, 0x3b, 0xeb, 0x02,
    0x42, 0x05, 0x84, 0xa9, 0x54, 0xea, 0x5a, 0xd0, 0x21, 0x8e, 0x20, 0x42,
    0x42, 0xdf, 0x43, 0xc7, 0x2d, 0xbc, 0x5a, 0x85, 0x88, 0xa1, 0x08, 0x08,
    0xd1, 0x1a, 0xc0, 0x60, 0xc7, 0xf6, 0x56, 0x0e, 0x74, 0xb7, 0x91, 0x5e,
    0xaa, 0x60, 0xaf, 0xb9, 0x34, 0x47, 0x15, 0x1d, 0x4d, 0x11, 0x12, 0xa6,
    0x62, 0x65, 0xad, 0x06, 0x90, 0x01, 0x7c, 0x20, 0x00, 0x88, 0x01, 0x6f,
    0x02, 0x3f, 0x03, 0xe1, 0xff, 0xe8, 0x09, 0xe0, 0x23, 0xa0, 0x0f, 0xe8,
    0x15, 0x40, 0x04, 0xb0, 0x80, 0x6d, 0xdc, 0xf9, 0xa4, 0x9b, 0xd7, 0x47,
    0xb4, 0xf5, 0x03, 0xdd, 0xba, 0xf4, 0x7a, 0x52, 0x1b, 0x28, 0x01, 0x45,
    0xb1, 0x0e, 0x44, 0x80, 0xc4, 0x7a, 0x47, 0x01, 0x03, 0xfe, 0xf2, 0x30,
    0xff, 0xb9, 0xc2, 0xf5, 0xeb, 0x37, 0x00, 0x07, 0x70, 0x44, 0x18, 0x86,
    0x08, 0x21, 0xe4, 0x3a, 0xb6, 0xd1, 0xe2, 0x3f, 0xc0, 0x1b, 0x78, 0x00,
    0x04, 0xbf, 0x03, 0x1e, 0x77, 0x0d, 0xd9, 0x5f, 0x62, 0x24, 0x6b, 0x00,
    0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};


/*
 *  Constructs a SaveImageSettingsDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
SaveImageSettingsDialog::SaveImageSettingsDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    QImage img;
    img.loadFromData( image0_data, sizeof( image0_data ), "PNG" );
    image0 = img;
    if ( !name )
	setName( "SaveImageSettingsDialog" );
    setIcon( image0 );
    SaveImageSettingsDialogLayout = new QHBoxLayout( this, 11, 6, "SaveImageSettingsDialogLayout"); 

    layout5 = new QVBoxLayout( 0, 0, 6, "layout5"); 

    PreviewCaptionLabel = new QLabel( this, "PreviewCaptionLabel" );
    layout5->addWidget( PreviewCaptionLabel );

    PreviewLabel = new QLabel( this, "PreviewLabel" );
    PreviewLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, PreviewLabel->sizePolicy().hasHeightForWidth() ) );
    PreviewLabel->setAlignment( int( QLabel::AlignCenter ) );
    layout5->addWidget( PreviewLabel );
    SaveImageSettingsDialogLayout->addLayout( layout5 );

    layout15 = new QVBoxLayout( 0, 0, 6, "layout15"); 

    SizeGroup = new QButtonGroup( this, "SizeGroup" );
    SizeGroup->setColumnLayout(0, Qt::Vertical );
    SizeGroup->layout()->setSpacing( 6 );
    SizeGroup->layout()->setMargin( 11 );
    SizeGroupLayout = new QVBoxLayout( SizeGroup->layout() );
    SizeGroupLayout->setAlignment( Qt::AlignTop );

    PixelsRadioButton = new QRadioButton( SizeGroup, "PixelsRadioButton" );
    PixelsRadioButton->setChecked( TRUE );
    SizeGroupLayout->addWidget( PixelsRadioButton );

    layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 
    spacer1 = new QSpacerItem( 41, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout1->addItem( spacer1 );

    PixelsWideSpinBox = new QSpinBox( SizeGroup, "PixelsWideSpinBox" );
    PixelsWideSpinBox->setMaxValue( 100000 );
    layout1->addWidget( PixelsWideSpinBox );

    PixelsWideLabel = new QLabel( SizeGroup, "PixelsWideLabel" );
    layout1->addWidget( PixelsWideLabel );

    PixelsHighSpinBox = new QSpinBox( SizeGroup, "PixelsHighSpinBox" );
    PixelsHighSpinBox->setMaxValue( 100000 );
    layout1->addWidget( PixelsHighSpinBox );

    PixelsHighLabel = new QLabel( SizeGroup, "PixelsHighLabel" );
    layout1->addWidget( PixelsHighLabel );
    SizeGroupLayout->addLayout( layout1 );

    PercentRadioButton = new QRadioButton( SizeGroup, "PercentRadioButton" );
    SizeGroupLayout->addWidget( PercentRadioButton );

    layout2 = new QHBoxLayout( 0, 0, 6, "layout2"); 
    spacer2 = new QSpacerItem( 51, 31, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout2->addItem( spacer2 );

    PercentWideSpinBox = new QSpinBox( SizeGroup, "PercentWideSpinBox" );
    PercentWideSpinBox->setEnabled( FALSE );
    PercentWideSpinBox->setMaxValue( 100000 );
    layout2->addWidget( PercentWideSpinBox );

    PercentWideLabel = new QLabel( SizeGroup, "PercentWideLabel" );
    PercentWideLabel->setEnabled( FALSE );
    layout2->addWidget( PercentWideLabel );

    PercentHighSpinBox = new QSpinBox( SizeGroup, "PercentHighSpinBox" );
    PercentHighSpinBox->setEnabled( FALSE );
    PercentHighSpinBox->setMaxValue( 100000 );
    layout2->addWidget( PercentHighSpinBox );

    PercentHighLabel = new QLabel( SizeGroup, "PercentHighLabel" );
    PercentHighLabel->setEnabled( FALSE );
    layout2->addWidget( PercentHighLabel );
    SizeGroupLayout->addLayout( layout2 );

    AspectRatioCheckBox = new QCheckBox( SizeGroup, "AspectRatioCheckBox" );
    AspectRatioCheckBox->setChecked( TRUE );
    SizeGroupLayout->addWidget( AspectRatioCheckBox );

    line1 = new QFrame( SizeGroup, "line1" );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setFrameShape( QFrame::HLine );
    SizeGroupLayout->addWidget( line1 );

    QualitySlider = new QSlider( SizeGroup, "QualitySlider" );
    QualitySlider->setMaxValue( 100 );
    QualitySlider->setValue( 50 );
    QualitySlider->setOrientation( QSlider::Horizontal );
    QualitySlider->setTickmarks( QSlider::Below );
    QualitySlider->setTickInterval( 10 );
    SizeGroupLayout->addWidget( QualitySlider );

    layout6 = new QHBoxLayout( 0, 0, 6, "layout6"); 

    LowQualityLabel = new QLabel( SizeGroup, "LowQualityLabel" );
    layout6->addWidget( LowQualityLabel );

    HighQualityLabel = new QLabel( SizeGroup, "HighQualityLabel" );
    layout6->addWidget( HighQualityLabel );
    SizeGroupLayout->addLayout( layout6 );
    layout15->addWidget( SizeGroup );

    layout14 = new QHBoxLayout( 0, 0, 6, "layout14"); 
    spacer5 = new QSpacerItem( 51, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout14->addItem( spacer5 );

    CancelButton = new QPushButton( this, "CancelButton" );
    layout14->addWidget( CancelButton );

    OkButton = new QPushButton( this, "OkButton" );
    OkButton->setDefault( TRUE );
    OkButton->setFlat( FALSE );
    layout14->addWidget( OkButton );
    layout15->addLayout( layout14 );
    SaveImageSettingsDialogLayout->addLayout( layout15 );
    languageChange();
    resize( QSize(598, 346).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( OkButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( CancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( AspectRatioCheckBox, SIGNAL( toggled(bool) ), this, SLOT( AspectRatioCheckBox_toggled(bool) ) );
    connect( PixelsRadioButton, SIGNAL( toggled(bool) ), this, SLOT( PixelsRadioButton_toggled(bool) ) );
    connect( PercentRadioButton, SIGNAL( toggled(bool) ), this, SLOT( PercentRadioButton_toggled(bool) ) );
    connect( PixelsWideSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( PixelsWideSpinBox_valueChanged(int) ) );
    connect( PixelsHighSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( PixelsHighSpinBox_valueChanged(int) ) );
    connect( PercentWideSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( PercentWideSpinBox_valueChanged(int) ) );
    connect( PercentHighSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( PercentHighSpinBox_valueChanged(int) ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
SaveImageSettingsDialog::~SaveImageSettingsDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void SaveImageSettingsDialog::languageChange()
{
    setCaption( tr( "Save Image - Settings" ) );
    PreviewCaptionLabel->setText( tr( "Preview:" ) );
    PreviewLabel->setText( tr( "No Preview" ) );
    SizeGroup->setTitle( tr( "Image Size" ) );
    PixelsRadioButton->setText( tr( "Use pixels:" ) );
    PixelsWideLabel->setText( tr( "width" ) );
    PixelsHighLabel->setText( tr( "height" ) );
    PercentRadioButton->setText( tr( "Use percent (of original size):" ) );
    PercentWideLabel->setText( tr( "width" ) );
    PercentHighLabel->setText( tr( "height" ) );
    AspectRatioCheckBox->setText( tr( "Keep aspect ratio (width/height)" ) );
    LowQualityLabel->setText( tr( "<p align=\"left\">Small file,<br>low quality</p>" ) );
    HighQualityLabel->setText( tr( "<p align=\"right\">Large file,<br>high quality</p>" ) );
    CancelButton->setText( tr( "Cancel" ) );
    OkButton->setText( tr( "OK" ) );
}

