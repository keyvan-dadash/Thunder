
#pragma once

#include <atomic>
#include <memory>

#include <thunder/datastructures/BaseAtomicQueue.hpp>


namespace thunder {

  namespace datastructures {



      template <typename Element>
      class AtomicQueue : BaseAtomicQueue<Element>
      {
        public:
          explicit AtomicQueue() = default;

          ~AtomicQueue() = default;

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
          
          std::shared_ptr<Element> front() override;

          std::shared_ptr<Element> back();

          int pop() override;

          int pop_back();

          bool isEmpty() override;

          int getSizeOfQueue() override;

        private:
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

          std::atomic<Node*> head_ = nullptr;
          std::atomic<size_t> size_{0};

      };

      using AtomicQueueOperationStatus = AtomicQueue<int>::AtomicQueueStatus::Status;

  }
}



#include <thunder/datastructures/AtomicQueue.cpp>