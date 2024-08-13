#include "Base36.hpp"

/**
* @file Base36.cpp
* @brief The source file for the base36 handler of the TtEL Minecraft Alpha Level Mapper
* This is used on the folder names of each chunk to find their chunk position
* @author Team theEeveeLovers Sean
* @date August 13th, 2024
*/

std::string Base36_Handler::to_base36(unsigned long long number)
{
	std::string Base36digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string Result = "";

	// if the number is 0 skip the whole conversion process as it will achieve the same result
	if (number == 0)
		return "0";

	// iterate through each digit of the number and convert them to base36
	while (number > 0) {
		Result += Base36digits[number % 36ULL];
		number /= 36ULL;
	}

	// reverse the result since base36 is big endian and most computers are little endian
	std::reverse(Result.begin(), Result.end());
	return Result;

}

unsigned long long Base36_Handler::from_base36(std::string base36)
{
	std::string Base36digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	unsigned long long Result = 0;
	unsigned long long Power = 1;

	// if the base36 is 0 skip the whole conversion process as it will achieve the same result
	if (base36 == "0")
		return 0;

	// iterate through each digit of the number and convert them from base36
	for (int i = base36.length() - 1; i >= 0; i--) {
		char digit = base36[i];
		int value = Base36digits.find(digit);

		// if the digit wasn't found meaning the string is not a valid base36 number
		if (value == std::string::npos)
			throw std::invalid_argument("Invalid base36 character");

		Result += value * Power;
		Power *= 36ULL;

	}
	return Result;
}
