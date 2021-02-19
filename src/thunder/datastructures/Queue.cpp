
#pragma once

#include <memory>
#include <iostream>

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
    int Queue<Element>::push(Element&& element)
    {
      std::unique_ptr<Node> node(new Node(std::forward<Element>(element)));
      node->next = this->head_.release();

      this->head_.reset(node.get());
      node.release();
      this->size_++;

      return static_cast<int>(BaseQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }

    template<typename Element>
    int Queue<Element>::tryPush(Element&& element, int maxSize)
    {
      if (this->size_ >= maxSize) {
        return static_cast<int>(BaseQueueStatus::CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE);
      }

      std::unique_ptr<Node> node(new Node(std::forward<Element>(element)));
      node->next = this->head_.release();

      this->head_.reset(node.get());
      node.release();
      this->size_++;

      return static_cast<int>(BaseQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }

    template<typename Element>
    Element Queue<Element>::front()
    {
      Node *next{this->head_.get()};
      while (next->next != nullptr) {
        next = next->next;
      }

      return next->element;
    }

    template<typename Element>
    int Queue<Element>::pop()
    {
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