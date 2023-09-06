#define ARRAYSIZE 100000
#include <ctime>
#include <random>
#include <shared_mutex>
#include <vector>

#include "rwlatch.h"
class TwoPhaseLockingManager {
 public:
  TwoPhaseLockingManager(int arrSize = ARRAYSIZE, int lockGranu = 1)
      : lockGranu_(lockGranu), mtx_arr_size_((ARRAYSIZE - 1) / lockGranu_ + 1) {
    e.seed(time(0));
    decltype(u)::param_type param{0, 2000};
    u.param(param);
  }
  void Transaction1(int j, int i);

 private:
  std::default_random_engine e;
  std::uniform_int_distribution<int> u;

  int lockGranu_;
  ReaderWriterLatch mtx_arr_[ARRAYSIZE];
  int mtx_arr_size_;
  int arr_[ARRAYSIZE];
};