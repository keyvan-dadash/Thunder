
#pragma once

#include <memory>

#include <thunder/datastructures/Queue.hpp>


namespace thunder {

  namespace datastructures {

    // Queue(Queue&& other) noexcept
    // {

    // }

    template<typename Element>
    Queue<Element>::~Queue()
    {

    }
    
    template<typename Element>
    typename Queue<Element>::BaseQueueStatus::Status 
    Queue<Element>::push(Element&& element)
    {
      return BaseQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY;
    }

    template<typename Element>
    typename Queue<Element>::BaseQueueStatus::Status 
    Queue<Element>::tryPush(Element&& element, int maxSize)
    {
      return BaseQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY;
    }

    template<typename Element>
    Element Queue<Element>::front()
    {

      return NULL;
    }

    template<typename Element>
    typename Queue<Element>::BaseQueueStatus::Status 
    Queue<Element>::pop()
    {

      return BaseQueueStatus::ELEMENT_POPED_SUCCESSFULLY;
    }

    template<typename Element>
    bool Queue<Element>::isEmpty()
    {
      return true;
    }
  }



}