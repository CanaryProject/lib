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
	HistoryMarketOffer() = default;
	HistoryMarketOffer(uint32_t timestamp, uint32_t price, uint16_t itemId, uint16_t amount, MarketOfferState_t state) :
		timestamp(timestamp), price(price), itemId(itemId), amount(amount), state(state) {}

	uint32_t timestamp;
	uint32_t price;
	uint16_t itemId;
	uint16_t amount;
	MarketOfferState_t state;
};

struct MarketOffer {
	MarketOffer() = default;
	MarketOffer(uint32_t price, uint32_t timestamp, uint16_t amount, uint16_t counter, uint16_t itemId, std::string playerName) :
		price(price), timestamp(timestamp), amount(amount), counter(counter), itemId(itemId), playerName(std::move(playerName)) {}

	// copyable
	MarketOffer(const MarketOffer& rhs) :
		price(rhs.price), timestamp(rhs.timestamp), amount(rhs.amount), counter(rhs.counter), itemId(rhs.itemId), playerName(rhs.playerName) {}
	MarketOffer& operator=(const MarketOffer& rhs) {
		if (this != &rhs) {
			price = rhs.price;
			timestamp = rhs.timestamp;
			amount = rhs.amount;
			counter = rhs.counter;
			itemId = rhs.itemId;
			playerName = rhs.playerName;
		}
		return *this;
	}

	// moveable
	MarketOffer(MarketOffer&& rhs) noexcept :
		price(rhs.price), timestamp(rhs.timestamp), amount(rhs.amount), counter(rhs.counter), itemId(rhs.itemId), playerName(std::move(rhs.playerName)) {}
	MarketOffer& operator=(MarketOffer&& rhs) noexcept {
		if (this != &rhs) {
			price = rhs.price;
			timestamp = rhs.timestamp;
			amount = rhs.amount;
			counter = rhs.counter;
			itemId = rhs.itemId;
			playerName = std::move(rhs.playerName);
		}
		return *this;
	}

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
	uint32_t numTransactions = 0;
	uint32_t highestPrice = 0;
	uint64_t totalPrice = 0;
	uint32_t lowestPrice = 0;
};

struct ModalWindow
{
	std::vector<std::pair<std::string, uint8_t>> buttons, choices;
	std::string title, message;
	uint32_t id;
	uint8_t defaultEnterButton, defaultEscapeButton;
	bool priority;

	ModalWindow(uint32_t id, std::string title, std::string message) :
    title(std::move(title)), message(std::move(message)), id(id), defaultEnterButton(0xFF), defaultEscapeButton(0xFF), priority(false) {}
};

struct ShopInfo {
	ShopInfo() = default;
	ShopInfo(uint16_t itemId, int32_t subType = 0, uint32_t buyPrice = 0, uint32_t sellPrice = 0, std::string realName = "") :
		itemId(itemId), subType(subType), buyPrice(buyPrice), sellPrice(sellPrice), realName(std::move(realName)) {}

	// copyable
	ShopInfo(const ShopInfo& rhs) :
		itemId(rhs.itemId), subType(rhs.subType), buyPrice(rhs.buyPrice), sellPrice(rhs.sellPrice), realName(rhs.realName) {}
	ShopInfo& operator=(const ShopInfo& rhs) {
		if (this != &rhs) {
			itemId = rhs.itemId;
			subType = rhs.subType;
			buyPrice = rhs.buyPrice;
			sellPrice = rhs.sellPrice;
			realName = rhs.realName;
		}
		return *this;
	}

	// moveable
	ShopInfo(ShopInfo&& rhs) noexcept :
		itemId(rhs.itemId), subType(rhs.subType), buyPrice(rhs.buyPrice), sellPrice(rhs.sellPrice), realName(std::move(rhs.realName)) {}
	ShopInfo& operator=(ShopInfo&& rhs) noexcept {
		if (this != &rhs) {
			itemId = rhs.itemId;
			subType = rhs.subType;
			buyPrice = rhs.buyPrice;
			sellPrice = rhs.sellPrice;
			realName = std::move(rhs.realName);
		}
		return *this;
  }

	uint16_t itemId = 0;
	int32_t subType = 1;
	uint32_t buyPrice = 0;
	uint32_t sellPrice = 0;
	std::string realName;
};

/*****************************
** Macros
*****************************/
using HistoryMarketOfferList = std::vector<HistoryMarketOffer>;
using MarketOfferList = std::vector<MarketOffer>;
using ShopInfoList = std::vector<ShopInfo>;

#endif