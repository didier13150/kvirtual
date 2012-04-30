/*****************************************************************************
 *      ____              ____    _       _   _                              *
 *     /# /_\_           |  _ \  (_)   __| | (_)   ___   _ __                *
 *    |  |/o\o\          | | | | | |  / _` | | |  / _ \ | '__|               *
 *    |  \\_/_/          | |_| | | | | (_| | | | |  __/ | |                  *
 *   / |_   |            |____/  |_|  \__,_| |_|  \___| |_|                  *
 *  |  ||\_ ~|                                                               *
 *  |  ||| \/                                                                *
 *  |  |||       Project : KVirtual : a KDE4 GUI frontend for KVM            *
 *  \//  |                                                                   *
 *   ||  |       Developper : Didier FABERT <didier.fabert@gmail.com>        *
 *   ||_  \      Date : 2012, April                                          *
 *   \_|  o|                                             ,__,                *
 *    \___/      Copyright (C) 2009 by didier fabert     (oo)____            *
 *     ||||__                                            (__)    )\          *
 *     (___)_)   File : kvirtualview.cpp                    ||--|| *         *
 *                                                                           *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .          *
 *****************************************************************************/

#include "kvirtualview.h"
#include "kvirtualoptions.h"
#include "kvirtualstorage.h"
#include "kvirtualiface.h"

#include "settings.h"

#include <KLocale>
#include <KLineEdit>
#include <KStandardDirs>

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
    QString img = KStandardDirs::locate( "appdata", distrib + ".svg" );

    if ( img.isNull() ) img = "linux.svg";
    _ui_kvirtualview_base.widget_logo->load ( img );
	emit( signalChangeDistribution( distrib ) );
}

void KVirtualView::setState( uint, bool state )
{
    _ui_kvirtualview_base.widget_logo->setEnabled( state );
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
    connect( _ui_kvirtualview_base.comboBox_iface_type_1,
             SIGNAL( currentIndexChanged( int ) ),
             SLOT( setType1Enable( int ) )
           );
    connect( _ui_kvirtualview_base.comboBox_iface_type_2,
             SIGNAL( currentIndexChanged( int ) ),
             SLOT( setType2Enable( int ) )
           );
    connect( _ui_kvirtualview_base.comboBox_iface_type_3,
             SIGNAL( currentIndexChanged( int ) ),
             SLOT( setType3Enable( int ) )
           );
    connect( _ui_kvirtualview_base.comboBox_iface_model_1,
             SIGNAL( currentIndexChanged( int ) ),
             SLOT( setIface1Enabled( int ) )

           );
    connect( _ui_kvirtualview_base.comboBox_iface_model_2,
             SIGNAL( currentIndexChanged( int ) ),
             SLOT( setIface2Enabled( int ) )

           );
    connect( _ui_kvirtualview_base.comboBox_iface_model_3,
             SIGNAL( currentIndexChanged( int ) ),
             SLOT( setIface3Enabled( int ) )

           );
}

void KVirtualView::setIface1Enabled( int id )
{
    bool state = true;
    if ( id <= 0 )
    {
        state = false;
        setType1Enable( 0 );
    }
    else
    {
        setType1Enable( 1 );
    }
    _ui_kvirtualview_base.comboBox_iface_type_1->setEnabled( state );
    _ui_kvirtualview_base.lineEdit_iface_mac_1->setEnabled( state );
}

void KVirtualView::setIface2Enabled( int id )
{
    bool state = true;
    if ( id <= 0 )
    {
        state = false;
        setType2Enable( 0 );
    }
    else
    {
        setType2Enable( 1 );
    }
    _ui_kvirtualview_base.comboBox_iface_type_2->setEnabled( state );
    _ui_kvirtualview_base.lineEdit_iface_mac_2->setEnabled( state );
}

void KVirtualView::setIface3Enabled( int id )
{
    bool state = true;
    if ( id <= 0 )
    {
        state = false;
        setType3Enable( 0 );
    }
    else
    {
        setType3Enable( 1 );
    }
    _ui_kvirtualview_base.comboBox_iface_type_3->setEnabled( state );
    _ui_kvirtualview_base.lineEdit_iface_mac_3->setEnabled( state );
}

void KVirtualView::setType1Enable( int id )
{
    bool state = true;
    if ( ! id )
    {
        state = false;
        _ui_kvirtualview_base.kurlrequester_scriptup_1->setEnabled( false );
        _ui_kvirtualview_base.kurlrequester_scriptdown_1->setEnabled( false );
        _ui_kvirtualview_base.checkBox_scriptup_1->setChecked( false );
        _ui_kvirtualview_base.checkBox_scriptdown_1->setChecked( false );
    }
    _ui_kvirtualview_base.lineEdit_iface_detail_1->setEnabled( state );
    _ui_kvirtualview_base.checkBox_scriptup_1->setEnabled( state );
    _ui_kvirtualview_base.checkBox_scriptdown_1->setEnabled( state );
}

void KVirtualView::setType2Enable( int id )
{
    bool state = true;
    if ( ! id )
    {
        state = false;
        _ui_kvirtualview_base.kurlrequester_scriptup_2->setEnabled( state );
        _ui_kvirtualview_base.kurlrequester_scriptdown_2->setEnabled( state );
        _ui_kvirtualview_base.checkBox_scriptup_2->setChecked( false );
        _ui_kvirtualview_base.checkBox_scriptdown_2->setChecked( false );
    }
    _ui_kvirtualview_base.lineEdit_iface_detail_2->setEnabled( state );
    _ui_kvirtualview_base.checkBox_scriptup_2->setEnabled( state );
    _ui_kvirtualview_base.checkBox_scriptdown_2->setEnabled( state );
}

void KVirtualView::setType3Enable( int id )
{
    bool state = true;
    if ( ! id )
    {
        state = false;
        _ui_kvirtualview_base.kurlrequester_scriptup_3->setEnabled( state );
        _ui_kvirtualview_base.kurlrequester_scriptdown_3->setEnabled( state );
        _ui_kvirtualview_base.checkBox_scriptup_3->setChecked( false );
        _ui_kvirtualview_base.checkBox_scriptdown_3->setChecked( false );
    }
    _ui_kvirtualview_base.lineEdit_iface_detail_3->setEnabled( state );
    _ui_kvirtualview_base.checkBox_scriptup_3->setEnabled( state );
    _ui_kvirtualview_base.checkBox_scriptdown_3->setEnabled( state );
}

void KVirtualView::loadOptions()
{
    KVirtualOptions::Display display;
    KVirtualStorage* storage;
	KVirtualIface* iface;
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
    _ui_kvirtualview_base.comboBox_boot->setCurrentIndex( (int) m_options->getBootDevice() );

    storage = m_options->getStorage( 0 );
    if ( storage )
    {
        _ui_kvirtualview_base.kurlrequester_storage_1->lineEdit()->setSqueezedTextEnabled( true );
        _ui_kvirtualview_base.kurlrequester_storage_1->lineEdit()->setText( storage->getFile() );
        _ui_kvirtualview_base.comboBox_storage_2->setCurrentIndex( storage->getStorageType() );
    }

    storage = m_options->getStorage( 1 );
    if ( storage )
    {
        _ui_kvirtualview_base.kurlrequester_storage_2->lineEdit()->setSqueezedTextEnabled( true );
        _ui_kvirtualview_base.kurlrequester_storage_2->lineEdit()->setText( storage->getFile() );
        _ui_kvirtualview_base.comboBox_storage_2->setCurrentIndex( storage->getStorageType() );
    }

    storage = m_options->getStorage( 2 );
    if ( storage )
    {
        _ui_kvirtualview_base.kurlrequester_storage_3->lineEdit()->setSqueezedTextEnabled( true );
        _ui_kvirtualview_base.kurlrequester_storage_3->lineEdit()->setText( storage->getFile() );
        _ui_kvirtualview_base.comboBox_storage_3->setCurrentIndex( storage->getStorageType() );
    }

    iface = m_options->getIface( 0 );
    if ( iface )
    {
        index = _ui_kvirtualview_base.comboBox_iface_model_1->findText( iface->getModel() );
        if ( index >= 0 )
            _ui_kvirtualview_base.comboBox_iface_model_1->setCurrentIndex( index );
        index = _ui_kvirtualview_base.comboBox_iface_type_1->findText( iface->getType() );
        if ( index >= 0 )
            _ui_kvirtualview_base.comboBox_iface_type_1->setCurrentIndex( index );
        _ui_kvirtualview_base.lineEdit_iface_detail_1->setText( iface->getFile() );
        _ui_kvirtualview_base.lineEdit_iface_mac_1->setText( iface->getHardwareAddress() );
        _ui_kvirtualview_base.kurlrequester_scriptup_1->lineEdit()->setText( iface->getScriptUp() );
        _ui_kvirtualview_base.kurlrequester_scriptdown_1->lineEdit()->setText( iface->getScriptDown() );
        _ui_kvirtualview_base.checkBox_scriptup_1->setChecked( iface->isScriptUpEnabled() );
        _ui_kvirtualview_base.checkBox_scriptdown_1->setChecked( iface->isScriptDownEnabled() );
    }

    iface = m_options->getIface( 1 );
    if ( iface )
    {
        index = _ui_kvirtualview_base.comboBox_iface_model_2->findText( iface->getModel() );
        if ( index >= 0 )
            _ui_kvirtualview_base.comboBox_iface_model_2->setCurrentIndex( index );
        index = _ui_kvirtualview_base.comboBox_iface_type_2->findText( iface->getType() );
        if ( index >= 0 )
            _ui_kvirtualview_base.comboBox_iface_type_2->setCurrentIndex( index );
        _ui_kvirtualview_base.lineEdit_iface_detail_2->setText( iface->getFile() );
        _ui_kvirtualview_base.lineEdit_iface_mac_2->setText( iface->getHardwareAddress() );
        _ui_kvirtualview_base.kurlrequester_scriptup_2->lineEdit()->setText( iface->getScriptUp() );
        _ui_kvirtualview_base.kurlrequester_scriptdown_2->lineEdit()->setText( iface->getScriptDown() );
        _ui_kvirtualview_base.checkBox_scriptup_2->setChecked( iface->isScriptUpEnabled() );
        _ui_kvirtualview_base.checkBox_scriptdown_2->setChecked( iface->isScriptDownEnabled() );
    }

    iface = m_options->getIface( 2 );
    if ( iface )
    {
        index = _ui_kvirtualview_base.comboBox_iface_model_3->findText( iface->getModel() );
        if ( index >= 0 )
            _ui_kvirtualview_base.comboBox_iface_model_3->setCurrentIndex( index );
        index = _ui_kvirtualview_base.comboBox_iface_type_3->findText( iface->getType() );
        if ( index >= 0 )
            _ui_kvirtualview_base.comboBox_iface_type_3->setCurrentIndex( index );
        _ui_kvirtualview_base.lineEdit_iface_detail_3->setText( iface->getFile() );
        _ui_kvirtualview_base.lineEdit_iface_mac_3->setText( iface->getHardwareAddress() );
        _ui_kvirtualview_base.kurlrequester_scriptup_3->lineEdit()->setText( iface->getScriptUp() );
        _ui_kvirtualview_base.kurlrequester_scriptdown_3->lineEdit()->setText( iface->getScriptDown() );
        _ui_kvirtualview_base.checkBox_scriptup_3->setChecked( iface->isScriptUpEnabled() );
        _ui_kvirtualview_base.checkBox_scriptdown_3->setChecked( iface->isScriptDownEnabled() );
    }
}

void KVirtualView::setOptions()
{
    QString buffer, buffer2;
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
    if ( _ui_kvirtualview_base.comboBox_video_card->currentIndex() )
    {
        m_options->setVideoCard( _ui_kvirtualview_base.comboBox_video_card->currentText() );
    }
    else
    {
        m_options->setVideoCard( QString() );
    }

    m_options->setBootDevice( _ui_kvirtualview_base.comboBox_boot->currentIndex() );
    if ( _ui_kvirtualview_base.radioButton_direct->isChecked() )
        display = KVirtualOptions::DISPLAY_DIRECT;
    else if ( _ui_kvirtualview_base.radioButton_vnc->isChecked() )
        display = KVirtualOptions::DISPLAY_VNC;
    else
        display = KVirtualOptions::DISPLAY_NONE;
    m_options->setDisplay( display );

    if ( _ui_kvirtualview_base.comboBox_iface_model_1->currentIndex() )
    {
        buffer = _ui_kvirtualview_base.comboBox_iface_model_1->currentText();
    }
    else
    {
        buffer.clear();
    }
    if ( _ui_kvirtualview_base.comboBox_iface_type_1->currentIndex() )
	{
		buffer2 = _ui_kvirtualview_base.comboBox_iface_type_1->currentText();
	}
	else
	{
		buffer2.clear();
	}
    m_options->setIface( 0,
                        buffer2,
                         _ui_kvirtualview_base.lineEdit_iface_detail_1->text(),
                         buffer,
                         _ui_kvirtualview_base.lineEdit_iface_mac_1->text()
                       );
    m_options->setScripts( 0,
                           _ui_kvirtualview_base.kurlrequester_scriptup_1->lineEdit()->text(),
                           _ui_kvirtualview_base.kurlrequester_scriptdown_1->lineEdit()->text()
                         );
    m_options->setScriptsEnabled( 0,
                                  _ui_kvirtualview_base.checkBox_scriptup_1->isChecked(),
                                  _ui_kvirtualview_base.checkBox_scriptdown_1->isChecked()
                                );

    if ( _ui_kvirtualview_base.comboBox_iface_model_2->currentIndex() )
    {
        buffer = _ui_kvirtualview_base.comboBox_iface_model_2->currentText();
    }
    else
    {
        buffer.clear();
    }
    if ( _ui_kvirtualview_base.comboBox_iface_type_2->currentIndex() )
	{
		buffer2 = _ui_kvirtualview_base.comboBox_iface_type_2->currentText();
	}
	else
	{
		buffer2.clear();
	}
    m_options->setIface( 1,
                         buffer2,
                         _ui_kvirtualview_base.lineEdit_iface_detail_2->text(),
                         buffer,
                         _ui_kvirtualview_base.lineEdit_iface_mac_2->text()
                       );
    m_options->setScripts( 1,
                           _ui_kvirtualview_base.kurlrequester_scriptup_2->lineEdit()->text(),
                           _ui_kvirtualview_base.kurlrequester_scriptdown_2->lineEdit()->text()
                         );
    m_options->setScriptsEnabled( 1,
                                  _ui_kvirtualview_base.checkBox_scriptup_2->isChecked(),
                                  _ui_kvirtualview_base.checkBox_scriptdown_2->isChecked()
                                );
    if ( _ui_kvirtualview_base.comboBox_iface_model_3->currentIndex() )
    {
        buffer = _ui_kvirtualview_base.comboBox_iface_model_3->currentText();
    }
    else
    {
        buffer.clear();
    }
    if ( _ui_kvirtualview_base.comboBox_iface_type_3->currentIndex() )
	{
		buffer2 = _ui_kvirtualview_base.comboBox_iface_type_3->currentText();
	}
	else
	{
		buffer2.clear();
	}
    m_options->setIface( 2,
                         buffer2,
                         _ui_kvirtualview_base.lineEdit_iface_detail_3->text(),
                         buffer,
                         _ui_kvirtualview_base.lineEdit_iface_mac_3->text()
                       );
    m_options->setScripts( 2,
                           _ui_kvirtualview_base.kurlrequester_scriptup_3->lineEdit()->text(),
                           _ui_kvirtualview_base.kurlrequester_scriptdown_3->lineEdit()->text()
                         );
    m_options->setScriptsEnabled( 2,
                                  _ui_kvirtualview_base.checkBox_scriptup_3->isChecked(),
                                  _ui_kvirtualview_base.checkBox_scriptdown_3->isChecked()
                                );

    m_options->setStorage( 0,
                           _ui_kvirtualview_base.comboBox_storage_1->currentIndex(),
                           _ui_kvirtualview_base.kurlrequester_storage_1->text()
                         );
    m_options->setStorage( 1,
                           _ui_kvirtualview_base.comboBox_storage_2->currentIndex(),
                           _ui_kvirtualview_base.kurlrequester_storage_2->text()
                         );
    m_options->setStorage( 2,
                           _ui_kvirtualview_base.comboBox_storage_3->currentIndex(),
                           _ui_kvirtualview_base.kurlrequester_storage_3->text()
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
// kate: indent-mode cstyle; space-indent on; indent-width 0;    replace-tabs off;
