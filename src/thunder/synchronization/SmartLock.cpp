#include <atomic>
#include <bit>
#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <thread>

#include <thunder/synchronization/SmartLock.hpp>
#include <thunder/utils/bit.hpp>

namespace thunder {

  namespace synchronization {

    void SmartLock::lock()
    {
      uintptr_t clear = kClear;
      if (state.compare_exchange_weak(clear, kIsLockedBit, std::memory_order_release, std::memory_order_relaxed))
      {
        return;
      }

      unlockSlow();
    }

    void SmartLock::unlock()
    {
      uintptr_t locked = kIsLockedBit;
      if (state.compare_exchange_weak(locked, kClear, std::memory_order_release, std::memory_order_relaxed))
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
          uintptr_t clear = kClear;
          if (state.compare_exchange_weak(clear, value | kIsLockedBit, std::memory_order_release, std::memory_order_relaxed))
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

        ThreadData* queueHead = thunder::utils::bit_cast<ThreadData*>(value & ~kQueueHeadMask);
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
          value |= thunder::utils::bit_cast<uintptr_t>(queueHead);
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
      while (true)
      {
        uintptr_t value = state.load(std::memory_order_acquire);

        if (value == kIsLockedBit)
        {
          if (state.compare_exchange_weak(value, 0, std::memory_order_release, std::memory_order_relaxed))
            return;

          std::this_thread::yield();
          continue;
        }

        if (value & kIsQueueLocked)
        {
          std::this_thread::yield();
          continue;
        }

        if (state.compare_exchange_weak(value, value | kIsQueueLocked, std::memory_order_release, std::memory_order_relaxed))
          break;
      }

      uintptr_t value = state.load(std::memory_order_acquire);

      ThreadData* queueHead = thunder::utils::bit_cast<ThreadData*>(value & ~kQueueHeadMask);

      ThreadData* nextHead = queueHead->next;

      if (nextHead)
        nextHead->tail = queueHead->tail;

      queueHead->next = nullptr;
      queueHead->tail = nullptr;

      value = state.load(std::memory_order_acquire);
      value &= ~kIsLockedBit;
      value &= ~kIsQueueLocked;
      value &= kQueueHeadMask;
      value |= thunder::utils::bit_cast<uintptr_t>(queueHead);
      state.store(value, std::memory_order_release);
      
      {
        std::scoped_lock<std::mutex> lock(queueHead->parkingLock);

        queueHead->shouldPark = false;

        queueHead->parkingCondition.notify_one();
      }
    }
  };
}

