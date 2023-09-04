
#include "TwoPhaseLockingManager.h"

#include <iostream>
#include <thread>
void TwoPhaseLockingManager::Transaction1(int j, int i) {
  std::cout << j << "  " << i << std::endl;
  // 死锁的主要诱因
  if (j == i || j == (i + 1) % ARRAYSIZE || j == (i + 2) % ARRAYSIZE) return;

  // std::cout << j << "  " << i << std::endl;
  if (i != j) mtx[i].RLock();
  if ((i + 1) % ARRAYSIZE != j) mtx[i + 1].RLock();
  if ((i + 2) % ARRAYSIZE != j) mtx[i + 2].RLock();
  mtx[j].WLock();

  arr[j] = arr[i] + arr[(i + 1) % ARRAYSIZE] + arr[(i + 2) % ARRAYSIZE];

  mtx[j].WUnlock();
  if ((i + 2) % ARRAYSIZE != j) mtx[i + 2].RUnlock();
  if ((i + 1) % ARRAYSIZE != j) mtx[i + 1].RUnlock();
  if (i != j) mtx[i].RUnlock();
}
