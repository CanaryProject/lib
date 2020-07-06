/**
 * Canary Lib - Canary Project a free 2D game platform
 * Copyright (C) 2020  Lucas Grossi <lucas.ggrossi@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef CANARY_LIB_STATIC_GLOBAL_H
#define CANARY_LIB_STATIC_GLOBAL_H

#include "../pch.hpp"

/*****************************
** Enums
*****************************/
enum OperatingSystem_t : uint8_t {
	CLIENTOS_NONE = 0,

	CLIENTOS_LINUX = 1,
	CLIENTOS_WINDOWS = 2,
	CLIENTOS_FLASH = 3,

	CLIENTOS_NEW_LINUX = 4,
	CLIENTOS_NEW_WINDOWS = 5,
	CLIENTOS_NEW_MACOS = 6,

	CLIENTOS_OTCLIENT_LINUX = 10,
	CLIENTOS_OTCLIENT_WINDOWS = 11,
	CLIENTOS_OTCLIENT_MAC = 12,

	CLIENTOS_TFC_ANDROID = 100,
	CLIENTOS_TFC_IPHONEOS = 101,
	CLIENTOS_TFC_MACOSX = 102,
	CLIENTOS_TFC_WIZ = 103,
	CLIENTOS_TFC_PANDORA = 104,
	CLIENTOS_TFC_PSP = 105,
	CLIENTOS_TFC_WINDOWS = 106,
	CLIENTOS_TFC_WINDOWSRT = 107,
	CLIENTOS_TFC_HAIKU = 108,
	CLIENTOS_TFC_NACL = 109,
	CLIENTOS_TFC_EMSCRIPTEN = 110,
	CLIENTOS_TFC_RASPBERRYPI = 111,
	CLIENTOS_TFC_UNIX = 112,
};

enum SpellGroup_t : uint8_t {
	SPELLGROUP_NONE = 0,
	SPELLGROUP_ATTACK = 1,
	SPELLGROUP_HEALING = 2,
	SPELLGROUP_SUPPORT = 3,
	SPELLGROUP_SPECIAL = 4,
};

enum ThreadState : uint8_t {
	THREAD_STATE_RUNNING,
	THREAD_STATE_CLOSING,
	THREAD_STATE_TERMINATED,
};

#endif
