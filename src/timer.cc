#include "timer/timer.hpp"

namespace timer {

Timer::Timer() : Timer(-1) {}

Timer::Timer(int repeat) : repeat_(repeat) {}

Timer::~Timer() { Stop(); }

bool Timer::Stop() {
  // when work_thread is in sleep, you can't stop immediately (how to fix it?)
  // two thread ?
  // condition variable ?
  is_active_.store(false);
  active_condition_.notify_one();

  if (work_thread_) {
    work_thread_->join();
  }
  return true;
}

bool Timer::IsActive() { return is_active_.load(std::memory_order_acquire); }

}  // namespace timer