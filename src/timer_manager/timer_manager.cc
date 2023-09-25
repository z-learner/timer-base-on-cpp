#include "timer_manager/timer_manager.hpp"

namespace timer_manager {

std::chrono::steady_clock::time_point TimerManager::Update() {
  if (timers_.empty()) {
    return std::chrono::steady_clock::time_point();
  }

  auto time_now = std::chrono::steady_clock::now();
  for (auto it = timers_.begin(); it != timers_.end();) {
    if (it->first > time_now) {
      return it->first;
    }

    it->second->on_time();  // do something

    auto t = std::move(it->second);

    it = timers_.erase(it);

    if (t->repeat_ != 0) {
      auto next_time = t->time_point_;
      auto new_it = timers_.insert({next_time, std::move(t)});

      if (it == timers_.end() || new_it->first < it->first) {
        it = new_it;
      }
    }
  }

  return std::chrono::steady_clock::time_point();
}

bool TimerManager::Run() {
  is_runing_.store(true, std::memory_order_acquire);

  work_thread_ = std::make_unique<std::thread>([this]() {
    auto sleep_time = std::chrono::milliseconds(0);
    while (this->is_runing_.load(std::memory_order_release)) {
      std::unique_lock<std::mutex> lk(this->active_mutex_);

      this->active_cond_.wait_for(lk, sleep_time);

      if (!this->is_runing_.load(std::memory_order_release)) {
        return;
      }

      auto next_runing_time = this->Update();
      if (next_runing_time == std::chrono::steady_clock::time_point()) {
        // timers is empty
        sleep_time = std::chrono::milliseconds(1000);  // max 1S
      } else {
        sleep_time =
            std::max(std::chrono::duration_cast<std::chrono::milliseconds>(
                         next_runing_time - std::chrono::steady_clock::now()),
                     std::chrono::milliseconds(0));
      }
    }
  });

  return true;
}

bool TimerManager::Stop() {
  is_runing_.store(false, std::memory_order_acquire);
  active_cond_.notify_one();
  if (work_thread_) {
    work_thread_->join();
  }

  return true;
}

}  // namespace timer_manager