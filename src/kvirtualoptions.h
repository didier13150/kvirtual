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
 *     (___)_)   File : kvirtualoptions.h                   ||--|| *         *
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

#ifndef KVIRTUALOPTIONS_H
#define KVIRTUALOPTIONS_H

#include <QStringList>
#include <QMap>

class KVirtualDevice
{
public:

	typedef enum {
		NONE    = 0,
		DISK    = 1,
		CDROM   = 2,
		FLOPPY  = 3,
	}StorageType;
	
	KVirtualDevice();
	KVirtualDevice( const StorageType type , const QString & );
	KVirtualDevice( const QString &, const QString &, const QString &, const QString & );
	~KVirtualDevice();

	int getStorageType() const;
	const QString & getType() const;
	const QString & getFile() const;
	const QString & getModel() const;
	const QString & getHardwareAddress() const;

protected:
	StorageType m_storageType;
	QString m_type;
	QString m_file;
	QString m_model;
	QString m_hwaddr;
};


class KVirtualOptions
{
public:
	/**
	 * Default Constructor
	 */
	KVirtualOptions();

	/**
	 * Default Destructor
	 */
	virtual ~KVirtualOptions();

	typedef enum {
		DISPLAY_DIRECT,
		DISPLAY_VNC,
		DISPLAY_NONE,
	}Display;

	typedef enum {
		BOOT_ON_DISK    = 0,
		BOOT_ON_CDROM   = 1,
		BOOT_ON_FLOPPY  = 2,
		BOOT_ON_NETWORK = 3,
	}BootOrder;

	QStringList getNeededVirtualSwitch();

	const QStringList & getArgs();

	const QString & getName() const;
	const QString & getDistrib() const;
	const QString & getDescription() const;
	const QString & getKeyboard() const;
	uint getMemory() const;
	uint getVncPort() const;
	uint getNbCPU() const;
	Display getDisplay() const;
	BootOrder getBootDevice() const;
	KVirtualDevice* getStorage( uint ) const;
	KVirtualDevice* getIface( uint ) const;
	const QString & getVideoCard() const;
	bool isUsbSupported() const;
	bool isSnapshotEnabled() const;

public slots:
	void setName( const QString & );
	void setDistrib( const QString & );
	void setDescription( const QString & );
	void setIface( uint, const QString &, const QString &, const QString &, const QString & );
	void setStorage( uint, const KVirtualDevice::StorageType , const QString & );
	void setStorage( uint, const int , const QString & );
	void setUsbSupported( bool );
	void setSnapshotEnabled( bool );
	void setVideoCard( const QString & );
	void setMemory( uint );
	void setNbCPU( uint );
	void setBootDevice( BootOrder );
	void setBootDevice( int );
	void setKeyboard( const QString & );
	void setVncPort( uint );
	void setDisplay( Display );
	void load( const QString & file );
	void save( const QString & file );
	void setUsedSwitch( const QString & );
	void clear();

protected:
	QStringList m_opts;
	QStringList m_usedSwitches;
	QMap<uint, KVirtualDevice*> m_storages;
	QMap<uint, KVirtualDevice*> m_ifaces;
	QString m_file;
	QString m_name;
	QString m_distrib;
	QString m_description;
	QString m_videoCard;
	BootOrder m_bootDevice;
	QString m_keyboard;
	Display m_display;
	uint m_vncport;
	uint m_memory;
	uint m_cpus;
	bool m_usb;
	bool m_snapshot;

signals:
	void errorEncoured( const QString & );
};

inline QString DisplayToString( KVirtualOptions::Display );
inline QString BootOrderToString( KVirtualOptions::BootOrder );
inline KVirtualOptions::Display DisplayFromString( const QString & );
inline KVirtualOptions::BootOrder BootOrderFromString( const QString & );

#endif // _KVIRTUALOPTIONS_H_
// kate: indent-mode cstyle; replace-tabs off; tab-width 4;
