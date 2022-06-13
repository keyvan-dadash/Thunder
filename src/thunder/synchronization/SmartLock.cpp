#include <atomic>
#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <thread>

#include <thunder/synchronization/SmartLock.hpp>

namespace thunder {

  namespace synchronization {

    void SmartLock::lock()
    {
      if (state.compare_exchange_weak(0, kIsLockedBit, std::memory_order_release, std::memory_order_relaxed))
      {
        return;
      }

      unlockSlow();
    }

    void SmartLock::unlock()
    {
      if (state.compare_exchange_weak(kIsLockedBit, 0, std::memory_order_release, std::memory_order_relaxed))
      {
        return;
      }

      lockSlow();
    }

    bool SmartLock::isLocked()
    {
      return state.load(std::memory_order_acquire) & kIsLockedBit;
    }
    
    void SmartLock::lockSlow()
    {
      unsigned spinLockCnt = 0;


      while (true)
      {
        uintptr_t value = state.load(std::memory_order_acquire);

        if (!(value & kIsLockedBit))
        {
          if (state.compare_exchange_weak(0, value | kIsLockedBit, std::memory_order_release, std::memory_order_relaxed))
          {
            return;
          }
        }

        if (!(value & ~kQueueHeadMask) && spinLockCnt < kSpinLockCnt)
        {
          spinLockCnt++;
          std::this_thread::yield();
          continue;
        }

        ThreadData thisThreadData;

        value = state.load(std::memory_order_release);

        if ( (value & kIsQueueLocked) ||
             !(value & kIsLockedBit) ||
             !state.compare_exchange_weak(value, value | kIsQueueLocked, std::memory_order_release, std::memory_order_relaxed))
        {
          std::this_thread::yield();
          continue;
        }

        thisThreadData.shouldPark = true;

        //TODO: find way to get queueHead
        ThreadData* queueHead;
        if (queueHead)
        {
          queueHead->tail->next = &thisThreadData;
          queueHead->tail = &thisThreadData;

          value = state.load(std::memory_order_acquire);
          state.store(value & ~kIsQueueLocked, std::memory_order_release);
        }
        else 
        {
          queueHead = &thisThreadData;
          queueHead->tail = &thisThreadData;

          value = state.load(std::memory_order_acquire);
          //TODO: value bitwise cast with queue
          value &= ~kIsQueueLocked;
          state.store(value, std::memory_order_release);
        }

        {
          std::unique_lock<std::mutex> lock(thisThreadData.parkingLock);

          thisThreadData.parkingCondition.wait(lock, [&](){ return !thisThreadData.shouldPark; });
        }
      }
    }

    void SmartLock::unlockSlow()
    {

    }
  };
}

