#pragma once

#include "ThreadPool.hpp"

#include <assert.h>
#include <future>
#include <memory>
#include <thread>

#include "Task.hpp"
#include "thunder/datastructures/ConcurrentQueue.hpp"

namespace thunder {

namespace pools {

template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
thread_local thunder::datastructures::ConcurrentQueue<Task, ThreadQueueSize>*
    ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::local_tasks_queue_ptr_;

template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
thread_local size_t
    ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::thread_queue_index_;

template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::ThreadPool(
    ThreadPoolOptions options)
    : options_(options) {
  size_t numebrOfThreads = this->options_.numberOfThreads;
  assert(options_.threads_option.size() <= numebrOfThreads && "There are more threads option than number of given threads");
  threads_.reserve(numebrOfThreads);
  thread_queues_.reserve(numebrOfThreads);

  for (int i = 0; i < numebrOfThreads; i++) {
    thread_queues_.push_back(
        std::unique_ptr<
            thunder::datastructures::ConcurrentQueue<Task, ThreadQueueSize>>(
            new thunder::datastructures::ConcurrentQueue<Task,
                                                         ThreadQueueSize>()));
  }

  int remain = numebrOfThreads - options_.threads_option.size();
  int thread_index = 0;
  for (auto& th_option : options_.threads_option) {
    auto th = std::thread(&ThreadPool::handleTasks, this, thread_index++);
    if (th_option.set_sche)
      ThreadUtils::setScheduling(th, th_option.policy, th_option.priority);
    if (th_option.set_cpu_affinity)
      ThreadUtils::setCPUAffinity(th, th_option.cpu_ids);

    threads_.push_back(std::move(th));
  }

  for (int i = 0; i < remain; i++) {
    threads_.push_back(std::thread(&ThreadPool::handleTasks, this, thread_index++));
  }
}

template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::~ThreadPool() {
  done();
}

template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
template <typename FuncType>
std::future<typename std::result_of_t<FuncType()>>
ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::submit(FuncType func) {
  typedef typename std::result_of_t<FuncType()> funcResultType;

  std::packaged_task<funcResultType()> task(func);

  std::future<funcResultType> result(task.get_future());

  if (local_tasks_queue_ptr_) {
    local_tasks_queue_ptr_->tryPush(std::move(task));
  } else {
    pool_tasks_queue_.tryPush(std::move(task));
  }

  return result;
}

template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
void ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::handleTasks(
    size_t queue_index) {
  thread_queue_index_ = queue_index;
  local_tasks_queue_ptr_ = thread_queues_[thread_queue_index_].get();

  while (this->isRunning_) {
    Task task;
    // std::cout << queue_index << std::endl;
    if (popTaskFromLocalQueue(task) || popTaskFromGlobalQueue(task) ||
        popTaskFromOtherThreads(task)) {
      task();
    } else {
      std::this_thread::yield();
    }
  }
}

template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
bool ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::popTaskFromLocalQueue(
    Task& task) {
  if (local_tasks_queue_ptr_) {
    auto result = local_tasks_queue_ptr_->pop(task);
    if (result == thunder::datastructures::ConcurrentQueueOperationStatus::
                      ELEMENT_POPED_SUCCESSFULLY)
      return true;
  }

  return false;
}

template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
bool ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::popTaskFromGlobalQueue(
    Task& task) {
  // std::cout << "hehe" << std::endl;
  auto result = pool_tasks_queue_.pop(task);
  if (result == thunder::datastructures::ConcurrentQueueOperationStatus::
                    ELEMENT_POPED_SUCCESSFULLY)
    return true;

  return false;
}

template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
bool ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::popTaskFromOtherThreads(
    Task& task) {
  int thread_size = thread_queues_.size();
  for (int i = 0; i < thread_size; i++) {
    int index = (thread_queue_index_ + i + 1) % thread_size;
    auto result = thread_queues_[index]->pop(task);
    if (result == thunder::datastructures::ConcurrentQueueOperationStatus::
                      ELEMENT_POPED_SUCCESSFULLY)
      return true;
  }

  return false;
}

template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
void ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::done() {
  if (!isRunning_) return;

  size_t numebrOfThreads = this->options_.numberOfThreads;

  isRunning_ = false;
  for (int i = 0; i < numebrOfThreads; i++) {
    threads_.at(i).join();
  }
}
};  // namespace pools
}  // namespace thunder
