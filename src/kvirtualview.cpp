/***************************************************************************
 *   Copyright (C) 2012 by Didier Fabert <didier.fabert@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "kvirtualview.h"
#include "kvirtualoptions.h"
#include "settings.h"

#include <KLocale>
#include <KLineEdit>
#include <KLed>
#include <KStandardDirs>

#include <QPixmap>

#include <sys/sysinfo.h>

KVirtualView::KVirtualView( QWidget * )
{

	m_options = 0;
    _ui_kvirtualview_base.setupUi( this );
    settingsChanged();
    setAutoFillBackground( true );
	setPixmap( "linux" );
    connect( _ui_kvirtualview_base.comboBox_distrib,
             SIGNAL( currentIndexChanged ( const QString & ) ),
             SLOT( setPixmap( const QString & ) )
           );
}

KVirtualView::~KVirtualView()
{
}

void KVirtualView::settingsChanged()
{
    emit signalChangeStatusbar( i18n( "Settings changed" ) );
}

void KVirtualView::setPixmap( const QString & distrib )
{
	QPixmap pixmap;
	
	QString buffer = KStandardDirs::locate( "appdata", distrib + ".png" );

	if ( buffer.isNull() ) buffer = "linux";
	pixmap.load( buffer );
	_ui_kvirtualview_base.label_logo->setPixmap( pixmap );
}

void KVirtualView::setState( uint, bool state )
{
	if ( state )
		_ui_kvirtualview_base.kled_state->setState( KLed::On );
	else
		_ui_kvirtualview_base.kled_state->setState( KLed::Off );
}

void KVirtualView::initOptions( KVirtualOptions* opts )
{
    struct sysinfo data;
    int ram, cpus;

    m_options = opts;
    sysinfo( &data );
    ram = data.totalram / 1024 /1024;
	cpus = sysconf( _SC_NPROCESSORS_ONLN );
	if ( cpus <= 1 )
	{
		cpus = 1;
		_ui_kvirtualview_base.kintspinbox_nb_of_cpu->setEnabled( false );
		_ui_kvirtualview_base.horizontalSlider_nb_of_cpu->setEnabled( false );
	}
    _ui_kvirtualview_base.horizontalSlider_memory->setRange( 0, (int) ram );
    _ui_kvirtualview_base.kintspinbox_memory->setRange( 0, (int) ram );
	_ui_kvirtualview_base.kintspinbox_nb_of_cpu->setRange( 1, (int) cpus );
	_ui_kvirtualview_base.horizontalSlider_nb_of_cpu->setRange( 1, (int) cpus );
    connect( _ui_kvirtualview_base.horizontalSlider_memory,
             SIGNAL( valueChanged ( int ) ),
             _ui_kvirtualview_base.kintspinbox_memory,
             SLOT( setValue( int ) )
           );
    connect( _ui_kvirtualview_base.kintspinbox_memory,
             SIGNAL( valueChanged ( int ) ),
             _ui_kvirtualview_base.horizontalSlider_memory,
             SLOT( setValue( int ) )
           );
    connect( _ui_kvirtualview_base.horizontalSlider_nb_of_cpu,
             SIGNAL( valueChanged ( int ) ),
             _ui_kvirtualview_base.kintspinbox_nb_of_cpu,
             SLOT( setValue( int ) )
           );
    connect( _ui_kvirtualview_base.kintspinbox_nb_of_cpu,
             SIGNAL( valueChanged ( int ) ),
             _ui_kvirtualview_base.horizontalSlider_nb_of_cpu,
             SLOT( setValue( int ) )
           );
}

void KVirtualView::loadOptions()
{
    KVirtualOptions::BootOrder boot;
    KVirtualOptions::Display display;
    KVirtualDevice* device;
    int index;

	if ( ! m_options ) return;
	
    _ui_kvirtualview_base.lineEdit_name->setText( m_options->getName() );
    _ui_kvirtualview_base.textEdit_description->setHtml( m_options->getDescription() );
    _ui_kvirtualview_base.kintspinbox_memory->setValue( (int) m_options->getMemory() );
    _ui_kvirtualview_base.kintspinbox_nb_of_cpu->setValue( (int) m_options->getNbCPU() );
    _ui_kvirtualview_base.spinBox_vncport->setValue( (int) m_options->getVncPort() );
    _ui_kvirtualview_base.checkBox_usb->setChecked( m_options->isUsbSupported() );
    _ui_kvirtualview_base.checkBox_snapshot->setChecked( m_options->isSnapshotEnabled() );

    index = _ui_kvirtualview_base.comboBox_distrib->findText( m_options->getDistrib() );
    if ( index >= 0 )
        _ui_kvirtualview_base.comboBox_distrib->setCurrentIndex( index );
    index = _ui_kvirtualview_base.comboBox_display_keyboard->findText( m_options->getKeyboard() );
    if ( index >= 0 )
        _ui_kvirtualview_base.comboBox_display_keyboard->setCurrentIndex( index );
    index = _ui_kvirtualview_base.comboBox_video_card->findText( m_options->getVideoCard() );
    if ( index >= 0 )
        _ui_kvirtualview_base.comboBox_video_card->setCurrentIndex( index );

    display = m_options->getDisplay();
    switch ( display )
    {
    case KVirtualOptions::DISPLAY_DIRECT:
    {
        _ui_kvirtualview_base.radioButton_none->setChecked( false );
        _ui_kvirtualview_base.radioButton_direct->setChecked( true );
        _ui_kvirtualview_base.radioButton_vnc->setChecked( false );
        break;
    }
    case KVirtualOptions::DISPLAY_VNC:
    {
        _ui_kvirtualview_base.radioButton_none->setChecked( false );
        _ui_kvirtualview_base.radioButton_direct->setChecked( false );
        _ui_kvirtualview_base.radioButton_vnc->setChecked( true );
        break;
    }
    case KVirtualOptions::DISPLAY_NONE:
    {
        _ui_kvirtualview_base.radioButton_none->setChecked( true );
        _ui_kvirtualview_base.radioButton_direct->setChecked( false );
        _ui_kvirtualview_base.radioButton_vnc->setChecked( false );
        break;
    }
    }
    boot = m_options->getBootDevice();
    switch ( boot )
    {
    case KVirtualOptions::BOOT_ON_DISK:
    {
        index = _ui_kvirtualview_base.comboBox_boot->findText( "disk" );
        break;
    }
    case KVirtualOptions::BOOT_ON_NETWORK:
    {
        index = _ui_kvirtualview_base.comboBox_boot->findText( "network" );
        break;
    }
    case KVirtualOptions::BOOT_ON_CDROM:
    {
        index = _ui_kvirtualview_base.comboBox_boot->findText( "cdrom" );
        break;
    }
    case KVirtualOptions::BOOT_ON_FLOPPY:
    {
        index = _ui_kvirtualview_base.comboBox_boot->findText( "floppy" );
        break;
    }
    }
    if ( index >= 0 )
        _ui_kvirtualview_base.comboBox_boot->setCurrentIndex( index );

    device = m_options->getStorage( 0 );
	if ( device )
	{
    _ui_kvirtualview_base.kurlrequester_storage_1->lineEdit()->setSqueezedTextEnabled( true );
    _ui_kvirtualview_base.kurlrequester_storage_1->lineEdit()->setText( device->getFile() );
    index = _ui_kvirtualview_base.comboBox_storage_1->findText( device->getType() );
    if ( index >= 0 )
        _ui_kvirtualview_base.comboBox_storage_1->setCurrentIndex( index );
	}

    device = m_options->getStorage( 1 );
	if ( device )
	{
    _ui_kvirtualview_base.kurlrequester_storage_2->lineEdit()->setSqueezedTextEnabled( true );
    _ui_kvirtualview_base.kurlrequester_storage_2->lineEdit()->setText( device->getFile() );
    index = _ui_kvirtualview_base.comboBox_storage_2->findText( device->getType() );
    if ( index >= 0 )
        _ui_kvirtualview_base.comboBox_storage_2->setCurrentIndex( index );
	}

    device = m_options->getIface( 0 );
	if ( device )
	{
    index= _ui_kvirtualview_base.comboBox_iface_model_1->findText( device->getModel() );
    if ( index >= 0 )
        _ui_kvirtualview_base.comboBox_iface_model_1->setCurrentIndex( index );
    index = _ui_kvirtualview_base.comboBox_iface_type_1->findText( device->getType() );
    if ( index >= 0 )
        _ui_kvirtualview_base.comboBox_iface_type_1->setCurrentIndex( index );
    _ui_kvirtualview_base.lineEdit_iface_detail_1->setText( device->getFile() );
    _ui_kvirtualview_base.lineEdit_iface_mac_1->setText( device->getHardwareAddress() );
	}
	
    device = m_options->getIface( 1 );
	if ( device )
	{
    index= _ui_kvirtualview_base.comboBox_iface_model_2->findText( device->getModel() );
    if ( index >= 0 )
        _ui_kvirtualview_base.comboBox_iface_model_2->setCurrentIndex( index );
    index = _ui_kvirtualview_base.comboBox_iface_type_2->findText( device->getType() );
    if ( index >= 0 )
        _ui_kvirtualview_base.comboBox_iface_type_2->setCurrentIndex( index );
    _ui_kvirtualview_base.lineEdit_iface_detail_2->setText( device->getFile() );
    _ui_kvirtualview_base.lineEdit_iface_mac_2->setText( device->getHardwareAddress() );
	}
}

void KVirtualView::setOptions()
{
    QString buffer;
    KVirtualOptions::BootOrder boot;
    KVirtualOptions::Display display;

    if ( not m_options )
    {
        return;
    }
    m_options->setName( _ui_kvirtualview_base.lineEdit_name->text() );
    m_options->setDistrib( _ui_kvirtualview_base.comboBox_distrib->currentText() );
    m_options->setDescription( _ui_kvirtualview_base.textEdit_description->toHtml() );
    m_options->setMemory( (uint) _ui_kvirtualview_base.kintspinbox_memory->value() );
    m_options->setNbCPU( (uint) _ui_kvirtualview_base.kintspinbox_nb_of_cpu->value() );
    m_options->setUsbSupported( _ui_kvirtualview_base.checkBox_usb->isChecked() );
	m_options->setSnapshotEnabled( _ui_kvirtualview_base.checkBox_snapshot->isChecked() );
    m_options->setVncPort( (uint) _ui_kvirtualview_base.spinBox_vncport->value() );
    m_options->setKeyboard( _ui_kvirtualview_base.comboBox_display_keyboard->currentText() );
    m_options->setVideoCard( _ui_kvirtualview_base.comboBox_video_card->currentText() );
    buffer = _ui_kvirtualview_base.comboBox_boot->currentText();
    if ( buffer == "disk" )
        boot = KVirtualOptions::BOOT_ON_DISK;
    else if ( buffer == "network" )
        boot = KVirtualOptions::BOOT_ON_NETWORK;
    else if ( buffer == "cdrom" )
        boot = KVirtualOptions::BOOT_ON_CDROM;
    else
        boot = KVirtualOptions::BOOT_ON_FLOPPY;
    m_options->setBootDevice( boot );
    if ( _ui_kvirtualview_base.radioButton_direct->isChecked() )
        display = KVirtualOptions::DISPLAY_DIRECT;
    else if ( _ui_kvirtualview_base.radioButton_vnc->isChecked() )
        display = KVirtualOptions::DISPLAY_VNC;
    else
        display = KVirtualOptions::DISPLAY_NONE;
    m_options->setDisplay( display );

    m_options->setIface( 0,
                         _ui_kvirtualview_base.comboBox_iface_type_1->currentText(),
                         _ui_kvirtualview_base.lineEdit_iface_detail_1->text(),
                         _ui_kvirtualview_base.comboBox_iface_model_1->currentText(),
                         _ui_kvirtualview_base.lineEdit_iface_mac_1->text()
                       );
    m_options->setIface( 1,
                         _ui_kvirtualview_base.comboBox_iface_type_2->currentText(),
                         _ui_kvirtualview_base.lineEdit_iface_detail_2->text(),
                         _ui_kvirtualview_base.comboBox_iface_model_2->currentText(),
                         _ui_kvirtualview_base.lineEdit_iface_mac_2->text()
                       );
    m_options->setStorage( 0,
                           _ui_kvirtualview_base.comboBox_storage_1->currentText(),
                           _ui_kvirtualview_base.kurlrequester_storage_1->text()
                         );
    m_options->setStorage( 1,
                           _ui_kvirtualview_base.comboBox_storage_2->currentText(),
                           _ui_kvirtualview_base.kurlrequester_storage_2->text()
                         );
}

void KVirtualView::addOutput( const QString & message )
{
    _ui_kvirtualview_base.textBrowser_output->insertHtml( "<font color=\"black\">" + message + "</font><br>" );
}

void KVirtualView::addError( const QString & message )
{
    _ui_kvirtualview_base.textBrowser_output->insertHtml( "<font color=\"red\">" + message + "</font><br>" );
}

void KVirtualView::setOutput( const QString & message )
{
    _ui_kvirtualview_base.textBrowser_output->insertPlainText( message );
}

void KVirtualView::toggleOutput()
{
    if ( _ui_kvirtualview_base.textBrowser_output->isVisible() )
    {
        _ui_kvirtualview_base.textBrowser_output->setVisible( false );
    }
    else
    {
        _ui_kvirtualview_base.textBrowser_output->setVisible( true );
    }
}

#include "kvirtualview.moc"
// kate: indent-mode cstyle; space-indent on; indent-width 0;  replace-tabs off;
