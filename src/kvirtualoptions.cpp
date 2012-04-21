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

#include "kvirtualoptions.h"
#include <QList>
#include <QDebug>
#include <QDomDocument>
#include <QFile>

KVirtualDevice::KVirtualDevice()
{
}

KVirtualDevice::KVirtualDevice( const QString & type, const QString & file )
{
	m_type = type;
	m_file = file;
}

KVirtualDevice::KVirtualDevice( const QString & type,
                                const QString & file,
                                const QString & model,
                                const QString & hwaddr
                              )
{
	m_type = type;
	m_file = file;
	m_model = model;
	m_hwaddr = hwaddr;
}

KVirtualDevice::~KVirtualDevice()
{
}

const QString & KVirtualDevice::getType() const
{
	return m_type;
}

const QString & KVirtualDevice::getFile() const
{
	return m_file;
}

const QString & KVirtualDevice::getModel() const
{
	return m_model;
}

const QString & KVirtualDevice::getHardwareAddress() const
{
	return m_hwaddr;
}

/***************************************************************************/
KVirtualOptions::KVirtualOptions()
{
	clear();
}

KVirtualOptions::~KVirtualOptions()
{

}

void KVirtualOptions::clear()
{
	m_usb = false;
	m_snapshot = false;
	m_memory = 128;
	m_display = KVirtualOptions::DISPLAY_DIRECT;
	m_bootDevice = KVirtualOptions::BOOT_ON_DISK;
	m_vncport = 1;
	m_keyboard = "fr";
	m_videoCard = "std";
	m_description.clear();
	m_name.clear();
	m_distrib = "centos";
	m_storages.clear();
	m_ifaces.clear();
	m_opts.clear();

}

QStringList KVirtualOptions::getNeededVirtualSwitch()
{
	QStringList needed;
	QString buffer;

	QList<uint>::ConstIterator it;
	QList<uint> keys = m_ifaces.keys();

	for ( it = keys.begin() ; it != keys.end() ; ++it )
	{
		if ( m_ifaces[*it]->getType() == "vde" )
		{
			buffer = m_ifaces[*it]->getFile();

			if ( not m_usedSwitches.contains( buffer ) )
				needed << buffer;
		}
	}

	return needed;
}

const QStringList & KVirtualOptions::getArgs()
{
	QString buffer, id;
	QList<uint>::ConstIterator it;
	QList<uint> keys;

	m_opts.clear();
	buffer.setNum( m_memory );
	m_opts << "-m" << buffer;

	if ( m_cpus > 1 )
	{
		buffer.setNum( m_cpus );
		m_opts << "-smp" << buffer;
	}

	keys = m_storages.keys();

	for ( it = keys.begin() ; it != keys.end() ; ++it )
	{
		buffer = "file=" + m_storages[*it]->getFile() + ",media=" + m_storages[*it]->getType();
		m_opts << "-drive" << buffer;
	}

	keys = m_ifaces.keys();

	for ( it = keys.begin() ; it != keys.end() ; ++it )
	{
		id.setNum( *it );
		buffer = "nic,macaddr=" + m_ifaces[*it]->getHardwareAddress() + ",model=" + m_ifaces[*it]->getModel() + ",vlan=" + id;
		m_opts << "-net" << buffer;

		buffer = m_ifaces[*it]->getType();

		if ( buffer == "tap" )
		{
			buffer += ",ifname=" + m_ifaces[*it]->getFile() + ",script=no";
		}
		else if ( buffer == "vde" )
		{
			buffer += ",sock=" + m_ifaces[*it]->getFile();
		}

		buffer += ",vlan=" + id;

		m_opts << "-net" << buffer;
	}

	switch ( m_display )
	{

		case KVirtualOptions::DISPLAY_DIRECT:
		{
			buffer = "sdl";
			break;
		}

		case KVirtualOptions::DISPLAY_VNC:
		{
			buffer.setNum( m_vncport );
			buffer.prepend( "vnc=:" );
			break;
		}

		default:
		{
			buffer = "none";
			break;
		}

	}

	m_opts << "-display" << buffer;

	if ( m_usb )
		m_opts << "-usb";

	if ( m_snapshot )
		m_opts << "-snapshot";

	if ( m_keyboard.isEmpty() )
		m_opts << "-k" << m_keyboard;

	return m_opts;
}

void KVirtualOptions::setName( const QString & name )
{
	m_name = name;
}

void KVirtualOptions::setDistrib( const QString & distrib )
{
	m_distrib = distrib;
}

void KVirtualOptions::setDescription( const QString & desc )
{
	m_description = desc;
}

void KVirtualOptions::setMemory( uint mem )
{
	m_memory = mem;
}

void KVirtualOptions::setNbCPU( uint nb )
{
	m_cpus = nb;
}

void KVirtualOptions::setStorage( uint id, const QString & type, const QString & file )
{
	if ( m_storages.contains( id ) )
	{
		delete m_storages[id];
	}

	KVirtualDevice* storage = new KVirtualDevice( type, file );

	m_storages[id] = storage;
}

void KVirtualOptions::setIface( uint id,
                                const QString & type,
                                const QString & file,
                                const QString & model,
                                const QString & mac
                              )
{
	if ( m_ifaces.contains( id ) )
	{
		delete m_ifaces[id];
	}

	KVirtualDevice* iface = new KVirtualDevice( type, file, model, mac );

	m_ifaces[id] = iface;
}

void KVirtualOptions::setUsbSupported( bool state )
{
	m_usb = state;
}

void KVirtualOptions::setSnapshotEnabled( bool enable )
{
	m_snapshot = enable;
}

void KVirtualOptions::setVideoCard( const QString & model )
{
	m_videoCard = model;
}

void KVirtualOptions::setBootDevice( KVirtualOptions::BootOrder device )
{
	m_bootDevice = device;
}

void KVirtualOptions::setKeyboard( const QString & keyboard )
{
	m_keyboard = keyboard;
}

void KVirtualOptions::setVncPort( uint port )
{
	m_vncport = port;
}

void KVirtualOptions::setDisplay( KVirtualOptions::Display type )
{
	m_display = type;
}

void KVirtualOptions::setUsedSwitch( const QString & vswitch )
{
	if ( not m_usedSwitches.contains( vswitch ) )
		m_usedSwitches << vswitch;
}

void KVirtualOptions::load( const QString & filename )
{
	if ( filename.isNull() )
	{
		qDebug() << "filename is null";
		return;
	}

	QDomDocument doc;

	QDomElement root, element, element2;
	QDomNode node, child;
	QString buffer;

	QFile file( filename );

	if ( !file.open( QIODevice::ReadOnly ) )
	{
		qDebug() << "Can't open filename in read mode";
		return;
	}

	if ( !doc.setContent( &file ) )
	{
		file.close();
		qDebug() << "Can't parse XML file";
		return;
	}

	file.close();

	root = doc.documentElement();

	if ( root.tagName() != "host" )
	{
		qDebug() << "Wrong root node" << root.tagName();
		return;
	}
	setDistrib( root.attribute( "distribution" ) );

	node = root.firstChild();

	while ( ! node.isNull() )
	{
		element = node.toElement();

		if ( ! element.isNull() )
		{
			if ( element.tagName() == "name" )
			{
				setName( element.text() );
			}

			if ( element.tagName() == "description" )
			{
				setDescription( element.text() );
			}

			if ( element.tagName() == "system" )
			{
				child = element.firstChild();

				while ( ! child.isNull() )
				{
					element2 = child.toElement();

					if ( element2.tagName() == "memory" )
					{
						setMemory( element2.attribute( "value" ).toUInt() );
					}

					if ( element2.tagName() == "boot" )
					{
						buffer = element2.attribute( "device" );
						setBootDevice( BootOrderFromString( buffer ) );
					}

					if ( element2.tagName() == "usb" )
					{
						setUsbSupported( element2.attribute( "enabled" ).toInt() );
					}

					if ( element2.tagName() == "snapshot" )
					{
						setSnapshotEnabled( element2.attribute( "enabled" ).toInt() );
					}

					if ( element2.tagName() == "core" )
					{
						setNbCPU( element2.attribute( "value" ).toUInt() );
					}

					child = child.nextSibling();
				}
			}

			if ( element.tagName() == "display" )
			{
				buffer = element.attribute( "method" );
				setDisplay( DisplayFromString( buffer ) );
				child = element.firstChild();

				while ( ! child.isNull() )
				{
					element2 = child.toElement();

					if ( element2.tagName() == "card" )
					{
						setVideoCard( element2.attribute( "model" ) );
					}

					if ( element2.tagName() == "vnc" )
					{
						setVncPort( element2.attribute( "port" ).toUInt() );
					}

					if ( element2.tagName() == "keyboard" )
					{
						setKeyboard( element2.attribute( "model" ) );
					}

					child = child.nextSibling();
				}
			}

			if ( element.tagName() == "storages" )
			{
				child = element.firstChild();

				while ( ! child.isNull() )
				{
					element2 = child.toElement();

					if ( element2.tagName() == "storage" )
					{
						setStorage( element2.attribute( "id" ).toUInt(),
						            element2.attribute( "type" ),
						            element2.attribute( "file" )
						          );
					}

					child = child.nextSibling();
				}
			}

			if ( element.tagName() == "network" )
			{
				child = element.firstChild();

				while ( ! child.isNull() )
				{
					element2 = child.toElement();

					if ( element2.tagName() == "iface" )
					{
						setIface( element2.attribute( "id" ).toUInt(),
						          element2.attribute( "type" ),
						          element2.attribute( "file" ),
						          element2.attribute( "model" ),
						          element2.attribute( "addr" )
						        );
					}

					child = child.nextSibling();
				}
			}
		}

		node = node.nextSibling();
	}
}

void KVirtualOptions::save( const QString & filename )
{
	if ( filename.isNull() )
	{
		return;
	}

	QDomDocument doc;

	QDomElement root, element, element2;
	QDomNode node, child;
	QDomCDATASection cdata;
	QString buffer;
	QList<uint> keys;
	QList<uint>::ConstIterator it;

	QFile file( filename );

	if ( !file.open( QIODevice::WriteOnly ) ) return;

	root = doc.createElement( "host" );

	root.setAttribute( "distribution", getDistrib() );

	doc.appendChild( root );

	element = doc.createElement( "name" );

	root.appendChild( element );

	cdata = doc.createCDATASection( getName() );

	element.appendChild( cdata );

	element = doc.createElement( "description" );

	root.appendChild( element );

	cdata = doc.createCDATASection( getDescription() );

	element.appendChild( cdata );

	element = doc.createElement( "system" );

	root.appendChild( element );

	element2 = doc.createElement( "memory" );

	element2.setAttribute( "value", buffer.setNum( getMemory() ) );

	element2.setAttribute( "unit", "mb" );

	element.appendChild( element2 );

	element2 = doc.createElement( "boot" );

	element2.setAttribute( "device", BootOrderToString( getBootDevice() ) );

	element.appendChild( element2 );

	element2 = doc.createElement( "usb" );

	element2.setAttribute( "enabled", buffer.setNum( isUsbSupported() ) );

	element.appendChild( element2 );

	element2 = doc.createElement( "snapshot" );

	element2.setAttribute( "enabled", buffer.setNum( isSnapshotEnabled() ) );

	element.appendChild( element2 );

	element2 = doc.createElement( "core" );

	element2.setAttribute( "value", buffer.setNum( getNbCPU() ) );

	element.appendChild( element2 );

	element = doc.createElement( "display" );

	element.setAttribute( "method", DisplayToString( getDisplay() ) );

	root.appendChild( element );

	element2 = doc.createElement( "card" );

	element2.setAttribute( "model", getVideoCard() );

	element.appendChild( element2 );

	element2 = doc.createElement( "vnc" );

	element2.setAttribute( "port", buffer.setNum( getVncPort() ) );

	element.appendChild( element2 );

	element2 = doc.createElement( "keyboard" );

	element2.setAttribute( "model", getKeyboard() );

	element.appendChild( element2 );

	//TODO not implemented. Is it very usefull ?
	element2 = doc.createElement( "memory" );

	element2.setAttribute( "value", "8" );

	element2.setAttribute( "unit", "mb" );

	element.appendChild( element2 );

	element = doc.createElement( "storages" );

	root.appendChild( element );

	keys = m_storages.keys();

	for ( it = keys.begin() ; it != keys.end() ; ++it )
	{
		element2 = doc.createElement( "storage" );
		element2.setAttribute( "id", buffer.setNum( *it ) );
		element2.setAttribute( "type", m_storages[*it]->getType() );
		element2.setAttribute( "file", m_storages[*it]->getFile() );

		element.appendChild( element2 );
	}

	element = doc.createElement( "network" );

	root.appendChild( element );

	keys = m_ifaces.keys();

	for ( it = keys.begin() ; it != keys.end() ; ++it )
	{
		element2 = doc.createElement( "iface" );
		element2.setAttribute( "id", buffer.setNum( *it ) );
		element2.setAttribute( "type", m_ifaces[*it]->getType() );
		element2.setAttribute( "file", m_ifaces[*it]->getFile() );
		element2.setAttribute( "model", m_ifaces[*it]->getModel() );
		element2.setAttribute( "addr", m_ifaces[*it]->getHardwareAddress() );
		element2.setAttribute( "vlan", buffer );

		element.appendChild( element2 );
	}

	QTextStream stream( &file );

	stream << "<?xml version=\"1\"?>\n";
	stream << doc.toString();

	file.close();
}

const QString & KVirtualOptions::getName() const
{
	return m_name;
}

const QString & KVirtualOptions::getDistrib() const
{
	return m_distrib;
}

const QString & KVirtualOptions::getDescription() const
{
	return m_description;
}

uint KVirtualOptions::getMemory() const
{
	return m_memory;
}

uint KVirtualOptions::getNbCPU() const
{
	return m_cpus;
}

KVirtualDevice* KVirtualOptions::getStorage( uint id ) const
{
	if ( ! m_storages.contains( id ) )
	{
		return 0;
	}

	return m_storages[ id ];
}

KVirtualDevice* KVirtualOptions::getIface( uint id ) const
{
	if ( ! m_ifaces.contains( id ) )
	{
		return 0;
	}

	return m_ifaces[ id ];
}

const QString & KVirtualOptions::getVideoCard() const
{
	return m_videoCard;
}

bool KVirtualOptions::isUsbSupported() const
{
	return m_usb;
}

bool KVirtualOptions::isSnapshotEnabled() const
{
	return m_snapshot;
}

uint KVirtualOptions::getVncPort() const
{
	return m_vncport;
}

KVirtualOptions::Display KVirtualOptions::getDisplay() const
{
	return m_display;
}

KVirtualOptions::BootOrder KVirtualOptions::getBootDevice() const
{
	return m_bootDevice;
}

const QString & KVirtualOptions::getKeyboard() const
{
	return m_keyboard;
}

inline QString DisplayToString( KVirtualOptions::Display display )
{
	QString buffer;

	switch ( display )
	{

		case KVirtualOptions::DISPLAY_DIRECT:
		{
			buffer = "direct";
			break;
		}

		case KVirtualOptions::DISPLAY_VNC:
		{
			buffer = "vnc";
			break;
		}

		case KVirtualOptions::DISPLAY_NONE:
		{
			buffer = "none";
			break;
		}
	}

	return buffer;
}

inline QString BootOrderToString( KVirtualOptions::BootOrder order )
{
	QString buffer;

	switch ( order )
	{

		case KVirtualOptions::BOOT_ON_DISK:
		{
			buffer = "disk";
			break;
		}

		case KVirtualOptions::BOOT_ON_NETWORK:
		{
			buffer = "network";
			break;
		}

		case KVirtualOptions::BOOT_ON_CDROM:
		{
			buffer = "cdrom";
			break;
		}

		case KVirtualOptions::BOOT_ON_FLOPPY:
		{
			buffer = "floppy";
			break;
		}
	}

	return buffer;
}

inline KVirtualOptions::Display DisplayFromString( const QString & display )
{
	KVirtualOptions::Display type;

	if ( display == "direct" ) type = KVirtualOptions::DISPLAY_DIRECT;
	else if ( display == "vnc" ) type = KVirtualOptions::DISPLAY_VNC;
	else type = KVirtualOptions::DISPLAY_NONE;

	return type;
}

inline KVirtualOptions::BootOrder BootOrderFromString( const QString & order )
{
	KVirtualOptions::BootOrder boot;

	if ( order == "floppy" ) boot = KVirtualOptions::BOOT_ON_FLOPPY;
	else if ( order == "network" ) boot = KVirtualOptions::BOOT_ON_NETWORK;
	else if ( order == "cdrom" ) boot = KVirtualOptions::BOOT_ON_CDROM;
	else boot = KVirtualOptions::BOOT_ON_DISK;

	return boot;
}


// kate: indent-mode cstyle; replace-tabs off; tab-width 4;        replace-tabs off;
