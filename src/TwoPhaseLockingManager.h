#define ARRAYSIZE 100000
#include <ctime>
#include <random>
#include <shared_mutex>

#include "rwlatch.h"
class TwoPhaseLockingManager {
 public:
  TwoPhaseLockingManager(int arrSize = ARRAYSIZE) {
    e.seed(time(0));
    decltype(u)::param_type param{0, 1000};
    u.param(param);
  }
  void Transaction1(int j, int i);

 private:
  std::default_random_engine e;
  std::uniform_int_distribution<int> u;

  double lockGranu;
  ReaderWriterLatch mtx[ARRAYSIZE];
  int arr[ARRAYSIZE];
};