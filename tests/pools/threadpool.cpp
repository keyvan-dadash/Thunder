#include <string>
#include <thread>
#include <chrono>

#include <gtest/gtest.h>

#include <thunder/pools/ThreadPool.hpp>

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
  for (int i = 0; i < options.numberOfThreads - 1; i++)
  {
    thread_pool.submit([&] () {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        test = 12;
    });
  }

  auto result = thread_pool.submit([&] () {
      test += 10;
  });

  result.get();
  EXPECT_EQ(test, 20);
  thread_pool.done();
}
