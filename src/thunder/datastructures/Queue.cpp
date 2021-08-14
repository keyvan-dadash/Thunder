
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
      , tail_( std::move(rhs.tail_))
    {
      
    }

    template<typename Element>
    Queue<Element>& 
    Queue<Element>::operator=(Queue<Element>&& rhs) noexcept
    {
      this->size_( std::move(rhs.size_) );
      this->head_.reset( std::move(rhs.head_) );
      this->tail_.reset( std::move(rhs.tail_) );

      return *this;
    }

    template<typename Element>
    Queue<Element>::~Queue()
    {
      // if (this->size_ > 0 && this->head_.get() != nullptr) {
      //   while (this->size_ > 0)
      //   {
      //     if (this->pop() == QueueStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY) {
      //       return;
      //     }
      //   }
      // }
    }
    
    template<typename Element>
    template <typename T>
    int Queue<Element>::push(T&& t)
    {
      std::unique_ptr<Node> node(new Node(std::forward<T>(t)));
      node->next = this->head_.release();

      this->head_.reset(node.get());
      if (this->size_ == 0)
      {
        this->tail_.reset(node.get());
      }
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
      if (this->size_ == 0)
      {
        this->tail_.reset(node.get());
      }
      
      node.release();
      this->size_++;

      return static_cast<int>(BaseQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }

    template<typename Element>
    void Queue<Element>::front(Element& element)
    {
      if (this->size_ == 0 || this->head_.get() == nullptr) {
        return; 
      }

      Node *next{this->tail_.get()};
      element = next->element;
    }

    template<typename Element>
    void Queue<Element>::back(Element& element)
    {
      if (this->size_ == 0 || this->head_.get() == nullptr) {
        return;
      }

      Node *next{this->head_.get()};

      element = next->element;
    }

    template<typename Element>
    int Queue<Element>::pop(Element& element)
    {

      if (this->size_ == 0 || this->head_.get() == nullptr) {
        return QueueStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY; 
      }

      Node *next{this->head_.get()};
      while (next->next != nullptr && next->next->next != nullptr) {
        next = next->next;
      }

      element = next->next->element;

      delete next->next;
      next->next = nullptr;

      this->size_--;

      return QueueStatus::ELEMENT_POPED_SUCCESSFULLY;
    }

    template<typename Element>
    bool Queue<Element>::isEmpty()
    {
      return this->size_ > 0 ? false : true;
    }

    template<typename Element>
    int Queue<Element>::getSizeOfQueue()
    {
      return this->size_;
    }


  }



}