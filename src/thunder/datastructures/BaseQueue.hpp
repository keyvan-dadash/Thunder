
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
                ELEMENT_POPED_SUCCESSFULLY,
                CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE,
                OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY
              };
          };


          virtual int push(Element&& element) = 0;

          virtual int tryPush(Element&& element, int maxSize) = 0;

          virtual Element front() = 0;

          virtual int pop() = 0;

          virtual bool isEmpty() = 0;

     };

  }
}