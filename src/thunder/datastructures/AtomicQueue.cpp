
#pragma once

#include <atomic>


#include <thunder/datastructures/AtomicQueue.hpp>


namespace thunder {

  namespace datastructures {

    template< typename Element>
    int AtomicQueue<Element>::push(Element&& element)
    {
        std::unique_ptr<Node> node(new Node(std::forward<Element>(element)));
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
    int AtomicQueue<Element>::tryPush(Element&& element, int maxSize)
    {
        if (this->size_.load(std::memory_order_relaxed) >= maxSize) {
            return static_cast<int>(BaseAtomicQueueStatus::CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE);
        }

        std::unique_ptr<Node> node(new Node(std::forward<Element>(element)));
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
    bool AtomicQueue<Element>::isEmpty()
    {
      return this->size_.load(std::memory_order_relaxed) > 0 ? false : true;
    }

  }
}