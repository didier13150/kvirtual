/***************************************************************************
 *   Copyright (C) %{CURRENT_YEAR} by %{AUTHOR} <%{EMAIL}>                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef KVIRTUALOPTIONS_H
#define KVIRTUALOPTIONS_H

#include <QStringList>
#include <QMap>

class KVirtualDevice
{
public:
	KVirtualDevice();
	KVirtualDevice( const QString &, const QString & );
	KVirtualDevice( const QString &, const QString &, const QString &, const QString & );
	~KVirtualDevice();

	const QString & getType() const;
	const QString & getFile() const;
	const QString & getModel() const;
	const QString & getHardwareAddress() const;

protected:
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
		BOOT_ON_DISK,
		BOOT_ON_NETWORK,
		BOOT_ON_CDROM,
		BOOT_ON_FLOPPY,
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
	const QString & getKvmExec() const;
	const QString & getVdeSwitchExec() const;

public slots:
	void setName( const QString & );
	void setDistrib( const QString & );
	void setDescription( const QString & );
	void setIface( uint, const QString &, const QString &, const QString &, const QString & );
	void setStorage( uint, const QString &, const QString & );
	void setUsbSupported( bool );
	void setSnapshotEnabled( bool );
	void setVideoCard( const QString & );
	void setMemory( uint );
	void setNbCPU( uint );
	void setBootDevice( BootOrder );
	void setKeyboard( const QString & );
	void setVncPort( uint );
	void setDisplay( Display );
	void load( const QString & file );
	void save( const QString & file );
	void setUsedSwitch( const QString & );
	void clear();
	void setKvmExec( const QString & );
	void setVdeSwitchExec( const QString & );

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

	QString m_kvmExec;
	QString m_vdeExec;

signals:
	void errorEncoured( const QString & );
};

inline QString DisplayToString( KVirtualOptions::Display );
inline QString BootOrderToString( KVirtualOptions::BootOrder );
inline KVirtualOptions::Display DisplayFromString( const QString & );
inline KVirtualOptions::BootOrder BootOrderFromString( const QString & );

#endif // _KVIRTUALOPTIONS_H_
// kate: indent-mode cstyle; replace-tabs off; tab-width 4;
