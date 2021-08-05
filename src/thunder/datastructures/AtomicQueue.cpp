
#pragma once

#include <atomic>
#include <iostream>


#include <thunder/datastructures/AtomicQueue.hpp>


namespace thunder {

  namespace datastructures {

    template<typename Element>
    template<typename T> 
    int AtomicQueue<Element>::push(T&& t)
    {
        std::unique_ptr<Node> node(new Node(std::forward<T>(t)));
        auto head = this->head_.load(std::memory_order_relaxed);
        node->next = head;

        while (true) {
            if (head_.compare_exchange_strong(
                head,
                node.get(),
                std::memory_order_acq_rel,
                std::memory_order_relaxed
            )) {
                node.release();
                this->size_.fetch_add(1);
                return static_cast<int>(BaseAtomicQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY);
            }
        }
    }

    template< typename Element>
    template<typename T>
    int AtomicQueue<Element>::tryPush(T&& t, int maxSize)
    {
        if (this->size_.load(std::memory_order_relaxed) >= maxSize) {
            return static_cast<int>(BaseAtomicQueueStatus::CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE);
        }

        std::unique_ptr<Node> node(new Node(std::forward<T>(t)));
        auto head = this->head_.load(std::memory_order_relaxed);
        node->next = head;

        while (true) {
            if (head_.compare_exchange_strong(
                head,
                node.get(),
                std::memory_order_acq_rel,
                std::memory_order_relaxed
            )) {
                node.release();
                this->size_.fetch_add(1);
                return static_cast<int>(BaseAtomicQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY);
            }
        }
    }

    template<typename Element>
    std::shared_ptr<Element> 
    AtomicQueue<Element>::front()
    {
      if (this->size_.load(std::memory_order_relaxed) == 0 || this->head_.load(std::memory_order_relaxed) == nullptr) {
        return std::make_shared<Element>(); 
      }

      Node *next{this->head_.load(std::memory_order_relaxed)};
      while (next->next != nullptr) {
        next = next->next;
      }
      
      return std::make_shared<Element>(next->element);
    }

    template<typename Element>
    std::shared_ptr<Element> 
    AtomicQueue<Element>::back()
    {
      if (this->size_.load(std::memory_order_relaxed) == 0 || this->head_.load(std::memory_order_relaxed) == nullptr) {
        return std::make_shared<Element>(); 
      }

      Node *next{this->head_.load(std::memory_order_relaxed)};
      
      return std::make_shared<Element>(next->element);
    }

    template<typename Element>
    int AtomicQueue<Element>::pop()
    {
      if (this->size_.load(std::memory_order_relaxed) == 0 ||
       this->head_.load(std::memory_order_relaxed) == nullptr) {
        return BaseAtomicQueueStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY;
      }

      Node *next{this->head_.load(std::memory_order_relaxed)};
      while (next->next != nullptr && next->next->next != nullptr) {
        next = next->next;
      }

      if (next->next == nullptr) {
        auto head = head_.load(std::memory_order_relaxed);
        while(!head_.compare_exchange_strong(
            head,
            nullptr,
            std::memory_order_acq_rel,
            std::memory_order_relaxed
        ));
      } else if (next->next->next == nullptr) {
        Node *tmp = std::exchange(next->next, nullptr);
        delete tmp;

      } else {
        return static_cast<int>(BaseAtomicQueueStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
      }

      this->size_.fetch_sub(1);
      return static_cast<int>(BaseAtomicQueueStatus::ELEMENT_POPED_SUCCESSFULLY);
    }

    template<typename Element>
    int AtomicQueue<Element>::pop_back()
    {
      if (this->size_.load(std::memory_order_relaxed) == 0 ||
       this->head_.load(std::memory_order_relaxed) == nullptr) {
        return BaseAtomicQueueStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY;
      }
      

      Node *next{this->head_.load(std::memory_order_relaxed)};

      auto head = head_.load(std::memory_order_relaxed);

      while (true) {
        if (!head_.compare_exchange_strong(
          head,
          head->next,
          std::memory_order_acq_rel,
          std::memory_order_relaxed
        )) {
          if (head == nullptr) {
            break;
          }
        }
      }

      this->size_.fetch_sub(1);
      return static_cast<int>(BaseAtomicQueueStatus::ELEMENT_POPED_SUCCESSFULLY);
    }


    template<typename Element>
    bool AtomicQueue<Element>::pop_and_get_front(Element& element)
    {
      if (this->size_ == 0 || this->head_.get() == nullptr) {
        return NULL; 
      }

      Node *next{this->head_.load(std::memory_order_relaxed)};
      while (next->next != nullptr && next->next->next != nullptr) {
        next = next->next;
      }

      element = next->next->element;

      delete next->next;
      next->next = nullptr;

      this->size_--;
      return element;
    }

    template<typename Element>
    bool AtomicQueue<Element>::pop_and_get_back(Element& element)
    {
      if (this->size_ == 0 || this->head_.get() == nullptr) {
        return NULL; 
      }

      Node *next{this->head_.load(std::memory_order_relaxed)};
      element = next->element;

      auto head = head_.load(std::memory_order_relaxed);

      while (true) {
        if (!head_.compare_exchange_strong(
          head,
          head->next,
          std::memory_order_acq_rel,
          std::memory_order_relaxed
        )) {
          if (head == nullptr) {
            break;
          }
        }
      }

      this->size_--;
      return element;
    }



    template<typename Element>
    bool AtomicQueue<Element>::isEmpty()
    {
      return this->size_.load(std::memory_order_relaxed) > 0 ? false : true;
    }

    template<typename Element>
    int AtomicQueue<Element>::getSizeOfQueue()
    {
      return this->size_.load(std::memory_order_relaxed);
    }

  }
}