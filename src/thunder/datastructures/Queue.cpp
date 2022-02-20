
#pragma once

#include <memory>
#include <iostream>

#include <thunder/datastructures/Queue.hpp>


namespace thunder {

  namespace datastructures {

    template<typename Element>
    Queue<Element>::Queue(Queue<Element>&& rhs) noexcept
      : size_( std::move(rhs.size_ ))
      , head_( std::move(rhs.head_ ))
    {
      
    }

    template<typename Element>
    Queue<Element>& 
    Queue<Element>::operator=(Queue<Element>&& rhs) noexcept
    {
      size_ = rhs.size_;
      head_.reset( std::move(rhs.head_) );

      return *this;
    }
    
    template<typename Element>
    template <typename T>
    int Queue<Element>::push(T&& t)
    {
      std::unique_ptr<Node> node(new Node(std::forward<T>(t)));
      node->next = head_.release();

      head_.reset(node.get());
      node.release();
      size_++;

      return static_cast<int>(BaseQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }

    template<typename Element>
    template <typename T>
    int Queue<Element>::tryPush(T&& t, int maxSize)
    {
      if (size_ >= maxSize) {
        return static_cast<int>(BaseQueueStatus::CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE);
      }

      std::unique_ptr<Node> node(new Node(std::forward<T>(t)));
      node->next = head_.release();

      head_.reset(node.get());   
      node.release();
      size_++;

      return static_cast<int>(BaseQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }

    template<typename Element>
    void Queue<Element>::front(Element& element)
    {
      if (size_ == 0 || head_.get() == nullptr) {
        return; 
      }

      Node *next{head_.get()};
      while (next->next != nullptr) {
        next = next->next;
      }
      element = next->element;
    }

    template<typename Element>
    void Queue<Element>::back(Element& element)
    {
      if (size_ == 0 || head_.get() == nullptr) {
        return;
      }

      Node *next{head_.get()};

      element = next->element;
    }

    template<typename Element>
    int Queue<Element>::pop(Element& element)
    {
      if (size_ == 0 || head_.get() == nullptr)
      {
        return QueueStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY; 
      }

      Node *next{head_.get()};
      while (next->next != nullptr && next->next->next != nullptr)
      {
        next = next->next;
      }

      if (next->next != nullptr)
      {
        element = next->next->element;
        delete next->next;
        next->next = nullptr;
      }
      else
      {
        // Case which is we have only one element
        element = next->element;
        head_.reset();
      }

      size_--;

      return QueueStatus::ELEMENT_POPED_SUCCESSFULLY;
    }

    template<typename Element>
    bool Queue<Element>::isEmpty()
    {
      return size_ > 0 ? false : true;
    }

    template<typename Element>
    int Queue<Element>::getSizeOfQueue()
    {
      return size_;
    }
  }
}