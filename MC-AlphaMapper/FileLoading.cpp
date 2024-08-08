#include "FileLoading.hpp"

/**
* @file FileLoading.cpp
* @brief The source file for the file handler of the TtEL Minecraft Alpha Level Mapper
* @author Team theEeveeLovers Sean
* @date August 6th, 2024
*/

bool gzipInflate(const std::string& compressedBytes, std::string& uncompressedBytes) {
	if (compressedBytes.size() == 0) {
		uncompressedBytes = compressedBytes;
		return true;
	}

	uncompressedBytes.clear();

	unsigned full_length = compressedBytes.size();
	unsigned half_length = compressedBytes.size() / 2;

	unsigned uncompLength = full_length;
	char* uncomp = (char*)calloc(sizeof(char), uncompLength);

	z_stream strm;
	strm.next_in = (Bytef*)compressedBytes.c_str();
	strm.avail_in = compressedBytes.size();
	strm.total_out = 0;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;

	bool done = false;

	if (inflateInit2(&strm, (16 + MAX_WBITS)) != Z_OK) {
		free(uncomp);
		return false;
	}

	while (!done) {
		// If our output buffer is too small  
		if (strm.total_out >= uncompLength) {
			// Increase size of output buffer  
			char* uncomp2 = (char*)calloc(sizeof(char), uncompLength + half_length);
			memcpy(uncomp2, uncomp, uncompLength);
			uncompLength += half_length;
			free(uncomp);
			uncomp = uncomp2;
		}

		strm.next_out = (Bytef*)(uncomp + strm.total_out);
		strm.avail_out = uncompLength - strm.total_out;

		// Inflate another chunk.  
		int err = inflate(&strm, Z_SYNC_FLUSH);
		if (err == Z_STREAM_END) done = true;
		else if (err != Z_OK && err != Z_BUF_ERROR) {
			SDL_LogError(0, "zlib couldn't extract the file because: '%s'\n", strm.msg);
			if (strm.msg != NULL) {
				throw std::runtime_error(strm.msg);
				//ASSERT(err == Z_OK && "zlib was unable to uncompress the file!");
			}
			break;
		}
		else if (err == Z_BUF_ERROR) {
			break;
		}
		else if (err != Z_STREAM_END && err != Z_OK && err != Z_BUF_ERROR) {
			SDL_LogError(0, "zlib couldn't extract the file because: '%s'\n", strm.msg);
		}
	}

	int result = inflateEnd(&strm);
	if (result != Z_OK && result != Z_BUF_ERROR) {
		free(uncomp);
		return false;
	}

	for (size_t i = 0; i < strm.total_out; ++i) {
		uncompressedBytes += uncomp[i];
	}
	free(uncomp);
	return true;
}

bool LEVEL_DATA::loadFile(std::string path)
{
	// close file to prevent any read access related errors
	closeFile();
	RWops = SDL_RWFromFile(path.c_str(), "r+b");
	if (RWops == NULL) {
		SDL_LogError(0, "SDL could not open the file '%s'! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return false;
	}
	else {
		//char *read_buffer = new char[SDL_RWsize(RWops) + 1];
		// 
		////Uint8 GZipHeader[3];
		////SDL_RWread(RWops, &GZipHeader, sizeof(Uint8), 3);
		////if (!checkGZheader(GZipHeader)) {
			////SDL_LogError(0, "GZip header is not valid!\n");
			////return false;
		////}
		if (false) {
			// something's very wrong here if this gets run
			ASSERT(false == false && "Broken compiler, please reconsider your life decisions");
			return false; 
		}
		else {
			char char_to_buffer[2] = { '\0', '\0' };
			for (int i = 0; i < SDL_RWsize(RWops); i++) {
				SDL_RWread(RWops, &char_to_buffer, sizeof(char), 1);
				//if (char_to_buffer == '\0')
				//	char_to_buffer = 0xFF;

				//read_buffer[i] = char_to_buffer;
				if (char_to_buffer[0] == '\0')
					compressedData.append("\n");
				else
					compressedData.append(&char_to_buffer[0]);
			}
			//SDL_RWread(RWops, read_buffer, sizeof(char), SDL_RWsize(RWops)); bad code
			SDL_RWclose(RWops); // close the stream for right now as we don't need to write anything yet
			//compressedData = std::string(read_buffer);
			//const char* compressed_pointer = compressedData.data();
			//std::string decompressed_data = gzip::decompress(compressed_pointer, compressedData.size());

			std::string decompressed_data;
			if (!gzipInflate(compressedData, decompressed_data)) {
				ASSERT(false && "Could not uncompress!");
				return false;
			}

			std::string outPath = path.append("_temp");
			out_RWops = SDL_RWFromFile(outPath.c_str(), "w+");
			if (out_RWops == NULL) {
				SDL_LogError(0, "SDL could not make new temp file '%s'! SDL Error: %s\n", outPath.c_str(), SDL_GetError());
				return false;
			}
			else {
				SDL_RWwrite(out_RWops, decompressed_data.data(), decompressed_data.size(), 1);
				SDL_RWclose(out_RWops);
				out_RWops = SDL_RWFromFile(outPath.c_str(), "r+");
				return true;
			}
		}
	}
}

void LEVEL_DATA::closeFile(void)
{
	if (RWops != NULL)
		SDL_RWclose(RWops);
	if (out_RWops != NULL)
		SDL_RWclose(out_RWops);
}

bool LEVEL_DATA::checkGZheader(Uint8* bytes)
{
	Uint8 ByteArray[3] = { bytes[0], bytes[1], bytes[2] };
	if (ByteArray[0] != 0x1Fu)
		return false;
	else if (ByteArray[1] != 0x8Bu)
		return false;
	else if (ByteArray[2] != 0x08u)
		return false;
	else
		return true;
}
