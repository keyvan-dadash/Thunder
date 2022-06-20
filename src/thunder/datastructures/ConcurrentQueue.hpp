#pragma once

#include <atomic>
#include <memory>
#include <utility>
#include <cstddef>

#include <thunder/datastructures/BaseConcurrentQueue.hpp>

namespace thunder {

  namespace datastructures {

    #ifdef __cpp_lib_hardware_interference_size
        using std::hardware_constructive_interference_size;
        using std::hardware_destructive_interference_size;
    #else
        // 64 bytes on x86-64 │ L1_CACHE_BYTES │ L1_CACHE_SHIFT │ __cacheline_aligned │ ...
        constexpr std::size_t hardware_constructive_interference_size
            = 2 * sizeof(std::max_align_t);
        constexpr std::size_t hardware_destructive_interference_size
            = 2 * sizeof(std::max_align_t);
    #endif


      template <typename Element, std::size_t QueueSize = 128>
      class ConcurrentQueue : BaseConcurrentQueue<Element>
      {
        public:
          explicit ConcurrentQueue();

          ~ConcurrentQueue();

          using BaseConcurrentQueueStatus = typename BaseConcurrentQueue<Element>::BaseConcurrentQueueStatus;

          class ConcurrentQueueStatus 
          {
            public:
              enum Status
              {
                ELEMENT_PUSHED_SUCCESSFULLY = BaseConcurrentQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY,
                ELEMENT_POPED_SUCCESSFULLY,
                CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE,
                OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY
              };
          };

          template <typename T>
          int forcePush(T&& t);

          template <typename T>
          int tryPush(T&& t);
          
          int pop(Element& element) override;

          bool isEmpty() override;

          int getSizeOfQueue() override;

        private:

          template<typename T>
          void push_atomic(T&& t, int head);

          void pop_atomic(Element& elem, int tail);

          static const std::size_t kQueueSize = QueueSize;


          class CellStates 
          {
            public:
              enum States
              {
                STORED = 1,
                STORING,
                LOADED,
                LOADING,
                EMPTY
              };
          };

          alignas(hardware_destructive_interference_size) std::atomic<int16_t> head_;
          alignas(hardware_destructive_interference_size) std::atomic<int16_t> tail_; 

          typedef typename std::aligned_storage_t<sizeof(Element), alignof(Element)> ElementStorage;

          ElementStorage elementArray_[QueueSize];

          std::atomic_int8_t states_[QueueSize];

          std::atomic<size_t> size_{0};
      };

      using ConcurrentQueueOperationStatus = ConcurrentQueue<int>::ConcurrentQueueStatus::Status;
  }
}

#include <thunder/datastructures/ConcurrentQueue.cpp>

