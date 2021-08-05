


#include <thread>
#include <memory>
#include <future>


#include "ThreadPool.hpp"



namespace thunder {

  namespace pools {


    template<typename FuncType>
    std::future<
      typename std::result_of_t<FuncType()>
    > ThreadPool::submit(FuncType func)
    {

    }

    void ThreadPool::handleTasks()
    {

      while(this->isRunning_)
      {
        
      }

      
    }







  };
}