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

#ifndef CANARY_LIB_STATIC_MESSAGE_H
#define CANARY_LIB_STATIC_MESSAGE_H

#include "../pch.hpp"

/*****************************
********* Enums
*****************************/
enum ChannelEvent_t : uint8_t {
	CHANNELEVENT_JOIN = 0,
	CHANNELEVENT_LEAVE = 1,
	CHANNELEVENT_INVITE = 2,
	CHANNELEVENT_EXCLUDE = 3,
};

// TODO: Unify message enums
//  toClient message functions protocolgame.h
enum MessageMode : uint8_t {
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

#endif