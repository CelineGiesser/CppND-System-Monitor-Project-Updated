#include <string>
#include <math.h>
#include "format.h"
#include <sstream>
#include <iostream>
#include <iomanip>

using std::string;

// Convert a time in second in a time in a HH:MM:SS format
string Format::ElapsedTime(long seconds) { 
    double hh=floor(seconds/3600);
    double mm=floor((seconds%3600)/60);
    double ss=seconds%60;
    std::stringstream streamTime;
    streamTime<<std::setw(2)<<std::setfill('0')<<hh<<":"<<std::setw(2)<<std::setfill('0')<<mm<<":"<<std::setw(2)<<std::setfill('0')<<ss;
    return streamTime.str(); }