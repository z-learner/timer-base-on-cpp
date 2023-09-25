#include "timer_manager/timer.hpp"

#include <chrono>
namespace timer_manager {

Timer::Timer() : Timer(-1) {}
Timer::Timer(int repeat)
    : time_point_(std::chrono::steady_clock::now()),
      period_ms_(1000),
      repeat_(repeat) {}

Timer::~Timer() {}

void Timer::on_time() {
  if (!func_ || repeat_ == 0) {
    return;
  }

  func_();

  time_point_ += period_ms_;

  if (repeat_ > 0) {
    --repeat_;
  }
}

}  // namespace timer_manager