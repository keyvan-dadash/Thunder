/**
 * @file Queue.hpp
 */
#pragma once

#include <iostream>
#include <memory>
#include <thunder/datastructures/BaseQueue.hpp>
#include <utility>

namespace thunder {

namespace datastructures {

/**
 * \brief Queue is simple queue.
 * \tparam Element is the item which store in queue.
 *
 * Queue is simple and generic queue.
 */
template <typename Element>
class Queue : public BaseQueue<Element> {
 public:
  Queue() {}

  ~Queue() = default;

  Queue(Queue<Element>&& other) noexcept;
  Queue<Element>& operator=(Queue<Element>&& rhs) noexcept;

  using BaseQueueStatus = typename BaseQueue<Element>::BaseQueueStatus;

  /**
   * \brief Class for represent status
   *
   * Every operation should return one of the staus that is in this particular class, and also, 
   * other class's which is inherited from Queue can change status since most
   * functions are returning int type.
   */
  class QueueStatus : public BaseQueueStatus {
   public:
    enum Status {
      ELEMENT_PUSHED_SUCCESSFULLY =
          BaseQueueStatus::Status::ELEMENT_PUSHED_SUCCESSFULLY,
      ELEMENT_POPED_SUCCESSFULLY,
      CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE,
      OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY
    };
  };

  /**
   * \brief Push element by force in queue even when queue is full.
   * \tparam Element to push.
   * \return operation status.
   */
  template <typename T>
  int push(T&& t);

  /**
   * \brief Try push element to queue when queue is not reached it's capacity.
   * \tparam Element to push.
   * \param T is the item to push.
   * \param if queue size is more than maxSize then operation whould fail.
   * \return operation status.
   */
  template <typename T>
  int tryPush(T&& t, int maxSize);
  
  /**
   * \brief Get element from front of the queue.
   * \param Element for stroing front item
   * \return void
   */
  void front(Element& element) override;

  /**
   * \brief Get element from back of the queue.
   * \param Element for stroing back item.
   * \return void
   */
  void back(Element& element) override;

  /**
   * \brief Pop element from queue.
   * \param Store poped item to Element
   * \return operation status.
   */
  int pop(Element& element) override;

  /**
   * \brief Check whatever is queue full or empty.
   * \return Boolean which represent if queue is empty or full.
   */
  bool isEmpty() override;

  /**
   * \brief Return size of queue.
   * \return Size of queue.
   */
  int getSizeOfQueue() override;

 private:

  /**
   * \brief Node class is a wrapper for element in queue.
   */
  struct Node {
   public:
    Element element = NULL; /// Element inside Node

    /**
     * Create a Node.
     * \brief Constructor of Node.
     * \tparam T is the type of Element.
     * \paramt t is the item which put in element attribute.
     */
    template <typename T>
    explicit Node(T&& t) : element(std::forward<T>(t)) {}

    Node operator=(Node&& other) { element(other.element); }

    Node* next = nullptr; /// Pointer to next Node which hold next element in the queue.
  };

  std::unique_ptr<Node> head_ = nullptr; /// Head of the queue.
  size_t size_{0}; /// Size of the Queue.
};

using QueueOperationStatus = Queue<int>::QueueStatus::Status;

}  // namespace datastructures
}  // namespace thunder

#include <thunder/datastructures/Queue.cpp>
