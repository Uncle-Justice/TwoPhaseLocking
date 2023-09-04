
#include "TwoPhaseLockingManager.h"

#include <iostream>
#include <thread>
void TwoPhaseLockingManager::Transaction1(int j, int i) {
  std::cout << j << "  " << i << std::endl;
  int iArr[3]{i, (i + 1) % ARRAYSIZE, (i + 2) % ARRAYSIZE};
  // 死锁的主要诱因
  // if (j == i || j == (i + 1) % ARRAYSIZE || j == (i + 2) % ARRAYSIZE) return;

  for (int k = 0; k < 3; k++) {
    if (iArr[k] != j) mtx[iArr[k]].RLock();
  }

  while (!mtx[j].WTryLockWithOvertime()) {
    for (int k = 2; k >= 0; k--) {
      if (iArr[k] != j) mtx[iArr[k]].RUnlock();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(u(e)));

    for (int k = 0; k < 3; k++) {
      if (iArr[k] != j) mtx[iArr[k]].RLock();
    }
  }

  arr[j] = arr[iArr[0]] + arr[iArr[1]] + arr[iArr[2]];

  mtx[j].WUnlock();
  for (int k = 2; k >= 0; k--) {
    if (iArr[k] != j) mtx[iArr[k]].RUnlock();
  }
}
