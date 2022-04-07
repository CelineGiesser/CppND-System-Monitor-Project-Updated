#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <math.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

//Read and return the system memory utilization
float LinuxParser::MemoryUtilization(){  
  string line;
  string key,value,unit;
  float memData [2];
  std::ifstream fileStream(kProcDirectory + kMeminfoFilename);
  if (fileStream.is_open()) {
    for (int iData=0;iData<2;iData++){
      std::getline(fileStream, line);
      std::istringstream lineStream(line);
      lineStream >> key >> value >>unit;
      memData[iData]=std::stol(value);
      }
  }
  return (memData[0]-memData[1])/memData[0]; }

//Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptimeSystem, timeIdle;
  string line;
  std::ifstream fileStream(kProcDirectory + kUptimeFilename);
  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream linestream(line);
    linestream >> uptimeSystem >> timeIdle;
  }
  return std::stol(uptimeSystem); }

// Read and return the diverse CPU utilization parameters
vector<string> LinuxParser::CpuUtilization() { 
  string key;
  string cpuUser,cpuNice,cpuSyst,cpuIdle,cpuIowait,cpuIrq,cpuSoftirq,cpuSteal,cpuGuest,cpuGuestnice;
  string line;
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream lineStream(line);
    lineStream >> key >> cpuUser >> cpuNice >> cpuSyst >> cpuIdle >> cpuIowait >> cpuIrq >> cpuSoftirq 
    >> cpuSteal >> cpuGuest >> cpuGuestnice;
    ///// To be nicer written with push_back
  }
  return {cpuUser,cpuNice,cpuSyst,cpuIdle,cpuIowait,cpuIrq,cpuSoftirq,cpuSteal,cpuGuest,cpuGuestnice}; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key;
  string value;
  string line;
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::istringstream lineStream(line);
      while (lineStream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0; }

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key;
  string value;
  string line;
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::istringstream lineStream(line);
      while (lineStream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value); 
        }
      }
    }
  }
  return 0; }

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {   
  string line, address{"0"};
  std::ifstream fileStream(kProcDirectory + to_string(pid)+kCmdlineFilename);
  if (fileStream.is_open()) {
    std::getline(fileStream, line);}
    std::istringstream lineStream(line);
    lineStream >> address;
  return address; }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line;
  string key, value{"0"};
  std::ifstream fileStream(kProcDirectory + to_string(pid)+kStatusFilename);
  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::istringstream lineStream(line);
      while (lineStream >> key >> value) {
        if (key == "VmSize:") {
          return value; 
        }
      }
    }
  }  
  return value; }

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string key,value{"0"};
  string line;
  std::ifstream fileStream(kProcDirectory + to_string(pid)+kStatusFilename);
  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::istringstream lineStream(line);
      while (lineStream >> key >> value) {
        if (key == "Uid:") {
          return value; 
        }
      }
    }
  }  
  return value; }

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string name{"LaraCroft"}, x, value;
  string line;
  std::ifstream fileStream(kPasswordPath);
  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream(line);
      while (lineStream >> name >> x >>value) {
        if (value == Uid(pid)) {
          return name; 
        }
      }
    }
  }  
  return name; }

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string value;
  string line;
  std::ifstream fileStream(kProcDirectory + to_string(pid)+kStatFilename);
  if (fileStream.is_open()) {
    std::getline(fileStream, line); 
    std::istringstream lineStream(line);
    for (int i=0;i<=21;i++) {
      lineStream >> value;
    }
  }
  return std::stol(value); }
  
  // Read and return the diverse CPU utilization parameters of a process
  vector<string> LinuxParser::ProcessCpuUtilization(int pid) { 
  string value;
  string utime,stime,cutime,cstime;
  string line;
  std::ifstream fileStream(kProcDirectory + to_string(pid) + kStatFilename);
  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream lineStream(line);
    for (int i=0;i<13;i++) {
      lineStream >> value;
    }
    lineStream >> utime >> stime >> cutime >> cstime;
  }
    ///// To be nicer written with push_back
  return {utime,stime,cutime,cstime}; }
