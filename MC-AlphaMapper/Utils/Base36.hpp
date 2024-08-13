#include "../globals.h"
#include <string>
#include <algorithm>

/**
* @file FileLoading.hpp
* @brief The header file for the base36 handler of the TtEL Minecraft Alpha Level Mapper
* This is used on the folder names of each chunk to find their chunk position
* @author Team theEeveeLovers Sean
* @date August 13th, 2024
*/

class Base36_Handler {
public:
	/**
	 * @brief Converts a number to base36
	 * @param number - number to convert
	 * @return the converted base36 string
	 */
	std::string to_base36(unsigned long long number);

	/**
	 * @brief Converts a number from base36
	 * @param base36 - base36 to convert
	 * @return the converted number
	 */
	unsigned long long from_base36(std::string base36);
};