#define ARRAYSIZE 100000
#include <shared_mutex>

class TwoPhaseLockingManager {
 public:
  // TwoPhaseLockingManager();
  TwoPhaseLockingManager(int arrSize = ARRAYSIZE) {}
  void Transaction1(int j, int i);

 private:
  double lockGranu;
  std::shared_mutex mtx[ARRAYSIZE];
  int arr[ARRAYSIZE];
};