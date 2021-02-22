

#include <string>


#include <gtest/gtest.h>

#include <thunder/datastructures/AtomicQueue.hpp>

TEST(AtomicQueueBasicOperations, CreateQueue) {
    EXPECT_NO_THROW({
        thunder::datastructures::AtomicQueue<int> queue;
    });
    EXPECT_NO_THROW({
        thunder::datastructures::AtomicQueue<char> queue;
    });
    EXPECT_NO_THROW({
        thunder::datastructures::AtomicQueue<float> queue;
    });
    EXPECT_NO_THROW({
        thunder::datastructures::AtomicQueue<std::string> queue;
    });
}

TEST(AtomicQueueBasicOperations, EmptyQueue) {
    {
        thunder::datastructures::AtomicQueue<int> queue;
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::AtomicQueue<char> queue;
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::AtomicQueue<float> queue;
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::AtomicQueue<std::string> queue;
        EXPECT_EQ(queue.isEmpty(), true);
    }
}

TEST(AtomicQueueBasicOperations, PushElementToQueue) {
    {
        thunder::datastructures::AtomicQueue<int> queue;
        EXPECT_EQ(queue.push(5), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::AtomicQueue<char> queue;
        EXPECT_EQ(queue.push('c'), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::AtomicQueue<float> queue;
        EXPECT_EQ(queue.push(5.1), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::AtomicQueue<std::string> queue;
        EXPECT_EQ(queue.push("foo"), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
        EXPECT_EQ(queue.push(std::string("bar")), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
}

TEST(AtomicQueueBasicOperations, TryPushElementToQueue) {
    {
        thunder::datastructures::AtomicQueue<int> queue;
        EXPECT_EQ(queue.tryPush(5, 10), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::AtomicQueue<char> queue;
        EXPECT_EQ(queue.tryPush('c', 10), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::AtomicQueue<float> queue;
        EXPECT_EQ(queue.tryPush(5.1, 10), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::AtomicQueue<std::string> queue;
        EXPECT_EQ(queue.tryPush("foo", 10), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
        EXPECT_EQ(queue.tryPush(std::string("bar"), 10), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
}

TEST(AtomicQueueBasicOperations, GetFrontElement) {
    {
        thunder::datastructures::AtomicQueue<int> queue;
        queue.push(5);
        EXPECT_EQ(*queue.front(), 5);
    }
    {
        thunder::datastructures::AtomicQueue<char> queue;
        queue.push('c');
        EXPECT_EQ(*queue.front(), 'c');
    }
    {
        thunder::datastructures::AtomicQueue<float> queue;
        queue.push((float)5.1);
        EXPECT_EQ(*queue.front(), (float)5.1);
    }
    {
        thunder::datastructures::AtomicQueue<std::string> queue;
        queue.push("foo");
        EXPECT_EQ(*queue.front(), "foo");
        queue.push(std::string("bar"));
        EXPECT_EQ(*queue.front(), "foo");
    }
}

TEST(AtomicQueueBasicOperations, PopElementFromQueue) {
    {
        thunder::datastructures::AtomicQueue<int> queue;
        queue.push(5);
        EXPECT_EQ(queue.pop(), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::AtomicQueue<char> queue;
        queue.push('c');
        EXPECT_EQ(queue.pop(), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::AtomicQueue<float> queue;
        queue.push(5.1);
        EXPECT_EQ(queue.pop(), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::AtomicQueue<std::string> queue;
        queue.push("foo");
        queue.push(std::string("bar"));
        EXPECT_EQ(queue.pop(), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
        EXPECT_EQ(queue.isEmpty(), false);
        EXPECT_EQ(queue.pop(), thunder::datastructures::AtomicQueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
        EXPECT_EQ(queue.isEmpty(), true);
    }
}

TEST(AtomicQueueBasicOperations, PopElementFromQueueAndCheckFront) {
    {
        thunder::datastructures::AtomicQueue<int> queue;
        queue.push(5);
        queue.push(6);
        queue.push(7);
        queue.push(8);
        queue.push(9);

        EXPECT_EQ(*queue.front(), 5);
        queue.pop();
        EXPECT_EQ(*queue.front(), 6);
        queue.pop();
        EXPECT_EQ(*queue.front(), 7);
        queue.pop();
        EXPECT_EQ(*queue.front(), 8);
        queue.pop();
        EXPECT_EQ(*queue.front(), 9);
    }
    {
        thunder::datastructures::AtomicQueue<char> queue;
        queue.push('a');
        queue.push('b');
        queue.push('c');
        queue.push('d');
        queue.push('e');
        queue.push('f');

        EXPECT_EQ(*queue.front(), 'a');
        queue.pop();
        EXPECT_EQ(*queue.front(), 'b');
        queue.pop();
        EXPECT_EQ(*queue.front(), 'c');
        queue.pop();
        EXPECT_EQ(*queue.front(), 'd');
        queue.pop();
        EXPECT_EQ(*queue.front(), 'e');
        queue.pop();
        EXPECT_EQ(*queue.front(), 'f');
    }
    {
        thunder::datastructures::AtomicQueue<float> queue;
        queue.push(5.1);
        queue.push(5.2);
        queue.push(5.3);
        queue.push(5.4);
        queue.push(5.5);
        queue.push(5.6);

        EXPECT_EQ(*queue.front(), (float)5.1);
        queue.pop();
        EXPECT_EQ(*queue.front(), (float)5.2);
        queue.pop();
        EXPECT_EQ(*queue.front(), (float)5.3);
        queue.pop();
        EXPECT_EQ(*queue.front(), (float)5.4);
        queue.pop();
        EXPECT_EQ(*queue.front(), (float)5.5);
        queue.pop();
        EXPECT_EQ(*queue.front(), (float)5.6);
    }
    {
        thunder::datastructures::AtomicQueue<std::string> queue;
        queue.push("test1");
        queue.push("test2");
        queue.push("test3");
        queue.push("test4");
        queue.push("test5");
        queue.push("test6");

        EXPECT_EQ(*queue.front(), "test1");
        queue.pop();
        EXPECT_EQ(*queue.front(), "test2");
        queue.pop();
        EXPECT_EQ(*queue.front(), "test3");
        queue.pop();
        EXPECT_EQ(*queue.front(), "test4");
        queue.pop();
        EXPECT_EQ(*queue.front(), "test5");
        queue.pop();
        EXPECT_EQ(*queue.front(), "test6");
    }
}