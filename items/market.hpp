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

#ifndef CANARY_LIB_ITEMS_MARKET_H
#define CANARY_LIB_ITEMS_MARKET_H

#include "../pch.hpp"

/*****************************
********* Enums
*****************************/
enum MarketAction_t : uint8_t {
	MARKETACTION_BUY,
	MARKETACTION_SELL,
};

enum MarketOfferState_t : uint8_t {
	OFFERSTATE_ACTIVE,
	OFFERSTATE_CANCELLED,
	OFFERSTATE_EXPIRED,
	OFFERSTATE_ACCEPTED,

	OFFERSTATE_ACCEPTEDEX = 255,
};

/*****************************
** Structs
*****************************/
struct HistoryMarketOffer {
	uint32_t timestamp;
	uint32_t price;
	uint16_t itemId;
	uint16_t amount;
	MarketOfferState_t state;
};

struct MarketOffer {
	uint32_t price;
	uint32_t timestamp;
	uint16_t amount;
	uint16_t counter;
	uint16_t itemId;
	std::string playerName;
};

struct MarketOfferEx {
	MarketOfferEx() = default;
	MarketOfferEx(MarketOfferEx&& other) :
		id(other.id), playerId(other.playerId), timestamp(other.timestamp), price(other.price),
		amount(other.amount), counter(other.counter), itemId(other.itemId), type(other.type),
		playerName(std::move(other.playerName)) {}

	uint32_t id;
	uint32_t playerId;
	uint32_t timestamp;
	uint32_t price;
	uint16_t amount;
	uint16_t counter;
	uint16_t itemId;
	MarketAction_t type;
	std::string playerName;
};

struct MarketStatistics {
	MarketStatistics() {
		numTransactions = 0;
		highestPrice = 0;
		totalPrice = 0;
		lowestPrice = 0;
	}

	uint32_t numTransactions;
	uint32_t highestPrice;
	uint64_t totalPrice;
	uint32_t lowestPrice;
};

struct ModalWindow
{
	std::list<std::pair<std::string, uint8_t>> buttons, choices;
	std::string title, message;
	uint32_t id;
	uint8_t defaultEnterButton, defaultEscapeButton;
	bool priority;

	ModalWindow(uint32_t id, std::string title, std::string message)
		: title(std::move(title)), message(std::move(message)), id(id), defaultEnterButton(0xFF), defaultEscapeButton(0xFF), priority(false) {}
};

struct ShopInfo {
	uint16_t itemId;
	int32_t subType;
	uint32_t buyPrice;
	uint32_t sellPrice;
	std::string realName;

	ShopInfo() {
		itemId = 0;
		subType = 1;
		buyPrice = 0;
		sellPrice = 0;
	}

	ShopInfo(uint16_t itemId, int32_t subType = 0, uint32_t buyPrice = 0, uint32_t sellPrice = 0, std::string realName = "")
		: itemId(itemId), subType(subType), buyPrice(buyPrice), sellPrice(sellPrice), realName(std::move(realName)) {}
};

/*****************************
** Macros
*****************************/
using HistoryMarketOfferList = std::list<HistoryMarketOffer>;
using MarketOfferList = std::list<MarketOffer>;
using ShopInfoList = std::vector<ShopInfo>;

#endif