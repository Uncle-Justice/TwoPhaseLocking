
#include "TwoPhaseLockingManager.h"

#include <iostream>
#include <set>
#include <thread>
void TwoPhaseLockingManager::Transaction1(int j, int i) {
  // std::cout << j << "  " << i << std::endl;
  // 死锁的主要诱因
  // if (j == i || j == (i + 1) % ARRAYSIZE || j == (i + 2) % ARRAYSIZE) return;
  int write_mtx_id = j / lockGranu_;
  std::set<int> read_mtx_id_set;
  read_mtx_id_set.insert(i / lockGranu_);
  read_mtx_id_set.insert((i + 1) % ARRAYSIZE / lockGranu_);
  read_mtx_id_set.insert((i + 2) % ARRAYSIZE / lockGranu_);
  for (auto read_mtx_id = read_mtx_id_set.begin();
       read_mtx_id != read_mtx_id_set.end(); ++read_mtx_id) {
    if (*read_mtx_id != write_mtx_id) {
      mtx_arr_[*read_mtx_id].RLock();
    }
  }

  while (!mtx_arr_[write_mtx_id].WTryLockWithOvertime()) {
    for (auto read_mtx_id = read_mtx_id_set.rbegin();
         read_mtx_id != read_mtx_id_set.rend(); ++read_mtx_id) {
      if (*read_mtx_id != write_mtx_id) {
        mtx_arr_[*read_mtx_id].RUnlock();
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(u(e)));

    for (auto read_mtx_id = read_mtx_id_set.begin();
         read_mtx_id != read_mtx_id_set.end(); ++read_mtx_id) {
      if (*read_mtx_id != write_mtx_id) {
        mtx_arr_[*read_mtx_id].RLock();
      }
    }
  }

  arr_[j] = arr_[i] + arr_[(i + 1) % ARRAYSIZE] + arr_[(i + 2) % ARRAYSIZE];

  mtx_arr_[write_mtx_id].WUnlock();
  for (auto read_mtx_id = read_mtx_id_set.rbegin();
       read_mtx_id != read_mtx_id_set.rend(); ++read_mtx_id) {
    if (*read_mtx_id != write_mtx_id) {
      mtx_arr_[*read_mtx_id].RUnlock();
    }
  }
}
