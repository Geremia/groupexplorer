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


void PrintImageSettingsDialog::init()
{
    signalsLive = TRUE;
    previewLive = FALSE;
    printer.setFullPage( TRUE );
}


int PrintImageSettingsDialog::exec( QPixmap p )
{
    if ( !p.width() || !p.height() ) {
        qWarning( "PrintImageSettingsDialog::exec(): ERROR: 0x0 pixmap" );
        return QDialog::Rejected;
    }

    QWMatrix shrink;
    double scale = 50.0 / double( QMAX( originalWidth = p.width(),
                                        originalHeight = p.height() ) );
    shrink.scale( scale, scale );
    littleCopy = p.xForm( shrink );
    
    getPrinterData();

    signalsLive = FALSE;
    PixelsRadioButton->setChecked( TRUE );
    PixelsRadioButton_toggled( TRUE );
    PixelsWideSpinBox->setValue( QMIN( originalWidth, pageWidth - 2 * dpiX ) );
    PixelsHighSpinBox->setValue( QMIN( originalHeight, pageHeight - 2 * dpiY ) );
    signalsLive = TRUE;
    AspectRatioCheckBox->setChecked( TRUE );
    LeftMarginEdit->setText( "1.00" );
    TopMarginEdit->setText( "1.00" );
    updateOtherNumbers();

    // so QDialog::exec() below won't kill us early
    //clearWFlags( WDestructiveClose );
    setAttribute( Qt::WA_DeleteOnClose, FALSE );

    previewLive = TRUE;
    updatePreview();
    int tryDialog = QDialog::exec();
    previewLive = FALSE;
    
    if ( tryDialog == QDialog::Accepted ) {
        for ( int i = 0 ; i < (int)printer.numCopies() ; i++ ) { 
            if ( i ) printer.newPage();
            
            QPainter painter( &printer );
            int left = int( LeftMarginEdit->text().toDouble() * dpiX );
            int top = int( TopMarginEdit->text().toDouble() * dpiY );
            int width = PixelsWideSpinBox->value();
            int height = PixelsHighSpinBox->value();
            QWMatrix resizer;
            resizer.scale( double( width ) / double( originalWidth ),
                           double( height ) / double( originalHeight ) );
            QPixmap rescaled = p.xForm( resizer );
            painter.drawPixmap( left, top, rescaled );
        }
    }
    
    return tryDialog;
}


void PrintImageSettingsDialog::AspectRatioCheckBox_toggled( bool /*on*/ )
{
    correctAspectRatio( TRUE );
}


void PrintImageSettingsDialog::updateOtherNumbers()
{
    if ( PixelsRadioButton->isChecked() ) {
        if ( !dpiX || !dpiY ) {
            qWarning( QString( "PrintImageSettingsDialog::updateOtherNumbers() "
                               "with dpiX x dpiY == %1 x %2" )
                      .arg( dpiX ).arg( dpiY ) );
            return;
        }
        InchesWideEdit->setText( inchesText(
            ( 100 * PixelsWideSpinBox->value() / dpiX ) / 100.0 ) );
        InchesHighEdit->setText( inchesText(
            ( 100 * PixelsHighSpinBox->value() / dpiY ) / 100.0 ) );
    } else {
        PixelsWideSpinBox->setValue(
            int( InchesWideEdit->text().toDouble() * dpiX ) );
        PixelsHighSpinBox->setValue(
            int( InchesHighEdit->text().toDouble() * dpiY ) );
    }
    updatePreview();
}


void PrintImageSettingsDialog::PixelsRadioButton_toggled( bool on )
{
    PixelsWideSpinBox->setEnabled( on );
    PixelsWideLabel->setEnabled( on );
    PixelsHighSpinBox->setEnabled( on );
    PixelsHighLabel->setEnabled( on );
    InchesWideEdit->setEnabled( !on );
    InchesWideLabel->setEnabled( !on );
    InchesHighEdit->setEnabled( !on );
    InchesHighLabel->setEnabled( !on );
}


void PrintImageSettingsDialog::InchesRadioButton_toggled( bool on )
{
    InchesWideEdit->setEnabled( on );
    InchesWideLabel->setEnabled( on );
    InchesHighEdit->setEnabled( on );
    InchesHighLabel->setEnabled( on );
    PixelsWideSpinBox->setEnabled( !on );
    PixelsWideLabel->setEnabled( !on );
    PixelsHighSpinBox->setEnabled( !on );
    PixelsHighLabel->setEnabled( !on );
}


void PrintImageSettingsDialog::correctAspectRatio( bool baseOnX )
{
    if ( signalsLive ) { // prevent recursion
        signalsLive = FALSE;
        if ( AspectRatioCheckBox->isChecked() ) {
            if ( PixelsRadioButton->isChecked() ) {
                if ( baseOnX ) {
                    if ( !originalWidth ) return;
                    PixelsHighSpinBox->setValue(
                        PixelsWideSpinBox->value() * originalHeight / originalWidth );
                } else {
                    if ( !originalHeight ) return;
                    PixelsWideSpinBox->setValue(
                        PixelsWideSpinBox->value() * originalWidth / originalHeight );
                }
            } else {
                if ( baseOnX ) {
                    if ( !originalHeight ) return;
                    InchesHighEdit->setText( inchesText(
                        InchesWideEdit->text().toDouble()
                        * originalWidth / originalHeight ) );
                } else {
                    if ( !originalWidth ) return;
                    InchesWideEdit->setText( inchesText(
                        InchesWideEdit->text().toDouble()
                        * originalHeight / originalWidth ) );
                }
            }
        }
        updateOtherNumbers();
        signalsLive = TRUE;
    }
}


void PrintImageSettingsDialog::PixelsWideSpinBox_valueChanged( int )
{
    correctAspectRatio( TRUE );
}


void PrintImageSettingsDialog::PixelsHighSpinBox_valueChanged( int )
{
    correctAspectRatio( FALSE );
}


void PrintImageSettingsDialog::PrinterSetupButton_clicked()
{
    if ( printer.setup() ) getPrinterData();
}


void PrintImageSettingsDialog::updatePreview()
{
    if ( previewLive ) {
        if ( !pageWidth || !pageHeight ) {
            qWarning( QString( "PrintImageSettingsDialog::updatePreview() "
                               "with page size %1 x %2" )
                      .arg( pageWidth ).arg( pageHeight ) );
            return;
        }

        QSize paperSize( pageWidth, pageHeight );
        paperSize.scale( 200, 200, Qt::KeepAspectRatio );
        double scale = double( paperSize.width() ) / double( pageWidth );
        
        if ( paperSize.width() && paperSize.height() ) {
            if ( !littleCopy.width() || !littleCopy.height() ) {
                qWarning( QString( "PrintImageSettingsDialog::updatePreview() "
                                   "with littleCopy dimensions %1 x %2" )
                          .arg( littleCopy.width() ).arg( littleCopy.height() ) );
            }

            QPixmap previewPixmap( paperSize.width(), paperSize.height() );
            QPainter p( &previewPixmap );
            
            p.setPen( Qt::NoPen );
            p.setBrush( Qt::white );
            p.drawRect( 0, 0, paperSize.width(), paperSize.height() );
            
            int left = int( LeftMarginEdit->text().toDouble() * dpiX * scale );
            int top = int( TopMarginEdit->text().toDouble() * dpiY * scale );
            int width = PixelsWideSpinBox->value();
            int height = PixelsHighSpinBox->value();
            QWMatrix resizer;
            resizer.scale( width * scale / littleCopy.width(),
                           height * scale / littleCopy.height() );
            QPixmap rescaled = littleCopy.xForm( resizer );
            p.drawPixmap( left, top, rescaled );
            
            PreviewLabel->setPixmap( previewPixmap );
        } else {
            PreviewLabel->setText( "No Preview" );
        }
    }
}


void PrintImageSettingsDialog::getPrinterData()
{
    dpiX = printer.logicalDpiX();
    dpiY = printer.logicalDpiY();
    pageWidth = printer.width();
    pageHeight = printer.height();
    updatePreview();
}


void PrintImageSettingsDialog::InchesWideEdit_textChanged( const QString & )
{
    correctAspectRatio( TRUE );
}


void PrintImageSettingsDialog::InchesHighEdit_textChanged( const QString & )
{
    correctAspectRatio( FALSE );
}


QString PrintImageSettingsDialog::inchesText( double inches )
{
    QString result;
    result = result.sprintf( "%0.2f", inches );
    while ( result.length() < 5 ) result = "0" + result;
    return result;
}


void PrintImageSettingsDialog::OkButton_clicked()
{
    QString problem;
    if ( TopMarginEdit->text().toDouble() < 1 )
        problem += "The top margin is smaller than one inch.\n";
    if ( LeftMarginEdit->text().toDouble() < 1 )
        problem += "The left margin is smaller than one inch.\n";
    if ( !problem.isEmpty() ) {
        int result = QMessageBox::warning(
                this, "Margin warning",
                problem +
                "These margins may be too small for your printer.\n"
                "Do you wish to try printing with these margins anyway?",
                QMessageBox::Ok, QMessageBox::Cancel );
        if ( result == QMessageBox::Ok ) accept();
    } else
        accept();
}
