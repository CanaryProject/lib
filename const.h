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

#ifndef CANARY_LIB_CONST_H
#define CANARY_LIB_CONST_H

// TODO: Unify message enums
//  toClient message functions protocolgame.h
enum MessageMode {
	MessageNone = 0,
	MessageSay = 1,
	MessageWhisper = 2,
	MessageYell = 3,
	MessagePrivateFrom = 4,
	MessagePrivateTo = 5,
	MessageChannelManagement = 6,
	MessageChannel = 7,
	MessageChannelHighlight = 8,
	MessageSpell = 9,
	MessageNpcFrom = 10,
	MessageNpcTo = 11,
	MessageGamemasterBroadcast = 12,
	MessageGamemasterChannel = 13,
	MessageGamemasterPrivateFrom = 14,
	MessageGamemasterPrivateTo = 15,
	MessageLogin = 16,
	MessageWarning = 17,
	MessageGame = 18,
	MessageFailure = 19,
	MessageLook = 20,
	MessageDamageDealed = 21,
	MessageDamageReceived = 22,
	MessageHeal = 23,
	MessageExp = 24,
	MessageDamageOthers = 25,
	MessageHealOthers = 26,
	MessageExpOthers = 27,
	MessageStatus = 28,
	MessageLoot = 29,
	MessageTradeNpc = 30,
	MessageGuild = 31,
	MessagePartyManagement = 32,
	MessageParty = 33,
	MessageBarkLow = 34,
	MessageBarkLoud = 35,
	MessageReport = 36,
	MessageHotkeyUse = 37,
	MessageTutorialHint = 38,
	MessageThankyou = 39,
	MessageMarket = 40,
	MessageMana = 41,
	MessageBeyondLast = 42,

	// deprecated
	MessageMonsterYell = 43,
	MessageMonsterSay = 44,
	MessageRed = 45,
	MessageBlue = 46,
	MessageRVRChannel = 47,
	MessageRVRAnswer = 48,
	MessageRVRContinue = 49,
	MessageGameHighlight = 50,
	MessageNpcFromStartBlock = 51,
	LastMessage = 52,
	MessageInvalid = 255
};

enum FluidColors_t {
	FLUID_EMPTY,
	FLUID_BLUE,
	FLUID_RED,
	FLUID_BROWN,
	FLUID_GREEN,
	FLUID_YELLOW,
	FLUID_WHITE,
	FLUID_PURPLE,
};

// TODO: unify fluids
// 	toClient fluids function tools.h
// enum FluidTypes_t {
// 	FLUID_NONE,
// 	FLUID_WATER,
// 	FLUID_MANA,
// 	FLUID_BEER,
// 	FLUID_OIL,
// 	FLUID_BLOOD,
// 	FLUID_SLIME,
// 	FLUID_MUD,
// 	FLUID_LEMONADE,
// 	FLUID_MILK,
// 	FLUID_WINE,
// 	FLUID_LIFE,
// 	FLUID_URINE,
// 	FLUID_RUM,
// 	FLUID_FRUITJUICE,
// 	FLUID_COCONUTMILK,
// 	FLUID_TEA,
// 	FLUID_MEAD,
// 	FLUID_INK,
// 	FLUID_LAVA,
// 	FLUID_SWAMP
// };

enum Color_t {
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

enum Icons_t {
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

enum Skulls_t {
	SKULL_NONE = 0,
	SKULL_YELLOW = 1,
	SKULL_GREEN = 2,
	SKULL_WHITE = 3,
	SKULL_RED = 4,
	SKULL_BLACK = 5,
	SKULL_ORANGE = 6,
};

enum PartyShields_t {
	SHIELD_NONE = 0,
	SHIELD_WHITEYELLOW = 1,
	SHIELD_WHITEBLUE = 2,
	SHIELD_BLUE = 3,
	SHIELD_YELLOW = 4,
	SHIELD_BLUE_SHAREDEXP = 5,
	SHIELD_YELLOW_SHAREDEXP = 6,
	SHIELD_BLUE_NOSHAREDEXP_BLINK = 7,
	SHIELD_YELLOW_NOSHAREDEXP_BLINK = 8,
	SHIELD_BLUE_NOSHAREDEXP = 9,
	SHIELD_YELLOW_NOSHAREDEXP = 10,
	SHIELD_GRAY = 11,
};

enum GuildEmblems_t {
	GUILDEMBLEM_NONE = 0,
	GUILDEMBLEM_ALLY = 1,
	GUILDEMBLEM_ENEMY = 2,
	GUILDEMBLEM_NEUTRAL = 3,
	GUILDEMBLEM_MEMBER = 4,
	GUILDEMBLEM_OTHER = 5,
};

#endif
