#pragma once
#include "globals.h"
#include "Types/NBT.h"

#define BYTE Uint8

/**
* @file FileLoading.hpp
* @brief The header file for the file handler of the TtEL Minecraft Alpha Level Mapper
* @author Team theEeveeLovers Sean
* @date August 6th, 2024
*/

/**
 * @brief An enum representing the types of NBT tags. 
 * Made according to https://web.archive.org/web/20110723210920/http://www.minecraft.net/docs/NBT.txt
 */


/**
 * @brief A class representing the level.dat file
 */
class LEVEL_DATA {

protected:
	// the file stream context
	SDL_RWops* RWops;
	// the Gzip compressed data in the file
	std::string compressedData;
	// the seed of the world
	LongTag seed;
};