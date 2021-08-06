
#pragma once

#include <atomic>
#include <memory>
#include <utility>


#include <thunder/datastructures/BaseAtomicQueue.hpp>


namespace thunder {

  namespace datastructures {



      template <typename Element>
      class AtomicQueue : BaseAtomicQueue<Element>
      {
        public:
          explicit AtomicQueue();

          explicit AtomicQueue(int64_t maxSize);

          ~AtomicQueue();

          using BaseAtomicQueueStatus = typename BaseAtomicQueue<Element>::BaseAtomicQueueStatus;

          class AtomicQueueStatus 
          {
            public:
              enum Status
              {
                ELEMENT_PUSHED_SUCCESSFULLY = BaseAtomicQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY,
                ELEMENT_POPED_SUCCESSFULLY,
                CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE,
                OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY
              };
          };

          template<typename T>
          int push(T&& t);

          template<typename T>
          int tryPush(T&& t, int maxSize);
          
          void front(Element& element) override;

          void back(Element& element) override;

          int pop(Element& element) override;

          bool isEmpty() override;

          int getSizeOfQueue() override;

        private:

          int64_t getSizeNearPowerTwo(int64_t input_size);

          bool allocateBuffer();

          struct Node {
            public:    
              Element element = NULL;

              template <typename T>
              explicit Node(T&& t) : element(std::forward<T>(t)) {}

              Node operator=(Node&& other) {
                element(other.element);
              }
            
              Node *next = nullptr;
          };

          std::atomic<int16_t> head_;
          std::atomic<int16_t> tail_; 


          int64_t maxSize_ = 128;

          std::allocator<Element> alloca_;

          Element* elementArray_;

          std::atomic<size_t> size_{0};

      };

      using AtomicQueueOperationStatus = AtomicQueue<int>::AtomicQueueStatus::Status;

  }
}



#include <thunder/datastructures/AtomicQueue.cpp>