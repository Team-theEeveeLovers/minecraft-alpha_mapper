#include "FileLoading.hpp"

/**
* @file FileLoading.cpp
* @brief The source file for the file handler of the TtEL Minecraft Alpha Level Mapper
* @author Team theEeveeLovers Sean
* @date August 6th, 2024
*/

bool LEVEL_DATA::loadFile(std::string path)
{
	RWops = SDL_RWFromFile(path.c_str(), "r+");
	if (RWops == NULL) {
		SDL_LogError(0, "SDL could not open the file '%s'! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return false;
	}
	else {
		SDL_RWread(RWops, &compressedData, 1, SDL_RWsize(RWops));
		const char* compressed_pointer = compressedData.data();
		std::string decompressed_data = gzip::decompress(compressed_pointer, compressedData.size());
		SDL_RWclose(RWops); // close the stream for right now as we don't need to write anything yet

		std::string outPath = path.append("_temp");
		out_RWops = SDL_RWFromFile(outPath.c_str(), "w+");
		if (out_RWops == NULL) {
			SDL_LogError(0, "SDL could not make new temp file '%s'! SDL Error: %s\n", outPath.c_str(), SDL_GetError());
			return false;
		}
		else {
			SDL_RWwrite(out_RWops, decompressed_data.data(), decompressed_data.size(), 1);
		}
	}
}

void LEVEL_DATA::closeFile(void)
{
	if (RWops != NULL)
		SDL_RWclose(RWops);
}
