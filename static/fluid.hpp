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

#ifndef CANARY_LIB_STATIC_FLUID_H
#define CANARY_LIB_STATIC_FLUID_H

#include "../pch.hpp"

/*****************************
********* Enums
*****************************/
enum FluidColors_t : uint8_t {
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
// enum FluidTypes_t : uint8_t {
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

#endif