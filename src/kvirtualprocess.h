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
 *     (___)_)   File : kvirtualprocess.h                   ||--|| *         *
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

#ifndef KVIRTUALPROCESS_H
#define KVIRTUALPROCESS_H

#include <KProcess>


class KVirtualProcess : public KProcess
{
    Q_OBJECT

public:
	typedef enum
	{
		SWITCH,
		HOST,
		CREATE_IMG,
	}ProcessType;

    /**
     * Default Constructor
     */
    KVirtualProcess( uint, ProcessType );

    /**
     * Default Destructor
     */
    virtual ~KVirtualProcess();

	ProcessType getVirtualType();


protected:
    uint m_id;
	ProcessType m_type;

protected slots:
    void sendReadyReadStandardOutput();
    void sendReadyReadStandardError();
    void sendFinished( int, QProcess::ExitStatus );
	void sendStarted();

signals:
    void readyReadStandardOutput( uint );
    void readyReadStandardError( uint );
    void finished( uint, int, QProcess::ExitStatus );
	void started( uint );
};


#endif // _KVIRTUALPROCESS_H_
// kate: indent-mode cstyle; space-indent on; indent-width 0;  replace-tabs off;
