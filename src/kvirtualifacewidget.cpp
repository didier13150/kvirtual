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
 *     (___)_)   File : kvirtualifacewidget.cpp             ||--|| *         *
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

#include "kvirtualifacewidget.h"


KVirtualIfaceWidget::KVirtualIfaceWidget( QWidget *parent )
		: QWidget( parent )
{
	ui_widget.setupUi( this );
	connect( ui_widget.comboBox_type,
	         SIGNAL( currentIndexChanged( int ) ),
	         SLOT( enableScripts( int ) )
	       );
	connect( ui_widget.comboBox_model,
	         SIGNAL( currentIndexChanged( int ) ),
	         SLOT( enableWidgets( int ) )
	       );
	connect( ui_widget.lineEdit_file,
	         SIGNAL( textChanged( QString ) ),
	         SLOT( emitFileChanged( QString ) )
	       );
	connect( ui_widget.comboBox_type,
	         SIGNAL( currentIndexChanged( QString ) ),
	         SLOT( emitTypeChanged( QString ) )
	       );
	connect( ui_widget.comboBox_model,
	         SIGNAL( currentIndexChanged( QString ) ),
	         SLOT( emitModelChanged( QString ) )
	       );
	connect( ui_widget.lineEdit_mac,
	         SIGNAL( textChanged( QString ) ),
	         SLOT( emitHwAddrChanged( QString ) )
	       );
	connect( ui_widget.lineEdit_file,
	         SIGNAL( textChanged( QString ) ),
	         SLOT( emitFileChanged( QString ) )
	       );
	connect( ui_widget.checkBox_scriptdown,
	         SIGNAL( stateChanged( int ) ),
	         SLOT( emitScriptDownEnabled( int ) )
	       );
	connect( ui_widget.checkBox_scriptup,
	         SIGNAL( stateChanged( int ) ),
	         SLOT( emitScriptUpEnabled( int ) )
	       );
	connect( ui_widget.kurlrequester_scriptup,
	         SIGNAL( textChanged( QString ) ),
	         SLOT( emitScriptUpChanged( QString ) )
	       );
	connect( ui_widget.kurlrequester_scriptdown,
	         SIGNAL( textChanged( QString ) ),
	         SLOT( emitScriptDownChanged( QString ) )
	       );
	connect( ui_widget.checkBox_scriptup,
	         SIGNAL( stateChanged( int ) ),
	         SLOT( enableScriptUp( int ) )

	       );
	connect( ui_widget.checkBox_scriptdown,
	         SIGNAL( stateChanged( int ) ),
	         SLOT( enableScriptDown( int ) )

	       );
}

KVirtualIfaceWidget::~KVirtualIfaceWidget()
{
}

void KVirtualIfaceWidget::setFile( const QString & file )
{
	ui_widget.lineEdit_file->setText( file );
}

void KVirtualIfaceWidget::setType( const QString & type )
{
	int index = ui_widget.comboBox_type->findText( type );

	if ( index < 0 ) index = 0;

	ui_widget.comboBox_type->setCurrentIndex( index );
}

void KVirtualIfaceWidget::setModel( const QString & model )
{
	int index = ui_widget.comboBox_model->findText( model );

	if ( index < 0 ) index = 0;

	ui_widget.comboBox_model->setCurrentIndex( index );
}

void KVirtualIfaceWidget::setHwAddr( const QString & addr )
{
	ui_widget.lineEdit_mac->setText( addr );
}

void KVirtualIfaceWidget::setScriptUp( const QString & script )
{
	ui_widget.kurlrequester_scriptup->setText( script );
}

void KVirtualIfaceWidget::setScriptDown( const QString & script )
{
	ui_widget.kurlrequester_scriptdown->setText( script );
}

void KVirtualIfaceWidget::setScriptUpEnabled( bool state )
{
	ui_widget.checkBox_scriptup->setChecked( state );
}

void KVirtualIfaceWidget::setScriptDownEnabled( bool state )
{
	ui_widget.checkBox_scriptdown->setChecked( state );
}

void KVirtualIfaceWidget::setIfaceID( uint id )
{
	m_ifaceID = id;
}

uint KVirtualIfaceWidget::getIfaceID()
{
	return m_ifaceID;
}

void KVirtualIfaceWidget::emitFileChanged( const QString & file )
{
	emit( fileChanged( m_ifaceID, file ) );
}

void KVirtualIfaceWidget::emitTypeChanged( const QString & type )
{
	if ( ui_widget.comboBox_type->currentIndex() )
	{
		emit( typeChanged( m_ifaceID, type ) );
	}

	else
	{
		emit( typeChanged( m_ifaceID, QString() ) );
	}
}

void KVirtualIfaceWidget::emitModelChanged( const QString & model )
{
	if ( ui_widget.comboBox_model->currentIndex() )
	{
		emit( modelChanged( m_ifaceID, model ) );
	}

	else
	{
		emit( modelChanged( m_ifaceID, QString() ) );
	}
}

void KVirtualIfaceWidget::emitHwAddrChanged( const QString & addr )
{
	emit( hwAddrChanged( m_ifaceID, addr ) );
}

void KVirtualIfaceWidget::emitScriptUpChanged( const QString & script )
{
	emit( scriptUpChanged( m_ifaceID, script ) );
}

void KVirtualIfaceWidget::emitScriptDownChanged( const QString & script )
{
	emit( scriptDownChanged( m_ifaceID, script ) );
}

void KVirtualIfaceWidget::emitScriptUpEnabled( int state )
{
	emit( scriptUpEnabled( m_ifaceID, ( bool ) state ) );
}

void KVirtualIfaceWidget::emitScriptDownEnabled( int state )
{
	emit( scriptDownEnabled( m_ifaceID, ( bool ) state ) );
}

void KVirtualIfaceWidget::enableWidgets( int index )
{
	bool enabled = ( bool ) index;
	ui_widget.label_connect->setEnabled( enabled );
	ui_widget.label_mac->setEnabled( enabled );
	ui_widget.comboBox_type->setEnabled( enabled );
	ui_widget.lineEdit_file->setEnabled( enabled );
	ui_widget.lineEdit_mac->setEnabled( enabled );
	ui_widget.pushButton_hwaddr->setEnabled( enabled );

	if ( index )
	{
		enableScripts( ui_widget.comboBox_type->currentIndex() );
		enableScriptUp( ui_widget.checkBox_scriptup->isChecked() );
		enableScriptDown( ui_widget.checkBox_scriptdown->isChecked() );
	}

	else
	{
		enableScripts( 0 );
		ui_widget.checkBox_scriptup->setChecked( false );
		ui_widget.checkBox_scriptdown->setChecked( false );
	}
}

void KVirtualIfaceWidget::enableScripts( int index )
{
	bool enabled = false;

	if ( index == 1 ) enabled = true;

	ui_widget.checkBox_scriptdown->setEnabled( enabled );

	ui_widget.checkBox_scriptup->setEnabled( enabled );

	ui_widget.kurlrequester_scriptdown->setEnabled( enabled );

	ui_widget.kurlrequester_scriptup->setEnabled( enabled );

	ui_widget.lineEdit_file->setEnabled(( bool ) index );

	enableScriptUp( ui_widget.checkBox_scriptup->isChecked() );

	enableScriptDown( ui_widget.checkBox_scriptdown->isChecked() );
}

void KVirtualIfaceWidget::enableScriptUp( int state )
{
	ui_widget.kurlrequester_scriptup->setEnabled(( bool ) state );
}

void KVirtualIfaceWidget::enableScriptDown( int state )
{
	ui_widget.kurlrequester_scriptdown->setEnabled(( bool ) state );
}

#include "kvirtualifacewidget.moc"
// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;  replace-tabs off;  replace-tabs off;  replace-tabs off;  replace-tabs off;
