
#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <string>
#include <thread>
#include <thunder/datastructures/ConcurrentQueue.hpp>
#include <thunder/datastructures/Queue.hpp>
#include <vector>

template <typename... Args, typename... FuncArgs>
std::vector<std::thread*> createNumberOfThreadAndDoOperation(
    int numberOfThreads, std::function<void(Args... args)> operation,
    FuncArgs... funcargs) {
  std::vector<std::thread*> threads;
  for (int i = 0; i < numberOfThreads; i++) {
    std::thread* thread = new std::thread(operation, funcargs...);
    threads.push_back(thread);
  }

  return threads;
}

TEST(ConcurrentQueueFailureOperations, MultiThreadedQueueSizeCheck) {
  {
    thunder::datastructures::ConcurrentQueue<int> queue;

    std::function<void(int)> operation = [&](int element) {
      EXPECT_EQ(queue.forcePush(element),
                thunder::datastructures::ConcurrentQueueOperationStatus::
                    ELEMENT_PUSHED_SUCCESSFULLY);
    };

    auto threads = createNumberOfThreadAndDoOperation(
        std::thread::hardware_concurrency(), operation, 3);

    for (int i = 0; i < threads.size(); i++) {
      (*(threads.at(i))).join();
    }

    EXPECT_EQ(queue.getSizeOfQueue(), (int)std::thread::hardware_concurrency());
  }
  {
    thunder::datastructures::ConcurrentQueue<char> queue;

    std::function<void(char)> operation = [&](char element) {
      EXPECT_EQ(queue.forcePush(element),
                thunder::datastructures::ConcurrentQueueOperationStatus::
                    ELEMENT_PUSHED_SUCCESSFULLY);
    };

    auto threads = createNumberOfThreadAndDoOperation(
        std::thread::hardware_concurrency(), operation, 'c');

    for (int i = 0; i < threads.size(); i++) {
      (*(threads.at(i))).join();
    }

    EXPECT_EQ(queue.getSizeOfQueue(), (int)std::thread::hardware_concurrency());
  }
  {
    thunder::datastructures::ConcurrentQueue<float> queue;

    std::function<void(float)> operation = [&](float element) {
      EXPECT_EQ(queue.forcePush(element),
                thunder::datastructures::ConcurrentQueueOperationStatus::
                    ELEMENT_PUSHED_SUCCESSFULLY);
    };

    auto threads = createNumberOfThreadAndDoOperation(
        std::thread::hardware_concurrency(), operation, (float)5.1);

    for (int i = 0; i < threads.size(); i++) {
      (*(threads.at(i))).join();
    }

    EXPECT_EQ(queue.getSizeOfQueue(), (int)std::thread::hardware_concurrency());
  }
  {
    thunder::datastructures::ConcurrentQueue<std::string> queue;

    std::function<void(std::string)> operation = [&](std::string element) {
      EXPECT_EQ(queue.forcePush(element),
                thunder::datastructures::ConcurrentQueueOperationStatus::
                    ELEMENT_PUSHED_SUCCESSFULLY);
    };

    auto threads = createNumberOfThreadAndDoOperation(
        std::thread::hardware_concurrency(), operation, "foo");

    for (int i = 0; i < threads.size(); i++) {
      (*(threads.at(i))).join();
    }

    EXPECT_EQ(queue.getSizeOfQueue(), (int)std::thread::hardware_concurrency());
  }
}

TEST(ConcurrentQueueFailureOperations, HugeMultiThreadedQueueSizeCheck) {
  static constexpr int kNumberOfCpus = 16;
  static constexpr std::size_t kQueueSize = (std::size_t)(kNumberOfCpus * 1000);
  {
    thunder::datastructures::ConcurrentQueue<int, kQueueSize> queue;

    std::function<void(void)> operation = [&]() {
      for (int i = 0; i < 1000; i++) {
        queue.forcePush(i);
      }
    };

    auto threads = createNumberOfThreadAndDoOperation(kNumberOfCpus, operation);

    for (int i = 0; i < threads.size(); i++) {
      (*(threads.at(i))).join();
    }

    EXPECT_EQ(queue.getSizeOfQueue(), kQueueSize);
  }
  {
    thunder::datastructures::ConcurrentQueue<char, kQueueSize> queue;

    std::function<void(void)> operation = [&]() {
      for (int i = 0; i < 1000; i++) {
        queue.forcePush('t');
      }
    };

    auto threads = createNumberOfThreadAndDoOperation(kNumberOfCpus, operation);

    for (int i = 0; i < threads.size(); i++) {
      (*(threads.at(i))).join();
    }

    EXPECT_EQ(queue.getSizeOfQueue(), kQueueSize);
  }
  {
    thunder::datastructures::ConcurrentQueue<float, kQueueSize> queue;

    std::function<void(void)> operation = [&]() {
      for (int i = 0; i < 1000; i++) {
        queue.forcePush((float)i * 1.4);
      }
    };

    auto threads = createNumberOfThreadAndDoOperation(kNumberOfCpus, operation);

    for (int i = 0; i < threads.size(); i++) {
      (*(threads.at(i))).join();
    }

    EXPECT_EQ(queue.getSizeOfQueue(), kQueueSize);
  }
  {
    thunder::datastructures::ConcurrentQueue<std::string, kQueueSize> queue;

    std::function<void(void)> operation = [&]() {
      for (int i = 0; i < 1000; i++) {
        queue.forcePush(std::string("test" + std::to_string(i)));
      }
    };

    auto threads = createNumberOfThreadAndDoOperation(kNumberOfCpus, operation);

    for (int i = 0; i < threads.size(); i++) {
      (*(threads.at(i))).join();
    }

    EXPECT_EQ(queue.getSizeOfQueue(), kQueueSize);
  }
}