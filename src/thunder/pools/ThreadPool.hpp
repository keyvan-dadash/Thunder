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

class ThreadUtils {
 public:

   static bool setScheduling(std::thread &th, int policy, int priority) {
     sched_param sch_params;
     sch_params.sched_priority = priority;
     if (pthread_setschedparam(th.native_handle(), policy, &sch_params)) {
       return false;
     }

     return true;
   }

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

struct ThreadOptions {
  std::string thread_name;
  bool set_sche = false;
  int policy;
  int priority;
  bool set_cpu_affinity = false;
  std::vector<int> cpu_ids;
};

struct ThreadPoolOptions {
  size_t numberOfThreads;
  std::vector<ThreadOptions> threads_option;
};

template <size_t ThreadPoolQueueSize = 1024, size_t ThreadQueueSize = 512>
class ThreadPool {
 public:
  ThreadPool(ThreadPoolOptions options);

  ThreadPool(ThreadPool&& other) = delete;
  ThreadPool& operator=(ThreadPool&& rhs) = delete;

  ThreadPool(const ThreadPool& other) = delete;
  ThreadPool& operator=(const ThreadPool& rhs) = delete;

  ~ThreadPool();

  template <typename FuncType>
  std::future<typename std::result_of_t<FuncType()>> submit(FuncType func);

  void handleTasks(size_t queue_index);

  void done();

 private:
  bool popTaskFromLocalQueue(Task& task);

  bool popTaskFromOtherThreads(Task& task);

  bool popTaskFromGlobalQueue(Task& task);

  ThreadPoolOptions options_;

  std::vector<std::thread> threads_;

  std::vector<std::unique_ptr<
      thunder::datastructures::ConcurrentQueue<Task, ThreadQueueSize>>>
      thread_queues_;

  static thread_local thunder::datastructures::ConcurrentQueue<
      Task, ThreadQueueSize>* local_tasks_queue_ptr_;

  static thread_local size_t thread_queue_index_;

  thunder::datastructures::ConcurrentQueue<Task, ThreadPoolQueueSize>
      pool_tasks_queue_;

  bool isRunning_ = true;
};
};  // namespace pools
}  // namespace thunder

#include <thunder/pools/ThreadPool.cpp>
