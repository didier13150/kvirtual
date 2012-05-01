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
 *     (___)_)   File : kvirtualiface.cpp                   ||--|| *         *
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


#include "kvirtualiface.h"

KVirtualIface::KVirtualIface()
{
    m_isScriptUp = false;
    m_isScriptDown = false;
}

KVirtualIface::KVirtualIface( const QString & type,
                              const QString & file,
                              const QString & model,
                              const QString & hwaddr
                            )
{
    m_type = type;
    m_file = file;
    m_model = model;
    m_hwaddr = hwaddr;
    m_isScriptUp = false;
    m_isScriptDown = false;
}

KVirtualIface::~KVirtualIface()
{
}

void KVirtualIface::setHardwareAddress( const QString & hwaddr )
{
    m_hwaddr = hwaddr;
}

void KVirtualIface::setModel( const QString & model )
{
    m_model = model;
}

void KVirtualIface::setScriptUp( const QString & script )
{
    m_scriptUp = script;
}

void KVirtualIface::setScriptDown( const QString & script )
{
    m_scriptDown = script;
}

void KVirtualIface::setScriptUpEnabled( bool state )
{
    m_isScriptUp = state;
}

void KVirtualIface::setScriptDownEnabled( bool state )
{
    m_isScriptDown = state;
}

const QString & KVirtualIface::getModel() const
{
    return m_model;
}

const QString & KVirtualIface::getHardwareAddress() const
{
    return m_hwaddr;
}

const QString & KVirtualIface::getScriptUp() const
{
    return m_scriptUp;
}

const QString & KVirtualIface::getScriptDown() const
{
    return m_scriptDown;
}

bool KVirtualIface::isScriptUpEnabled() const
{
    return m_isScriptUp;
}

bool KVirtualIface::isScriptDownEnabled() const
{
    return m_isScriptDown;
}
// kate: indent-mode cstyle; space-indent on; indent-width 0; 
