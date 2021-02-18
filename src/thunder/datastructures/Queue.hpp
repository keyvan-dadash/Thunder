
#pragma once

#include <utility>
#include <memory>


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

          typename BaseQueueStatus::Status push(Element&& element) override;

          typename BaseQueueStatus::Status tryPush(Element&& element, int maxSize) override;
          
          Element front() override;

          typename BaseQueueStatus::Status pop() override;

          bool isEmpty() override;

        private:
          struct Node {    
            Element element;

            template <typename T>
            explicit Node(T&& t) : element(std::forward<T>(element)) {}
            
            Node *next{};
          };

          std::unique_ptr<Node> head_ = nullptr;
          size_t size_{0};
      };

      using QueueOperationStatus = Queue<int>::QueueStatus::Status;


  }
}


#include <thunder/datastructures/Queue.cpp>