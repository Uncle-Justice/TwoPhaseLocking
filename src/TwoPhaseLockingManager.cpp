#define ARRAYSIZE 100000
#include "TwoPhaseLockingManager.h"

void TwoPhaseLockingManager::Transaction1(int j, int i) {
  std::shared_lock<std::shared_mutex> rLock1(mtx[i]);
  std::shared_lock<std::shared_mutex> rLock2(mtx[i]);
  std::shared_lock<std::shared_mutex> rLock3(mtx[i]);

  std::unique_lock<std::shared_mutex> wLock(mtx[j]);
  arr[j] = arr[i] + arr[i + 1] + arr[i + 1];

  wLock.unlock();
  rLock3.unlock();
  rLock2.unlock();
  rLock1.unlock();
}
