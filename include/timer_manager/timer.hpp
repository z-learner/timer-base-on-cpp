#pragma once

#include <chrono>
#include <functional>
#include <iostream>
namespace timer_manager {

class Timer {
  friend class TimerManager;

 public:
  Timer();
  Timer(int repeat);

  ~Timer();

  template <typename F, typename... Args>
  void callback(int nano_second, F&& f, Args... args);

  void on_time();

 private:
  std::chrono::steady_clock::time_point time_point_;
  std::function<void()> func_{nullptr};
  std::chrono::milliseconds period_ms_{1000};  // 1S
  int repeat_{-1};                             // no stop
};

template <typename F, typename... Args>
void Timer::callback(int mill_second, F&& f, Args... args) {
  period_ms_ = std::chrono::milliseconds(mill_second);
  func_ = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
}

}  // namespace timer_manager