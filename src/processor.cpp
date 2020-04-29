#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{
    // Get the string values from Linux parser
    vector<string> jiffies = LinuxParser::CpuUtilization();

    // Calculations from https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
    
    // current time
    idle_       = stoi(jiffies[3]) + stoi(jiffies[4]);
    non_idle_   = stoi(jiffies[1]) + stoi(jiffies[1]) + + stoi(jiffies[2])
                    + + stoi(jiffies[5]) + + stoi(jiffies[7]);
    total_ = idle_ + non_idle_;

    // calculate utilization
    utilization_ = 1.f * (((total_ - prev_total_) - (idle_ - prev_idle_)))
                            / (total_ - prev_total_);

    // update prev time
    prev_idle_ = idle_;
    prev_total_ = total_;

    return utilization_; 
}