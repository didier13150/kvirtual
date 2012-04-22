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
 *     (___)_)   File :                                     ||--|| *         *
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

#ifndef KVIRTUAL_H
#define KVIRTUAL_H

#include <QProcess>

#include <KXmlGuiWindow>

#include "ui_prefs_base.h"

class QPrinter;
class KToggleAction;
class KUrl;

class KVirtualView;
class KVirtualOptions;
class KVirtualProcess;
class KSystemTrayIcon;
class KVirtualCreateImg;

/**
 * This class serves as the main window for KVirtual.  It handles the
 * menus, toolbars and status bars.
 *
 * @short Main window class
 * @author %{AUTHOR} <%{EMAIL}>
 * @version %{VERSION}
 */

class KVirtual : public KXmlGuiWindow
{
	Q_OBJECT

public:
	/**
	 * Default Constructor
	 */
	KVirtual();

	/**
	 * Default Destructor
	 */
	virtual ~KVirtual();

	/**
	 * Load vitual host file
	 */
	void load( const QString & filename );

private slots:
	void fileNew();
	void fileOpen();
	void fileSave();
	void fileSaveAs();
	void optionsPreferences();
	void startVirtual();
	void terminateVirtual();
	void killVirtual();
	void readStarted( uint );
	void readData( uint );
	void readError( uint );
	void closeProcess( uint, int, QProcess::ExitStatus );
	void startVde( const QString & );
	void setKvmExe( const QString & );
	void setVdeSwitchExe( const QString & );
	void setQemuImgCreator( const QString & );
	void showCreateVDiskDlg();
	void createVDisk( const QString &, const QString &, const QString & );

private:
	void setupActions();
	uint getID();

private:
	Ui::prefs_base ui_prefs_base ;
	KVirtualView *m_view;
	KVirtualCreateImg *m_create;
	KSystemTrayIcon *m_systray;
	QMap<uint, KVirtualProcess*> m_processes;
	uint m_id;
	KVirtualOptions* m_options;

	QPrinter   *m_printer;
	KToggleAction *m_toolbarAction;
	KToggleAction *m_statusbarAction;
	QString m_confFilename;

signals:
	void vmStateChanged( uint, bool );
};

#endif // _KVIRTUAL_H_
// kate: indent-mode cstyle; replace-tabs off; tab-width 4; 
