#include <string>
#include <vector>
#include <math.h>
#include <thread>
#include <chrono>
#include <unistd.h>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;

// Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<string> prevCpuParameter=LinuxParser::CpuUtilization();
    // Sum idle and non idle parameters
    prevIdle_=std::stol(prevCpuParameter[3])+std::stol(prevCpuParameter[4]);
    prevNonIdle_=std::stol(prevCpuParameter[0])+std::stol(prevCpuParameter[1])+std::stol(prevCpuParameter[2])
    +std::stol(prevCpuParameter[5])+std::stol(prevCpuParameter[6])+std::stol(prevCpuParameter[7]);  
    // Wait 1s
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    vector<string> cpuParameter=LinuxParser::CpuUtilization();
    // Sum idle and non idle parameters
    idle_=std::stol(cpuParameter[3])+std::stol(cpuParameter[4]);
    nonIdle_=std::stol(cpuParameter[0])+std::stol(cpuParameter[1])+std::stol(cpuParameter[2])
    +std::stol(cpuParameter[5])+std::stol(cpuParameter[6])+std::stol(cpuParameter[7]);
    // Calculate the difference between the 0.1s of the idle and total (idle+non idle) parameters
    idleD_=idle_-prevIdle_;
    totalD_=(idle_+nonIdle_)-(prevIdle_+prevNonIdle_);

    return (static_cast<float>(totalD_-idleD_)/static_cast<float>(totalD_)); }