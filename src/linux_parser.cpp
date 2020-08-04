#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "format.h"

using std::stof;
using std::stol;
using std::stoi;
using std::stod;
using std::string;
using std::to_string;
using std::vector;
using std::cout;

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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> version;
  }
  return version;
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

float LinuxParser::MemoryUtilization() { 
  float totalmemory;
  float freememory;
  string line;
  string key;
  float value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
    if(filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
        while(linestream >> key >> value) {
          if(key == "MemTotal:") {
            totalmemory = value;
          }
          else if(key == "MemFree:") {
            freememory = value;
          }
        }
      }
    }
  return (totalmemory-freememory) / totalmemory; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string totalUpTime;
  float returnUpTime = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
    if(filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        if(linestream >> totalUpTime) {
          returnUpTime = stof(totalUpTime);
        }
      }
    }
  return returnUpTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return LinuxParser::UpTime() * sysconf(_SC_CLK_TCK);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string utime;
  string stime;
  string cutime;
  string cstime;
  float totalJiffies = 0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if(filestream.is_open()) {
  	std::getline(filestream, line);
  	std::istringstream linestream(line);
  	if(linestream >> utime) {
  	  totalJiffies += stof(utime);
  	}
  	if(linestream >> stime) {
  	  totalJiffies += stof(stime);
 	 }
 	 if(linestream >> cutime) {
      totalJiffies += stof(cutime);
 	 }
 	 if(linestream >> cstime) {
      totalJiffies += stof(cstime);
  	 }
  }
  return totalJiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string line;
  string key;
  string cpu;
  string jiffiesString;
  float jiffies = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()) {
  	while(std::getline(filestream, line)) {
  	  std::istringstream linestream(line);
  	  linestream >> cpu >> jiffiesString;
	    //jiffiesString = cpu;
	    int space1 = jiffiesString.find(" ");
	    string j2 = jiffiesString.substr(space1 + 1);
	    int space2 = j2.find(" ");
	    string j3 = jiffiesString.substr(space2 + 1);
	    int space3 = j3.find(" ");
	    string j4 = jiffiesString.substr(space3 + 1);
	    int space4 = j4.find(" ");
	    string j5 = jiffiesString.substr(space4 + 1);
	    int space5 = j5.find(" ");
	    string j6 = jiffiesString.substr(space5 + 1);
	    int space6 = j6.find(" ");
	    string j7 = jiffiesString.substr(space6 + 1);
	    int space7 = j7.find(" ");
        auto cpu1 = stol(jiffiesString.substr(0, space1 - 1));
        auto cpu2 = stol(jiffiesString.substr(space1 + 1, space2 - 1));
        auto cpu3 = stol(jiffiesString.substr(space2 + 1, space3 - 1));
        auto cpu6 = stol(jiffiesString.substr(space5 + 1, space6 - 1));
        auto cpu7 = stol(jiffiesString.substr(space6 + 1, space7 - 1));
        auto cpu8 = stol(jiffiesString.substr(space7 + 1, jiffiesString.length()));
	    jiffies = cpu1 + cpu2 + cpu3 + cpu6 + cpu7 + cpu8;
    }
  }
  return jiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line;
  string cpu;
  string jiffiesString;
  float jiffies = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> jiffiesString;
      int space1 = jiffiesString.find(" ");
      string j2 = jiffiesString.substr(space1 + 1);
      int space2 = j2.find(" ");
      string j3 = jiffiesString.substr(space2 + 1);
      int space3 = j3.find(" ");
      string j4 = jiffiesString.substr(space3 + 1);
      int space4 = j4.find(" ");
      string j5 = jiffiesString.substr(space4 + 1);
      int space5 = j5.find(" ");
      auto cpu4 = stol(jiffiesString.substr(space3 + 1, space4 - 1));
      auto cpu5 = stol(jiffiesString.substr(space4 + 1, space5 - 1));
      jiffies = cpu4 + cpu5;
  }
  return jiffies;
}

float LinuxParser::CpuUtilization() {
    float active = ActiveJiffies();
    float idle = IdleJiffies();
    if (active + idle > 0.0f) { return active / (active + idle); }
    return 0.0f;
}

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization(int pid) { 
  string line;
  string key;
  string Cpus_allowed;
  float returnValue;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> Cpus_allowed >> returnValue;
    }
  }
  return returnValue; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string processes;
  string returnString;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> processes >> returnString;
  }
  return stoi(returnString);
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string procs_running;
  string returnString;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key >> value;
    if(key == "procs_running ") {
      returnString = value;
    }
  }
  int running = 0;
  if(returnString != "") {
    running = stoi(returnString);
  }
  return running;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string VmSize;
  string returnValue;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> VmSize >> returnValue;
    }
  }
  return to_string(std::stod(returnValue)/1000);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, key, value, returnValue;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(filestream.is_open()) {
  	while(std::getline(filestream, line)) {
  	  std::istringstream linestream(line);
  	  linestream >> key >> value;
      if(key == "Uid:") { returnValue = value; }
    }
  }
  return returnValue;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line, key, value, x, username;
  bool found{false};
  string uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line) && !found) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> x >> value; 
      if (value == uid) {
        username = key;
        found = true;       
      }
    }
  }
  return username; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  long uptime = 0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kUptimeFilename);
  if(filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for(int i = 0; i <= 21; i++) {
      linestream >> uptime;
    }
  }
  return UpTime() - (uptime / sysconf(_SC_CLK_TCK));
}
