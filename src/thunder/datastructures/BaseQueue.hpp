/**
 * @file BaseQueue.hpp
 * Contains BaseQueue class which all other implementation of queue's inherite from.
 */
#pragma once

namespace thunder {
namespace datastructures {

/**
 *  \brief Abstract class for queue.
 *  \tparam Element type for storing in queue.
 *
 *  This class specify what api's should general queue expose.
 */
template <typename Element>
class BaseQueue {
 public:
  explicit BaseQueue() = default;

  ~BaseQueue() = default;

  /**
   * \brief Class for represent status
   *
   * Every operation should return one of the staus that is in this particular class, and also, 
   * other class's which is inherited from BaseQueue can change status since most
   * functions are returning int type.
   */
  class BaseQueueStatus {
   public:
    enum Status {
      ELEMENT_PUSHED_SUCCESSFULLY = 1, /// Return when push operation done succesfully.
      ELEMENT_POPED_SUCCESSFULLY, /// Return when pop operation done successfully.
      CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE, /// Return when queue is already full and cannot insert in it.
      OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY /// Return when queue is already empty and pop operation requested
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
  virtual void front(Element& element) = 0;

  /**
   * \brief Get element from back of the queue.
   * \param Element for stroing back item.
   * \return void
   */
  virtual void back(Element& element) = 0;

  /**
   * \brief Pop element from queue.
   * \param Store poped item to Element
   * \return operation status.
   */
  virtual int pop(Element& element) = 0;

  /**
   * \brief Check whatever is queue full or empty.
   * \return Boolean which represent if queue is empty or full.
   */
  virtual bool isEmpty() = 0;

  /**
   * \brief Return size of queue.
   * \return Size of queue.
   */
  virtual int getSizeOfQueue() = 0;
};

}  // namespace datastructures
}  // namespace thunder
