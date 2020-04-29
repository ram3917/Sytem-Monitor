#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

#include "linux_parser.h"

using std::vector;
using std::string;
using std::stoi;

class Processor 
{
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
   // current times 
   long idle_, non_idle_, total_;
   long prev_idle_ = 0, prev_total_ = 0;

   float utilization_  = 0.0f;
};

#endif