#include <thread>
#include <gtest/gtest.h>

thread_local long long shared = 0;

void threadFunc() {
  while (shared < 1L) {
    shared++;
  }
}


TEST(ConcTest, Basic) {
  std::thread t1(threadFunc);
  std::thread t2(threadFunc);

  t1.join();
  t2.join();
}

