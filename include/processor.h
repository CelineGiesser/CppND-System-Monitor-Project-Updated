#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 long idle_, nonIdle_;
 long prevIdle_{0},prevNonIdle_{0}; 
 long idleD_, totalD_;
};

#endif