
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

          ~Queue();

        //   Queue(Queue&& other) noexcept;
        //   Queue operator=(Queue&& other) noexcept;

          using BaseQueueStatus = typename BaseQueue<Element>::BaseQueueStatus;

          class QueueStatus : public BaseQueueStatus{
            public: 
              enum Status
              {
                ELEMENT_PUSHED_SUCCESSFULLY = BaseQueueStatus::Status::ELEMENT_PUSHED_SUCCESSFULLY,
                ELEMENT_POPED_SUCCESSFULLY
              };
          };

          int push(Element&& element) override;

          int tryPush(Element&& element, int maxSize) override;
          
          Element front() override;

          int pop() override;

          bool isEmpty() override;

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

          std::unique_ptr<Node> head_ = nullptr;
          size_t size_{0};
      };

      using QueueOperationStatus = Queue<int>::QueueStatus::Status;


  }
}


#include <thunder/datastructures/Queue.cpp>