
#pragma once


#include <memory>
#include <thread>
#include <future>
#include <vector>

#include "thunder/datastructures/AtomicQueue.hpp"

#include "Task.hpp"

namespace thunder {


  namespace pools {

    struct ThreadPoolOptions
    {
      size_t numberOfThreads;
    };


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


      void handleTasks();

      void done();

    private:

      ThreadPoolOptions options_;

      std::vector<std::thread> threads_;

      // static thread_local std::unique_ptr<thunder::datastructures::AtomicQueue<Task>> local_tasks_queue_ptr_;

      // thunder::datastructures::AtomicQueue<Task> pool_tasks_queue_;

      bool isRunning_ = true;


    };

  };


}