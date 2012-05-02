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
	connect( _ui_kvirtualview_base.comboBox_storage_1,
			 SIGNAL( currentIndexChanged( int ) ),
			 SLOT( syncStorageType1( int ) )
		   );
	connect( _ui_kvirtualview_base.kurlrequester_storage_1,
			 SIGNAL( textChanged( QString ) ),
			 SLOT( syncStorageFile1( QString ) )
		   );
	connect( _ui_kvirtualview_base.kurlrequester_storage_2,
			 SIGNAL( textChanged( QString ) ),
			 SLOT( syncStorageFile2( QString ) )
		   );
	connect( _ui_kvirtualview_base.kurlrequester_storage_3,
			 SIGNAL( textChanged( QString ) ),
			 SLOT( syncStorageFile3( QString ) )
		   );
	connect( _ui_kvirtualview_base.comboBox_storage_2,
			 SIGNAL( currentIndexChanged( int ) ),
			 SLOT( syncStorageType2( int ) )
		   );
	connect( _ui_kvirtualview_base.comboBox_storage_3,
			 SIGNAL( currentIndexChanged( int ) ),
			 SLOT( syncStorageType3( int ) )
		   );
	connect( _ui_kvirtualview_base.comboBox_iface_type_1,
			 SIGNAL( currentIndexChanged( QString ) ),
			 SLOT( syncIfaceType1( QString ) )
		   );
	connect( _ui_kvirtualview_base.comboBox_iface_type_2,
			 SIGNAL( currentIndexChanged( QString ) ),
			 SLOT( syncIfaceType3( QString ) )
		   );
	connect( _ui_kvirtualview_base.comboBox_iface_type_2,
			 SIGNAL( currentIndexChanged( QString ) ),
			 SLOT( syncIfaceType3( QString ) )
		   );
	connect( _ui_kvirtualview_base.lineEdit_iface_detail_1,
			 SIGNAL( textEdited( QString ) ),
			 SLOT( syncIfaceFile1( QString ) )
		   );
	connect( _ui_kvirtualview_base.lineEdit_iface_detail_2,
			 SIGNAL( textEdited( QString ) ),
			 SLOT( syncIfaceFile2( QString ) )
		   );
	connect( _ui_kvirtualview_base.lineEdit_iface_detail_3,
			 SIGNAL( textEdited( QString ) ),
			 SLOT( syncIfaceFile3( QString ) )
		   );
	connect( _ui_kvirtualview_base.comboBox_iface_model_1,
			 SIGNAL( currentIndexChanged( QString ) ),
			 SLOT( syncIfaceModel1( QString ) )
		   );
	connect( _ui_kvirtualview_base.comboBox_iface_model_2,
			 SIGNAL( currentIndexChanged( QString ) ),
			 SLOT( syncIfaceModel2( QString ) )
		   );
	connect( _ui_kvirtualview_base.comboBox_iface_model_3,
			 SIGNAL( currentIndexChanged( QString ) ),
			 SLOT( syncIfaceModel3( QString ) )
		   );
	connect( _ui_kvirtualview_base.lineEdit_iface_mac_1,
			 SIGNAL( textEdited( QString ) ),
			 SLOT( syncIfaceHwAddr1( QString ) )
		   );
	connect( _ui_kvirtualview_base.lineEdit_iface_mac_2,
			 SIGNAL( textEdited( QString ) ),
			 SLOT( syncIfaceHwAddr2( QString ) )
		   );
	connect( _ui_kvirtualview_base.lineEdit_iface_mac_3,
			 SIGNAL( textEdited( QString ) ),
			 SLOT( syncIfaceHwAddr3( QString ) )
		   );
	connect( _ui_kvirtualview_base.kurlrequester_scriptup_1,
			 SIGNAL( textChanged( QString ) ),
			 SLOT( syncIfaceScriptUp1( QString ) )
		   );
	connect( _ui_kvirtualview_base.kurlrequester_scriptup_2,
			 SIGNAL( textChanged( QString ) ),
			 SLOT( syncIfaceScriptUp2( QString ) )
		   );
	connect( _ui_kvirtualview_base.kurlrequester_scriptup_3,
			 SIGNAL( textChanged( QString ) ),
			 SLOT( syncIfaceScriptUp3( QString ) )
		   );
	connect( _ui_kvirtualview_base.kurlrequester_scriptdown_1,
			 SIGNAL( textChanged( QString ) ),
			 SLOT( syncIfaceScriptDown1( QString ) )
		   );
	connect( _ui_kvirtualview_base.kurlrequester_scriptdown_2,
			 SIGNAL( textChanged( QString ) ),
			 SLOT( syncIfaceScriptDown2( QString ) )
		   );
	connect( _ui_kvirtualview_base.kurlrequester_scriptdown_3,
			 SIGNAL( textChanged( QString ) ),
			 SLOT( syncIfaceScriptDown3( QString ) )
		   );
	connect( _ui_kvirtualview_base.checkBox_scriptup_1,
			 SIGNAL( stateChanged( int ) ),
			 SLOT( syncIfaceScriptUpState1( int ) )
		   );
	connect( _ui_kvirtualview_base.checkBox_scriptup_2,
			 SIGNAL( stateChanged( int ) ),
			 SLOT( syncIfaceScriptUpState2( int ) )
		   );
	connect( _ui_kvirtualview_base.checkBox_scriptup_3,
			 SIGNAL( stateChanged( int ) ),
			 SLOT( syncIfaceScriptUpState3( int ) )
		   );
	connect( _ui_kvirtualview_base.checkBox_scriptdown_1,
			 SIGNAL( stateChanged( int ) ),
			 SLOT( syncIfaceScriptDownState1( int ) )
		   );
	connect( _ui_kvirtualview_base.checkBox_scriptdown_2,
			 SIGNAL( stateChanged( int ) ),
			 SLOT( syncIfaceScriptDownState2( int ) )
		   );
	connect( _ui_kvirtualview_base.checkBox_scriptdown_3,
			 SIGNAL( stateChanged( int ) ),
			 SLOT( syncIfaceScriptDownState3( int ) )
		   );
}

void KVirtualView::syncStorageType1( int type )
{
	KVirtualStorage* storage = m_options->getStorage( 0 );
	storage->setTypeID( type );
}

void KVirtualView::syncStorageType2( int type )
{
	KVirtualStorage* storage = m_options->getStorage( 1 );
	storage->setTypeID( type );
}

void KVirtualView::syncStorageType3( int type )
{
	KVirtualStorage* storage = m_options->getStorage( 2 );
	storage->setTypeID( type );
}

void KVirtualView::syncStorageFile1( const QString & file )
{
	KVirtualStorage* storage = m_options->getStorage( 0 );
	storage->setFile( file );
}

void KVirtualView::syncStorageFile2( const QString & file )
{
	KVirtualStorage* storage = m_options->getStorage( 1 );
	storage->setFile( file );
}

void KVirtualView::syncStorageFile3( const QString & file )
{
	KVirtualStorage* storage = m_options->getStorage( 2 );
	storage->setFile( file );
}

void KVirtualView::syncIfaceScriptUp1( const QString & script )
{
	KVirtualIface* iface = m_options->getIface( 0 );
	iface->setScriptUp( script );
}

void KVirtualView::syncIfaceScriptUp2( const QString & script )
{
	KVirtualIface* iface = m_options->getIface( 1 );
	iface->setScriptUp( script );
}

void KVirtualView::syncIfaceScriptUp3( const QString & script )
{
	KVirtualIface* iface = m_options->getIface( 2 );
	iface->setScriptUp( script );
}

void KVirtualView::syncIfaceScriptDown1( const QString & script )
{
	KVirtualIface* iface = m_options->getIface( 0 );
	iface->setScriptDown( script );
}

void KVirtualView::syncIfaceScriptDown2( const QString & script )
{
	KVirtualIface* iface = m_options->getIface( 1 );
	iface->setScriptDown( script );
}

void KVirtualView::syncIfaceScriptDown3( const QString & script )
{
	KVirtualIface* iface = m_options->getIface( 2 );
	iface->setScriptDown( script );
}

void KVirtualView::syncIfaceScriptUpState1( int state )
{
	KVirtualIface* iface = m_options->getIface( 0 );
	iface->setScriptUpEnabled(( bool ) state );
}

void KVirtualView::syncIfaceScriptUpState2( int state )
{
	KVirtualIface* iface = m_options->getIface( 1 );
	iface->setScriptUpEnabled(( bool ) state );
}

void KVirtualView::syncIfaceScriptUpState3( int state )
{
	KVirtualIface* iface = m_options->getIface( 2 );
	iface->setScriptUpEnabled(( bool ) state );
}

void KVirtualView::syncIfaceScriptDownState1( int state )
{
	KVirtualIface* iface = m_options->getIface( 0 );
	iface->setScriptDownEnabled(( bool ) state );
}

void KVirtualView::syncIfaceScriptDownState2( int state )
{
	KVirtualIface* iface = m_options->getIface( 1 );
	iface->setScriptDownEnabled(( bool ) state );
}

void KVirtualView::syncIfaceScriptDownState3( int state )
{
	KVirtualIface* iface = m_options->getIface( 2 );
	iface->setScriptDownEnabled(( bool ) state );
}

void KVirtualView::syncIfaceType1( const QString & type )
{
	KVirtualIface* iface = m_options->getIface( 0 );
	iface->setType( type );
}

void KVirtualView::syncIfaceType2( const QString & type )
{
	KVirtualIface* iface = m_options->getIface( 1 );
	iface->setType( type );
}

void KVirtualView::syncIfaceType3( const QString & type )
{
	KVirtualIface* iface = m_options->getIface( 2 );
	iface->setType( type );
}

void KVirtualView::syncIfaceFile1( const QString & file )
{
	KVirtualIface* iface = m_options->getIface( 0 );
	iface->setFile( file );
}

void KVirtualView::syncIfaceFile2( const QString & file )
{
	KVirtualIface* iface = m_options->getIface( 1 );
	iface->setFile( file );
}

void KVirtualView::syncIfaceFile3( const QString & file )
{
	KVirtualIface* iface = m_options->getIface( 2 );
	iface->setFile( file );
}

void KVirtualView::syncIfaceModel1( const QString & model )
{
	KVirtualIface* iface = m_options->getIface( 0 );
	iface->setModel( model );
}

void KVirtualView::syncIfaceModel2( const QString & model )
{
	KVirtualIface* iface = m_options->getIface( 1 );
	iface->setModel( model );
}

void KVirtualView::syncIfaceModel3( const QString & model )
{
	KVirtualIface* iface = m_options->getIface( 2 );
	iface->setModel( model );
}

void KVirtualView::syncIfaceHwAddr1( const QString & hwaddr )
{
	KVirtualIface* iface = m_options->getIface( 0 );
	iface->setHardwareAddress( hwaddr );
}

void KVirtualView::syncIfaceHwAddr2( const QString & hwaddr )
{
	KVirtualIface* iface = m_options->getIface( 1 );
	iface->setHardwareAddress( hwaddr );
}

void KVirtualView::syncIfaceHwAddr3( const QString & hwaddr )
{
	KVirtualIface* iface = m_options->getIface( 2 );
	iface->setHardwareAddress( hwaddr );
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

	storage = m_options->getStorage( 0 );

	if ( storage )
	{
		_ui_kvirtualview_base.kurlrequester_storage_1->lineEdit()->setSqueezedTextEnabled( true );
		_ui_kvirtualview_base.kurlrequester_storage_1->lineEdit()->setText( storage->getFile() );
		_ui_kvirtualview_base.comboBox_storage_1->setCurrentIndex( storage->getTypeID() );
	}

	storage = m_options->getStorage( 1 );

	if ( storage )
	{
		_ui_kvirtualview_base.kurlrequester_storage_2->lineEdit()->setSqueezedTextEnabled( true );
		_ui_kvirtualview_base.kurlrequester_storage_2->lineEdit()->setText( storage->getFile() );
		_ui_kvirtualview_base.comboBox_storage_2->setCurrentIndex( storage->getTypeID() );
	}

	storage = m_options->getStorage( 2 );

	if ( storage )
	{
		_ui_kvirtualview_base.kurlrequester_storage_3->lineEdit()->setSqueezedTextEnabled( true );
		_ui_kvirtualview_base.kurlrequester_storage_3->lineEdit()->setText( storage->getFile() );
		_ui_kvirtualview_base.comboBox_storage_3->setCurrentIndex( storage->getTypeID() );
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
// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;      replace-tabs off;
