#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor
Process::Process(int pid) : pid_(pid)
{
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

void Process::CalculateUtil()
{
    // total time
    auto total_time_jiff = LinuxParser::ActiveJiffies(pid_);

    float usageTime = total_time_jiff/sysconf(_SC_CLK_TCK);

    float totalTime = LinuxParser::UpTime() - LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK);

    this->utilization_ = (usageTime - usageOld_) / (totalTime - timeOld_);

    usageOld_  = usageTime;
    timeOld_   = totalTime;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() 
{
    return utilization_; 
}
// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_) ; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const 
{ 
    return (a.utilization_ < this->utilization_);
}