#pragma once

#include <memory>
#include <thread>
#include <future>
#include <vector>

#include "thunder/datastructures/ConcurrentQueue.hpp"

#include "Task.hpp"

namespace thunder {

  namespace pools {

    struct ThreadPoolOptions
    {
      size_t numberOfThreads;
    };

    template <size_t ThreadPoolQueueSize = 1024, size_t ThreadQueueSize = 512>
    class ThreadPool
    {
    public:

      ThreadPool(ThreadPoolOptions options);

      ThreadPool(ThreadPool&& other) = delete;
      ThreadPool& operator=(ThreadPool&& rhs) = delete;

      ThreadPool(const ThreadPool& other) = delete;
      ThreadPool& operator=(const ThreadPool& rhs) = delete;

      ~ThreadPool();

      template<typename FuncType>
      std::future<
        typename std::result_of_t<FuncType()>
      > submit(FuncType func);

      void handleTasks(size_t queue_index);

      void done();

    private:

      bool popTaskFromLocalQueue(Task& task);

      bool popTaskFromOtherThreads(Task& task);

      bool popTaskFromGlobalQueue(Task& task);

      ThreadPoolOptions options_;

      std::vector<std::thread> threads_;

      std::vector<std::unique_ptr<thunder::datastructures::ConcurrentQueue<Task, ThreadQueueSize>>> thread_queues_;

      static thread_local thunder::datastructures::ConcurrentQueue<Task, ThreadQueueSize> *local_tasks_queue_ptr_;

      static thread_local size_t thread_queue_index_;

      thunder::datastructures::ConcurrentQueue<Task, ThreadPoolQueueSize> pool_tasks_queue_;

      bool isRunning_ = true;
    };
  };
}
