#include <gtest/gtest.h>

#include <ctime>
#include <random>
#include <thread>
#include <vector>

#include "TwoPhaseLockingManager.h"
int add(int a, int b) { return a + b; }

TEST(testCase, test0) {
  TwoPhaseLockingManager *TPLManagerInstance = new TwoPhaseLockingManager();
  int threadNum = 10;
  int freq = 100;
  std::default_random_engine e;
  std::uniform_int_distribution<int> u(0, 99999);  // 左闭右闭区间
  e.seed(time(0));

  auto task = [&](int freq) {
    for (int i = 0; i < freq; i++) {
      TPLManagerInstance->Transaction1(u(e), u(e));
    }
  };

  std::vector<std::thread> threads;
  threads.reserve(threadNum);

  for (int i = 0; i < threadNum; i++) {
    threads.emplace_back(std::thread(task, freq));
  }

  for (int i = 0; i < threadNum; i++) {
    threads[i].join();
  }
  EXPECT_EQ(add(2, 3), 5);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
