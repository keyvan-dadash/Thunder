
#pragma once

#include <thunder/datastructures/BaseQueue.hpp>


namespace thunder {

  namespace datastructures {



      template <typename Element>
      class BaseConcurrentQueue
      {
        public:
          explicit BaseConcurrentQueue() = default;

          ~BaseConcurrentQueue() = default;

          class BaseConcurrentQueueStatus 
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
          int forcePush(T&& t);

          template <typename T>
          int tryPush(T&& t, int maxSize);

          //TODO: lock elements
          virtual void front(Element& element) = 0;

          virtual void back(Element& element) = 0;

          virtual int pop(Element& element) = 0;

          virtual bool isEmpty() = 0;

          virtual int getSizeOfQueue() = 0;

      };

  }
}