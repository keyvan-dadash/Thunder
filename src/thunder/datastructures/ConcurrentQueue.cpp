#pragma once

#include <atomic>
#include <iostream>
#include <emmintrin.h>

#include <thunder/datastructures/ConcurrentQueue.hpp>

namespace thunder {

  namespace datastructures {

    template <typename Element, std::size_t QueueSize>
    ConcurrentQueue<Element, QueueSize>::ConcurrentQueue()
    {
      for (int i = 0; i < QueueSize; i++)
      {
        states_[i].store(CellStates::EMPTY, std::memory_order_release);
      }
      head_.store(0, std::memory_order_release);
      tail_.store(0, std::memory_order_release);
    }


    template <typename Element, std::size_t QueueSize>
    ConcurrentQueue<Element, QueueSize>::~ConcurrentQueue()
    {

    }

    template <typename Element, std::size_t QueueSize>
    template<typename T> 
    int ConcurrentQueue<Element, QueueSize>::forcePush(T&& t)
    {
      auto head = head_.fetch_add(1, std::memory_order_relaxed);

      push_atomic(std::forward<T>(t), head % kQueueSize);

      size_.fetch_add(1, std::memory_order_release);

      return BaseConcurrentQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY;
    }

    template< typename Element, std::size_t QueueSize>
    template<typename T>
    int ConcurrentQueue<Element, QueueSize>::tryPush(T&& t)
    {
      auto head = head_.load(std::memory_order_relaxed);
      do
      {
        if (std::abs(head - tail_.load(std::memory_order_relaxed)) >= kQueueSize)
        {
          return BaseConcurrentQueueStatus::CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE;
        }
      } while (!head_.compare_exchange_strong(head, head + 1, std::memory_order_release));

      push_atomic(std::forward<T>(t), head % kQueueSize);

      size_.fetch_add(1, std::memory_order_release);

      return BaseConcurrentQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY;
    }

    template <typename Element, std::size_t QueueSize>
    int ConcurrentQueue<Element, QueueSize>::pop(Element& element)
    {
      auto tail = tail_.load(std::memory_order_relaxed);
      do
      {
        if (std::abs(head_.load(std::memory_order_relaxed) - tail) <= 0)
        {
          return BaseConcurrentQueueStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY;
        }
      } while(!tail_.compare_exchange_strong(tail, tail + 1, std::memory_order_release));

      pop_atomic(element, tail % kQueueSize);

      size_.fetch_sub(1, std::memory_order_release);

      return BaseConcurrentQueueStatus::ELEMENT_POPED_SUCCESSFULLY;
    }

    template <typename Element, std::size_t QueueSize>
    template<typename T>
    void ConcurrentQueue<Element, QueueSize>::push_atomic(T&& t, int head)
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
          do
            _mm_pause();
          while(state.load(std::memory_order_acquire) != CellStates::EMPTY);
        }
      }
    }

    template <typename Element, std::size_t QueueSize>
    void ConcurrentQueue<Element, QueueSize>::pop_atomic(Element& elem, int tail)
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
           do
            _mm_pause();
          while(state.load(std::memory_order_acquire) != CellStates::STORED);
        }
      }
    }

    template <typename Element, std::size_t QueueSize>
    bool ConcurrentQueue<Element, QueueSize>::isEmpty()
    {
      return size_.load(std::memory_order_relaxed) > 0 ? false : true;
    }

    template <typename Element, std::size_t QueueSize>
    int ConcurrentQueue<Element, QueueSize>::getSizeOfQueue()
    {
      return size_.load(std::memory_order_relaxed);
    }
  }
}
