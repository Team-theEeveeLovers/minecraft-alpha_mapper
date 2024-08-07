#pragma once
#include "globals.h"

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
enum tagType {
	TAG_End = 0x00,
	TAG_Byte,
	TAG_Short,
	TAG_Int,
	TAG_Long,
	TAG_Float,
	TAG_Double,
	TAG_Byte_Array,
	TAG_String,
	TAG_List,
	TAG_Compound
};

/**
 * @brief A class representing each NBT tag
 */
class tag {
public:
	// the type of the tag
	tagType Type;
	// the name of the tag
	std::string Name;
	// the length of the tag's name
	BYTE NameLength;
};

class ByteTag : public tag {
	// the tag is byte type
	tagType Type = TAG_Byte;
	// the byte value contained within the tag
	BYTE value = 0x00;
};

class LongTag : public tag {
	// the tag is long type
	tagType Type = TAG_Long;


	// we are using long long here because normal long is 32-bit but Java longs are 64-bit
	
	// the long value contained within the tag 
	long long value = 0x00i64; 
};

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