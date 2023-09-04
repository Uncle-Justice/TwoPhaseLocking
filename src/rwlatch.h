
#include <mutex>  // NOLINT
#include <shared_mutex>
#include <thread>
/**
 * Reader-Writer latch backed by std::mutex.
 */
class ReaderWriterLatch {
 public:
  /**
   * Acquire a write latch.
   */
  void WLock() { mutex_.lock(); }
  bool WTryLockWithOvertime(){
    return mutex_.try_lock_for(std::chrono::milliseconds(2000));
  }
  /**
   * Release a write latch.
   */
  void WUnlock() { mutex_.unlock(); }

  /**
   * Acquire a read latch.
   */
  void RLock() { mutex_.lock_shared(); }

  /**
   * Release a read latch.
   */
  void RUnlock() { mutex_.unlock_shared(); }

 private:
  std::shared_timed_mutex mutex_;
};
