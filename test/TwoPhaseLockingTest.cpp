#include <gtest/gtest.h>

#include <ctime>
#include <random>
#include <thread>
#include <vector>

#include "TwoPhaseLockingManager.h"

TEST(testCase, test0) {
  int thread_num = 1024;
  int freq = 10000;
  int array_size = 100000;
  int lock_grau = 1;
  TwoPhaseLockingManager *TPLManagerInstance =
      new TwoPhaseLockingManager(array_size, lock_grau);

  std::default_random_engine e;
  std::uniform_int_distribution<int> u(0, array_size - 1);
  e.seed(time(0));

  auto task = [&](int freq) {
    for (int i = 0; i < freq; i++) {
      TPLManagerInstance->Transaction1(u(e), u(e));
    }
  };

  std::vector<std::thread> threads;
  threads.reserve(thread_num);

  for (int i = 0; i < thread_num; i++) {
    threads.emplace_back(std::thread(task, freq));
  }

  for (int i = 0; i < thread_num; i++) {
    threads[i].join();
  }
  EXPECT_EQ(add(2, 3), 5);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
