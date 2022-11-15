/**
 * \brief ThreadPool.hpp
 * Contains ThreadPool class which help execute tasks in better order.
 */
#pragma once

#include <future>
#include <memory>
#include <thread>
#include <vector>
#include <pthread.h>

#include "Task.hpp"
#include "thunder/datastructures/ConcurrentQueue.hpp"

namespace thunder {

namespace pools {

/**
 * \brief ThreadUtils class is the collection of utils for setting specific settings on threads.
 */
class ThreadUtils {
 public:
   /**
    * \brief setScheduling is the function which set policy and priority on the given thread.
    * \param th is the std::thread.
    * \param policy is the desire scheduling policy of the given thread.
    * \param priority is the desire priority of the given thread.
    * 'return boolean which represent if the operation successful or not.
    */
   static bool setScheduling(std::thread &th, int policy, int priority) {
     sched_param sch_params;
     sch_params.sched_priority = priority;
     if (pthread_setschedparam(th.native_handle(), policy, &sch_params)) {
       return false;
     }

     return true;
   }

   /**
    * \brief setCPUAffinity is the function which set CPU affinity on the given thread.
    * \param th is the std::thread.
    * \param cpu_ids is the std::vector which contains CPUs id that the given thread can schedule on.
    * 'return boolean which represent if the operation successful or not.
    */
   static bool setCPUAffinity(std::thread &th, std::vector<int> &cpu_ids) {
     cpu_set_t cpuset;
     CPU_ZERO(&cpuset);
     for (auto& x : cpu_ids) {
       CPU_SET(x, &cpuset);
     }
     if (pthread_setaffinity_np(th.native_handle(), sizeof(cpu_set_t), &cpuset)) {
       return false;
     }

     return true;
   }
};

/**
 * \brief ThreadOptions is the single thread option.
 */
struct ThreadOptions {
  std::string thread_name; /// name for the thread.
  bool set_sche = false; /// should set specific schedule option.
  int policy; /// policy scheduling of the thread.
  int priority; /// priority of the thread.
  bool set_cpu_affinity = false; /// should set specific CPU affinity option.
  std::vector<int> cpu_ids; /// CPU ids that thread allow to schedule on.
};

/**
 * \brief ThreadPoolOptions is the class which contains options for startup the ThreadPool.
 */
struct ThreadPoolOptions {
  size_t numberOfThreads; /// Number of threads that the ThreadPool should release.
  std::vector<ThreadOptions> threads_option; /// vector of the threads with specific option.
};

/**
 * \brief ThreadPool is the class which help to organize tasks on the diffrent threads.
 * \tparam ThreadPoolQueueSize is the size of global task queue in the ThreadPool.
 * \tparam ThreadQueueSize is the size of local task queue in the each Thread.
 *
 * ThreadPool class is the utility which help to spawn multiple threads and organize
 * too many tasks in a way that conection reduce to minimum and achieve best performance, and
 * also provide rich and easy API's for developers to controll their multithread software.
 */
template <size_t ThreadPoolQueueSize = 1024, size_t ThreadQueueSize = 512>
class ThreadPool {
 public:
  /**
   * \brief Constructor of ThreadPool.
   * \param options is the ThreadPoolOptions which contains options for starting ThreadPool.
   */
  ThreadPool(ThreadPoolOptions options);

  ThreadPool(ThreadPool&& other) = delete;
  ThreadPool& operator=(ThreadPool&& rhs) = delete;

  ThreadPool(const ThreadPool& other) = delete;
  ThreadPool& operator=(const ThreadPool& rhs) = delete;

  ~ThreadPool();

  /**
   * \brief submit task to ThreadPool.
   * \tparam FuncType is the type of function.
   * \param func is the actual function which execute in the ThreadPool.
   * \return std::future of the function return type.
   */
  template <typename FuncType>
  std::future<typename std::result_of_t<FuncType()>> submit(FuncType func);

  /**
   * \brief stop all threads and wait for running threads to stop.
   */
  void done();

 private:
  /**
   * \brief handleTasks is the function which each thread of ThreadPool execute.
   * \param queue_index of the thread_queues in order to store in the local storage of the thread.
   */
  void handleTasks(size_t queue_index);
  
  /**
   * \brief popTaskFromLocalQueue tryies to pop task from local storage of the thread
   * if the thread is part of the ThreadPool.
   * \param task which store poped task from local queue.
   */
  bool popTaskFromLocalQueue(Task& task);

  /**
   * \brief popTaskFromOtherThreads tryies to steal task from other threads local queue.
   * \param task which store poped task from other threads queue.
   */
  bool popTaskFromOtherThreads(Task& task);

  /**
   * \brief popTaskFromGlobalQueue tryies to pop task from global task queue.
   * \param task which store poped task from global queue.
   */
  bool popTaskFromGlobalQueue(Task& task);

  ThreadPoolOptions options_; /// options for the ThreadPool.

  std::vector<std::thread> threads_; /// List of the threads in the ThreadPool.

  std::vector<std::unique_ptr<
      thunder::datastructures::ConcurrentQueue<Task, ThreadQueueSize>>>
      thread_queues_; /// List of all threads local task queue.

  static thread_local thunder::datastructures::ConcurrentQueue<
      Task, ThreadQueueSize>* local_tasks_queue_ptr_; /// Thread local task queue.

  static thread_local size_t thread_queue_index_; /// Thread queue index.

  thunder::datastructures::ConcurrentQueue<Task, ThreadPoolQueueSize>
      pool_tasks_queue_; /// Global task queue.

  bool isRunning_ = true; /// Status of the ThreadPool.
};
};  // namespace pools
}  // namespace thunder

#include <thunder/pools/ThreadPool.cpp>
