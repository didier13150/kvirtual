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
 *     (___)_)   File : kvirtualifacewidget.h               ||--|| *         *
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


#ifndef KVIRTUALIFACEWIDGET_H
#define KVIRTUALIFACEWIDGET_H

#include "ui_kvirtualiface_widget.h"

class KVirtualIfaceWidget : public QWidget
{
    Q_OBJECT

public:
    KVirtualIfaceWidget( QWidget *parent = 0 );
    virtual ~KVirtualIfaceWidget();

public slots:
	void setIfaceID( uint );
	uint getIfaceID();
	void setFile( const QString & );
	void setType( const QString & );
	void setModel( const QString & );
	void setHwAddr( const QString & );
	void setScriptUp( const QString & );
	void setScriptDown( const QString & );
	void setScriptUpEnabled( bool );
	void setScriptDownEnabled( bool );
	void setRandomHwAddr();
	
protected:
    Ui::KVirtualIfaceView ui_widget;
	uint m_ifaceID;

protected slots:
	void emitFileChanged( const QString & );
	void emitTypeChanged( const QString & );
	void emitModelChanged( const QString & );
	void emitHwAddrChanged( const QString & );
	void emitScriptUpChanged( const QString & );
	void emitScriptDownChanged( const QString & );
	void emitScriptUpEnabled( int );
	void emitScriptDownEnabled( int );
	void enableWidgets( int );
	void enableScriptUp( int );
	void enableScriptDown( int );
	void enableScripts( int );
	
signals:
	void fileChanged( uint, const QString & );
	void typeChanged( uint, const QString & );
	void modelChanged( uint, const QString & );
	void hwAddrChanged( uint, const QString & );
	void scriptUpChanged( uint, const QString & );
	void scriptDownChanged( uint, const QString & );
	void scriptUpEnabled( uint, bool );
	void scriptDownEnabled( uint, bool );
};

#endif // KVIRTUALIFACEWIDGET_H
// kate: indent-mode cstyle; replace-tabs off; tab-width 4;
