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

	// reset initalization flag
	initalized = false;

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
				//
				//read_buffer[i] = char_to_buffer;


				// check if the value is NULL because it would be considered a terminator by the append function
				// we use a count value of 1 to force the append function to add the NULL terminator
				// TODO: Use the count value in the normal append to remove the need for the if statement
				if (char_to_buffer[0] == '\0') 
					compressedData.append("\0", 1);
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

				// free memory as file now has data
				decompressed_data.clear();


				out_RWops = SDL_RWFromFile(outPath.c_str(), "r+");
				if (out_RWops == NULL) {
					SDL_LogError(0, "SDL could not reopen out file! SDL Error: %s\n", SDL_GetError());
					ASSERT(false && "Could not reopen out file!");
					return false;
				}

				if (!readFile()) {
					SDL_LogError(0, "Could not read the level file!\n");
					return false;
				}
				else {
					initalized = true;
				}
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

bool LEVEL_DATA::readFile()
{

	// make a buffer for data reading and processing
	BYTE dataBuffer[16];

	// Initial read
	if (SDL_RWread(out_RWops, dataBuffer, sizeof(BYTE), 1) == 0) {
		throw std::invalid_argument("Error: Zero-length level file!");
		return false;
	}

	// find root tag
	if (dataBuffer[0] != 0x0A) {
		throw std::invalid_argument("Error: Invalid level file!");
		return false;
	}
	else {
		// read again
		if (SDL_RWread(out_RWops, dataBuffer, sizeof(BYTE), 2) == 0) {
			throw std::invalid_argument("Error: invalid length level file!");
			return false;
		}
		// copy two Uint8s to Uint16 value
		Uint16* NameLength_Pointer = &root.NameLength;

		// copy value in reverse order because different endianness
		memcpy(NameLength_Pointer, &dataBuffer[1], 1);
		memcpy(NameLength_Pointer+1, &dataBuffer[0], 1);
		
		if (root.NameLength != 0) {
			throw std::invalid_argument("Error: not a valid NBT file!");
			return false;
		}

		// read more
		if (SDL_RWread(out_RWops, dataBuffer, sizeof(BYTE), 3) == 0) {
			throw std::invalid_argument("Error: invalid length level file!");
			return false;
		}

		// find Data tag
		if (dataBuffer[0] != 0x0A) {
			throw std::invalid_argument("Error: Invalid level file!");
			return false;
		}
		else {
			// copy two Uint8s to Uint16 value
			NameLength_Pointer = &data.NameLength;

			// copy value in reverse order because different endianness
			memcpy(NameLength_Pointer, &dataBuffer[2], 1);
			memcpy(NameLength_Pointer + 1, &dataBuffer[1], 1);

			if (data.NameLength != 4) {
				throw std::invalid_argument("Error: not a valid level file!");
				return false;
			}
			else {
				// read name
				if (SDL_RWread(out_RWops, dataBuffer, sizeof(BYTE), 4) == 0) {
					throw std::invalid_argument("Error: invalid length NBT tag!");
					return false;
				}
				// null terminator to prevent corrupt
				dataBuffer[4] = 0x00;
				// add to name
				data.Name.append(reinterpret_cast<char*>(dataBuffer));
				
				// read again
				if (SDL_RWread(out_RWops, dataBuffer, sizeof(BYTE), 3) == 0) {
					throw std::invalid_argument("Error: invalid length level file!");
					return false;
				}
				// Find last playtime
				if (dataBuffer[0] == 0x04) {
					// copy two Uint8s to Uint16 value
					NameLength_Pointer = &lastPlay.NameLength;

					// copy value in reverse order because different endianness
					memcpy(NameLength_Pointer, &dataBuffer[2], 1);
					memcpy(NameLength_Pointer + 1, &dataBuffer[1], 1);

					if (SDL_RWread(out_RWops, dataBuffer, sizeof(BYTE), lastPlay.NameLength) == 0) {
						throw std::invalid_argument("Error: invalid length level file!");
						return false;
					}
					else {
						// null terminator to prevent corrupt
						dataBuffer[10] = 0x00;
						// add to name
						lastPlay.Name.append(reinterpret_cast<char*>(dataBuffer));

						if (SDL_RWread(out_RWops, dataBuffer, sizeof(BYTE), 8) == 0) {
							throw std::invalid_argument("Error: invalid length long tag!");
							return false;
						}
						else {
							Sint64* longValue_PTR = &lastPlay.value;
							Uint8* casted_longValue_PTR = reinterpret_cast<Uint8*>(longValue_PTR); // cast because adding to the pointer increments in 8 byte segments

							// copy value in reverse order because different endianness
							memcpy(casted_longValue_PTR + 0, &dataBuffer[7], sizeof(BYTE));
							memcpy(casted_longValue_PTR + 1, &dataBuffer[6], sizeof(BYTE));
							memcpy(casted_longValue_PTR + 2, &dataBuffer[5], sizeof(BYTE));
							memcpy(casted_longValue_PTR + 3, &dataBuffer[4], sizeof(BYTE));
							memcpy(casted_longValue_PTR + 4, &dataBuffer[3], sizeof(BYTE));
							memcpy(casted_longValue_PTR + 5, &dataBuffer[2], sizeof(BYTE));
							memcpy(casted_longValue_PTR + 6, &dataBuffer[1], sizeof(BYTE));
							memcpy(casted_longValue_PTR + 7, &dataBuffer[0], sizeof(BYTE));
						}
					}
				}

			}
		}
	}

	return true;
}
