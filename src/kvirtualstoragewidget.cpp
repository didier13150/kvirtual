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
 *     (___)_)   File : kvirtualstoragewidget.cpp           ||--|| *         *
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

#include "kvirtualstoragewidget.h"

KVirtualStorageWidget::KVirtualStorageWidget( QWidget *parent )
		: QWidget( parent )
{
	ui_widget.setupUi( this );
	connect( ui_widget.kurlrequester_file,
	         SIGNAL( textChanged( QString ) ),
	         SLOT( emitFileChanged( QString ) )
	       );
	connect( ui_widget.comboBox_type,
	         SIGNAL( currentIndexChanged( QString ) ),
	         SLOT( emitTypeChanged( QString ) )
	       );
	connect( ui_widget.comboBox_interface,
	         SIGNAL( currentIndexChanged( QString ) ),
	         SLOT( emitInterfaceChanged( QString ) )
	       );
}

KVirtualStorageWidget::~KVirtualStorageWidget()
{
}

void KVirtualStorageWidget::setID( uint id )
{
	m_id = id;
}

uint KVirtualStorageWidget::getID()
{
	return m_id;
}


void KVirtualStorageWidget::enableWidgets( bool state )
{
	ui_widget.kurlrequester_file->setEnabled( state );
	ui_widget.comboBox_interface->setEnabled( state );
	ui_widget.label_interface->setEnabled( state );
	ui_widget.label_file->setEnabled( state );
}

void KVirtualStorageWidget::emitFileChanged( const QString &  file )
{
	emit( fileChanged( m_id, file ) );
}

void KVirtualStorageWidget::emitTypeChanged( const QString & type )
{
	if ( ui_widget.comboBox_type->currentIndex() )
	{
		enableWidgets( true );
		emit( typeChanged( m_id, type ) );
	}
	else
	{
		enableWidgets( false );
		emit( typeChanged( m_id, QString() ) );
	}
}

void KVirtualStorageWidget::emitInterfaceChanged( const QString & interface )
{
	if ( ui_widget.comboBox_interface->currentIndex() )
		emit( interfaceChanged( m_id, interface ) );
	else
		emit( interfaceChanged( m_id, QString() ) );
}

#include "kvirtualstoragewidget.moc"
// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;
