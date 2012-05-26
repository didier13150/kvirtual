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
 *     (___)_)   File : kvirtualoptions.cpp                 ||--|| *         *
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

#include "kvirtualoptions.h"
#include "kvirtualstorage.h"
#include "kvirtualiface.h"

#include <QList>
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QUuid>
#include <KStandardDirs>

#include <stdlib.h>

KVirtualOptions::KVirtualOptions()
{
	clear();
}

KVirtualOptions::~KVirtualOptions()
{
	QMapIterator<uint, KVirtualStorage*> it1( m_storages );

	while( it1.hasNext() )
	{
		it1.next();

		if( it1.value() )
		{
			delete it1.value();
		}
	}

	QMapIterator<uint, KVirtualIface*> it2( m_ifaces );

	while( it2.hasNext() )
	{
		it2.next();

		if( it2.value() )
		{
			delete it2.value();
		}
	}
}

void KVirtualOptions::clear()
{
	m_usb = false;
	m_snapshot = false;
	m_memory = 128;
	m_display = KVirtualOptions::DISPLAY_DIRECT;
	m_bootDevice = KVirtualOptions::BOOT_ON_DISK;
	m_vncport = 1;
	m_cpus = 1;
	m_keyboard = "fr";
	m_videoCard = "std";
	m_description.clear();
	m_name.clear();
	m_distrib = "linux";
	m_storages.clear();
	m_ifaces.clear();
	m_opts.clear();
	m_uuid = QUuid::createUuid().toString();
	m_uuid.remove( "{" ).remove( "}" );

}

QStringList KVirtualOptions::getNeededVirtualSwitch()
{
	QStringList needed;
	QString buffer;

	QList<uint>::ConstIterator it;
	QList<uint> keys = m_ifaces.keys();

	for( it = keys.begin() ; it != keys.end() ; ++it )
	{
		if( m_ifaces[*it]->getType() == "vde" )
		{
			buffer = m_ifaces[*it]->getFile();

			if( not m_usedSwitches.contains( buffer ) )
				needed << buffer;
		}
	}

	return needed;
}

const QStringList & KVirtualOptions::getArgs()
{
	QString buffer, id, img;
	QList<uint>::ConstIterator it;
	QList<uint> keys;

	m_opts.clear();

	m_opts << "-uuid" << getUUID();

	m_opts << "-name" << getName();

	buffer.setNum( m_memory );
	m_opts << "-m" << buffer;

	if( m_cpus > 1 )
	{
		buffer.setNum( m_cpus );
		m_opts << "-smp" << buffer;
	}

	keys = m_storages.keys();

	for( it = keys.begin() ; it != keys.end() ; ++it )
	{
		if( m_storages[*it]->getTypeID() )
		{
			buffer = "file=" + m_storages[*it]->getFile() + ",media=" + m_storages[*it]->getType();

			if( ! m_storages[*it]->getInterface().isEmpty() )
			{
				buffer.append( ",if=" + m_storages[*it]->getInterface() );
			}

			m_opts << "-drive" << buffer;
		}
	}

	keys = m_ifaces.keys();

	for( it = keys.begin() ; it != keys.end() ; ++it )
	{
		if( ! m_ifaces[*it]->getModel().isEmpty() )
		{
			id.setNum( *it );
			buffer = "nic,macaddr=" + m_ifaces[*it]->getHardwareAddress() + ",model=" + m_ifaces[*it]->getModel() + ",vlan=" + id;
			m_opts << "-net" << buffer;

			buffer = m_ifaces[*it]->getType();

			if( buffer == "tap" )
			{
				buffer += ",ifname=" + m_ifaces[*it]->getFile();

				if( ! m_ifaces[*it]->getScriptUp().isEmpty() &&
				        ! m_ifaces[*it]->getScriptDown().isEmpty() &&
				        m_ifaces[*it]->isScriptUpEnabled() &&
				        m_ifaces[*it]->isScriptDownEnabled()
				  )
				{
					//TODO improve this part
					buffer += ",script=" + m_ifaces[*it]->getScriptUp();
					buffer += ",downscript=" + m_ifaces[*it]->getScriptDown();
				}

				else
				{
					buffer += ",script=no";
				}
			}

			else if( buffer == "vde" )
			{
				buffer += ",sock=" + m_ifaces[*it]->getFile();
			}

			else
			{
				buffer.clear();
			}

			if( ! buffer.isEmpty() )
			{
				buffer += ",vlan=" + id;
				m_opts << "-net" << buffer;
			}
		}
	}

	switch( m_display )
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

	if( m_usb )
		m_opts << "-usb";

	if( m_snapshot )
		m_opts << "-snapshot";

	if( ! m_keyboard.isEmpty() )
		m_opts << "-k" << m_keyboard;

	if( m_videoCard.isEmpty() )
	{
		buffer = "none";
	}

	else
	{
		buffer = m_videoCard;
	}

	m_opts << "-vga" << buffer;

	switch( m_bootDevice )
	{

		case KVirtualOptions::BOOT_ON_DISK:
		{
			buffer = "c";
			break;
		}

		case KVirtualOptions::BOOT_ON_CDROM:
		{
			buffer = "d";
			break;
		}

		case KVirtualOptions::BOOT_ON_FLOPPY:
		{
			buffer = "a";
			break;
		}

		case KVirtualOptions::BOOT_ON_NETWORK:
		{
			buffer = "n";
			break;
		}
	}

	buffer.prepend( "once=" );

	//TODO uncomment when kvm not crash with this option
	/*img = KStandardDirs::locate( "appdata", "boot.jpg" );
	if ( ! img.isNull() )
	{
		//buffer += ",splash=" + img + ",splash-time=5000";
	}*/
	m_opts << "-boot" << buffer;


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

void KVirtualOptions::setMemory( int mem )
{
	m_memory = ( uint ) mem;
}

void KVirtualOptions::setNbCPU( int nb )
{
	m_cpus = ( uint ) nb;
}

KVirtualStorage* KVirtualOptions::addStorage( uint id )
{
	if( m_storages.contains( id ) )
	{
		delete m_storages[id];
	}

	KVirtualStorage* storage = new KVirtualStorage();

	m_storages[id] = storage;
	return m_storages[id];
}

KVirtualIface* KVirtualOptions::addIface( uint id )
{
	if( m_ifaces.contains( id ) )
	{
		delete m_ifaces[id];
	}

	KVirtualIface* iface = new KVirtualIface();

	m_ifaces[id] = iface;
	return m_ifaces[ id ];
}

void KVirtualOptions::setUsbSupported( int state )
{
	m_usb = ( bool ) state;
}

void KVirtualOptions::setSnapshotEnabled( int enable )
{
	m_snapshot = ( bool ) enable;
}

void KVirtualOptions::setVideoCard( const QString & model )
{
	m_videoCard = model;
}

void KVirtualOptions::setBootDevice( int device )
{
	m_bootDevice = ( KVirtualOptions::BootOrder ) device;
}

void KVirtualOptions::setKeyboard( const QString & keyboard )
{
	m_keyboard = keyboard;
}

void KVirtualOptions::setVncPort( int port )
{
	m_vncport = ( uint ) port;
}

void KVirtualOptions::setDisplay( KVirtualOptions::Display type )
{
	m_display = type;
}

void KVirtualOptions::setDisplay( int type )
{
	m_display = ( KVirtualOptions::Display ) type;
}

void KVirtualOptions::setUUID( const QString & uuid )
{
	m_uuid = uuid;
}

void KVirtualOptions::setStorageFile( uint id, const QString & file )
{
	KVirtualStorage* storage = getAutoCreateStorage( id );
	storage->setFile( file );
}

void KVirtualOptions::setStorageType( uint id, int type )
{
	KVirtualStorage* storage = getAutoCreateStorage( id );
	storage->setTypeID( type );
}

void KVirtualOptions::setStorageInterface( uint id, const QString & interface )
{
	KVirtualStorage* storage = getAutoCreateStorage( id );
	storage->setInterface( interface );
}

void KVirtualOptions::setIfaceModel( uint id, const QString & model )
{
	KVirtualIface* iface = getAutoCreateIface( id );
	iface->setModel( model );
}

void KVirtualOptions::setIfaceType( uint id, const QString & type )
{
	KVirtualIface* iface = getAutoCreateIface( id );
	iface->setType( type );
}

void KVirtualOptions::setIfaceFile( uint id, const QString & file )
{
	KVirtualIface* iface = getAutoCreateIface( id );
	iface->setFile( file );
}

void KVirtualOptions::setIfaceHwAddr( uint id, const QString & addr )
{
	KVirtualIface* iface = getAutoCreateIface( id );
	iface->setHardwareAddress( addr );
}

void KVirtualOptions::setIfaceScriptUp( uint id, const QString & script )
{
	KVirtualIface* iface = getAutoCreateIface( id );
	iface->setScriptUp( script );
}

void KVirtualOptions::setIfaceScriptDown( uint id, const QString & script )
{
	KVirtualIface* iface = getAutoCreateIface( id );
	iface->setScriptDown( script );
}

void KVirtualOptions::setIfaceScriptUpEnabled( uint id, bool state )
{
	KVirtualIface* iface = getAutoCreateIface( id );
	iface->setScriptUpEnabled( state );
}

void KVirtualOptions::setIfaceScriptDownEnabled( uint id, bool state )
{
	KVirtualIface* iface = getAutoCreateIface( id );
	iface->setScriptDownEnabled( state );
}

void KVirtualOptions::setUsedSwitch( const QString & vswitch )
{
	if( not m_usedSwitches.contains( vswitch ) )
		m_usedSwitches << vswitch;
}

const QString & KVirtualOptions::getRandomHwAddr( uint id )
{
	QMapIterator<uint, KVirtualIface*> it( m_ifaces );
	QString hwaddr = "52:54:00:12:", lastdigit;
	uint digit = ( uint ) random() % 255;
	bool used = false;

	switch( id )
	{
		case 0:
		{
			hwaddr.append( "34:" );
			break;
		}
		case 1:
		{
			hwaddr.append( "36:" );
			break;
		}
		default:
		{
			hwaddr.append( "38:" );
			break;
		}
	}

	do
	{
		used = false;
		lastdigit.setNum( digit, 16 );
		hwaddr.append( lastdigit.rightJustified( 2, '0' ).toUpper() );

		// Check if addr is already in use
		while( it.hasNext() )
		{
			it.next();

			if( hwaddr == it.value()->getHardwareAddress() )
			{
				used = true;
				digit += 2;
			}
		}
	}
	while( used );

	m_ifaces[id]->setHardwareAddress( hwaddr );
	return m_ifaces[id]->getHardwareAddress();
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

KVirtualStorage* KVirtualOptions::getStorage( uint id ) const
{
	if( ! m_storages.contains( id ) )
	{
		return 0;
	}

	return m_storages[ id ];
}

KVirtualStorage* KVirtualOptions::getAutoCreateStorage( uint id )
{
	if( ! m_storages.contains( id ) )
	{
		KVirtualStorage* storage = new KVirtualStorage();
		m_storages[ id ] = storage;
	}

	return m_storages[ id ];
}

KVirtualIface* KVirtualOptions::getIface( uint id ) const
{
	if( ! m_ifaces.contains( id ) )
	{
		return 0;
	}

	return m_ifaces[ id ];
}

KVirtualIface* KVirtualOptions::getAutoCreateIface( uint id )
{
	if( ! m_ifaces.contains( id ) )
	{
		KVirtualIface* iface = new KVirtualIface();
		m_ifaces[ id ] = iface;
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

const QString & KVirtualOptions::getUUID() const
{
	return m_uuid;
}

void KVirtualOptions::printConfig()
{
	QList<uint> keys;
	QList<uint>::ConstIterator id;

	qDebug() << "name" << getName();
	qDebug() << "uuid" << getUUID();
	qDebug() << "description" << getDescription();
	qDebug() << "memory" << getMemory();
	qDebug() << "boot device" << getBootDevice();
	qDebug() << "usb support" << isUsbSupported();
	qDebug() << "snapshot mode" << isSnapshotEnabled();
	qDebug() << "number of core" << getNbCPU();
	qDebug() << "display" << DisplayToString( getDisplay() );
	qDebug() << "video card" << getVideoCard();
	qDebug() << "vnc port" << getVncPort();
	qDebug() << "keyboard" << getKeyboard();

	keys = m_storages.keys();

	for( id = keys.begin() ; id != keys.end() ; ++id )
	{
		qDebug() << "*******************************************************";
		qDebug() << "storage( id" << *id << ") type" << getStorage( *id )->getTypeID();
		qDebug() << "storage( id" << *id << ") file" << getStorage( *id )->getFile();
		qDebug() << "storage( id" << *id << ") interface" << getStorage( *id )->getInterface();
	}

	keys = m_ifaces.keys();

	for( id = keys.begin() ; id != keys.end() ; ++id )
	{
		qDebug() << "*******************************************************";
		qDebug() << "iface( id" << *id << ") type" << getIface( *id )->getType();
		qDebug() << "iface( id" << *id << ") file" << getIface( *id )->getFile();
		qDebug() << "iface( id" << *id << ") model" << getIface( *id )->getModel();
		qDebug() << "iface( id" << *id << ") hwaddr" << getIface( *id )->getHardwareAddress();
		qDebug() << "iface( id" << *id << ") scriptup" << getIface( *id )->getScriptUp();
		qDebug() << "iface( id" << *id << ") scriptdown" << getIface( *id )->getScriptDown();
		qDebug() << "iface( id" << *id << ") scriptup enable" << getIface( *id )->isScriptUpEnabled();
		qDebug() << "iface( id" << *id << ") scriptdown enable" << getIface( *id )->isScriptDownEnabled();
	}

	qDebug() << "*******************************************************";
}

bool KVirtualOptions::isModified( const QString & filename ) const
{
	if( filename.isNull() )
	{
		qDebug() << "filename is null";
		return true;
	}

	QDomDocument doc;

	QDomElement root, element, element2, element3;
	QDomNode node, child, littlechild;
	QString buffer;
	uint id;

	QFile file( filename );

	if( !file.open( QIODevice::ReadOnly ) )
	{
		qDebug() << "Can't open filename in read mode";
		return true;
	}

	if( !doc.setContent( &file ) )
	{
		file.close();
		qDebug() << "Can't parse XML file";
		return true;
	}

	file.close();

	root = doc.documentElement();

	if( root.tagName() != "host" )
	{
		qDebug() << "Wrong root node" << root.tagName();
		return true;
	}

	if( root.attribute( "distribution" ) != getDistrib() )
	{
		return true;
	}

	if( root.attribute( "uuid" ) != getUUID() )
	{
		return true;
	}

	node = root.firstChild();

	while( ! node.isNull() )
	{
		element = node.toElement();

		if( ! element.isNull() )
		{
			if( element.tagName() == "name" )
			{
				if( getName() != element.text() )
				{
					qDebug() << "name is not sync" << getName() << element.text();
					return true;
				}
			}

			if( element.tagName() == "description" )
			{
				if( getDescription() != element.text() )
				{
					qDebug() << "description is not sync" << getDescription() << element.text();
					return true;
				}
			}

			if( element.tagName() == "system" )
			{
				child = element.firstChild();

				while( ! child.isNull() )
				{
					element2 = child.toElement();

					if( element2.tagName() == "memory" )
					{
						if( getMemory() !=  element2.attribute( "value" ).toUInt() )
						{
							qDebug() << "memory is not sync" << getMemory() << element2.attribute( "value" );
							return true;
						}
					}

					if( element2.tagName() == "boot" )
					{
						buffer = element2.attribute( "device" );

						if( getBootDevice() != BootOrderFromString( buffer ) )
						{
							qDebug() << "boot device is not sync" << getBootDevice() << BootOrderFromString( buffer );
							return true;
						}
					}

					if( element2.tagName() == "usb" )
					{
						if( isUsbSupported() != element2.attribute( "enabled" ).toInt() )
						{
							qDebug() << "usb support is not sync" << isUsbSupported() << element2.attribute( "enabled" );
							return true;
						}
					}

					if( element2.tagName() == "snapshot" )
					{
						if( isSnapshotEnabled() != element2.attribute( "enabled" ).toInt() )
						{
							qDebug() << "snapshot mode not sync" << isSnapshotEnabled() << element2.attribute( "enabled" );
							return true;
						}
					}

					if( element2.tagName() == "core" )
					{
						if( getNbCPU() != element2.attribute( "value" ).toUInt() )
						{
							qDebug() << "number of core not sync" << getNbCPU() << element2.attribute( "value" );
							return true;
						}
					}

					child = child.nextSibling();
				}
			}

			if( element.tagName() == "display" )
			{
				buffer = element.attribute( "method" );

				if( getDisplay() != DisplayFromString( buffer ) )
				{
					qDebug() << "display not sync" << DisplayToString( getDisplay() ) << buffer;
					return true;
				}

				child = element.firstChild();

				while( ! child.isNull() )
				{
					element2 = child.toElement();

					if( element2.tagName() == "card" )
					{
						if( getVideoCard() != element2.attribute( "model" ) )
						{
							qDebug() << "video card not sync" << getVideoCard() << element2.attribute( "model" );
							return true;
						}
					}

					if( element2.tagName() == "vnc" )
					{
						if( getVncPort() != element2.attribute( "port" ).toUInt() )
						{
							qDebug() << "vnc port not sync" << getVncPort() << element2.attribute( "port" );
							return true;
						}
					}

					if( element2.tagName() == "keyboard" )
					{
						if( getKeyboard() != element2.attribute( "model" ) )
						{
							qDebug() << "keyboard not sync" << getKeyboard() << element2.attribute( "model" );
							return true;
						}
					}

					child = child.nextSibling();
				}
			}

			if( element.tagName() == "storages" )
			{
				child = element.firstChild();

				while( ! child.isNull() )
				{
					element2 = child.toElement();

					if( element2.tagName() == "storage" )
					{
						id = element2.attribute( "id" ).toUInt();

						if( element2.attribute( "type" ).toInt() != getStorage( id )->getTypeID() )
						{
							qDebug() << "storage type not sync, id" << id << getStorage( id )->getTypeID() << element2.attribute( "type" );
							return true;
						}

						if( element2.attribute( "file" ) != getStorage( id )->getFile() )
						{
							qDebug() << "storage file not sync, id" << id << getStorage( id )->getFile() << element2.attribute( "file" );
							return true;
						}

						if( element2.attribute( "interface" ) != getStorage( id )->getInterface() )
						{
							qDebug() << "storage interface not sync, id" << id << getStorage( id )->getInterface() << element2.attribute( "interface" );
							return true;
						}
					}

					child = child.nextSibling();
				}
			}

			if( element.tagName() == "network" )
			{
				child = element.firstChild();

				while( ! child.isNull() )
				{
					element2 = child.toElement();

					if( element2.tagName() == "iface" )
					{
						id = element2.attribute( "id" ).toUInt();

						if( element2.attribute( "type" ) != getIface( id )->getType() )
						{
							qDebug() << "iface type not sync, id" << id << getIface( id )->getType() << element2.attribute( "type" );
							return true;
						}

						if( element2.attribute( "file" ) != getIface( id )->getFile() )
						{
							qDebug() << "iface file not sync, id" << id << getIface( id )->getFile() << element2.attribute( "file" );
							return true;
						}

						if( element2.attribute( "model" ) != getIface( id )->getModel() )
						{
							qDebug() << "iface model not sync, id" << id << getIface( id )->getModel() << element2.attribute( "model" );
							return true;
						}

						if( element2.attribute( "addr" ) != getIface( id )->getHardwareAddress() )
						{
							qDebug() << "iface hwaddr not sync, id" << id << getIface( id )->getHardwareAddress() << element2.attribute( "addr" );
							return true;
						}

						littlechild = element2.firstChild();

						while( ! littlechild.isNull() )
						{
							element3 = littlechild.toElement();

							if( element3.tagName() == "scripts" )
							{
								if( getIface( id )->getScriptUp() != element3.attribute( "up" ) )
								{
									qDebug() << "iface scriptup not sync, id" << id << getIface( id )->getScriptUp() << element3.attribute( "up" );
									return true;
								}

								if( getIface( id )->getScriptDown() != element3.attribute( "down" ) )
								{
									qDebug() << "iface scriptdown not sync, id" << id << getIface( id )->getScriptDown() << element3.attribute( "down" );
									return true;
								}

								int flag = element3.attribute( "flag" ).toInt();

								if( getIface( id )->isScriptUpEnabled() != ( flag & KVirtualIface::SCRIPT_UP ) )
								{
									qDebug() << "iface scriptup enable not sync, id" << id << getIface( id )->isScriptUpEnabled() << ( flag & KVirtualIface::SCRIPT_UP );
									return true;
								}

								if( getIface( id )->isScriptDownEnabled() != ( flag & KVirtualIface::SCRIPT_DOWN ) )
								{
									qDebug() << "iface scriptdown enable not sync, id" << id << getIface( id )->isScriptDownEnabled() << ( flag & KVirtualIface::SCRIPT_DOWN );
									return true;
								}
							}

							littlechild = littlechild.nextSibling();
						}
					}

					child = child.nextSibling();
				}
			}
		}

		node = node.nextSibling();
	}

	return false;
}

void KVirtualOptions::load( const QString & filename )
{
	if( filename.isNull() )
	{
		qDebug() << "filename is null";
		return;
	}

	QDomDocument doc;

	QDomElement root, element, element2, element3;
	QDomNode node, child, littlechild;
	QString buffer;
	uint id;
	KVirtualIface * iface;
	KVirtualStorage * storage;

	QFile file( filename );

	if( !file.open( QIODevice::ReadOnly ) )
	{
		qDebug() << "Can't open filename in read mode";
		return;
	}

	if( !doc.setContent( &file ) )
	{
		file.close();
		qDebug() << "Can't parse XML file";
		return;
	}

	file.close();

	root = doc.documentElement();

	if( root.tagName() != "host" )
	{
		qDebug() << "Wrong root node" << root.tagName();
		return;
	}

	if( ! root.attribute( "distribution" ).isEmpty() )
		setDistrib( root.attribute( "distribution" ) );

	if( ! root.attribute( "uuid" ).isEmpty() )
		setUUID( root.attribute( "uuid" ) );

	node = root.firstChild();

	while( ! node.isNull() )
	{
		element = node.toElement();

		if( ! element.isNull() )
		{
			if( element.tagName() == "name" )
			{
				setName( element.text() );
			}

			if( element.tagName() == "description" )
			{
				setDescription( element.text() );
			}

			if( element.tagName() == "system" )
			{
				child = element.firstChild();

				while( ! child.isNull() )
				{
					element2 = child.toElement();

					if( element2.tagName() == "memory" )
					{
						setMemory( element2.attribute( "value" ).toUInt() );
					}

					if( element2.tagName() == "boot" )
					{
						buffer = element2.attribute( "device" );
						setBootDevice( BootOrderFromString( buffer ) );
					}

					if( element2.tagName() == "usb" )
					{
						setUsbSupported( element2.attribute( "enabled" ).toInt() );
					}

					if( element2.tagName() == "snapshot" )
					{
						setSnapshotEnabled( element2.attribute( "enabled" ).toInt() );
					}

					if( element2.tagName() == "core" )
					{
						setNbCPU( element2.attribute( "value" ).toUInt() );
					}

					child = child.nextSibling();
				}
			}

			if( element.tagName() == "display" )
			{
				buffer = element.attribute( "method" );
				setDisplay( DisplayFromString( buffer ) );
				child = element.firstChild();

				while( ! child.isNull() )
				{
					element2 = child.toElement();

					if( element2.tagName() == "card" )
					{
						setVideoCard( element2.attribute( "model" ) );
					}

					if( element2.tagName() == "vnc" )
					{
						setVncPort( element2.attribute( "port" ).toUInt() );
					}

					if( element2.tagName() == "keyboard" )
					{
						setKeyboard( element2.attribute( "model" ) );
					}

					child = child.nextSibling();
				}
			}

			if( element.tagName() == "storages" )
			{
				child = element.firstChild();

				while( ! child.isNull() )
				{
					element2 = child.toElement();

					if( element2.tagName() == "storage" )
					{
						id = element2.attribute( "id" ).toUInt();
						storage = addStorage( id );
						storage->setTypeID( ( KVirtualStorage::Type ) element2.attribute( "type" ).toInt() );
						storage->setFile( element2.attribute( "file" ) );
						storage->setInterface( element2.attribute( "interface" ) );
					}

					child = child.nextSibling();
				}
			}

			if( element.tagName() == "network" )
			{
				child = element.firstChild();

				while( ! child.isNull() )
				{
					element2 = child.toElement();

					if( element2.tagName() == "iface" )
					{
						id = element2.attribute( "id" ).toUInt();
						iface = addIface( id );
						iface->setType( element2.attribute( "type" ) );
						iface->setFile( element2.attribute( "file" ) );
						iface->setModel( element2.attribute( "model" ) );
						iface->setHardwareAddress( element2.attribute( "addr" ) );

						littlechild = element2.firstChild();

						while( ! littlechild.isNull() )
						{
							element3 = littlechild.toElement();

							if( element3.tagName() == "scripts" )
							{
								iface->setScriptUp( element3.attribute( "up" ) );
								iface->setScriptDown( element3.attribute( "down" ) );

								int flag = element3.attribute( "flag" ).toInt();
								iface->setScriptUpEnabled( flag & KVirtualIface::SCRIPT_UP );
								iface->setScriptDownEnabled( flag & KVirtualIface::SCRIPT_DOWN );
							}

							littlechild = littlechild.nextSibling();
						}
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
	if( filename.isNull() )
	{
		return;
	}

	QDomDocument doc;

	QDomElement root, element, element2, element3;
	QDomNode node, child, littlechild;
	QDomCDATASection cdata;
	QString buffer;
	QList<uint> keys;
	QList<uint>::ConstIterator it;
	int flag;

	QFile file( filename );

	if( !file.open( QIODevice::WriteOnly ) )
		return;

	root = doc.createElement( "host" );

	root.setAttribute( "distribution", getDistrib() );
	root.setAttribute( "uuid", getUUID() );

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
	//element2 = doc.createElement( "memory" );
	//element2.setAttribute( "value", "8" );
	//element2.setAttribute( "unit", "mb" );
	//element.appendChild( element2 );

	element = doc.createElement( "storages" );

	root.appendChild( element );

	keys = m_storages.keys();

	for( it = keys.begin() ; it != keys.end() ; ++it )
	{
		element2 = doc.createElement( "storage" );
		element2.setAttribute( "id", buffer.setNum( *it ) );
		element2.setAttribute( "type", buffer.setNum( ( int ) m_storages[*it]->getTypeID() ) );
		element2.setAttribute( "file", m_storages[*it]->getFile() );
		element2.setAttribute( "interface", m_storages[*it]->getInterface() );

		element.appendChild( element2 );
	}

	element = doc.createElement( "network" );

	root.appendChild( element );

	keys = m_ifaces.keys();

	for( it = keys.begin() ; it != keys.end() ; ++it )
	{
		element2 = doc.createElement( "iface" );
		element2.setAttribute( "id", buffer.setNum( *it ) );
		element2.setAttribute( "type", m_ifaces[*it]->getType() );
		element2.setAttribute( "file", m_ifaces[*it]->getFile() );
		element2.setAttribute( "model", m_ifaces[*it]->getModel() );
		element2.setAttribute( "addr", m_ifaces[*it]->getHardwareAddress() );
		element2.setAttribute( "vlan", buffer );

		element3 = doc.createElement( "scripts" );
		element3.setAttribute( "up", m_ifaces[*it]->getScriptUp() );
		element3.setAttribute( "down", m_ifaces[*it]->getScriptDown() );

		flag = 0;

		if( m_ifaces[*it]->isScriptUpEnabled() )

		{
			flag |= KVirtualIface::SCRIPT_UP;
		}

		if( m_ifaces[*it]->isScriptDownEnabled() )
		{
			flag |= KVirtualIface::SCRIPT_DOWN;
		}

		element3.setAttribute( "flag", buffer.setNum( flag ) );

		element2.appendChild( element3 );

		element.appendChild( element2 );
	}

	QTextStream stream( &file );

	stream << "<?xml version=\"1\"?>\n";
	stream << doc.toString();

	file.close();
}

inline QString DisplayToString( KVirtualOptions::Display display )
{
	QString buffer;

	switch( display )
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

	switch( order )
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

	if( display == "direct" )
		type = KVirtualOptions::DISPLAY_DIRECT;

	else if( display == "vnc" )
		type = KVirtualOptions::DISPLAY_VNC;

	else
		type = KVirtualOptions::DISPLAY_NONE;

	return type;
}

inline KVirtualOptions::BootOrder BootOrderFromString( const QString & order )
{
	KVirtualOptions::BootOrder boot;

	if( order == "floppy" )
		boot = KVirtualOptions::BOOT_ON_FLOPPY;

	else if( order == "network" )
		boot = KVirtualOptions::BOOT_ON_NETWORK;

	else if( order == "cdrom" )
		boot = KVirtualOptions::BOOT_ON_CDROM;

	else
		boot = KVirtualOptions::BOOT_ON_DISK;

	return boot;
}

#include "kvirtualoptions.moc"

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 


