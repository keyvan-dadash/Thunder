#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <benchmark/benchmark.h>

#include <thunder/datastructures/ConcurrentQueue.hpp>

template <typename T>
class Queue
{
 public:
 
  T pop()
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
    auto item = queue_.front();
    queue_.pop();
    return item;
  }
 
  void pop(T& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
    item = queue_.front();
    queue_.pop();
  }
 
  void push(const T& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(item);
    mlock.unlock();
    cond_.notify_one();
  }
 
  void push(T&& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(std::move(item));
    mlock.unlock();
    cond_.notify_one();
  }
 
 private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

typedef struct 
{
  size_t counter;
} task_t;

static void BE_ConcurrentQueue(benchmark::State& state) {
    // state.PauseTiming();

    for (auto _ : state) {
    thunder::datastructures::ConcurrentQueue<task_t, 8000> queue;

    std::vector<std::thread> threads;

    int total = 0;

    int count_per_thread = 1000;
    // state.ResumeTiming();
    for (size_t i = 0; i < 8; i++) {
        
        std::thread t1([&](){

            for (size_t j = 0; j < count_per_thread; j++)
            {
                queue.forcePush(task_t{j});
            }
        });

        threads.push_back(std::move(t1));
    }

    for (size_t i = 0; i < 8; i++) {
        
        std::thread t1([&](){

            for (size_t j = 0; j < count_per_thread; j++)
            {
              task_t task;
              auto status = queue.pop(task);
              if (status == 
                  thunder::datastructures::ConcurrentQueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY)
              {
                j--;
              }
            }
            
        });

        threads.push_back(std::move(t1));

    }
    for (int i = 0; i < 16; i++) {
        threads.at(i).join();
    }
    }
}
BENCHMARK(BE_ConcurrentQueue)->Range(8, 8<<16)->UseRealTime();

static void BE_QueueMutex(benchmark::State& state) {
    // state.PauseTiming();

    for (auto _ : state) {
    Queue<task_t> queue;

    std::vector<std::thread> threads;

    int total = 0;

    int count_per_thread = 1000;
    std::mutex m;
    // state.ResumeTiming();
    for (size_t i = 0; i < 8; i++) {
        
        std::thread t1([&](){

            for (size_t j = 0; j < count_per_thread; j++)
            {
                queue.push(task_t{j});
            }
        });

        threads.push_back(std::move(t1));
    }

    for (size_t i = 0; i < 8; i++) {
        
        std::thread t1([&](){

            for (size_t j = 0; j < count_per_thread; j++)
            {
              task_t task;
              queue.pop(task);
            }
        });

        threads.push_back(std::move(t1));
    }
    for (int i = 0; i < 16; i++) {
        threads.at(i).join();
    }
    }
}
BENCHMARK(BE_QueueMutex)->Range(8, 8<<16)->UseRealTime();

BENCHMARK_MAIN();
