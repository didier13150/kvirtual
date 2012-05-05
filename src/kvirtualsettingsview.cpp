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
 *     (___)_)   File : kvirtualsettings.h                  ||--|| *         *
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

#include "kvirtualsettingsview.h"
#include <KFileDialog>
#include <QPushButton>

KVirtualSettingsView::KVirtualSettingsView()
{
	ui_prefs_base.setupUi( this );
	connect( ui_prefs_base.pushButton_kvm, SIGNAL( clicked() ), SLOT( searchKvm() ) );
	connect( ui_prefs_base.pushButton_vde, SIGNAL( clicked() ), SLOT( searchVde() ) );
	connect( ui_prefs_base.pushButton_qemuimg, SIGNAL( clicked() ), SLOT( searchQemuImg() ) );
}

KVirtualSettingsView::~KVirtualSettingsView()
{
}

void KVirtualSettingsView::searchKvm()
{
	ui_prefs_base.kcfg_kvm_exe->setText( search() );
}

void KVirtualSettingsView::searchVde()
{
	ui_prefs_base.kcfg_vde_switch_exe->setText( search() );
}

void KVirtualSettingsView::searchQemuImg()
{
	ui_prefs_base.kcfg_qemu_img_creator_exe->setText( search() );
}

QString KVirtualSettingsView::search()
{
	KUrl url = KUrl::fromPath( "/usr/bin/" );

	return KFileDialog::getOpenFileName( url, "*", this );
}

#include "kvirtualsettingsview.moc"
