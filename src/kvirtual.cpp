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
 *     (___)_)   File : kvirtual.cpp                        ||--|| *         *
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

#include "kvirtual.h"
#include "kvirtualview.h"
#include "kvirtualprocess.h"
#include "kvirtualcreateimg.h"

#include "settings.h"

#include <QtGui/QDropEvent>
#include <QtGui/QPainter>
#include <QtGui/QPrinter>

#include <KConfigDialog>
#include <KStatusBar>

#include <KAction>
#include <KActionCollection>
#include <KStandardAction>
#include <KIconLoader>
#include <KStatusNotifierItem>
#include <KFileDialog>
#include <KLineEdit>
#include <KMessageBox>
#include <KStandardDirs>

#include <KLocale>


KVirtual::KVirtual()
		: KXmlGuiWindow(),
		m_view( new KVirtualView( this ) ),
		m_printer( 0 )
{
	m_id = 0;

	m_options = new KVirtualOptions();
	m_view->initOptions( m_options );
	connect( m_view,
			 SIGNAL( signalChangeDistribution( QString ) ),
			 SLOT( changeIcon( QString ) )
		   );

	// accept dnd
	setAcceptDrops( true );

	// tell the KXmlGuiWindow that this is indeed the main widget
	setCentralWidget( m_view );

	// then, setup our actions
	setupActions();

	// add a status bar
	statusBar()->show();

	// a call to KXmlGuiWindow::setupGUI() populates the GUI
	// with actions, using KXMLGUI.
	// It also applies the saved mainwindow settings, if any, and ask the
	// mainwindow to automatically save settings if changed: window size,
	// toolbar position, icon size, etc.
	setupGUI();

	m_systray = new KStatusNotifierItem( this );
	m_systray->setStandardActionsEnabled( true );
	m_systray->activate();
	m_systray->setStatus( KStatusNotifierItem::Passive );
	changeIcon( m_options->getDistrib() );

	m_create = new KVirtualCreateImg( this );
}

KVirtual::~KVirtual()
{
	QMapIterator<uint, KVirtualProcess*> it( m_processes );

	terminateAll();
	sleep( 1 );
	killAll();

	while ( it.hasNext() )
	{
		it.next();

		if ( it.value() )
		{
			delete it.value();
		}
	}

	delete m_systray;

	delete m_create;
}

void KVirtual::terminateAll()
{
	QMapIterator<uint, KVirtualProcess*> it( m_processes );

	while ( it.hasNext() )
	{
		it.next();

		if ( it.value() )
		{
			if ( it.value()->state() != QProcess::NotRunning )
			{
				it.value()->terminate();
			}
		}
	}
}

void KVirtual::killAll()
{
	QMapIterator<uint, KVirtualProcess*> it( m_processes );

	while ( it.hasNext() )
	{
		it.next();

		if ( it.value() )
		{
			if ( it.value()->state() != QProcess::NotRunning )
			{
				it.value()->kill();
			}
		}
	}
}

void KVirtual::setupActions()
{
	KStandardAction::openRecent( this, SLOT( fileNew() ), actionCollection() );
	KStandardAction::openNew( this, SLOT( fileNew() ), actionCollection() );
	KStandardAction::open( this, SLOT( fileOpen() ), actionCollection() );
	KStandardAction::save( this, SLOT( fileSave() ), actionCollection() );
	KStandardAction::saveAs( this, SLOT( fileSaveAs() ), actionCollection() );
	KStandardAction::quit( qApp, SLOT( quit() ), actionCollection() );

	KStandardAction::preferences( this, SLOT( optionsPreferences() ), actionCollection() );

	// custom menu and menu item - the slot is in the class KVirtualView
	KAction *runvm = new KAction( KIcon( "run-build" ), i18n( "Start VM" ), this );
	actionCollection()->addAction( QLatin1String( "start_vm" ), runvm );
	connect( runvm, SIGNAL( triggered( bool ) ), this, SLOT( startVirtual() ) );

	KAction *terminatevm = new KAction( KIcon( "dialog-close" ), i18n( "Terminate VM" ), this );
	actionCollection()->addAction( QLatin1String( "terminate_vm" ), terminatevm );
	connect( terminatevm, SIGNAL( triggered( bool ) ), this, SLOT( terminateVirtual() ) );

	KAction *killvm = new KAction( KIcon( "application-exit" ), i18n( "Kill VM" ), this );
	actionCollection()->addAction( QLatin1String( "kill_vm" ), killvm );
	connect( killvm, SIGNAL( triggered( bool ) ), this, SLOT( killVirtual() ) );

	// custom menu and menu item - the slot is in the class KVirtualView
	KAction *output = new KAction( KIcon( "view-process-system" ), i18n( "Show/Hide Output" ), this );
	actionCollection()->addAction( QLatin1String( "toggle_output" ), output );
	connect( output, SIGNAL( triggered( bool ) ), m_view, SLOT( toggleOutput() ) );

	// custom menu and menu item - the slot is in the class KVirtualView
	KAction *vdisk = new KAction( KIcon( "document-export-table" ), i18n( "Create a new virtual disk image" ), this );
	actionCollection()->addAction( QLatin1String( "new_vdisk" ), vdisk );
	connect( vdisk, SIGNAL( triggered( bool ) ), SLOT( showCreateVDiskDlg() ) );

	// test
	/*KAction *test = new KAction( KIcon( "edit-find" ), i18n( "Test config" ), this );
	actionCollection()->addAction( QLatin1String( "test_config" ), test );
	connect( test, SIGNAL( triggered( bool ) ), SLOT( exitCalled() ) );*/


	connect( this, SIGNAL( vmStateChanged( uint, bool ) ), m_view, SLOT( setState( uint, bool ) ) );
}

void KVirtual::changeIcon( const QString & distrib )
{
	QString img = KStandardDirs::locate( "appdata", distrib + ".png" );

	if ( img.isNull() )
	{
		img = KStandardDirs::locate( "appdata", "linux.png" );
	}

	m_systray->setIconByName( img );
}

bool KVirtual::queryClose() //exitCalled()
{
	hide();
	return false;
}

bool KVirtual::queryExit() //exitCalled()
{
	if ( m_options->isModified( m_confFilename ) )
	{
		switch ( KMessageBox::warningYesNoCancel( this,
				 i18n( "Save changes to virtual host definition document ?" ) ) )
		{

			case KMessageBox::Yes:
			{
				// save document here. If saving fails, return false;
				m_options->save( m_confFilename );
				break;
			}

			case KMessageBox::No:
			{
				break;
			}

			default: // cancel
			{
				return false;
			}
		}
	}

	return true;
}

void KVirtual::showCreateVDiskDlg()
{
	connect( m_create,
			 SIGNAL( accepted( const QString &, const QString &, const QString & ) ),
			 SLOT( createVDisk( const QString &, const QString &, const QString & ) )
		   );
	m_create->show();
}

void KVirtual::createVDisk( const QString & file, const QString & type, const QString & size )
{
	uint id = getID();
	KVirtualProcess * process = new KVirtualProcess( id, KVirtualProcess::CREATE_IMG );
	QStringList opts;
	QString buffer;

	opts << "create";
	opts << "-f" << type;
	opts << file;
	opts << size;

	process->setProgram( Settings::exeQemuImgCreator(), opts );
	process->setOutputChannelMode( KProcess::SeparateChannels );
	connect( process,
			 SIGNAL( readyReadStandardOutput( uint ) ),
			 SLOT( readData( uint ) )
		   );
	connect( process,
			 SIGNAL( readyReadStandardError( uint ) ),
			 SLOT( readError( uint ) )
		   );
	connect( process,
			 SIGNAL( finished( uint, int, QProcess::ExitStatus ) ),
			 SLOT( closeProcess( uint, int, QProcess::ExitStatus ) )
		   );
	connect( process,
			 SIGNAL( started( uint ) ),
			 SLOT( readStarted( uint ) )
		   );

	m_view->addOutput( process->program().join( " " ) );
	process->start();
	m_processes[id] = process;

	if ( process->error() == QProcess::FailedToStart || process->state() == QProcess::NotRunning )
	{
		buffer.setNum( id );
		buffer.prepend( "Process" );
		buffer.append( " failed to start" );
		m_view->addError( buffer );
	}
}

void KVirtual::load( const QString & filename )
{
	m_confFilename = filename;
	m_options->load( m_confFilename );
	m_view->loadOptions();
}

void KVirtual::fileNew()
{
	// this slot is called whenever the File->New menu is selected,
	// the New shortcut is pressed (usually CTRL+N) or the New toolbar
	// button is clicked

	// create a new window
	( new KVirtual )->show();
}

void KVirtual::fileOpen()
{
	KUrl url = KUrl::fromPath( QDir::homePath() );

	load( KFileDialog::getOpenFileName( url, "*.xml", this ) );
}

void KVirtual::fileSave()
{
	if ( m_confFilename.isNull() )
	{
		fileSaveAs();
		return;
	}

	m_options->save( m_confFilename );
}

void KVirtual::fileSaveAs()
{
	KUrl url = KUrl::fromPath( QDir::homePath() );

	m_confFilename = KFileDialog::getSaveFileName( url, "*.xml", this );
	m_options->save( m_confFilename );
}

void KVirtual::optionsPreferences()
{
	// The preference dialog is derived from prefs_base.ui
	//
	// compare the names of the widgets in the .ui file
	// to the names of the variables in the .kcfg file
	//avoid to have 2 dialogs shown
	if ( KConfigDialog::showDialog( "settings" ) )
	{
		return;
	}

	KConfigDialog *dialog = new KConfigDialog( this, "settings", Settings::self() );

	QWidget *generalSettingsDlg = new QWidget;
	ui_prefs_base.setupUi( generalSettingsDlg );
	ui_prefs_base.kurlrequester_exeKvm->lineEdit()->setText( Settings::exeKvm() );
	ui_prefs_base.kurlrequester_exeVdeSwitch->lineEdit()->setText( Settings::exeVdeSwitch() );
	ui_prefs_base.kurlrequester_exeQemuImgCreator->lineEdit()->setText( Settings::exeQemuImgCreator() );
	dialog->addPage( generalSettingsDlg, i18n( "Executable" ), "run-build" );
	connect( dialog, SIGNAL( settingsChanged( QString ) ), m_view, SLOT( settingsChanged() ) );
	connect( ui_prefs_base.kurlrequester_exeKvm->lineEdit(),
			 SIGNAL( textEdited( const QString & ) ),
			 SLOT( setKvmExe( const QString & ) )
		   );
	connect( ui_prefs_base.kurlrequester_exeVdeSwitch->lineEdit(),
			 SIGNAL( textEdited( const QString & ) ),
			 SLOT( setVdeSwitchExe( const QString & ) )
		   );
	connect( ui_prefs_base.kurlrequester_exeQemuImgCreator->lineEdit(),
			 SIGNAL( textEdited( const QString & ) ),
			 SLOT( setQemuImgCreator( const QString & ) )
		   );
	dialog->setAttribute( Qt::WA_DeleteOnClose );
	dialog->show();
}

void KVirtual::setKvmExe( const QString & exe )
{
	Settings::setExeKvm( exe );
}

void KVirtual::setVdeSwitchExe( const QString & exe )
{
	Settings::setExeVdeSwitch( exe );
}

void KVirtual::setQemuImgCreator( const QString & exe )
{
	Settings::setExeQemuImgCreator( exe );
}

uint KVirtual::getID()
{
	return m_id++;
}

void KVirtual::startVde( const QString & vswitch )
{
	uint id = getID();
	KVirtualProcess * process = new KVirtualProcess( id, KVirtualProcess::SWITCH );
	QStringList args;
	QDir dir( vswitch );
	QString buffer;

	if ( dir.exists() )
	{
		m_view->addError( "Virtual switch is already exists but is not handled by me" );
		return;
	}

	args << "-F" << "-sock" << vswitch;

	process->setProgram( Settings::exeVdeSwitch(), args );
	process->setOutputChannelMode( KProcess::SeparateChannels );
	connect( process,
			 SIGNAL( readyReadStandardOutput( uint ) ),
			 SLOT( readData( uint ) )
		   );
	connect( process,
			 SIGNAL( readyReadStandardError( uint ) ),
			 SLOT( readError( uint ) )
		   );
	connect( process,
			 SIGNAL( finished( uint, int, QProcess::ExitStatus ) ),
			 SLOT( closeProcess( uint, int, QProcess::ExitStatus ) )
		   );
	connect( process,
			 SIGNAL( started( uint ) ),
			 SLOT( readStarted( uint ) )
		   );

	m_view->addOutput( process->program().join( " " ) );
	process->start();
	m_processes[id] = process;

	if ( process->error() == QProcess::FailedToStart || process->state() == QProcess::NotRunning )
	{
		buffer.setNum( id );
		buffer.prepend( "Process" );
		buffer.append( " failed to start" );
		m_view->addError( buffer );
	}
}

void KVirtual::terminateVirtual()
{
	QList<uint>::ConstIterator it;
	QList<uint> keys;

	keys = m_processes.keys();

	for ( it = keys.begin() ; it != keys.end() ; ++it )
	{
		//if( m_processes[*it]->getVirtualType() != KVirtualProcess::HOST ) continue;
		if ( m_processes[*it] &&  m_processes[*it]->state() != QProcess::NotRunning )
		{
			m_processes[*it]->terminate();
		}
	}
}

void KVirtual::killVirtual()
{
	QList<uint>::ConstIterator it;
	QList<uint> keys;

	keys = m_processes.keys();

	for ( it = keys.begin() ; it != keys.end() ; ++it )
	{
		//if( m_processes[*it]->getVirtualType() != KVirtualProcess::HOST ) continue;
		if ( m_processes[*it] &&  m_processes[*it]->state() != QProcess::NotRunning )
		{
			m_processes[*it]->terminate();
		}
	}
}

void KVirtual::startVirtual()
{
	uint id = getID();
	KVirtualProcess * process = new KVirtualProcess( id, KVirtualProcess::HOST );
	QStringList vswitch;
	QString buffer;

	process->setProgram( Settings::exeKvm(), m_options->getArgs() );
	process->setOutputChannelMode( KProcess::SeparateChannels );
	connect( process,
			 SIGNAL( readyReadStandardOutput( uint ) ),
			 SLOT( readData( uint ) )
		   );
	connect( process,
			 SIGNAL( readyReadStandardError( uint ) ),
			 SLOT( readError( uint ) )
		   );
	connect( process,
			 SIGNAL( finished( uint, int, QProcess::ExitStatus ) ),
			 SLOT( closeProcess( uint, int, QProcess::ExitStatus ) )
		   );
	connect( process,
			 SIGNAL( started( uint ) ),
			 SLOT( readStarted( uint ) )
		   );

	vswitch = m_options->getNeededVirtualSwitch();

	for ( QStringList::Iterator it = vswitch.begin() ; it != vswitch.end() ; ++it )
	{
		m_view->addOutput( "Need a virtual switch: " + *it );
		startVde( *it );
	}

	m_view->addOutput( process->program().join( " " ) );

	process->start();
	m_processes[id] = process;

	if ( process->error() == QProcess::FailedToStart || process->state() == QProcess::NotRunning )
	{
		buffer.setNum( id );
		buffer.prepend( "Process" );
		buffer.append( " failed to start" );
		m_view->addError( buffer );
	}
}

void KVirtual::readStarted( uint id )
{
	KVirtualProcess* process = 0;
	QString buffer, message;

	if ( ! m_processes.contains( id ) || m_processes[id] == 0 )
	{
		return;
	}

	process = m_processes[id];

	if ( process->getVirtualType() == KVirtualProcess::HOST )
	{
		emit( vmStateChanged( id, true ) );
		m_systray->setStatus( KStatusNotifierItem::Active );
	}

	buffer.setNum( id );

	buffer.prepend( "Process" );
	QString buf2;
	buf2.setNum( process->pid() );
	buffer.append( " process started, PID is " + buf2 );
	m_view->addOutput( buffer );

}

void KVirtual::readData( uint id )
{
	KVirtualProcess* process = 0;
	QString buffer, message;

	if ( ! m_processes.contains( id ) || m_processes[id] == 0 )
	{
		return;
	}

	process = m_processes[id];

	buffer.setNum( id );
	buffer.prepend( "Process" );
	buffer += " " + QString( process->readAllStandardOutput() );
	m_view->addOutput( buffer );
}

void KVirtual::readError( uint id )
{
	KVirtualProcess* process = 0;
	QString buffer, message;

	if ( ! m_processes.contains( id ) || m_processes[id] == 0 )
	{
		return;
	}

	process = m_processes[id];

	buffer.setNum( id );
	buffer.prepend( "Process" );
	buffer += " " + QString( process->readAllStandardError() );
	m_view->addError( buffer );
}

void KVirtual::closeProcess( uint id, int retval, QProcess::ExitStatus status )
{
	KVirtualProcess* process = 0;
	QString buffer, message, description;

	if ( ! m_processes.contains( id ) || m_processes[id] == 0 )
	{
		return;
	}

	process = m_processes.take( id );

	buffer.setNum( id );
	buffer.prepend( "Process" );

	switch ( status )
	{

		case QProcess::NormalExit:
		{
			description = "Exit normally";
			break;
		}

		case QProcess::CrashExit:
		{
			description = "Crash";
			break;
		}
	}

	message += buffer + " terminated with retval ";

	buffer.setNum( retval );
	message += buffer + " ( " + description + " )";
	m_view->addOutput( message );
	disconnect( process,
				SIGNAL( readyReadStandardOutput( uint ) ) );
	disconnect( process,
				SIGNAL( readyReadStandardError( uint ) ) );
	disconnect( process,
				SIGNAL( finished( uint, int, QProcess::ExitStatus ) ) );

	if ( process->getVirtualType() == KVirtualProcess::HOST )
	{
		emit( vmStateChanged( id, false ) );
		m_systray->setStatus( KStatusNotifierItem::Passive );
	}

	delete process;
}

#include "kvirtual.moc"
// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;      replace-tabs off;
