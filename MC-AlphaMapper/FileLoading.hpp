#pragma once
#include "globals.h"
#include "Types/NBT.h"
#include <zlib.h>
#include <zconf.h>

#ifndef ZLIB_CONST
#define ZLIB_CONST
#endif

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
public:
	/**
	 * @brief Loads a level.dat file
	 * @param path - Filepath pointing to a level.dat file to load from
	 * @return True on success, or false on failure
	 */
	bool loadFile(std::string path);

	/**
	 * @brief Closes the file stream and frees memory
	 */
	void closeFile(void);
protected:
	// the input file stream context
	SDL_RWops* RWops = NULL;
	// the output file stream context
	SDL_RWops* out_RWops = NULL;
	// the Gzip compressed data in the file
	std::string compressedData;
	// the seed of the world
	LongTag seed;

	/**
	 * @brief Checks if a 3 byte header is valid for GZ
	 * @param bytes - A pointer to an array containing the 3 bytes bytes
	 * @return True if valid, false if invalid
	 */
	bool checkGZheader(Uint8 *bytes);
};

class CHUNK_DATA {
protected:
	LongTag lastUpdate;
};