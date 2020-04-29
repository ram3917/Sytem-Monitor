#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

// TODO : Used for debugging. Remove this
#include <iostream>

using std::stof;
using std::stoi;
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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{
    float Mem_Total, Mem_Free, Buffers;
    string line, key, value;
    std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if (stream.is_open()) 
    {
      while(std::getline(stream, line))
      {
          std::istringstream linestream(line);
        while (linestream >> key >> value) 
        {
          if (key == "MemTotal:") {
            Mem_Total = stof(value);
          }
          else if (key == "MemFree:") {
            Mem_Free = stof(value);
          } 
          else if (key == "Buffers:") {
            Buffers = stof(value);
          }
        }
      }
    }
  return (1.0f - (Mem_Free / (Mem_Total - Buffers))); 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{
  string line, key, value;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> value;
  }

  return stoi(key + value); 
}

/*
* JIFFIES CALCULATION FROM : 
https://knowledge.udacity.com/questions/129844
*/

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() 
{
  // // Ticks since last boot
  // long uptime = LinuxParser::UpTime();

  // long system_freq = sysconf(_SC_CLK_TCK);

  // // Uptime * freq = total ticks
  // return (uptime * system_freq); 

    return 0.0;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) 
{
    long total_time = 0;
    string line, value;
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
    if (stream.is_open()) 
    {
      std::getline(stream, line);
      std::istringstream linestream(line);
      int iter = 1;
      while (std::getline(linestream, value, ' ')) 
      {
        if (iter >= 14 && iter <= 17)
        { 
          total_time += stoi(value);
        }        
        ++iter;
      }
    }
  return total_time; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
  /// NOT USED
  //   long active_jiffies = 0;
  //   string line, value;
  //   std::ifstream stream(kProcDirectory + kStatFilename);
  //   if (stream.is_open()) 
  //   {
  //     std::getline(stream, line);
  //     std::istringstream linestream(line);
  //     while (linestream >> value) 
  //     {
  //       if (value == "cpu") 
  //       {
  //         // skip this
  //         continue;
  //       }
  //       else
  //       {
  //         active_jiffies += stoi(value);
  //       }
  //     }      
  //   }
  // return active_jiffies; 

    return 0.0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{
  /// NOT USED
  //   long idle_jiffies = 0;
  //   string line, value;
  //   std::ifstream stream(kProcDirectory + kStatFilename);
  //   if (stream.is_open()) 
  //   {
  //     std::getline(stream, line);
  //     std::istringstream linestream(line);
  //     int iter = 0;
  //     while (linestream >> value) 
  //     {
  //       // count jiffies for process
  //       if (iter >= 3 && iter <= 4) 
  //       {
  //         idle_jiffies += stoi(value);
  //       }
  //       // increment count
  //       ++iter;
  //     }
  //   }
  // return idle_jiffies;

    return 0.0;
}

// TODO: Read and return CPU utilization
// Used in processor to calcuate CPU Utilization
vector<string> LinuxParser::CpuUtilization() 
{
    vector<string> jiffies;
    string line, value;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) 
    {
      std::getline(stream, line);
      std::istringstream linestream(line);
      
      while (linestream >> value) 
      {
        if (value == "cpu") 
        {
          // skip this
          continue;
        }

        jiffies.push_back(value);
      }
    }

  // vector of jiffies
  return jiffies;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {   
    int processes;
    string line, key, value;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) 
    {
      while(std::getline(stream, line))
      {
        std::istringstream linestream(line);
        while (linestream >> key >> value) 
        {
          if (key == "processes") {
            processes = stoi(value);
          }
        }
      }
    }
  return processes; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{
    int processes;
    string line, key, value;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) 
    {
      while(std::getline(stream, line))
      {
        std::istringstream linestream(line);
        while (linestream >> key >> value) 
        {
          if (key == "procs_running") {
            processes = stoi(value);
          }
        }
      }
    }
  return processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{
    string line;
    std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
    if (stream.is_open()) 
    {
      while(std::getline(stream, line))
      {
        return line;
      }
    }
  return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{
    string line, key;
    int value;
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (stream.is_open()) 
    {
      while(std::getline(stream, line))
      {
        std::istringstream linestream(line);
        while (linestream >> key >> value) 
        {
          if (key == "VmSize:") 
          {
            return to_string(value / 1000);
          }
        }
      }
    }
  return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{
    string line, key, value;
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (stream.is_open()) 
    {
      while(std::getline(stream, line))
      {
        std::istringstream linestream(line);
        while (linestream >> key >> value) 
        {
          if (key == "Uid:") 
          {
            // If UID is found return and exit
            return value;
          } 
        }
      }
    }
  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{
    // Get uid
    auto uid_string = LinuxParser::Uid(pid);

    string line, word;
    std::ifstream stream(kPasswordPath);
    if (stream.is_open()) 
    {
      while(std::getline(stream, line))
      {
        std::stringstream linestream(line);

        vector<std::string> lineString;
        while(std::getline(linestream, word, ':')) 
        {
          lineString.push_back(word);          
        }

        if (lineString[2] == uid_string) 
        {
          // If UID is found return and exit
          return lineString[0];
        }        
      }
    }  
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{
   long uptime = 0;
    string line, value;
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
    if (stream.is_open()) 
    {
      std::getline(stream, line);
      std::istringstream linestream(line);

      // Iter 
      int iter = 1;
      while (std::getline(linestream, value, ' '))
      {
        if (22 == iter)
        { 
          uptime = stoi(value) / sysconf(_SC_CLK_TCK);
          break;
        }

        // increment iter
        ++iter;
      }
    }

    return uptime;
}