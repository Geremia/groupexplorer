//Added by me to ease porting
#include <QFileDialog>
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
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/


void SaveImageSettingsDialog::init()
{
    signalsLive = TRUE;
}


int SaveImageSettingsDialog::exec( QPixmap p, QString savePath )
{
    originalWidth = p.width();
    originalHeight = p.height();
    PercentRadioButton->setChecked( TRUE );
    PercentWideSpinBox->setValue( lastPercentWidth = 100 );
    PercentHighSpinBox->setValue( lastPercentHeight = 100 );
    updateOtherNumbers();
    PixelsRadioButton->setChecked( TRUE );
    PixelsRadioButton_toggled( TRUE );
    AspectRatioCheckBox->setChecked( TRUE );
    
    if ( !originalWidth && !originalHeight ) {
        qWarning( "SaveImageSettingsDialog::exec(): ERROR: 0x0 pixmap" );
        return -1;
    }
    
    QWMatrix shrink;
    double scale = 200.0 / double( QMAX( originalWidth, originalHeight ) );
    shrink.scale( scale, scale );
    QPixmap resizedCopy = p.xForm( shrink );
    PreviewLabel->setPixmap( resizedCopy );
    
    // so exec() in next line won't kill us early
    //clearWFlags( WDestructiveClose );
    setAttribute( Qt::WA_DeleteOnClose, FALSE );
    if ( QDialog::exec() == QDialog::Accepted ) {
        QString filename = QFileDialog::getSaveFileName(
            savePath, "Portable Network Graphics (*.png)", parentWidget(),
            "save file dialog", "Choose a filename to save under" );
        if ( !filename.isEmpty() ) {
            QWMatrix userScale;
            userScale.scale( double( PixelsWideSpinBox->value() )
                             / double( originalWidth ),
                             double( PixelsHighSpinBox->value() )
                             / double( originalHeight ) );
            resizedCopy = p.xForm( userScale );
            // the save could take awhile, so we show a window:
            QMessageBox M( "Saving file",
                           "Please wait while file is saved.",
                           QMessageBox::NoIcon,
                           QMessageBox::Ok,
                           QMessageBox::NoButton,
                           QMessageBox::NoButton,
                           this );
            M.setButtonText( QMessageBox::Ok, "Dismiss" );
            M.show(); // it will be destroyed when this function returns, after save
            if ( filename.right( 4 ).lower() != ".png" ) filename += ".png";
            resizedCopy.save( filename, "PNG", QualitySlider->value() );
            return 2; // success -- both settings OKed, and file saved
        }
        return 1; // settings dialog accepted, but file not saved
    }
    return 0;
}


void SaveImageSettingsDialog::AspectRatioCheckBox_toggled( bool )
{
    correctAspectRatio( TRUE );
}


void SaveImageSettingsDialog::updateOtherNumbers()
{
    if ( PixelsRadioButton->isChecked() ) {
        if ( !originalWidth || !originalHeight ) return;
        PercentWideSpinBox->setValue(
            lastPercentWidth = 100 * lastPixelWidth / originalWidth );
        PercentHighSpinBox->setValue(
            lastPercentHeight = 100 * lastPixelHeight / originalHeight );
    } else {
        PixelsWideSpinBox->setValue(
            lastPixelWidth = lastPercentWidth * originalWidth / 100 );
        PixelsHighSpinBox->setValue(
            lastPixelHeight = lastPercentHeight * originalHeight / 100 );
    }
}


void SaveImageSettingsDialog::PixelsRadioButton_toggled( bool on )
{
    PixelsWideSpinBox->setEnabled( on );
    PixelsWideLabel->setEnabled( on );
    PixelsHighSpinBox->setEnabled( on );
    PixelsHighLabel->setEnabled( on );
    PercentWideSpinBox->setEnabled( !on );
    PercentWideLabel->setEnabled( !on );
    PercentHighSpinBox->setEnabled( !on );
    PercentHighLabel->setEnabled( !on );
}


void SaveImageSettingsDialog::PercentRadioButton_toggled( bool on )
{
    PercentWideSpinBox->setEnabled( on );
    PercentWideLabel->setEnabled( on );
    PercentHighSpinBox->setEnabled( on );
    PercentHighLabel->setEnabled( on );
    PixelsWideSpinBox->setEnabled( !on );
    PixelsWideLabel->setEnabled( !on );
    PixelsHighSpinBox->setEnabled( !on );
    PixelsHighLabel->setEnabled( !on );
}


void SaveImageSettingsDialog::correctAspectRatio( bool baseOnX )
{
    if ( signalsLive ) {
        signalsLive = FALSE;
        if ( PixelsRadioButton->isChecked() ) {
            if ( baseOnX ) {
                if ( !originalWidth ) return;
                PixelsHighSpinBox->setValue( lastPixelHeight =
                    PixelsWideSpinBox->value() * originalHeight / originalWidth );
                lastPixelWidth = PixelsWideSpinBox->value();
            } else {
                if ( !originalHeight ) return;
                PixelsWideSpinBox->setValue( lastPixelWidth =
                    PixelsHighSpinBox->value() * originalWidth / originalHeight );
                lastPixelHeight = PixelsHighSpinBox->value();
            }
        } else {
            if ( baseOnX ) {
                PercentHighSpinBox->setValue(
                    lastPercentHeight = PercentWideSpinBox->value() );
                lastPercentWidth = PercentWideSpinBox->value();
            } else {
                PercentWideSpinBox->setValue(
                    lastPercentWidth = PercentHighSpinBox->value() );
                lastPercentHeight = PercentHighSpinBox->value();
            }
        }
        updateOtherNumbers();
        signalsLive = TRUE;
    }
}



void SaveImageSettingsDialog::PixelsWideSpinBox_valueChanged( int )
{
    if ( AspectRatioCheckBox->isChecked() ) correctAspectRatio( TRUE );
}


void SaveImageSettingsDialog::PixelsHighSpinBox_valueChanged( int )
{
    if ( AspectRatioCheckBox->isChecked() ) correctAspectRatio( FALSE );
}


void SaveImageSettingsDialog::PercentWideSpinBox_valueChanged( int )
{
    if ( AspectRatioCheckBox->isChecked() ) correctAspectRatio( TRUE );
}


void SaveImageSettingsDialog::PercentHighSpinBox_valueChanged( int )
{
    if ( AspectRatioCheckBox->isChecked() ) correctAspectRatio( FALSE );
}


