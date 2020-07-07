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

#ifndef CANARY_LIB_GLOBAL_MESSAGE_H
#define CANARY_LIB_GLOBAL_MESSAGE_H

#include "../pch.hpp"

/*****************************
********* Enums
*****************************/
enum ChannelEvent_t : uint8_t {
	CHANNELEVENT_JOIN,
	CHANNELEVENT_LEAVE,
	CHANNELEVENT_INVITE,
	CHANNELEVENT_EXCLUDE,
};

// TODO: Unify message enums
//  toClient message functions protocolgame.h
enum MessageMode : uint8_t {
	MessageNone,
	MessageSay,
	MessageWhisper,
	MessageYell,
	MessagePrivateFrom,
	MessagePrivateTo,
	MessageChannelManagement,
	MessageChannel,
	MessageChannelHighlight,
	MessageSpell,
	MessageNpcFrom,
	MessageNpcTo,
	MessageGamemasterBroadcast,
	MessageGamemasterChannel,
	MessageGamemasterPrivateFrom,
	MessageGamemasterPrivateTo,
	MessageLogin,
	MessageWarning,
	MessageGame,
	MessageFailure,
	MessageLook,
	MessageDamageDealed,
	MessageDamageReceived,
	MessageHeal,
	MessageExp,
	MessageDamageOthers,
	MessageHealOthers,
	MessageExpOthers,
	MessageStatus,
	MessageLoot,
	MessageTradeNpc,
	MessageGuild,
	MessagePartyManagement,
	MessageParty,
	MessageBarkLow,
	MessageBarkLoud,
	MessageReport,
	MessageHotkeyUse,
	MessageTutorialHint,
	MessageThankyou,
	MessageMarket,
	MessageMana,
	MessageBeyondLast,

	// deprecated
	MessageMonsterYell,
	MessageMonsterSay,
	MessageRed,
	MessageBlue,
	MessageRVRChannel,
	MessageRVRAnswer,
	MessageRVRContinue,
	MessageGameHighlight,
	MessageNpcFromStartBlock,
	LastMessage,
	MessageInvalid = 255
};

#endif