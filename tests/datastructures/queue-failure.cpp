

#include <string>


#include <gtest/gtest.h>

#include <thunder/datastructures/Queue.hpp>

TEST(QueueFailureOperations, EmptyQueue) {
    {
        thunder::datastructures::Queue<int> queue;
        queue.push(2);
        EXPECT_FALSE(queue.isEmpty());
    }
    {
        thunder::datastructures::Queue<char> queue;
        queue.push('c');
        EXPECT_FALSE(queue.isEmpty());
    }
    {
        thunder::datastructures::Queue<float> queue;
        queue.push((float)5.1);
        EXPECT_FALSE(queue.isEmpty());
    }
    {
        thunder::datastructures::Queue<std::string> queue;
        queue.push("test");
        EXPECT_FALSE(queue.isEmpty());
    }
}

TEST(QueueFailureOperations, TryPushElementToQueue) {
    {
        thunder::datastructures::Queue<int> queue;
        queue.push(6);
        EXPECT_EQ(queue.tryPush(5, 1), thunder::datastructures::QueueOperationStatus::CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE);
    }
    {
        thunder::datastructures::Queue<char> queue;
        queue.push('r');
        EXPECT_EQ(queue.tryPush('c', 1), thunder::datastructures::QueueOperationStatus::CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE);
    }
    {
        thunder::datastructures::Queue<float> queue;
        queue.push((float)6.2);
        queue.push((float)7.2);
        queue.push((float)8.2);
        EXPECT_EQ(queue.tryPush(5.1, 2), thunder::datastructures::QueueOperationStatus::CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE);
    }
    {
        thunder::datastructures::Queue<std::string> queue;
        queue.push("test");
        EXPECT_EQ(queue.tryPush("foo", 2), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
        EXPECT_EQ(queue.tryPush(std::string("bar"), 2), thunder::datastructures::QueueOperationStatus::CANNOT_INSERT_ELEMENT_QUEUE_SIZE_REACHED_TO_MAX_SIZE);
    }
}

TEST(QueueFailureOperations, PopElementFromQueue) {
    {
        thunder::datastructures::Queue<int> queue;
        int front;
        EXPECT_EQ(queue.pop(front), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
    }
    {
        thunder::datastructures::Queue<char> queue;
        char front;
        EXPECT_EQ(queue.pop(front), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
    }
    {
        thunder::datastructures::Queue<float> queue;
        float front;
        EXPECT_EQ(queue.pop(front), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
    }
    {
        thunder::datastructures::Queue<std::string> queue;
        std::string front;
        EXPECT_EQ(queue.pop(front), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
    }
}
