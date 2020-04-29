#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/

class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:

 // C'tor
 Process(int pid);

 void CalculateUtil();   

 // pid
 int pid_;
 float utilization_ = 0.f;

 // old data
 float timeOld_      = 0.0f; 
 float usageOld_     = 0.0f; 

 // Define friends
 friend class System;
};

#endif