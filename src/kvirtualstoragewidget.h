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
 *     (___)_)   File : kvirtualstoragewidget.h             ||--|| *         *
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


#ifndef KVIRTUALSTORAGEWIDGET_H
#define KVIRTUALSTORAGEWIDGET_H

#include "ui_kvirtualstorage_widget.h"


class KVirtualStorageWidget : public QWidget
{
    Q_OBJECT

public:

    KVirtualStorageWidget( QWidget *parent = 0 );
    virtual ~KVirtualStorageWidget();

public slots:
	void setStorageID( uint );
	uint getStorageID();
	void setType( const QString & );
	void setType( int );
	void setFile( const QString & );
	void setInterface( const QString & );

protected:
    Ui::KVirtualStorageView ui_widget;
	uint m_storageID;
	
	
protected slots:
	void emitFileChanged( const QString & );
	void emitTypeChanged( int );
	void emitInterfaceChanged( const QString & );
	void enableWidgets( int );
	
signals:
	void fileChanged( uint, const QString & );
	void typeChanged( uint, int );
	void interfaceChanged( uint, const QString & );
};

#endif // KVIRTUALSTORAGEWIDGET_H
// kate: indent-mode cstyle; replace-tabs off; tab-width 4;
