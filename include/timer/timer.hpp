#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <thread>
namespace timer {

class Timer {
 public:
  Timer();
  Timer(int repeat);
  ~Timer();

  template <typename F, typename... Arg>
  bool Start(int64_t nano_second, F&& func, Arg&&... args);

  bool Stop();

  bool IsActive();

 private:
  std::unique_ptr<std::thread> work_thread_{nullptr};
  std::atomic_bool is_active_{false};
  std::function<void()> func_{nullptr};
  int repeat_{-1};

  std::condition_variable active_condition_;
  std::mutex mutex_;
};

template <typename F, typename... Args>
bool Timer::Start(int64_t nano_second, F&& func, Args&&... args) {
  if (is_active_.load()) {
    return false;
  }

  func_ = std::bind(std::forward<F>(func), std::forward<Args>(args)...);

  is_active_.store(true);

  work_thread_ = std::make_unique<std::thread>([this, nano_second]() {
    auto repeat = this->repeat_;
    auto nano_sleep_time = nano_second;
    int64_t excution_time = 0;

    while (this->is_active_.load(std::memory_order_acq_rel) && repeat != 0) {
      if (!this->is_active_.load()) {
        return;
      }

      nano_sleep_time = nano_second - excution_time;
      if (nano_sleep_time <= 0) {
        std::cout << "excution time too long, no sleep, just run next times"
                  << std::endl;
      } else {
        std::unique_lock<std::mutex> lk(this->mutex_);
        this->active_condition_.wait_for(
            lk, nano_sleep_time * std::chrono::nanoseconds(1));
        if (!this->is_active_.load(std::memory_order_acquire)) {
          return;
        }
      }
      auto time_start = std::chrono::steady_clock::now();
      this->func_();
      excution_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          std::chrono::steady_clock::now() - time_start)
                          .count();

      if (repeat > 0) {
        --repeat;
      }
    }
    is_active_.store(false, std::memory_order_relaxed);
    return;
  });

  return true;
}

}  // namespace timer