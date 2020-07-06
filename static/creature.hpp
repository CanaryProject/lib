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

#ifndef CANARY_LIB_STATIC_CREATURE_H
#define CANARY_LIB_STATIC_CREATURE_H

#include "../pch.hpp"

/*****************************
********* Enums
*****************************/
enum CreatureType_t : uint8_t {
	CREATURETYPE_PLAYER = 0,
	CREATURETYPE_MONSTER = 1,
	CREATURETYPE_NPC = 2,
	CREATURETYPE_SUMMON_OWN = 3,
	CREATURETYPE_SUMMON_OTHERS = 4,
	CREATURETYPE_HIDDEN = 5,
};

enum Skulls_t : uint8_t {
	SKULL_NONE = 0,
	SKULL_YELLOW = 1,
	SKULL_GREEN = 2,
	SKULL_WHITE = 3,
	SKULL_RED = 4,
	SKULL_BLACK = 5,
	SKULL_ORANGE = 6,
};

// TODO: Creature icons on otclient
enum SpeechBubble_t : uint8_t {
	SPEECHBUBBLE_NONE = 0,
	SPEECHBUBBLE_NORMAL = 1,
	SPEECHBUBBLE_TRADE = 2,
	SPEECHBUBBLE_QUEST = 3,
	SPEECHBUBBLE_QUESTTRADER = 4,
};

/*****************************
********* Structs
*****************************/
struct Outfit_t {
	uint16_t lookTypeEx = 0;
	uint16_t lookMount = 0;
	uint16_t lookType = 0;
	uint8_t lookHead = 0;
	uint8_t lookBody = 0;
	uint8_t lookLegs = 0;
	uint8_t lookFeet = 0;
	uint8_t lookAddons = 0;
};

#endif