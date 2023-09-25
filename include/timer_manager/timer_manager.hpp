#pragma once

#include <atomic>
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <thread>

#include "timer_manager/timer.hpp"
namespace timer_manager {

class TimerManager {
 public:
  TimerManager() = default;
  ~TimerManager() = default;

  template <typename F, typename... Args>
  void schedule(int mill_second, F&& f, Args... args);

  template <typename F, typename... Args>
  void schedule(int mill_second, int repeat, F&& f, Args... args);

  bool Run();
  bool Stop();

 private:
  std::chrono::steady_clock::time_point Update();

 private:
  std::mutex active_mutex_;
  std::condition_variable active_cond_;
  std::multimap<std::chrono::steady_clock::time_point, std::shared_ptr<Timer>>
      timers_;
  std::unique_ptr<std::thread> work_thread_{nullptr};
  std::atomic_bool is_runing_{false};
};

template <typename F, typename... Args>
void TimerManager::schedule(int mill_second, F&& f, Args... args) {
  schedule(mill_second, -1, std::forward<F>(f), std::forward<Args>(args)...);
}

template <typename F, typename... Args>
void TimerManager::schedule(int mill_second, int repeat, F&& f, Args... args) {
  auto timer = std::make_shared<Timer>(repeat);
  timer->callback(mill_second, std::forward<F>(f), std::forward<Args>(args)...);
  {
    std::unique_lock<std::mutex> lk(active_mutex_);
    timers_.insert({timer->time_point_, std::move(timer)});
  }
  active_cond_.notify_one();
}

}  // namespace timer_manager