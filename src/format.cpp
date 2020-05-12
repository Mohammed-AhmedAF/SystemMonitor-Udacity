#include "format.h"

#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
	std::string hours;
	std::string minutes;
	
	int minutesInteger = (seconds/60);

	minutes = std::to_string((seconds/60)%60);
	hours = std::to_string(seconds/3600);
	return hours.append(":").append(minutes).append(":").append(std::to_string(seconds%60));
}
