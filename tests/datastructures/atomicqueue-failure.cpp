
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
#include <random>

#include <gtest/gtest.h>

#include <thunder/datastructures/AtomicQueue.hpp>

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
            queue.push(element);
        };

        auto threads = createNumberOfThreadAndDoOperation(std::thread::hardware_concurrency(), operation, 3);
        
        for (int i = 0; i < threads.size(); i++) {
            (*(threads.at(i))).join();
        }

        EXPECT_EQ(queue.getSizeOfQueue(), (int)std::thread::hardware_concurrency());
        
    }
    {
        thunder::datastructures::AtomicQueue<char> queue;
        queue.push('c');
        EXPECT_FALSE(queue.isEmpty());
    }
    {
        thunder::datastructures::AtomicQueue<float> queue;
        queue.push((float)5.1);
        EXPECT_FALSE(queue.isEmpty());
    }
    {
        thunder::datastructures::AtomicQueue<std::string> queue;
        queue.push("test");
        EXPECT_FALSE(queue.isEmpty());
    }
}