
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

          template <typename T>
          int push(T&& t);

          template <typename T>
          int tryPush(T&& t, int maxSize);

          virtual std::shared_ptr<Element> front() = 0;

          virtual int pop() = 0;

          virtual bool isEmpty() = 0;

          virtual int getSizeOfQueue() = 0;

     };

  }
}