

#include <string>


#include <gtest/gtest.h>

#include <thunder/datastructures/Queue.hpp>

TEST(QueueBasicOperations, CreateQueue) {
    EXPECT_NO_THROW({
        thunder::datastructures::Queue<int> queue;
    });
    EXPECT_NO_THROW({
        thunder::datastructures::Queue<char> queue;
    });
    EXPECT_NO_THROW({
        thunder::datastructures::Queue<float> queue;
    });
    EXPECT_NO_THROW({
        thunder::datastructures::Queue<std::string> queue;
    });
}

TEST(QueueBasicOperations, EmptyQueue) {
    {
        thunder::datastructures::Queue<int> queue;
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::Queue<char> queue;
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::Queue<float> queue;
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::Queue<std::string> queue;
        EXPECT_EQ(queue.isEmpty(), true);
    }
}

TEST(QueueBasicOperations, PushElementToQueue) {
    {
        thunder::datastructures::Queue<int> queue;
        EXPECT_EQ(queue.push(5), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::Queue<char> queue;
        EXPECT_EQ(queue.push('c'), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::Queue<float> queue;
        EXPECT_EQ(queue.push(5.1), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::Queue<std::string> queue;
        EXPECT_EQ(queue.push("foo"), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
        EXPECT_EQ(queue.push(std::string("bar")), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
}

TEST(QueueBasicOperations, TryPushElementToQueue) {
    {
        thunder::datastructures::Queue<int> queue;
        EXPECT_EQ(queue.tryPush(5, 10), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::Queue<char> queue;
        EXPECT_EQ(queue.tryPush('c', 10), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::Queue<float> queue;
        EXPECT_EQ(queue.tryPush(5.1, 10), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::Queue<std::string> queue;
        EXPECT_EQ(queue.tryPush("foo", 10), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
        EXPECT_EQ(queue.tryPush(std::string("bar"), 10), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
}

TEST(QueueBasicOperations, GetFrontElement) {
    {
        thunder::datastructures::Queue<int> queue;
        queue.push(5);
        EXPECT_EQ(queue.front(), 5);
    }
    {
        thunder::datastructures::Queue<char> queue;
        queue.push('c');
        EXPECT_EQ(queue.front(), 'c');
    }
    {
        thunder::datastructures::Queue<float> queue;
        queue.push((float)5.1);
        EXPECT_EQ(queue.front(), (float)5.1);
    }
    {
        thunder::datastructures::Queue<std::string> queue;
        queue.push("foo");
        EXPECT_EQ(queue.front(), "foo");
        queue.push(std::string("bar"));
        EXPECT_EQ(queue.front(), "foo");
    }
}

TEST(QueueBasicOperations, PopElementFromQueue) {
    {
        thunder::datastructures::Queue<int> queue;
        queue.push(5);
        EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::Queue<char> queue;
        queue.push('c');
        EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::Queue<float> queue;
        queue.push(5.1);
        EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::Queue<std::string> queue;
        queue.push("foo");
        queue.push(std::string("bar"));
        EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
        EXPECT_EQ(queue.isEmpty(), false);
        EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
        EXPECT_EQ(queue.isEmpty(), true);
    }
}