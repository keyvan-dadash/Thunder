

#include <thunder/datastructures/BaseQueue.hpp>


namespace thunder {

  namespace datastructures {



      template <typename Element>
      class BaseAtomicQueue : BaseQueue<Element>
      {
        public:
          explicit BaseAtomicQueue() = default;

          ~BaseAtomicQueue() = default;

          class AtomicBaseQueueStatus 
          {
            public:
              enum Status
              {
                ELEMENT_PUSHED_SUCCESSFULLY = BaseQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY,
                ELEMENT_POPED_SUCCESSFULLY,
                CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE,
                OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY
              };
          };

      };

  }
}