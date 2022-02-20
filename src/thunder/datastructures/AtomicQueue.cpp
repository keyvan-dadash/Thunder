
#pragma once

#include <atomic>
#include <iostream>


#include <thunder/datastructures/AtomicQueue.hpp>


namespace thunder {

  namespace datastructures {


    template <typename Element, std::size_t QueueSize>
    AtomicQueue<Element, QueueSize>::AtomicQueue()
    {
      for (int i = 0; i < QueueSize; i++)
      {
        states_[i].store(CellStates::EMPTY, std::memory_order_release);
      }
      head_.store(0, std::memory_order_release);
      tail_.store(0, std::memory_order_release);
    }


    template <typename Element, std::size_t QueueSize>
    AtomicQueue<Element, QueueSize>::~AtomicQueue()
    {

    }

    template <typename Element, std::size_t QueueSize>
    template<typename T> 
    int AtomicQueue<Element, QueueSize>::forcePush(T&& t)
    {
      int16_t head = head_.fetch_add(1, std::memory_order_relaxed);

      push_atomic(std::forward<T>(t), head);

      size_.fetch_add(1, std::memory_order_release);

      return BaseAtomicQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY;
    }

    template< typename Element, std::size_t QueueSize>
    template<typename T>
    int AtomicQueue<Element, QueueSize>::tryPush(T&& t, int maxSize)
    {
      auto head = head_.load(std::memory_order_relaxed);
      do
      {
        if (size_.load(std::memory_order_acquire) >= kQueueSize)
        {
          return BaseAtomicQueueStatus::CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE;
        }
      } while (!head_.compare_exchange_strong(head, head + 1, std::memory_order_release));

      push_atomic(std::forward<T>(t), head);

      size_.fetch_add(1, std::memory_order_release);

      return BaseAtomicQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY;
    }

    //TODO: think about front and back api's
    template <typename Element, std::size_t QueueSize>
    void 
    AtomicQueue<Element, QueueSize>::front(Element& element)
    {
      if (size_.load(std::memory_order_relaxed) == 0) {
        return; 
      }

      auto tail = tail_.load(std::memory_order_relaxed);

      while (true)
      {
        // element = elementArray_[tail];
        if (tail == tail_.load(std::memory_order_relaxed)) break;
        tail = tail_.load(std::memory_order_relaxed);
        if (size_.load(std::memory_order_relaxed) <= 0) break;
      }
    }

    template <typename Element, std::size_t QueueSize>
    void
    AtomicQueue<Element, QueueSize>::back(Element& element)
    {
      if (size_.load(std::memory_order_relaxed) == 0) {
        return; 
      }

      auto head = head_.load(std::memory_order_relaxed);

      while (true)
      {
        // element = elementArray_[head];
        if (head == head_.load(std::memory_order_relaxed)) break;
        head = head_.load(std::memory_order_relaxed);
        if (size_.load(std::memory_order_relaxed) <= 0) break;
      }
    }

    template <typename Element, std::size_t QueueSize>
    int AtomicQueue<Element, QueueSize>::pop(Element& element)
    {
      if (size_ <= 0)
      {
        return BaseAtomicQueueStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY;
      }

      //TODO: how we gurantee this?
      auto tail = tail_.fetch_add(1, std::memory_order_relaxed);

      pop_atomic(element, tail);

      size_.fetch_sub(1, std::memory_order_release);

      return BaseAtomicQueueStatus::ELEMENT_POPED_SUCCESSFULLY;
    }

    template <typename Element, std::size_t QueueSize>
    template<typename T>
    void AtomicQueue<Element, QueueSize>::push_atomic(T&& t, int head)
    {

      std::atomic_int8_t& state = states_[head];

      while (true)
      {
        int8_t expectState = CellStates::EMPTY;
        if (state.compare_exchange_strong(
          expectState,
          CellStates::STORING,
          std::memory_order_acquire,
          std::memory_order_relaxed
        ))
        {
          new ( std::addressof(elementArray_[head]) ) Element( std::forward<T>(t) );
          state.store(CellStates::STORED, std::memory_order_release);
          return;
        }
        else
        {
          //TODO: some hybrid mutexs
        }
      }
    }

    template <typename Element, std::size_t QueueSize>
    void AtomicQueue<Element, QueueSize>::pop_atomic(Element& elem, int tail)
    {
      
      std::atomic_int8_t& state = states_[tail];

      while (true)
      {
        int8_t expectState = CellStates::STORED;
        if (state.compare_exchange_strong(
          expectState,
          CellStates::LOADING,
          std::memory_order_acquire,
          std::memory_order_relaxed
        )) {
          elem = std::move(*std::launder(reinterpret_cast<Element*>( std::addressof(elementArray_[tail]) )));

          std::destroy_at(
            std::launder(reinterpret_cast<Element*>( std::addressof(elementArray_[tail]) ))
          );

          state.store(CellStates::EMPTY, std::memory_order_release);
          return;
        }
        else {
          //TODO: some hybrid mutexs
        }
      }
    }

    template <typename Element, std::size_t QueueSize>
    bool AtomicQueue<Element, QueueSize>::isEmpty()
    {
      return size_.load(std::memory_order_acquire) > 0 ? false : true;
    }

    template <typename Element, std::size_t QueueSize>
    int AtomicQueue<Element, QueueSize>::getSizeOfQueue()
    {
      return size_.load(std::memory_order_acquire);
    }

  }
}