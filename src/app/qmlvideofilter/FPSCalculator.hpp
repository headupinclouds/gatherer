#ifndef FPSCALCULATOR_HPP_
#define FPSCALCULATOR_HPP_

#include <chrono>

class FPSCalculator {
 public:
  using Clock = std::chrono::high_resolution_clock;
  using Duration = Clock::duration;
  using TimePoint = Clock::time_point;
  using MS = std::chrono::milliseconds;

  FPSCalculator();

  int fps();

 private:
  TimePoint last_;
  int count_;
  int fps_;
};

#endif // FPSCALCULATOR_HPP_
