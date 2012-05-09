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
 *     (___)_)   File : kvirtualview.h                      ||--|| *         *
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

#ifndef KVIRTUALVIEW_H
#define KVIRTUALVIEW_H

#include <QtGui/QWidget>

#include "ui_kvirtualview_base.h"

#include "kvirtualoptions.h"

class QPainter;

class QButtonGroup;

class KUrl;

class KVirtualOptions;

/**
 * This is the main view class for KVirtual.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
 * @short Main view
 * @author Didier Fabert <didier.fabert@gmail.com>
 * @version 1.0.0
 */

class KVirtualView : public QWidget, public Ui::kvirtualview_base
{
	Q_OBJECT

public:
	/**
	 * Default constructor
	 */
	KVirtualView( QWidget *parent );

	/**
	 * Destructor
	 */
	virtual ~KVirtualView();

	/**
	 * Initialize options
	 */
	void initOptions( KVirtualOptions* );

	/**
	 * Load options from config
	 */
	void loadOptions();

private:
	Ui::kvirtualview_base _ui_kvirtualview_base;

	KVirtualOptions* m_options;
	QButtonGroup* m_displayGroup;

	QMap<uint, KVirtualStorageWidget*> m_storages;
	QMap<uint, KVirtualIfaceWidget*> m_ifaces;

signals:
	/**
	 * Use this signal to change the content of the statusbar
	 */
	void signalChangeStatusbar( const QString& text );

	/**
	 * Use this signal to change the content of the caption
	 */
	void signalChangeCaption( const QString& text );

	void signalChangeDistribution( const QString& distrib );

public slots:
	void addOutput( const QString & );
	void addError( const QString & );
	void toggleOutput();
	void setState( uint, bool );

private slots:
	void settingsChanged();
	void setIcon( const QString & );
	void syncDescription();
	void syncVideoCard();

};

#endif // _KVIRTUALVIEW_H_
// kate: indent-mode cstyle; replace-tabs off; tab-width 4;   replace-tabs off;
