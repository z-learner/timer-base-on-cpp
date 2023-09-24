#include <iostream>

#include "timer/timer.hpp"

int test1() {
  auto work_func = []() {
    static int count = 0;
    std::cout << "test1 run " << ++count << " times" << std::endl;
  };

  timer::Timer time(5);  // repeat 5 times

  time.Start(1000000000, work_func);  // 1S

  while (time.IsActive()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}

int test2() {
  auto work_func = []() {
    static int count = 0;
    std::cout << "test2 run " << ++count << " times" << std::endl;
  };

  timer::Timer time(-1);  // repeat

  time.Start(1000000000, work_func);  // 1S

  int count = 0;
  while (time.IsActive() && count++ < 1000) {  // exit when count >= 1000
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}

int test3() {
  auto work_func = []() {
    static int count = 0;
    std::cout << "test3 run " << ++count << " times" << std::endl;
  };

  timer::Timer time(-1);  // repeat

  time.Start(1000000000, work_func);  // 1S

  while (time.IsActive()) {  // run....
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}

int main(int argc, char** argv) {
  test1();
  test2();
  test3();
}