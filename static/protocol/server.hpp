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

#ifndef CANARY_LIB_STATIC_PROTOCOL_SERVER_H
#define CANARY_LIB_STATIC_PROTOCOL_SERVER_H

#include "../pch.hpp"

/*****************************
********* Enums
*****************************/
namespace CanaryLib {
	enum GameServerOpcodes : uint8_t {
		GameServerLoginOrPendingState = 0xA,
		GameServerGMActions = 0xB,
		GameServerEnterGame = 0xF,
		GameServerUpdateNeeded = 0x11,
		GameServerLoginError = 0x14,
		GameServerLoginAdvice = 0x15,
		GameServerLoginWait = 0x16,
		GameServerLoginSuccess = 0x17,
		GameServerLoginToken = 0x18,
		GameServerStoreButtonIndicators = 0x19, // 1097
		GameServerPingBack = 0x1D,
		GameServerPing = 0x1E,
		GameServerChallenge = 0x1F,
		GameServerDeath = 0x28,

		// all in game opcodes must be greater than 50
		GameServerFirstGameOpcode = 0x32,

		// otclient ONLY
		GameServerExtendedOpcode = 0x32,

		// NOTE: add any custom opcodes in this range
		// 51 - 99
		GameServerChangeMapAwareRange = 0x33,

		// original tibia ONLY
		GameServerFullMap = 0x64,
		GameServerMapTopRow = 0x65,
		GameServerMapRightRow = 0x66,
		GameServerMapBottomRow = 0x67,
		GameServerMapLeftRow = 0x68,
		GameServerUpdateTile = 0x69,
		GameServerCreateOnMap = 0x6A,
		GameServerChangeOnMap = 0x6B,
		GameServerDeleteOnMap = 0x6C,
		GameServerMoveCreature = 0x6D,
		GameServerOpenContainer = 0x6E,
		GameServerCloseContainer = 0x6F,
		GameServerCreateContainer = 0x70,
		GameServerChangeInContainer = 0x71,
		GameServerDeleteInContainer = 0x72,
		GameServerSetInventory = 0x78,
		GameServerDeleteInventory = 0x79,
		GameServerOpenNpcTrade = 0x7A,
		GameServerPlayerGoods = 0x7B,
		GameServerCloseNpcTrade = 0x7C,
		GameServerOwnTrade = 0x7D,
		GameServerCounterTrade = 0x7E,
		GameServerCloseTrade = 0x7F,
		GameServerAmbient = 0x82,
		GameServerGraphicalEffect = 0x83,
		GameServerTextEffect = 0x84,
		GameServerMissleEffect = 0x85,
		GameServerMarkCreature = 0x86,
		GameServerTrappers = 0x87,
		GameServerCreatureHealth = 0x8C,
		GameServerCreatureLight = 0x8D,
		GameServerCreatureOutfit = 0x8E,
		GameServerCreatureSpeed = 0x8F,
		GameServerCreatureSkull = 0x90,
		GameServerCreatureParty = 0x91,
		GameServerCreatureUnpass = 0x92,
		GameServerCreatureMarks = 0x93,
		GameServerCreatureType = 0x95,
		GameServerEditText = 0x96,
		GameServerEditList = 0x97,
		GameServerBlessings = 0x9C,
		GameServerPreset = 0x9D,
		GameServerPremiumTrigger = 0x9E, // 1038
		GameServerPlayerDataBasic = 0x9F, // 950
		GameServerPlayerData = 0xA0,
		GameServerPlayerSkills = 0xA1,
		GameServerPlayerState = 0xA2,
		GameServerClearTarget = 0xA3,
		GameServerPlayerModes = 0xA7,
		GameServerSpellDelay = 0xA4, // 870
		GameServerSpellGroupDelay = 0xA5, // 870
		GameServerMultiUseDelay = 0xA6, // 870
		GameServerSetStoreDeepLink = 0xA8, // 1097
		GameServerTalk = 0xAA,
		GameServerChannels = 0xAB,
		GameServerOpenChannel = 0xAC,
		GameServerOpenPrivateChannel = 0xAD,
		GameServerRuleViolationChannel = 0xAE,
		GameServerRuleViolationRemove = 0xAF,
		GameServerRuleViolationCancel = 0xB0,
		GameServerRuleViolationLock = 0xB1,
		GameServerOpenOwnChannel = 0xB2,
		GameServerCloseChannel = 0xB3,
		GameServerTextMessage = 0xB4,
		GameServerCancelWalk = 0xB5,
		GameServerWalkWait = 0xB6,
		GameServerUnjustifiedStats = 0xB7,
		GameServerPvpSituations = 0xB8,
		GameServerFloorChangeUp = 0xBE,
		GameServerFloorChangeDown = 0xBF,
		GameServerChooseOutfit = 0xC8,
		GameServerVipAdd = 0xD2,
		GameServerVipState = 0xD3,
		GameServerVipLogout = 0xD4,
		GameServerTutorialHint = 0xDC,
		GameServerAutomapFlag = 0xDD,
		GameServerCoinBalance = 0xDF, // 1080
		GameServerStoreError = 0xE0, // 1080
		GameServerRequestPurchaseData = 0xE1, // 1080
		GameServerSendShowDescription = 0xEA,
		GameServerSendResourceBalance = 0xEE, // market module
		GameServerQuestLog = 0xF0,
		GameServerQuestLine = 0xF1,
		GameServerCoinBalanceUpdating = 0xF2, // 1080
		GameServerChannelEvent = 0xF3, // 910
		GameServerItemInfo = 0xF4, // 910
		GameServerPlayerInventory = 0xF5, // 910
		GameServerMarketEnter = 0xF6, // 944
		GameServerMarketLeave = 0xF7, // 944
		GameServerMarketDetail = 0xF8, // 944
		GameServerMarketBrowse = 0xF9, // 944
		GameServerModalDialog = 0xFA, // 960
		GameServerStore = 0xFB, // 1080
		GameServerStoreOffers = 0xFC, // 1080
		GameServerStoreTransactionHistory = 0xFD, // 1080
		GameServerStoreCompletePurchase = 0xFE, // 1080

		// 12.x +
		GameServerSendClientCheck = 0x63,
		GameServerSendGameNews = 0x98,
		GameServerSendBlessDialog = 0x9B,
		GameServerSendRestingAreaState = 0xA9,
		GameServerSendUpdateImpactTracker = 0xCC,
		GameServerSendItemsPrice = 0xCD,
		GameServerSendUpdateSupplyTracker = 0xCE,
		GameServerSendUpdateLootTracker = 0xCF,
		GameServerSendKillTrackerUpdate = 0xD1,
		GameServerSendBestiaryEntryChanged = 0xD9,
		GameServerSendDailyRewardCollectionState = 0xDE,
		GameServerSendOpenRewardWall = 0xE2,
		GameServerSendDailyReward = 0xE4,
		GameServerSendRewardHistory = 0xE5,
		GameServerSendPreyTimeLeft = 0xE7,
		GameServerSendPreyData = 0xE8,
		GameServerSendPreyRerollPrice = 0xE9,
		GameServerSendImbuementWindow = 0xEB,
		GameServerSendCloseImbuementWindow = 0xEC,
		GameServerSendError = 0xED,
		GameServerSendCollectionResource = 0xEE,
		GameServerSendTibiaTime = 0xEF
	};
}


#endif