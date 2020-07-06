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

#ifndef CANARY_LIB_STATIC_PROTOCOL_CLIENT_H
#define CANARY_LIB_STATIC_PROTOCOL_CLIENT_H

#include "../pch.hpp"

/*****************************
********* Enums
*****************************/
namespace CanaryLib {
	enum ClientOpcodes : uint8_t {
		ClientEnterAccount = 0x1,
		ClientPendingGame = 0xA,
		ClientEnterGame = 0xF,
		ClientLeaveGame = 0x14,
		ClientPing = 0x1D,
		ClientPingBack = 0x1E,

		// all in game opcodes must be equal or greater than 50
		ClientFirstGameOpcode = 0x32,

		// otclient ONLY
		ClientExtendedOpcode = 0x32,
		ClientChangeMapAwareRange = 0x33,

		// NOTE: add any custom opcodes in this range
		// 51 - 99

		// original tibia ONLY
		ClientAutoWalk = 0x64,
		ClientWalkNorth = 0x65,
		ClientWalkEast = 0x66,
		ClientWalkSouth = 0x67,
		ClientWalkWest = 0x68,
		ClientStop = 0x69,
		ClientWalkNorthEast = 0x6A,
		ClientWalkSouthEast = 0x6B,
		ClientWalkSouthWest = 0x6C,
		ClientWalkNorthWest = 0x6D,
		ClientTurnNorth = 0x6F,
		ClientTurnEast = 0x70,
		ClientTurnSouth = 0x71,
		ClientTurnWest = 0x72,
		ClientEquipItem = 0x77, // 910
		ClientMove = 0x78,
		ClientInspectNpcTrade = 0x79,
		ClientBuyItem = 0x7A,
		ClientSellItem = 0x7B,
		ClientCloseNpcTrade = 0x7C,
		ClientRequestTrade = 0x7D,
		ClientInspectTrade = 0x7E,
		ClientAcceptTrade = 0x7F,
		ClientRejectTrade = 0x80,
		ClientUseItem = 0x82,
		ClientUseItemWith = 0x83,
		ClientUseOnCreature = 0x84,
		ClientRotateItem = 0x85,
		ClientCloseContainer = 0x87,
		ClientUpContainer = 0x88,
		ClientEditText = 0x89,
		ClientEditList = 0x8A,
		ClientLook = 0x8C,
		ClientLookCreature = 0x8D,
		ClientTalk = 0x96,
		ClientRequestChannels = 0x97,
		ClientJoinChannel = 0x98,
		ClientLeaveChannel = 0x99,
		ClientOpenPrivateChannel = 0x9A,
		ClientOpenRuleViolation = 0x9B,
		ClientCloseRuleViolation = 0x9C,
		ClientCancelRuleViolation = 0x9D,
		ClientCloseNpcChannel = 0x9E,
		ClientChangeFightModes = 0xA0,
		ClientAttack = 0xA1,
		ClientFollow = 0xA2,
		ClientInviteToParty = 0xA3,
		ClientJoinParty = 0xA4,
		ClientRevokeInvitation = 0xA5,
		ClientPassLeadership = 0xA6,
		ClientLeaveParty = 0xA7,
		ClientShareExperience = 0xA8,
		ClientDisbandParty = 0xA9,
		ClientOpenOwnChannel = 0xAA,
		ClientInviteToOwnChannel = 0xAB,
		ClientExcludeFromOwnChannel = 0xAC,
		ClientCancelAttackAndFollow = 0xBE,
		ClientUpdateTile = 0xC9,
		ClientRefreshContainer = 0xCA,
		ClientBrowseField = 0xCB,
		ClientSeekInContainer = 0xCC,
		ClientRequestOutfit = 0xD2,
		ClientChangeOutfit = 0xD3,
		ClientMount = 0xD4, // 870
		ClientAddVip = 0xDC,
		ClientRemoveVip = 0xDD,
		ClientEditVip = 0xDE,
		ClientBugReport = 0xE6,
		ClientRuleViolation = 0xE7,
		ClientDebugReport = 0xE8,
		ClientTransferCoins = 0xEF, // 1080
		ClientRequestQuestLog = 0xF0,
		ClientRequestQuestLine = 0xF1,
		ClientNewRuleViolation = 0xF2, // 910
		ClientRequestItemInfo = 0xF3, // 910
		ClientMarketLeave = 0xF4, // 944
		ClientMarketBrowse = 0xF5, // 944
		ClientMarketCreate = 0xF6, // 944
		ClientMarketCancel = 0xF7, // 944
		ClientMarketAccept = 0xF8, // 944
		ClientAnswerModalDialog = 0xF9, // 960
		ClientOpenStore = 0xFA, // 1080
		ClientRequestStoreOffers = 0xFB, // 1080
		ClientBuyStoreOffer = 0xFC, // 1080
		ClientOpenTransactionHistory = 0xFD, // 1080
		ClientRequestTransactionHistory = 0xFE  // 1080
	};
}

#endif