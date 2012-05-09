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
 *    \___/      Copyright (C) 2012 by didier fabert     (oo)____            *
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

#include <QButtonGroup>

#include <sys/sysinfo.h>
#include <unistd.h>

#define MAX_IFACE 2
#define MAX_STORAGE 2

KVirtualView::KVirtualView( QWidget * )
{

	m_options = 0;
	_ui_kvirtualview_base.setupUi( this );
	settingsChanged();
	setAutoFillBackground( true );
	setIcon( "linux" );
	connect( _ui_kvirtualview_base.comboBox_distrib,
	         SIGNAL( currentIndexChanged( const QString & ) ),
	         SLOT( setIcon( const QString & ) )
	       );
	m_displayGroup = new QButtonGroup( this );
	m_displayGroup->addButton( _ui_kvirtualview_base.radioButton_none, ( int ) KVirtualOptions::DISPLAY_NONE );
	m_displayGroup->addButton( _ui_kvirtualview_base.radioButton_direct, ( int ) KVirtualOptions::DISPLAY_DIRECT );
	m_displayGroup->addButton( _ui_kvirtualview_base.radioButton_vnc, ( int ) KVirtualOptions::DISPLAY_VNC );

	_ui_kvirtualview_base.widget_storage0->setStorageID( 0 );
	_ui_kvirtualview_base.widget_storage1->setStorageID( 1 );
	_ui_kvirtualview_base.widget_storage2->setStorageID( 2 );
	_ui_kvirtualview_base.widget_iface0->setIfaceID( 0 );
	_ui_kvirtualview_base.widget_iface1->setIfaceID( 1 );
	_ui_kvirtualview_base.widget_iface2->setIfaceID( 2 );

	m_storages[0] = _ui_kvirtualview_base.widget_storage0;
	m_storages[1] = _ui_kvirtualview_base.widget_storage1;
	m_storages[2] = _ui_kvirtualview_base.widget_storage2;

	m_ifaces[0] = _ui_kvirtualview_base.widget_iface0;
	m_ifaces[1] = _ui_kvirtualview_base.widget_iface1;
	m_ifaces[2] = _ui_kvirtualview_base.widget_iface2;
}

KVirtualView::~KVirtualView()
{
	delete m_displayGroup;
}

void KVirtualView::settingsChanged()
{
	emit signalChangeStatusbar( i18n( "Settings changed" ) );
}

void KVirtualView::setIcon( const QString & distrib )
{
	QString img = KStandardDirs::locate( "appdata", distrib + ".svg" );

	if ( img.isNull() )
		img = "linux.svg";

	_ui_kvirtualview_base.widget_logo->load( img );

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
	ram = data.totalram / 1024 / 1024;
	cpus = sysconf( _SC_NPROCESSORS_ONLN );

	if ( cpus <= 1 )
	{
		cpus = 1;
		_ui_kvirtualview_base.kintspinbox_nb_of_cpu->setEnabled( false );
		_ui_kvirtualview_base.horizontalSlider_nb_of_cpu->setEnabled( false );
	}

	_ui_kvirtualview_base.horizontalSlider_memory->setRange( 0, ( int ) ram );

	_ui_kvirtualview_base.kintspinbox_memory->setRange( 0, ( int ) ram );
	_ui_kvirtualview_base.kintspinbox_nb_of_cpu->setRange( 1, ( int ) cpus );
	_ui_kvirtualview_base.horizontalSlider_nb_of_cpu->setRange( 1, ( int ) cpus );
	connect( _ui_kvirtualview_base.horizontalSlider_memory,
	         SIGNAL( valueChanged( int ) ),
	         _ui_kvirtualview_base.kintspinbox_memory,
	         SLOT( setValue( int ) )
	       );
	connect( _ui_kvirtualview_base.kintspinbox_memory,
	         SIGNAL( valueChanged( int ) ),
	         _ui_kvirtualview_base.horizontalSlider_memory,
	         SLOT( setValue( int ) )
	       );
	connect( _ui_kvirtualview_base.horizontalSlider_nb_of_cpu,
	         SIGNAL( valueChanged( int ) ),
	         _ui_kvirtualview_base.kintspinbox_nb_of_cpu,
	         SLOT( setValue( int ) )
	       );
	connect( _ui_kvirtualview_base.kintspinbox_nb_of_cpu,
	         SIGNAL( valueChanged( int ) ),
	         _ui_kvirtualview_base.horizontalSlider_nb_of_cpu,
	         SLOT( setValue( int ) )
	       );
	connect( _ui_kvirtualview_base.lineEdit_name,
	         SIGNAL( textEdited( QString ) ),
	         m_options,
	         SLOT( setName( QString ) )
	       );
	connect( _ui_kvirtualview_base.comboBox_distrib,
	         SIGNAL( currentIndexChanged( QString ) ),
	         m_options,
	         SLOT( setDistrib( QString ) )
	       );
	connect( _ui_kvirtualview_base.textEdit_description,
	         SIGNAL( textChanged() ),
	         SLOT( syncDescription() )
	       );
	connect( _ui_kvirtualview_base.kintspinbox_memory,
	         SIGNAL( valueChanged( int ) ),
	         m_options,
	         SLOT( setMemory( int ) )
	       );
	connect( _ui_kvirtualview_base.kintspinbox_nb_of_cpu,
	         SIGNAL( valueChanged( int ) ),
	         m_options,
	         SLOT( setNbCPU( int ) )
	       );
	connect( _ui_kvirtualview_base.checkBox_usb,
	         SIGNAL( stateChanged( int ) ),
	         m_options,
	         SLOT( setUsbSupported( int ) )
	       );
	connect( _ui_kvirtualview_base.checkBox_snapshot,
	         SIGNAL( stateChanged( int ) ),
	         m_options,
	         SLOT( setSnapshotEnabled( int ) )
	       );
	connect( _ui_kvirtualview_base.spinBox_vncport,
	         SIGNAL( valueChanged( int ) ),
	         m_options,
	         SLOT( setVncPort( int ) )
	       );
	connect( _ui_kvirtualview_base.comboBox_display_keyboard,
	         SIGNAL( currentIndexChanged( QString ) ),
	         m_options,
	         SLOT( setKeyboard( QString ) )
	       );
	connect( _ui_kvirtualview_base.comboBox_video_card,
	         SIGNAL( currentIndexChanged( QString ) ),
	         m_options,
	         SLOT( setVideoCard( QString ) )
	       );
	connect( m_displayGroup,
	         SIGNAL( buttonClicked( int ) ),
	         m_options,
	         SLOT( setDisplay( int ) )
	       );
	connect( _ui_kvirtualview_base.comboBox_boot,
	         SIGNAL( currentIndexChanged( int ) ),
	         m_options,
	         SLOT( setBootDevice( int ) )
	       );

	for ( uint it = 0 ; it <= MAX_STORAGE ; ++it )
	{
		connect( m_storages[it],
		         SIGNAL( fileChanged( uint, QString ) ),
		         m_options,
		         SLOT( setStorageFile( uint, QString ) )
		       );
		connect( m_storages[it],
		         SIGNAL( interfaceChanged( uint, QString ) ),
		         m_options,
		         SLOT( setStorageInterface( uint, QString ) )
		       );
		connect( m_storages[it],
		         SIGNAL( typeChanged( uint, int ) ),
		         m_options,
		         SLOT( setStorageType( uint, int ) )
		       );
	}

	for ( uint it = 0 ; it <= MAX_IFACE ; ++it )
	{
		connect( m_ifaces[it],
		         SIGNAL( typeChanged( uint, QString ) ),
		         m_options,
		         SLOT( setIfaceType( uint, QString ) )
		       );
		connect( m_ifaces[it],
		         SIGNAL( fileChanged( uint, QString ) ),
		         m_options,
		         SLOT( setIfaceFile( uint, QString ) )
		       );
		connect( m_ifaces[it],
		         SIGNAL( modelChanged( uint, QString ) ),
		         m_options,
		         SLOT( setIfaceModel( uint, QString ) )
		       );
		connect( m_ifaces[it],
		         SIGNAL( hwAddrChanged( uint, QString ) ),
		         m_options,
		         SLOT( setIfaceHwAddr( uint, QString ) )
		       );
		connect( m_ifaces[it],
		         SIGNAL( scriptUpChanged( uint, QString ) ),
		         m_options,
		         SLOT( setIfaceScriptUp( uint, QString ) )
		       );
		connect( m_ifaces[it],
		         SIGNAL( scriptDownChanged( uint, QString ) ),
		         m_options,
		         SLOT( setIfaceScriptDown( uint, QString ) )
		       );
		connect( m_ifaces[it],
		         SIGNAL( scriptUpEnabled( uint, bool ) ),
		         m_options,
		         SLOT( setIfaceScriptUpEnabled( uint, bool ) )
		       );
		connect( m_ifaces[it],
		         SIGNAL( scriptDownEnabled( uint, bool ) ),
		         m_options,
		         SLOT( setIfaceScriptDownEnabled( uint, bool ) )
		       );
	}
}

void KVirtualView::syncVideoCard()
{
	if ( _ui_kvirtualview_base.comboBox_video_card->currentIndex() )
	{
		m_options->setVideoCard( _ui_kvirtualview_base.comboBox_video_card->currentText() );
	}

	else
	{
		m_options->setVideoCard( QString() );
	}
}

void KVirtualView::syncDescription()
{
	m_options->setDescription( _ui_kvirtualview_base.textEdit_description->toHtml() );
}

void KVirtualView::loadOptions()
{
	KVirtualStorage* storage;
	KVirtualIface* iface;
	int index, display;

	if ( ! m_options )
		return;

	_ui_kvirtualview_base.lineEdit_name->setText( m_options->getName() );

	_ui_kvirtualview_base.textEdit_description->setHtml( m_options->getDescription() );

	_ui_kvirtualview_base.kintspinbox_memory->setValue(( int ) m_options->getMemory() );

	_ui_kvirtualview_base.kintspinbox_nb_of_cpu->setValue(( int ) m_options->getNbCPU() );

	_ui_kvirtualview_base.spinBox_vncport->setValue(( int ) m_options->getVncPort() );

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

	display = ( int ) m_options->getDisplay();

	m_displayGroup->button( display )->setChecked( true );

	_ui_kvirtualview_base.comboBox_boot->setCurrentIndex(( int ) m_options->getBootDevice() );

	for ( uint it = 0 ; it <= MAX_IFACE ; ++it )
	{
		storage = m_options->getStorage( it );

		if ( storage )
		{
			m_storages[it]->setFile( storage->getFile() );
			m_storages[it]->setType( storage->getTypeID() );
			m_storages[it]->setInterface( storage->getInterface() );
		}
	}

	for ( uint it = 0 ; it <= MAX_IFACE ; ++it )
	{
		iface = m_options->getIface( it );

		if ( iface )
		{
			m_ifaces[it]->setFile( iface->getFile() );
			m_ifaces[it]->setType( iface->getType() );
			m_ifaces[it]->setModel( iface->getModel() );
			m_ifaces[it]->setHwAddr( iface->getHardwareAddress() );
			m_ifaces[it]->setScriptUp( iface->getScriptUp() );
			m_ifaces[it]->setScriptDown( iface->getScriptDown() );
			m_ifaces[it]->setScriptUpEnabled( iface->isScriptUpEnabled() );
			m_ifaces[it]->setScriptDownEnabled( iface->isScriptDownEnabled() );
		}
	}
}

void KVirtualView::addOutput( const QString & message )
{
	_ui_kvirtualview_base.textBrowser_output->insertHtml( "<font color=\"black\">" + message + "</font><br>" );
}

void KVirtualView::addError( const QString & message )
{
	_ui_kvirtualview_base.textBrowser_output->insertHtml( "<font color=\"red\">" + message + "</font><br>" );
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
// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;  replace-tabs off;  replace-tabs off;  replace-tabs off;  replace-tabs off;  replace-tabs off;   replace-tabs off;  replace-tabs off;      replace-tabs off;
