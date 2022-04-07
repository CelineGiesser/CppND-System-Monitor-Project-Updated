#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process ::Process(int pid): pid_(pid) {
  CpuUtilizationCalc();
}

// Return this process's ID
int Process::Pid() { 
    return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { 
    return cpuUtil_; }

// Perform this process's CPU utilization calculation and save it in cpuUtil_
void Process:: CpuUtilizationCalc() {
    int clockTicks = sysconf(_SC_CLK_TCK);
    //Attempt to introduce a time delay, but this was not successfull
    /*vector<string> prevCpuParameter=LinuxParser::ProcessCpuUtilization(Pid());
    long prevTimeTotal=std::stol(prevCpuParameter[0])+std::stol(prevCpuParameter[1])+std::stol(prevCpuParameter[2])+std::stol(prevCpuParameter[3]);
    long prevUpTimeD=(LinuxParser::UpTime())*clockTicks-(LinuxParser::UpTime(Pid()));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    vector<string> cpuParameter = LinuxParser::ProcessCpuUtilization(Pid());
    long timeTotal=std::stol(cpuParameter[0])+std::stol(cpuParameter[1])+std::stol(cpuParameter[2])+std::stol(cpuParameter[3]);
    long upTimeD= (LinuxParser::UpTime())*clockTicks-(LinuxParser::UpTime(Pid()));
    if ((upTimeD-prevUpTimeD)==0){
        cpuUtil_=0.000001;}
    else{
        cpuUtil_=(static_cast<float>(timeTotal-prevTimeTotal)/static_cast<float>(upTimeD-prevUpTimeD));}*/
    
    // Calculation without time delay
    vector<string> cpuParameter = LinuxParser::ProcessCpuUtilization(Pid());
    long timeTotal=std::stol(cpuParameter[0])+std::stol(cpuParameter[1])+std::stol(cpuParameter[2])+std::stol(cpuParameter[3]);
    long upTimeD= (LinuxParser::UpTime())*clockTicks-(LinuxParser::UpTime(Pid()));
    cpuUtil_=(static_cast<float>(timeTotal)/static_cast<float>(upTimeD));
    }

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// Return this process's memory utilization in MB
string Process::Ram() { return to_string(stol(LinuxParser::Ram(Pid()))/1000); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return (this->cpuUtil_ > a.cpuUtil_) ? true : false; }