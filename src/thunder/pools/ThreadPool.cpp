


#include <thread>
#include <memory>
#include <future>


#include "thunder/datastructures/ConcurrentQueue.hpp"

#include "ThreadPool.hpp"
#include "Task.hpp"


namespace thunder {

  namespace pools {



    ThreadPool::ThreadPool(ThreadPoolOptions options) : options_(options_)
    {
      size_t numebrOfThreads = this->options_.numberOfThreads;
      this->threads_.reserve(numebrOfThreads);

      for (int i = 0; i < numebrOfThreads; i++) {
        this->threads_.push_back(
          std::thread(&ThreadPool::handleTasks, this)
        );
      }
      
    }

    ThreadPool::~ThreadPool()
    {
      this->done();
    }


    template<typename FuncType>
    std::future<
      typename std::result_of_t<FuncType()>
    > ThreadPool::submit(FuncType func)
    {
      typedef typename std::result_of_t<FuncType()> funcResultType;

      std::packaged_task<funcResultType()> task(func);

      std::future<funcResultType> result(task.get_future());

      // if (local_tasks_queue_ptr_ )
      // {
        // local_tasks_queue_ptr_->push(std::move(task));
      // }
      // else
      // {
        // pool_tasks_queue_.push(std::move(task));
      // }

      return result;
    }

    void ThreadPool::handleTasks()
    {

      // local_tasks_queue_ptr_.reset(new thunder::datastructures::AtomicQueue<Task>());

      while(this->isRunning_)
      {
        std::shared_ptr<Task> task = nullptr;
        // if (!local_tasks_queue_ptr_->isEmpty())
        // {
          // task = local_tasks_queue_ptr_->front();
          // local_tasks_queue_ptr_->pop(); //TODO: change all quques api
        // }
        // else 
        // {
          // task = pool_tasks_queue_.front();
          // pool_tasks_queue_.pop();
        // }

        // if (task != nullptr)
        // {
        //   (task.get())->operator()();
        // }
      }
      
    }

    void ThreadPool::done()
    {
      size_t numebrOfThreads = this->options_.numberOfThreads;
      
      this->isRunning_ = false;
      for (int i = 0; i < numebrOfThreads; i++) {
        this->threads_.at(i).join();
      }
    }
  };
}