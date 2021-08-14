
#pragma once

#include <atomic>
#include <iostream>


#include <thunder/datastructures/AtomicQueue.hpp>


namespace thunder {

  namespace datastructures {


    template <typename Element, int QueueSize>
    AtomicQueue<Element, QueueSize>::AtomicQueue()
    {
      this->allocateBuffer();
    }


    template <typename Element, int QueueSize>
    AtomicQueue<Element, QueueSize>::~AtomicQueue()
    {
      this->alloca_.deallocate(this->elementArray_, QueueSize);
    }

    template <typename Element, int QueueSize>
    template<typename T> 
    int AtomicQueue<Element, QueueSize>::push(T&& t)
    {
      auto head = this->head_.load(std::memory_order_relaxed);
      if ((head - 
            this->tail_.load(std::memory_order_relaxed)) <= 1 && this->size_ > 0)
      {
        return BaseAtomicQueueStatus::CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE;
      }

      do {
        if ((head - 
            this->tail_.load(std::memory_order_relaxed)) <= 1 && this->size_ > 0)
        {
          return BaseAtomicQueueStatus::CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE;
        }
      } while(!this->head_.compare_exchange_strong(head, head + 1, std::memory_order_release));

      new (&(this->elementArray_[head])) Element(std::forward<T>(t));



        // std::unique_ptr<Node> node(new Node(std::forward<T>(t)));
        // auto head = this->head_.load(std::memory_order_relaxed);
        // node->next = head;



        // while (true) {
        //     if (head_.compare_exchange_strong(
        //         head,
        //         node.get(),
        //         std::memory_order_acq_rel,
        //         std::memory_order_relaxed
        //     )) {
        //         node.release();
        //         this->size_.fetch_add(1);
        //         return static_cast<int>(BaseAtomicQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY);
        //     }
        // }
    }

    template< typename Element>
    template<typename T>
    int AtomicQueue<Element, QueueSize>::tryPush(T&& t, int maxSize)
    {
        // if (this->size_.load(std::memory_order_relaxed) >= maxSize) {
        //     return static_cast<int>(BaseAtomicQueueStatus::CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE);
        // }

        // std::unique_ptr<Node> node(new Node(std::forward<T>(t)));
        // auto head = this->head_.load(std::memory_order_relaxed);
        // node->next = head;

        // while (true) {
        //     if (head_.compare_exchange_strong(
        //         head,
        //         node.get(),
        //         std::memory_order_acq_rel,
        //         std::memory_order_relaxed
        //     )) {
        //         node.release();
        //         this->size_.fetch_add(1);
        //         return static_cast<int>(BaseAtomicQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY);
        //     }
        // }
    }

    template <typename Element, int QueueSize>
    void 
    AtomicQueue<Element, QueueSize>::front(Element& element)
    {
      if (this->size_.load(std::memory_order_relaxed) == 0) {
        return; 
      }

      auto tail = this->tail_.load(std::memory_order_relaxed);

      while (true)
      {
        element = this->elementArray_[tail];
        if (tail == this->tail_.load(std::memory_order_relaxed)) break;
        tail = this->tail_.load(std::memory_order_relaxed);
        if (this->size_.load(std::memory_order_relaxed) <= 0) break;
      }
    }

    template <typename Element, int QueueSize>
    void
    AtomicQueue<Element, QueueSize>::back(Element& element)
    {
      if (this->size_.load(std::memory_order_relaxed) == 0) {
        return; 
      }

      auto head = this->head_.load(std::memory_order_relaxed);

      while (true)
      {
        element = this->elementArray_[head];
        if (head == this->head_.load(std::memory_order_relaxed)) break;
        head = this->head_.load(std::memory_order_relaxed);
        if (this->size_.load(std::memory_order_relaxed) <= 0) break;
      }
    }

    template <typename Element, int QueueSize>
    int AtomicQueue<Element, QueueSize>::pop(Element& element)
    {
      auto tail = this->tail_.load(std::memory_order_relaxed);
      if (std::abs(tail - 
            this->head_.load(std::memory_order_relaxed)) <= 1 && this->size_ > 0)
      {
        return BaseAtomicQueueStatus::CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE;
      }

      do {
        if (std::abs(tail - 
            this->head_.load(std::memory_order_relaxed)) <= 1 && this->size_ > 0)
        {
          return BaseAtomicQueueStatus::CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE;
        }
      } while(!this->head_.compare_exchange_strong(tail, tail + 1, std::memory_order_release));

      new (&(this->elementArray_[head])) Element(std::forward<T>(t));


      // if (this->size_.load(std::memory_order_relaxed) == 0 ||
      //  this->head_.load(std::memory_order_relaxed) == nullptr) {
      //   return BaseAtomicQueueStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY;
      // }

      // Node *next{this->head_.load(std::memory_order_relaxed)};
      // while (next->next != nullptr && next->next->next != nullptr) {
      //   next = next->next;
      // }

      // if (next->next == nullptr) {
      //   auto head = head_.load(std::memory_order_relaxed);
      //   while(!head_.compare_exchange_strong(
      //       head,
      //       nullptr,
      //       std::memory_order_acq_rel,
      //       std::memory_order_relaxed
      //   ));
      // } else if (next->next->next == nullptr) {
      //   Node *tmp = std::exchange(next->next, nullptr);
      //   delete tmp;

      // } else {
      //   return static_cast<int>(BaseAtomicQueueStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
      // }

      // this->size_.fetch_sub(1);
      // return static_cast<int>(BaseAtomicQueueStatus::ELEMENT_POPED_SUCCESSFULLY);
    }

    // template <typename Element, int QueueSize>
    // int AtomicQueue<Element, QueueSize>::pop_back()
    // {
      // if (this->size_.load(std::memory_order_relaxed) == 0 ||
      //  this->head_.load(std::memory_order_relaxed) == nullptr) {
      //   return BaseAtomicQueueStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY;
      // }
      

      // Node *next{this->head_.load(std::memory_order_relaxed)};

      // auto head = head_.load(std::memory_order_relaxed);

      // while (true) {
      //   if (!head_.compare_exchange_strong(
      //     head,
      //     head->next,
      //     std::memory_order_acq_rel,
      //     std::memory_order_relaxed
      //   )) {
      //     if (head == nullptr) {
      //       break;
      //     }
      //   }
      // }

      // this->size_.fetch_sub(1);
      // return static_cast<int>(BaseAtomicQueueStatus::ELEMENT_POPED_SUCCESSFULLY);
    // }


    // template <typename Element, int QueueSize>
    // bool AtomicQueue<Element, QueueSize>::pop_and_get_front(Element& element)
    // {
      // if (this->size_ == 0 || this->head_.get() == nullptr) {
      //   return NULL; 
      // }

      // Node *next{this->head_.load(std::memory_order_relaxed)};
      // while (next->next != nullptr && next->next->next != nullptr) {
      //   next = next->next;
      // }

      // element = next->next->element;

      // delete next->next;
      // next->next = nullptr;

      // this->size_--;
      // return element;
    // }

    // template <typename Element, int QueueSize>
    // bool AtomicQueue<Element, QueueSize>::pop_and_get_back(Element& element)
    // {
      // if (this->size_ == 0 || this->head_.get() == nullptr) {
      //   return NULL; 
      // }

      // Node *next{this->head_.load(std::memory_order_relaxed)};
      // element = next->element;

      // auto head = head_.load(std::memory_order_relaxed);

      // while (true) {
      //   if (!head_.compare_exchange_strong(
      //     head,
      //     head->next,
      //     std::memory_order_acq_rel,
      //     std::memory_order_relaxed
      //   )) {
      //     if (head == nullptr) {
      //       break;
      //     }
      //   }
      // }

      // this->size_--;
      // return element;
    // }



    template <typename Element, int QueueSize>
    bool AtomicQueue<Element, QueueSize>::isEmpty()
    {
      return this->size_.load(std::memory_order_relaxed) > 0 ? false : true;
    }

    template <typename Element, int QueueSize>
    int AtomicQueue<Element, QueueSize>::getSizeOfQueue()
    {
      return this->size_.load(std::memory_order_relaxed);
    }


    template <typename Element, int QueueSize>
    int64_t AtomicQueue<Element, QueueSize>::getSizeNearPowerTwo(int64_t input_size)
    {
      input_size--;
      input_size |= input_size >> 1;
      input_size |= input_size >> 2;
      input_size |= input_size >> 4;
      input_size |= input_size >> 8;
      input_size |= input_size >> 16;
      input_size |= input_size >> 32;
      return ++input_size;
    }

    template <typename Element, int QueueSize>
    bool AtomicQueue<Element, QueueSize>::allocateBuffer()
    {
      this->elementArray_ = this->alloca_.allocate(QueueSize);
    }

  }
}