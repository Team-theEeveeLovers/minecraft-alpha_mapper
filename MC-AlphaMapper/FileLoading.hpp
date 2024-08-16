#pragma once
#include "globals.h"
#include "Types/NBT.h"
#include <zlib.h>

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
 * @brief A class representing the level.dat file
 */
class LEVEL_DATA {
private:
	// File signature using unused padding space in memory caused by alignment
	char signature[7] = { '\0', 'L', 'V', 'L', 'd', 't', '\0' };
public:
	// Is the file loaded?
	bool initalized = false;

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

	inline LongTag getLastPlayTime() { return lastPlay; }

protected:
	// the input file stream context
	SDL_RWops* RWops = NULL;
	// the output file stream context
	SDL_RWops* out_RWops = NULL;
	// the Gzip compressed data in the file
	std::string compressedData;

	// the root tag
	CompoundTag root;
	// Global level data
	CompoundTag data;
	// the last time the world was played
	LongTag lastPlay;
	// the seed of the world
	LongTag seed;

	/**
	 * @brief Checks if a 3 byte header is valid for GZ
	 * @param bytes - A pointer to an array containing the 3 bytes bytes
	 * @return True if valid, false if invalid
	 */
	bool checkGZheader(Uint8 *bytes);

	/**
	 * @brief INTERNAL FUNCTION: Reads data from the file
	 * @return True on success, or false on failure
	 */
	bool readFile();
};

class CHUNK_DATA {
protected:
	// the root tag
	CompoundTag root;

	// the tick when the chunk was last updated
	LongTag lastUpdate;
};