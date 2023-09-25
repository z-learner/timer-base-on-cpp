#include <iostream>

#include "timer/timer.hpp"

int test1() {
  std::cout << "-----------start test1 -----------" << std::endl;
  auto work_func = []() {
    static int count = 0;
    std::cout << "test1 run " << ++count << " times" << std::endl;
  };

  timer::Timer time(5);  // repeat 5 times

  time.Start(1000000000, work_func);  // 1S

  while (time.IsActive()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  std::cout << "-----------end test1 -----------" << std::endl;

  return 0;
}

int test2() {
  std::cout << "-----------start test2 -----------" << std::endl;

  auto work_func = []() {
    static int count = 0;
    std::cout << "test2 run " << ++count << " times" << std::endl;
  };

  timer::Timer time(-1);  // repeat

  time.Start(1000000000, work_func);  // 1S

  int count = 0;
  while (time.IsActive() && count++ < 100) {  // exit when count >= 1000
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  std::cout << "-----------end test2 -----------" << std::endl;

  return 0;
}

int test3() {
  std::cout << "-----------start test3 -----------" << std::endl;

  auto work_func = []() {
    static int count = 0;
    std::cout << "test3 run " << ++count << " times" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
  };

  timer::Timer time(5);  // repeat

  time.Start(1000000000, work_func);  // 1S, excution is too long

  while (time.IsActive()) {  // run....
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::cout << "-----------end test3 -----------" << std::endl;

  return 0;
}

int test4() {
  std::cout << "-----------start test4 -----------" << std::endl;

  auto work_func = []() {
    static int count = 0;
    std::cout << "test4 run " << ++count << " times" << std::endl;
  };

  timer::Timer time(-1);  // repeat

  time.Start(1000000000, work_func);  // 1S

  while (time.IsActive()) {  // run....
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  std::cout << "-----------end test4 -----------" << std::endl;

  return 0;
}

int main(int argc, char** argv) {
  test1();
  test2();
  test3();
  test4();
}