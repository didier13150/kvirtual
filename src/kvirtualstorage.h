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
 *     (___)_)   File : kvirtualstorage.cpp                 ||--|| *         *
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


#ifndef KVIRTUALSTORAGE_H
#define KVIRTUALSTORAGE_H

#include "kvirtualdevice.h"

class KVirtualStorage : public KVirtualDevice
{
    Q_OBJECT

public:
    typedef enum {
        NONE    = 0,
        DISK    = 1,
        CDROM   = 2,
        FLOPPY  = 3,
    }Type;

    KVirtualStorage();
    KVirtualStorage( const Type & type , const QString & );
    virtual ~KVirtualStorage();

    void setTypeID( const Type );
    void setTypeID( int );
    int getTypeID() const;

protected:
    void setTypeByTypeID( const Type & type );

protected:
    Type m_typeID;
};

#endif // KVIRTUALSTORAGE_H
// kate: indent-mode cstyle; space-indent on; indent-width 0;
