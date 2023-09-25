#include <iostream>

#include "timer_manager/timer_manager.hpp"

int main(int argc, char** argv) {
  timer_manager::TimerManager manager;

  manager.schedule(100, -1, []() {
    static int count = 0;
    std::cout << "test1 " << ++count << " times" << std::endl;
  });

  manager.schedule(50, 10, []() {
    static int count = 0;
    std::cout << "test2 " << ++count << " times" << std::endl;
  });

  manager.schedule(1000, -1, []() {
    static int count = 0;
    std::cout << "test3 " << ++count << " times" << std::endl;
  });

  manager.Run();

  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}