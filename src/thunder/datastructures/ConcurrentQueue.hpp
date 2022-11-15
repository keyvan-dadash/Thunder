/**
 * @file ConcurrentQueue.hpp
 * Contains ConcurrentQueue class which is thread safe and high performance queue.
 */
#pragma once

#include <atomic>
#include <cstddef>
#include <memory>
#include <thunder/datastructures/BaseConcurrentQueue.hpp>
#include <utility>

namespace thunder {

namespace datastructures {

#ifdef __cpp_lib_hardware_interference_size
using std::hardware_constructive_interference_size;
using std::hardware_destructive_interference_size;
#else
// 64 bytes on x86-64 │ L1_CACHE_BYTES │ L1_CACHE_SHIFT │ __cacheline_aligned │
// ...
constexpr std::size_t hardware_constructive_interference_size =
    2 * sizeof(std::max_align_t);
constexpr std::size_t hardware_destructive_interference_size =
    2 * sizeof(std::max_align_t);
#endif

/**
 * \brief ConcurrentQueue is a thread-safe and high-performance queue for multithread envirment.
 * \tparam Element is a item which ConcurrentQueue is storing.
 * \tparam QueueSize is maximum size of ConcurrentQueue.
 *
 * ConcurrentQueue can use in multithread are which safter and performance is critical. This class
 * use lock-free approach for reach it's best performance, so sometimes it's malleable that CPU
 * usage reach 100% because it use busy loop, and also for sake of performance this utility has
 * fixed size.
 */
template <typename Element, std::size_t QueueSize = 128>
class ConcurrentQueue : BaseConcurrentQueue<Element> {
 public:
  explicit ConcurrentQueue();

  ~ConcurrentQueue();

  using BaseConcurrentQueueStatus =
      typename BaseConcurrentQueue<Element>::BaseConcurrentQueueStatus;

  /**
   * \brief Class for represent status
   *
   * Every operation should return one of the staus that is in this particular class, and also, 
   * other class's which is inherited from ConcurrentQueue can change status since most
   * functions are returning int type.
   */
  class ConcurrentQueueStatus {
   public:
    enum Status {
      ELEMENT_PUSHED_SUCCESSFULLY =
          BaseConcurrentQueueStatus::ELEMENT_PUSHED_SUCCESSFULLY, /// Return when push operation done succesfully.
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
  int forcePush(T&& t);

  /**
   * \brief Try push element to queue when queue is not reached it's capacity.
   * \tparam Element to push.
   * \return operation status.
   */
  template <typename T>
  int tryPush(T&& t);

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
   * \brief Perform push in atomic manner.
   * \tparam T is the element for push 
   * \param head of underlaying queue for push into.
   */
  template <typename T>
  void push_atomic(T&& t, int head);

  /**
   * \brief Perform pop in atomic manner.
   * \param Element is for storing poped item.
   * \param tail of underlaying queue for pop from.
   */
  void pop_atomic(Element& elem, int tail);

  static const std::size_t kQueueSize = QueueSize; /// Maximum size of ConcurrentQueue.

  /**
   * \brief Class which represent each cell status.
   */
  class CellStates {
   public:
    enum States { 
      STORED = 1, /// STORED use when there is element in the cell.
      STORING, /// STORING use when a push operation is in progress.
      LOADED, /// LOADED use when pop operation is successful.
      LOADING, /// LOADING use when a pop operation is in progress.
      EMPTY /// EMPTY use when there is no element present in the cell.
    };
  };

  alignas(hardware_destructive_interference_size) std::atomic<int16_t> head_; /// head of queue.
  alignas(hardware_destructive_interference_size) std::atomic<int16_t> tail_; /// tail of queue.

  typedef typename std::aligned_storage_t<sizeof(Element), alignof(Element)>
      ElementStorage;

  ElementStorage elementArray_[QueueSize]; /// queue which elements are storing in.

  std::atomic_int8_t states_[QueueSize]; /// status of each cell in the queue.

  std::atomic<size_t> size_{0}; /// size of queue.
};

using ConcurrentQueueOperationStatus =
    ConcurrentQueue<int>::ConcurrentQueueStatus::Status;
}  // namespace datastructures
}  // namespace thunder

#include <thunder/datastructures/ConcurrentQueue.cpp>
