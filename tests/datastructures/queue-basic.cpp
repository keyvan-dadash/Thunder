

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
        int c;
        queue.front(c);
        EXPECT_EQ(c, 5);
    }
    {
        thunder::datastructures::Queue<char> queue;
        queue.push('c');
        char c;
        queue.front(c);
        EXPECT_EQ(c, 'c');
    }
    {
        thunder::datastructures::Queue<float> queue;
        queue.push((float)5.1);
        float c;
        queue.front(c);
        EXPECT_EQ(c, (float)5.1);
    }
    {
        thunder::datastructures::Queue<std::string> queue;
        queue.push("foo");
        std::string front;
        queue.front(front);
        EXPECT_EQ(front, "foo");

        queue.push(std::string("bar"));
        queue.front(front);
        EXPECT_EQ(front, "foo");
    }
}

TEST(QueueBasicOperations, PopElementFromQueue) {
    {
        thunder::datastructures::Queue<int> queue;
        queue.push(5);
        int c;
        EXPECT_EQ(queue.pop(c), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::Queue<char> queue;
        queue.push('c');
        char c;
        EXPECT_EQ(queue.pop(c), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::Queue<float> queue;
        queue.push(5.1);
        float c;
        EXPECT_EQ(queue.pop(c), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::Queue<std::string> queue;
        queue.push("foo");
        queue.push(std::string("bar"));
        std::string front;
        EXPECT_EQ(queue.pop(front), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
        EXPECT_EQ(queue.isEmpty(), false);
        EXPECT_EQ(queue.pop(front), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
        EXPECT_EQ(queue.isEmpty(), true);
    }
}

TEST(QueueBasicOperations, PopElementFromQueueAndCheckFront) {
    {
        thunder::datastructures::Queue<int> queue;
        queue.push(5);
        queue.push(6);
        queue.push(7);
        queue.push(8);
        queue.push(9);

        int front;
        queue.pop(front);
        EXPECT_EQ(front, 5);
        queue.pop(front);
        EXPECT_EQ(front, 6);
        queue.pop(front);
        EXPECT_EQ(front, 7);
        queue.pop(front);
        EXPECT_EQ(front, 8);
        queue.pop(front);
        EXPECT_EQ(front, 9);
    }
    {
        thunder::datastructures::Queue<char> queue;
        queue.push('a');
        queue.push('b');
        queue.push('c');
        queue.push('d');
        queue.push('e');
        queue.push('f');

        char front;
        queue.pop(front);
        EXPECT_EQ(front, 'a');
        queue.pop(front);
        EXPECT_EQ(front, 'b');
        queue.pop(front);
        EXPECT_EQ(front, 'c');
        queue.pop(front);
        EXPECT_EQ(front, 'd');
        queue.pop(front);
        EXPECT_EQ(front, 'e');
        queue.pop(front);
        EXPECT_EQ(front, 'f');
    }
    {
        thunder::datastructures::Queue<float> queue;
        queue.push(5.1);
        queue.push(5.2);
        queue.push(5.3);
        queue.push(5.4);
        queue.push(5.5);
        queue.push(5.6);

        float front;
        queue.pop(front);
        EXPECT_EQ(front, (float)5.1);
        queue.pop(front);
        EXPECT_EQ(front, (float)5.2);
        queue.pop(front);
        EXPECT_EQ(front, (float)5.3);
        queue.pop(front);
        EXPECT_EQ(front, (float)5.4);
        queue.pop(front);
        EXPECT_EQ(front, (float)5.5);
        queue.pop(front);
        EXPECT_EQ(front, (float)5.6);
    }
    {
        thunder::datastructures::Queue<std::string> queue;
        queue.push("test1");
        queue.push("test2");
        queue.push("test3");
        queue.push("test4");
        queue.push("test5");
        queue.push("test6");

        std::string front;
        queue.pop(front);
        EXPECT_EQ(front, "test1");
        queue.pop(front);
        EXPECT_EQ(front, "test2");
        queue.pop(front);
        EXPECT_EQ(front, "test3");
        queue.pop(front);
        EXPECT_EQ(front, "test4");
        queue.pop(front);
        EXPECT_EQ(front, "test5");
        queue.pop(front);
        EXPECT_EQ(front, "test6");
    }
}

TEST(QueueBasicOperations, MoveConstructor) {
    {
        thunder::datastructures::Queue<int> queue;
        queue.push(5);

        thunder::datastructures::Queue<int> secondQueue(std::move(queue));
        int front;
        EXPECT_EQ(queue.pop(front), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
        secondQueue.front(front);
        EXPECT_EQ(front, 5);
    }
    {
        thunder::datastructures::Queue<char> queue;
        queue.push('c');

        thunder::datastructures::Queue<char> secondQueue(std::move(queue));
        char front;
        EXPECT_EQ(queue.pop(front), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
        secondQueue.front(front);
        EXPECT_EQ(front, 'c');
    }
    {
        thunder::datastructures::Queue<float> queue;
        queue.push(5.1);
        
        thunder::datastructures::Queue<float> secondQueue(std::move(queue));
        float front;
        EXPECT_EQ(queue.pop(front), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
        secondQueue.front(front);
        EXPECT_EQ(front, (float)5.1);
    }
    {
        thunder::datastructures::Queue<std::string> queue;
        queue.push("foo");
        
        thunder::datastructures::Queue<std::string> secondQueue(std::move(queue));
        std::string front;
        EXPECT_EQ(queue.pop(front), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
        secondQueue.front(front);
        EXPECT_EQ(front, "foo");
    }
}

TEST(QueueBasicOperations, MoveAssignsOperator) {
    {
        thunder::datastructures::Queue<int> queue;
        queue.push(5);

        thunder::datastructures::Queue<int> secondQueue = std::move(queue);
        int front;
        EXPECT_EQ(queue.pop(front), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
        secondQueue.front(front);
        EXPECT_EQ(front, 5);
    }
    {
        thunder::datastructures::Queue<char> queue;
        queue.push('c');

        thunder::datastructures::Queue<char> secondQueue = std::move(queue);
        char front;
        EXPECT_EQ(queue.pop(front), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
        secondQueue.front(front);
        EXPECT_EQ(front, 'c');
    }
    {
        thunder::datastructures::Queue<float> queue;
        queue.push(5.1);
        
        thunder::datastructures::Queue<float> secondQueue = std::move(queue);
        float front;
        EXPECT_EQ(queue.pop(front), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
        secondQueue.front(front);
        EXPECT_EQ(front, (float)5.1);
    }
    {
        thunder::datastructures::Queue<std::string> queue;
        queue.push("foo");
        
        thunder::datastructures::Queue<std::string> secondQueue = std::move(queue);
        std::string front;
        EXPECT_EQ(queue.pop(front), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
        secondQueue.front(front);
        EXPECT_EQ(front, "foo");
    }
}