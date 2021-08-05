
#pragma once

#include <memory>

namespace thunder {


  namespace pools {

    class Task 
    {
    public:

      Task() = default;

      template<typename FuncType>
      Task(FuncType&& func) : Impl_(new Impl_type<FuncType>(std::move(func)))
      {
        
      }

      Task(Task&& other) : Impl_(std::move(other.Impl_))
      {
        
      }

      Task* operator=(Task&& rhs)
      {
        this->Impl_ = std::exchange(rhs.Impl_, this->Impl_);
      }


      Task(const Task& other) = delete;
      Task& operator=(const Task& rhs) = delete;

      void operator()()
      {
        this->Impl_->run();
      }


    private:

      struct Impl_base {
        virtual void run() = 0;
        virtual ~Impl_base() {}
      };

      template<typename Func>
      struct Impl_type {
        Func func;

        Impl_type(Func&& func) : func(std::move(func));
         {}

        void run() override
        {
          func();
        }
      };

      std::unique_ptr<Impl_base> Impl_;
    };

  };
}