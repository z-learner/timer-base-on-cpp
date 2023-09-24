#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
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
    if (this->repeat_ < 0) {
      while (this->is_active_.load(std::memory_order_acq_rel)) {
        if (!this->is_active_.load()) {
          return;
        }

        std::unique_lock<std::mutex> lk(this->mutex_);

        this->active_condition_.wait_for(
            lk, nano_second * std::chrono::nanoseconds(1));

        if (!this->is_active_.load()) {
          return;
        }

        this->func_();
      }
    }

    int done_time = 0;
    while (done_time++ < this->repeat_) {
      if (!this->is_active_.load(std::memory_order_acquire)) {
        return;
      }

      std::unique_lock<std::mutex> lk(this->mutex_);

      this->active_condition_.wait_for(
          lk, nano_second * std::chrono::nanoseconds(1));

      if (!this->is_active_.load(std::memory_order_acquire)) {
        return;
      }

      this->func_();
    }

    is_active_.store(false, std::memory_order_relaxed);
    return;
  });

  return true;
}

}  // namespace timer