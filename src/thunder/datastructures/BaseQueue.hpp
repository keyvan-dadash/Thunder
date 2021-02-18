
#pragma once

namespace thunder {
  namespace datastructures {

     template<typename Element>
     class BaseQueue 
     {
        public:
    
          explicit BaseQueue() = default;

          ~BaseQueue() = default;

          class BaseQueueStatus 
          {
            public:
              enum Status
              {
                ELEMENT_PUSHED_SUCCESSFULLY = 1,
                ELEMENT_POPED_SUCCESSFULLY
              };
          };


          virtual typename BaseQueueStatus::Status push(Element&& element) = 0;

          virtual typename BaseQueueStatus::Status tryPush(Element&& element, int maxSize) = 0;

          virtual Element front() = 0;

          virtual typename BaseQueueStatus::Status pop() = 0;

          virtual bool isEmpty() = 0;

     };

  }
}