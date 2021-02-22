
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
      this->size_( std::move(rhs.size_) );
      this->head_.reset( std::move(rhs.head_) );

      return *this;
    }

    template<typename Element>
    Queue<Element>::~Queue()
    {
      if (this->size_ > 0 && this->head_.get() != nullptr) {
        while (this->size_ > 0)
        {
          if (this->pop() == QueueStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY) {
            return;
          }
        }
      }
    }
    
    template<typename Element>
    template <typename T>
    int Queue<Element>::push(T&& t)
    {
      std::unique_ptr<Node> node(new Node(std::forward<T>(t)));
      node->next = this->head_.release();

      this->head_.reset(node.get());
      node.release();
      this->size_++;

      return static_cast<int>(BaseQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }

    template<typename Element>
    template <typename T>
    int Queue<Element>::tryPush(T&& t, int maxSize)
    {
      if (this->size_ >= maxSize) {
        return static_cast<int>(BaseQueueStatus::CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE);
      }

      std::unique_ptr<Node> node(new Node(std::forward<T>(t)));
      node->next = this->head_.release();

      this->head_.reset(node.get());
      node.release();
      this->size_++;

      return static_cast<int>(BaseQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }

    template<typename Element>
    std::shared_ptr<Element> Queue<Element>::front()
    {
      if (this->size_ == 0 || this->head_.get() == nullptr) {
        return std::make_shared<Element>(); 
      }

      Node *next{this->head_.get()};
      while (next->next != nullptr) {
        next = next->next;
      }

      return std::make_shared<Element>(next->element);
    }

    template<typename Element>
    int Queue<Element>::pop()
    {
      if (this->size_ == 0 || this->head_.get() == nullptr) {
        return QueueStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY;
      }

      Node *next{this->head_.get()};
      while (next->next != nullptr && next->next->next != nullptr) {
        next = next->next;
      }

      if (next->next == nullptr) {
        this->head_.reset();

      } else if (next->next->next == nullptr) {
        Node *tmp = std::exchange(next->next, nullptr);
        delete tmp;

      } else {
        return static_cast<int>(BaseQueueStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
      }

      this->size_--;
      return static_cast<int>(BaseQueueStatus::ELEMENT_POPED_SUCCESSFULLY);
    }

    template<typename Element>
    bool Queue<Element>::isEmpty()
    {
      return this->size_ > 0 ? false : true;
    }
  }



}