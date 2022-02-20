
#pragma once

#include <utility>
#include <memory>
#include <iostream>


#include <thunder/datastructures/BaseQueue.hpp>

namespace thunder {

  namespace datastructures {


      template<typename Element>
      class Queue : public BaseQueue<Element>
      {
        public:
          Queue() {}

          ~Queue() = default;

          Queue(Queue<Element>&& other) noexcept;
          Queue<Element>& operator=(Queue<Element>&& rhs) noexcept;

          using BaseQueueStatus = typename BaseQueue<Element>::BaseQueueStatus;

          class QueueStatus : public BaseQueueStatus{
            public: 
              enum Status
              {
                ELEMENT_PUSHED_SUCCESSFULLY = BaseQueueStatus::Status::ELEMENT_PUSHED_SUCCESSFULLY,
                ELEMENT_POPED_SUCCESSFULLY,
                CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE,
                OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY
              };
          };

          template <typename T>
          int push(T&& t);

          template <typename T>
          int tryPush(T&& t, int maxSize);
          
          void front(Element& element) override;

          void back(Element& element) override;

          int pop(Element& element) override;

          bool isEmpty() override;

          int getSizeOfQueue() override;

        private:
          struct Node
          {
            public:    
              Element element = NULL;

              template <typename T>
              explicit Node(T&& t) : element(std::forward<T>(t)) {}

              Node operator=(Node&& other) {
                element(other.element);
              }
            
              Node *next = nullptr;
          };

          std::unique_ptr<Node> head_ = nullptr;
          size_t size_{0};
      };

      using QueueOperationStatus = Queue<int>::QueueStatus::Status;


  }
}


#include <thunder/datastructures/Queue.cpp>