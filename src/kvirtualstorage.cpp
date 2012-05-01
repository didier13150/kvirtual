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
 *     (___)_)   File : kvirtualstorage.h                   ||--|| *         *
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

#include "kvirtualstorage.h"

KVirtualStorage::KVirtualStorage()
{
    m_typeID = KVirtualStorage::NONE;
}

KVirtualStorage::KVirtualStorage( const KVirtualStorage::Type type, const QString & file )
{
    m_typeID = type;
    switch ( type )
    {
    case KVirtualStorage::DISK:
    {
        m_type = "disk";
        break;
    }
    case KVirtualStorage::CDROM:
    {
        m_type = "cdrom";
        break;
    }
    case KVirtualStorage::FLOPPY:
    {
        m_type = "floppy";
        break;
    }
    case KVirtualStorage::NONE:
    {
        m_type = "none";
        break;
    }
    }
    m_file = file;
}

KVirtualStorage::~KVirtualStorage()
{
}

int KVirtualStorage::getTypeID() const
{
    return (int) m_typeID;
}

void KVirtualStorage::setTypeID( const Type id )
{
    m_typeID = id;
}

void KVirtualStorage::setTypeID( int id )
{
    m_typeID = ( KVirtualStorage::Type ) id;
}
// kate: indent-mode cstyle; space-indent on; indent-width 0; 
