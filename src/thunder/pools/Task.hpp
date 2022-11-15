/**
 * @file Task.hpp
 * Contains Task which will be use in area when we want execute and wrapper certain function.
 */
#pragma once

#include <memory>

namespace thunder {

namespace pools {

/**
 * \brief Task class wrapper and execute given function.
 */
class Task {
 public:
  Task() = default;

  /**
   * \brief Constructor of Task class.
   * \tparam FuncType is the type of function which will be wrapped.
   * \param func is the actual function to execute.
   */
  template <typename FuncType>
  Task(FuncType&& func) : Impl_(new Impl_type<FuncType>(std::move(func))) {}

  /**
   * \brief Move constructor.
   * \param other Task object.
   */
  Task(Task&& other) noexcept : Impl_(std::move(other.Impl_)) {}

  /**
   * \brief Move operator.
   * \param other Task object.
   */
  Task* operator=(Task&& rhs) noexcept {
    Impl_.swap(rhs.Impl_);

    return this;
  }

  Task(const Task& other) = delete;
  Task& operator=(const Task& rhs) = delete;

  void operator()() { Impl_->run(); }

 private:
  /**
   * \brief Impl_base is a abstract struct which specify what apis should function executor expose.
   */
  struct Impl_base {
    /**
     * \brief run the given function.
     */
    virtual void run() = 0;
    virtual ~Impl_base() {}
  };

  /**
   * \brief Impl_type is the struct which Implement Impl_base and execute function.
   * \tparam Func is the type of function.
   */
  template <typename Func>
  struct Impl_type : Impl_base {
    Func func; /// func attribute.

    /**
     * \brief Constructor of Impl_type.
     * \param func is the function to execute.
     */
    Impl_type(Func&& func) : func(std::move(func)) {}

    /**
     * \brief run the given function.
     */
    void run() { func(); }
  };

  std::unique_ptr<Impl_base> Impl_; /// Impl_ is the attribute which holds function executor.
};
};  // namespace pools
}  // namespace thunder
