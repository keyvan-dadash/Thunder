#include <gtest/gtest.h>

#include <chrono>
#include <string>
#include <thread>
#include <thunder/pools/ThreadPool.hpp>
#include <pthread.h>
#include <vector>

TEST(BasicThreadPool, CreateThreadPool) {
  thunder::pools::ThreadPoolOptions options;
  options.numberOfThreads = std::thread::hardware_concurrency();
  thunder::pools::ThreadPool thread_pool(options);
}

TEST(BasicThreadPool, SubmitWorkInThreadPool) {
  thunder::pools::ThreadPoolOptions options;
  options.numberOfThreads = std::thread::hardware_concurrency();
  thunder::pools::ThreadPool thread_pool(options);
  int test = 10;
  // Block all threads except ones to get result
  for (int i = 0; i < options.numberOfThreads - 1; i++) {
    thread_pool.submit([&]() {
      std::this_thread::sleep_for(std::chrono::seconds(2));
      test = 12;
    });
  }

  auto result = thread_pool.submit([&]() { test += 10; });

  result.get();
  EXPECT_EQ(test, 20);
  thread_pool.done();
}

TEST(BasicThreadPool, SetAffinityThreadAndScheduling) {
  thunder::pools::ThreadPoolOptions options;
  options.numberOfThreads = std::thread::hardware_concurrency();
  for (int i = 0; i < options.numberOfThreads; i++) {
    options.threads_option.push_back(thunder::pools::ThreadOptions {
      .set_sche = true,
      .policy = SCHED_FIFO,
      .priority = 99,
      .set_cpu_affinity = true,
      .cpu_ids = std::vector<int>({i})
    });
  }
  thunder::pools::ThreadPool thread_pool(options);

  for (int i = 0; i < options.numberOfThreads; i++) {
    thread_pool.submit([]() {
        for (long i = 0; i < 0xFFFFFF; i++)
          std::this_thread::yield();
    });
  }
  thread_pool.done();
}

TEST(BasicThreadPool, TestTooManyTasks) {
  thunder::pools::ThreadPoolOptions options;
  options.numberOfThreads = std::thread::hardware_concurrency();
  for (int i = 0; i < options.numberOfThreads; i++) {
    options.threads_option.push_back(thunder::pools::ThreadOptions {
      .set_sche = true,
      .policy = SCHED_FIFO,
      .priority = 99,
      .set_cpu_affinity = true,
      .cpu_ids = std::vector<int>({i})
    });
  }
  thunder::pools::ThreadPool thread_pool(options);

  std::vector<std::future<std::pair<int, std::thread::id>>> results;
  for (int i = 0; i < 1000; i++) {
    auto result = thread_pool.submit([]() -> std::pair<int, std::thread::id> {
        int oper = 1;
        int total = 0;
        for (int i = 0; i < 10000000; i++) {
          total += i * oper;
          oper *= -1;
        }

        return std::pair<int, std::thread::id>(total, std::this_thread::get_id());
    });

    results.push_back(std::move(result));
  }

  long total = 0;
  std::map<std::thread::id, int> tasks_done_per_thread;
  for (auto& result : results) {
    auto sum_thread_id = result.get();
    total += std::get<0>(sum_thread_id);
    auto thread_id = std::get<1>(sum_thread_id);

    auto it = tasks_done_per_thread.find(thread_id);
    if (it == tasks_done_per_thread.end())
      tasks_done_per_thread[thread_id] = 0;

    tasks_done_per_thread[thread_id] = tasks_done_per_thread[thread_id] + 1;
  }
  long expect = (long)5000000 * (long)1000 * -1;
  EXPECT_EQ(total, expect);

  for (auto const& task_per_thread : tasks_done_per_thread) {
    std::cout << "Thread with ID " << task_per_thread.first << " done " << task_per_thread.second << " tasks" << std::endl;
  }
  thread_pool.done();
}
