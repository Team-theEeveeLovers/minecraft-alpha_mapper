/**
* @file Time.h
* @brief A custom version of ctime for the TtEL Minecraft Alpha Level Mapper
* @author Team theEeveeLovers Sean
* @date August 13th, 2024
*/
#pragma once
#include "../globals.h"

/**
 * @brief A class for handling 64-bit Unix timestamps in milisecond format
 */
class Time {
private:
	/**
	 * @brief INTERNAL FUNCTION: Convert miliseconds to seconds
	 * @return unixTime in seconds
	 */
	Sint64 msToSecond() const {
		return unixTime / 1000;
	}
public:
	Sint64 unixTime;

	/**
	 * @brief Get the seconds value of the time
	 * @return The seconds value of the time
	 */
	Uint8 toSecond() const {
	//	Sint64 timeWithoutMS = unixTime / 1000;

		return msToSecond() % 60;
	}

	/**
	* @brief Get the minutes value of the time
	* @return The minutes value of the time
	*/
	Uint8 toMinute() const {
	//	Sint64 timeWithoutMS = unixTime / 1000;

		
		return int(msToSecond()/60) % 60;
	}

	/**
	* @brief Get the hours value of the time
	* @return The hours value of the time
	*/
	Uint8 toHour() const {
	//	Sint64 timeWithoutMS = unixTime / 1000;


		return int(msToSecond() / 60 / 60) % 24;
	}

	/**
	* @brief Get the days value of the time
	* @return The days value of the time
	*/
	Sint64 toDays() const {
	//	Sint64 timeWithoutMS = unixTime / 1000;

		return int(msToSecond() / 60 / 60 / 24);
	}

	/**
	* @brief Get the years value of the time
	* @return The years passed value of the time
	*/
	Sint64 toYears() const {
	//	Sint64 timeWithoutMS = unixTime / 1000;

		return int((msToSecond() / 60 / 60 / 24) / 365.25);
	}

	/**
	* @brief Get the year value of the time
	* @return The year value of the time
	*/
	Sint64 toYear() const {
		Sint64 years = toYears();

		return (1970 + years);
	}

	/**
	* @brief Get the day value of the time
	* @return The day value of the time
	*/
	Uint8 toDay() const {
		Sint64 days = toDays();
		Sint64 year = toYear();
		Uint8 months = toMonth();

		//days -= (toYears() * 365);

		if (days > 365) {
			while (days > 365) {
				days -= 365;
			}
		}


		bool leapYear = false;
		if (year % 4 == 0)
			leapYear = true;

		int month = 0;
		while (month < months) {
			month++;
			int daysInMonth = 0;
			if (month == 2) { // if Februrary
				daysInMonth = 28;

				if (leapYear) // toyear is a leap year so 29 days
					daysInMonth++;

			}
			else if (month <= 7) { // the month is July or before
				daysInMonth = 30;

				if (month & 1) // odd numbered month so 31 days
					daysInMonth++;

			}
			else { // the month is August or after
				daysInMonth = 30;

				if (!(month & 1)) // even numbered month so 31 days
					daysInMonth++;

			}

			if (days > daysInMonth) {
				days -= daysInMonth;
			}
			else {
				break;
			}
		}
		return days - 12;
	}

	/**
	* @brief Get the month value of the time
	* @return The month value of the time
	*/
	Uint8 toMonth() const {
		Sint64 days = toDays();
		Sint64 year = toYear();

		if (days > 365) {
			while (days > 365) {
				days -= 365;
			}
		}

		bool leapYear = false;
		if (year % 4 == 0)
			leapYear = true;

		int month = 0;
		while (month < 12) {
			month++;
			int daysInMonth = 0;
			if (month == 2) { // if Februrary
				daysInMonth = 28;
				if (leapYear) // toyear is a leap year so 29 days
					daysInMonth++;
			}
			else if (month <= 7) {
				daysInMonth = 30;
				if (month & 1) // odd numbered month so 31 days
					daysInMonth++;
			}
			else {
				daysInMonth = 30;
				if (!(month & 1)) // even numbered month so 31 days
					daysInMonth++;
			}
			if (days > daysInMonth) {
				days -= daysInMonth;
			}
			else {
				break;
			}
		}
		return month;
	}
};