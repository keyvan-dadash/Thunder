
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
#include <random>

#include <gtest/gtest.h>

#include <thunder/datastructures/AtomicQueue.hpp>
#include <thunder/datastructures/Queue.hpp>

template<typename... Args, typename... FuncArgs>
std::vector<std::thread*>
createNumberOfThreadAndDoOperation(int numberOfThreads, std::function<void(Args ...args)> operation, FuncArgs ...funcargs)
{
    std::vector<std::thread*> threads;
    for (int i = 0; i < numberOfThreads; i++) {
        std::thread* thread = new std::thread(operation, funcargs...);
        threads.push_back(thread);
    }
    
    return threads;
}

TEST(AtomicQueueFailureOperations, MultiThreadedQueueSizeCheck)
{
    {
        thunder::datastructures::AtomicQueue<int> queue;

        std::function<void(int)> operation = [&](int element) {
            EXPECT_EQ(queue.push(element), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
        };

        auto threads = createNumberOfThreadAndDoOperation(std::thread::hardware_concurrency(), operation, 3);
        
        for (int i = 0; i < threads.size(); i++) {
            (*(threads.at(i))).join();
        }

        EXPECT_EQ(queue.getSizeOfQueue(), (int)std::thread::hardware_concurrency());
        
    }
    {
        thunder::datastructures::AtomicQueue<char> queue;

        std::function<void(char)> operation = [&](char element) {
             EXPECT_EQ(queue.push(element), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
        };

        auto threads = createNumberOfThreadAndDoOperation(std::thread::hardware_concurrency(), operation, 'c');
        
        for (int i = 0; i < threads.size(); i++) {
            (*(threads.at(i))).join();
        }

        EXPECT_EQ(queue.getSizeOfQueue(), (int)std::thread::hardware_concurrency());
    }
    {
        thunder::datastructures::AtomicQueue<float> queue;

        std::function<void(float)> operation = [&](float element) {
             EXPECT_EQ(queue.push(element), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
        };

        auto threads = createNumberOfThreadAndDoOperation(std::thread::hardware_concurrency(), operation, (float)5.1);
        
        for (int i = 0; i < threads.size(); i++) {
            (*(threads.at(i))).join();
        }

        EXPECT_EQ(queue.getSizeOfQueue(), (int)std::thread::hardware_concurrency());
    }
    {
        thunder::datastructures::AtomicQueue<std::string> queue;

        std::function<void(std::string)> operation = [&](std::string element) {
             EXPECT_EQ(queue.push(element), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
        };

        auto threads = createNumberOfThreadAndDoOperation(std::thread::hardware_concurrency(), operation, "foo");
        
        for (int i = 0; i < threads.size(); i++) {
            (*(threads.at(i))).join();
        }

        EXPECT_EQ(queue.getSizeOfQueue(), (int)std::thread::hardware_concurrency());
    }
}

TEST(AtomicQueueFailureOperations, HugeMultiThreadedQueueSizeCheck)
{
    {
        thunder::datastructures::AtomicQueue<int> queue;

        std::function<void(void)> operation = [&]() {
            for (int i = 0; i < 1000; i++) {
                queue.push(i);
            }
        };

        auto threads = createNumberOfThreadAndDoOperation(std::thread::hardware_concurrency(), operation);
        
        for (int i = 0; i < threads.size(); i++) {
            (*(threads.at(i))).join();
        }

        EXPECT_EQ(queue.getSizeOfQueue(), (int)std::thread::hardware_concurrency() * 1000);
        
    }
    {
        thunder::datastructures::AtomicQueue<char> queue;

        std::function<void(void)> operation = [&]() {
            for (int i = 0; i < 1000; i++) {
                queue.push('t');
            }
        };

        auto threads = createNumberOfThreadAndDoOperation(std::thread::hardware_concurrency(), operation);
        
        for (int i = 0; i < threads.size(); i++) {
            (*(threads.at(i))).join();
        }

        EXPECT_EQ(queue.getSizeOfQueue(), (int)std::thread::hardware_concurrency() * 1000);
    }
    {
        thunder::datastructures::AtomicQueue<float> queue;

        std::function<void(void)> operation = [&]() {
            for (int i = 0; i < 1000; i++) {
                queue.push((float)i * 1.4);
            }
        };

        auto threads = createNumberOfThreadAndDoOperation(std::thread::hardware_concurrency(), operation);
        
        for (int i = 0; i < threads.size(); i++) {
            (*(threads.at(i))).join();
        }

        EXPECT_EQ(queue.getSizeOfQueue(), (int)std::thread::hardware_concurrency() * 1000);
    }
    {
        thunder::datastructures::AtomicQueue<std::string> queue;

        std::function<void(void)> operation = [&]() {
            for (int i = 0; i < 1000; i++) {
                queue.push(std::string("test" + std::to_string(i)));
            }
        };

        auto threads = createNumberOfThreadAndDoOperation(std::thread::hardware_concurrency(), operation);
        
        for (int i = 0; i < threads.size(); i++) {
            (*(threads.at(i))).join();
        }

        EXPECT_EQ(queue.getSizeOfQueue(), (int)std::thread::hardware_concurrency() * 1000);
    }
}