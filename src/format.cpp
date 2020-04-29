#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) 
{
    string time;
    int minutes = seconds / 60;
    int hours   = minutes / 60;

    time = std::to_string(hours) + ":" + std::to_string(minutes)
             +  ":" + std::to_string(seconds % 60);

    return time; 
}