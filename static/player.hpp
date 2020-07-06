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

#ifndef CANARY_LIB_STATIC_PLAYER_H
#define CANARY_LIB_STATIC_PLAYER_H

#include "../pch.hpp"

/*****************************
********* Enums
*****************************/
// TODO: join skills and special skills
enum skills_t : uint8_t {
	SKILL_FIST = 0,
	SKILL_CLUB = 1,
	SKILL_SWORD = 2,
	SKILL_AXE = 3,
	SKILL_DISTANCE = 4,
	SKILL_SHIELD = 5,
	SKILL_FISHING = 6,

	SKILL_MAGLEVEL = 7,
	SKILL_LEVEL = 8,

	SKILL_FIRST = SKILL_FIST,
	SKILL_LAST = SKILL_FISHING
};

enum SpecialSkills_t : uint8_t {
	SPECIALSKILL_CRITICALHITCHANCE,
	SPECIALSKILL_CRITICALHITAMOUNT,
	SPECIALSKILL_LIFELEECHCHANCE,
	SPECIALSKILL_LIFELEECHAMOUNT,
	SPECIALSKILL_MANALEECHCHANCE,
	SPECIALSKILL_MANALEECHAMOUNT,

	SPECIALSKILL_FIRST = SPECIALSKILL_CRITICALHITCHANCE,
	SPECIALSKILL_LAST = SPECIALSKILL_MANALEECHAMOUNT
};

enum VipStatus_t : uint8_t {
	VIPSTATUS_OFFLINE = 0,
	VIPSTATUS_ONLINE = 1,
	VIPSTATUS_PENDING = 2
};

#endif