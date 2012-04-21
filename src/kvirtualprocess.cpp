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

#include "kvirtualprocess.h"

KVirtualProcess::KVirtualProcess( uint id, KVirtualProcess::ProcessType type )
{
    m_id = id;
	m_type = type;
    connect( this, SIGNAL( readyReadStandardOutput() ), SLOT( sendReadyReadStandardOutput() ) );
    connect( this, SIGNAL( readyReadStandardError() ), SLOT( sendReadyReadStandardError() ) );
    connect( this, SIGNAL( started() ), SLOT( sendStarted() ) );
    connect( this, SIGNAL( finished( int, QProcess::ExitStatus ) ), SLOT( sendFinished( int, QProcess::ExitStatus ) ) );
}

KVirtualProcess::~KVirtualProcess()
{
}

KVirtualProcess::ProcessType KVirtualProcess::getVirtualType()
{
	return m_type;
}

void KVirtualProcess::sendStarted()
{
	emit( started( m_id ) );
}

void KVirtualProcess::sendReadyReadStandardOutput()
{
    emit( readyReadStandardOutput( m_id ) );
}

void KVirtualProcess::sendReadyReadStandardError()
{
    emit( readyReadStandardError( m_id ) );
}

void KVirtualProcess::sendFinished( int retval, QProcess::ExitStatus status )
{
    emit( finished( m_id, retval, status ) );
}
// kate: indent-mode cstyle; space-indent on; indent-width 0;  replace-tabs off;
