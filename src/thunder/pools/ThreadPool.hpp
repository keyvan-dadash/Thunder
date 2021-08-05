
#pragma once


#include <memory>
#include <thread>
#include <future>

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

      ThreadPool(ThreadPoolOptions options) : options_(options)
      {

      }

      ThreadPool(ThreadPool&& other) = delete;
      ThreadPool& operator=(ThreadPool&& rhs) = delete;

      
      ThreadPool(const ThreadPool& other) = delete;
      ThreadPool& operator=(const ThreadPool& rhs) = delete;

      ~ThreadPool()
      {

      }


      template<typename FuncType>
      std::future<
        typename std::result_of_t<FuncType()>
      > submit(FuncType func);


      void handleTasks();

    private:

      ThreadPoolOptions options_;

      thunder::datastructures::AtomicQueue<Task> tasks_;

      bool isRunning_;


    };

  };


}