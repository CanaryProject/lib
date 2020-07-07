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

#ifndef CANARY_LIB_GLOBAL_GRAPHICAL_H
#define CANARY_LIB_GLOBAL_GRAPHICAL_H

#include "../pch.hpp"

/*****************************
********* Enums
*****************************/
enum Color_t : uint8_t {
	COLOR_BLACK = 0,
	COLOR_BLUE = 5,
	COLOR_LIGHTGREEN = 30,
	COLOR_LIGHTBLUE = 35,
	COLOR_MAYABLUE = 95,
	COLOR_DARKRED = 108,
	COLOR_LIGHTGREY = 129,
	COLOR_SKYBLUE = 143,
	COLOR_PURPLE = 154,
	COLOR_ELECTRICPURPLE = 155,
	COLOR_RED = 180,
	COLOR_PASTELRED = 194,
	COLOR_ORANGE = 198,
	COLOR_YELLOW = 210,
	COLOR_WHITE_EXP = 215,
	COLOR_NONE = 255,
};

enum GuildEmblems_t : uint8_t {
	GUILDEMBLEM_NONE,
	GUILDEMBLEM_ALLY,
	GUILDEMBLEM_ENEMY,
	GUILDEMBLEM_NEUTRAL,
	GUILDEMBLEM_MEMBER,
	GUILDEMBLEM_OTHER,
};

enum Icons_t : uint64_t {
	ICON_POISON = 1 << 0,
	ICON_BURN = 1 << 1,
	ICON_ENERGY =  1 << 2,
	ICON_DRUNK = 1 << 3,
	ICON_MANASHIELD = 1 << 4,
	ICON_PARALYZE = 1 << 5,
	ICON_HASTE = 1 << 6,
	ICON_SWORDS = 1 << 7,
	ICON_DROWNING = 1 << 8,
	ICON_FREEZING = 1 << 9,
	ICON_DAZZLED = 1 << 10,
	ICON_CURSED = 1 << 11,
	ICON_PARTY_BUFF = 1 << 12,
	ICON_REDSWORDS = 1 << 13,
	ICON_PIGEON = 1 << 14,
	ICON_BLEEDING = 1 << 15,
};

enum MapMark_t : uint8_t {
	MAPMARK_TICK,
	MAPMARK_QUESTION,
	MAPMARK_EXCLAMATION,
	MAPMARK_STAR,
	MAPMARK_CROSS,
	MAPMARK_TEMPLE,
	MAPMARK_KISS,
	MAPMARK_SHOVEL,
	MAPMARK_SWORD,
	MAPMARK_FLAG,
	MAPMARK_LOCK,
	MAPMARK_BAG,
	MAPMARK_SKULL,
	MAPMARK_DOLLAR,
	MAPMARK_REDNORTH,
	MAPMARK_REDSOUTH,
	MAPMARK_REDEAST,
	MAPMARK_REDWEST,
	MAPMARK_GREENNORTH,
	MAPMARK_GREENSOUTH,
};

enum PartyShields_t : uint8_t {
	SHIELD_NONE,
	SHIELD_WHITEYELLOW,
	SHIELD_WHITEBLUE,
	SHIELD_BLUE,
	SHIELD_YELLOW,
	SHIELD_BLUE_SHAREDEXP,
	SHIELD_YELLOW_SHAREDEXP,
	SHIELD_BLUE_NOSHAREDEXP_BLINK,
	SHIELD_YELLOW_NOSHAREDEXP_BLINK,
	SHIELD_BLUE_NOSHAREDEXP,
	SHIELD_YELLOW_NOSHAREDEXP,
	SHIELD_GRAY,
};

/*****************************
********* Structures
*****************************/
struct LightInfo {
	uint8_t level = 0;
	uint8_t color = 0;
	constexpr LightInfo() = default;
	constexpr LightInfo(uint8_t level, uint8_t color) : level(level), color(color) {}
};

#endif