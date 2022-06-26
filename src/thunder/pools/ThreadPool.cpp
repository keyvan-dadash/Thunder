#include <thread>
#include <memory>
#include <future>

#include "thunder/datastructures/ConcurrentQueue.hpp"

#include "ThreadPool.hpp"
#include "Task.hpp"

namespace thunder {

  namespace pools {

    template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
    ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::ThreadPool(ThreadPoolOptions options) : options_(options_)
    {
      size_t numebrOfThreads = this->options_.numberOfThreads;
      threads_.reserve(numebrOfThreads);

      for (int i = 0; i < numebrOfThreads; i++) {
        threads_.push_back(
          std::thread(&ThreadPool::handleTasks, this, i)
        );
      }
    }

    template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
    ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::~ThreadPool()
    {
      done();
    }

    template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
    template<typename FuncType>
    std::future<
      typename std::result_of_t<FuncType()>
    > 
    ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::submit(FuncType func)
    {
      typedef typename std::result_of_t<FuncType()> funcResultType;

      std::packaged_task<funcResultType()> task(func);

      std::future<funcResultType> result(task.get_future());

      if (local_tasks_queue_ptr_ )
      {
        local_tasks_queue_ptr_->tryPush(std::move(task));
      }
      else
      {
        pool_tasks_queue_.tryPush(std::move(task));
      }

      return result;
    }

    template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
    void ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::handleTasks(size_t queue_index)
    {
      thread_queue_index_ = queue_index;
      local_tasks_queue_ptr_ = thread_queues_[thread_queue_index_].get();

      while(this->isRunning_)
      {
        Task task;

        if (popTaskFromLocalQueue(task) || popTaskFromGlobalQueue(task) || popTaskFromOtherThreads(task))
        {
          task();
        }
        else
        {
          std::this_thread::yield();
        }
      }
    }

    template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
    bool ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::popTaskFromLocalQueue(Task& task)
    {
      if (local_tasks_queue_ptr_)
      {
        auto result = local_tasks_queue_ptr_->pop(task);
        if (result == 
            thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY)
          return true;
      }

      return false;
    }

    template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
    bool ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::popTaskFromGlobalQueue(Task& task)
    {
      if (local_tasks_queue_ptr_)
      {
        auto result = pool_tasks_queue_.pop(task);
        if (result == 
            thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY)
          return true;
      }

      return false;
    }

    template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
    bool ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::popTaskFromOtherThreads(Task& task)
    {
      int thread_size = thread_queues_.size();
      for (int i = 0; i < thread_size; i++)
      {
        int index = (thread_queue_index_ + i + 1) % thread_size;
        auto result = thread_queues_[index]->pop(task);
        if (result == 
            thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY)
          return true;
      }

      return false;
    }
    
    template <size_t ThreadPoolQueueSize, size_t ThreadQueueSize>
    void ThreadPool<ThreadPoolQueueSize, ThreadQueueSize>::done()
    {
      size_t numebrOfThreads = this->options_.numberOfThreads;
      
      isRunning_ = false;
      for (int i = 0; i < numebrOfThreads; i++) {
        threads_.at(i).join();
      }
    }
  };
}
