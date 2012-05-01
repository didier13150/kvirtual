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
 *     (___)_)   File : kvirtualiface.h                     ||--|| *         *
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


#ifndef KVIRTUALIFACE_H
#define KVIRTUALIFACE_H

#include "kvirtualdevice.h"

class KVirtualIface : public KVirtualDevice
{
    Q_OBJECT

public:
    KVirtualIface();
    KVirtualIface( const QString &, const QString &, const QString &, const QString & );
    virtual ~KVirtualIface();

    void setScriptUp( const QString & );
    void setScriptDown( const QString & );
    void setScriptUpEnabled( bool );
    void setScriptDownEnabled( bool );
    void setHardwareAddress( const QString & );
    void setModel( const QString & );

    const QString & getModel() const;
    const QString & getHardwareAddress() const;
    const QString & getScriptUp() const;
    const QString & getScriptDown() const;

    bool isScriptUpEnabled() const;
    bool isScriptDownEnabled() const;

protected:
    QString m_model;
    QString m_hwaddr;
    QString m_scriptUp;
    QString m_scriptDown;
    bool m_isScriptUp;
    bool m_isScriptDown;
};

#endif // KVIRTUALIFACE_H
// kate: indent-mode cstyle; space-indent on; indent-width 0; 
