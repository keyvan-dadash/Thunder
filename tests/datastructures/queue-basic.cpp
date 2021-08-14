

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

// TEST(QueueBasicOperations, GetFrontElement) {
//     {
//         thunder::datastructures::Queue<int> queue;
//         queue.push(5);
//         EXPECT_EQ(*queue.front(), 5);
//     }
//     {
//         thunder::datastructures::Queue<char> queue;
//         queue.push('c');
//         EXPECT_EQ(*queue.front(), 'c');
//     }
//     {
//         thunder::datastructures::Queue<float> queue;
//         queue.push((float)5.1);
//         EXPECT_EQ(*queue.front(), (float)5.1);
//     }
//     {
//         thunder::datastructures::Queue<std::string> queue;
//         queue.push("foo");
//         EXPECT_EQ(*queue.front(), "foo");
//         queue.push(std::string("bar"));
//         EXPECT_EQ(*queue.front(), "foo");
//     }
// }

// TEST(QueueBasicOperations, PopElementFromQueue) {
//     {
//         thunder::datastructures::Queue<int> queue;
//         queue.push(5);
//         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
//         EXPECT_EQ(queue.isEmpty(), true);
//     }
//     {
//         thunder::datastructures::Queue<char> queue;
//         queue.push('c');
//         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
//         EXPECT_EQ(queue.isEmpty(), true);
//     }
//     {
//         thunder::datastructures::Queue<float> queue;
//         queue.push(5.1);
//         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
//         EXPECT_EQ(queue.isEmpty(), true);
//     }
//     {
//         thunder::datastructures::Queue<std::string> queue;
//         queue.push("foo");
//         queue.push(std::string("bar"));
//         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
//         EXPECT_EQ(queue.isEmpty(), false);
//         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
//         EXPECT_EQ(queue.isEmpty(), true);
//     }
// }

// TEST(QueueBasicOperations, PopElementFromQueueAndCheckFront) {
//     {
//         thunder::datastructures::Queue<int> queue;
//         queue.push(5);
//         queue.push(6);
//         queue.push(7);
//         queue.push(8);
//         queue.push(9);

//         EXPECT_EQ(*queue.front(), 5);
//         queue.pop();
//         EXPECT_EQ(*queue.front(), 6);
//         queue.pop();
//         EXPECT_EQ(*queue.front(), 7);
//         queue.pop();
//         EXPECT_EQ(*queue.front(), 8);
//         queue.pop();
//         EXPECT_EQ(*queue.front(), 9);
//     }
//     {
//         thunder::datastructures::Queue<char> queue;
//         queue.push('a');
//         queue.push('b');
//         queue.push('c');
//         queue.push('d');
//         queue.push('e');
//         queue.push('f');

//         EXPECT_EQ(*queue.front(), 'a');
//         queue.pop();
//         EXPECT_EQ(*queue.front(), 'b');
//         queue.pop();
//         EXPECT_EQ(*queue.front(), 'c');
//         queue.pop();
//         EXPECT_EQ(*queue.front(), 'd');
//         queue.pop();
//         EXPECT_EQ(*queue.front(), 'e');
//         queue.pop();
//         EXPECT_EQ(*queue.front(), 'f');
//     }
//     {
//         thunder::datastructures::Queue<float> queue;
//         queue.push(5.1);
//         queue.push(5.2);
//         queue.push(5.3);
//         queue.push(5.4);
//         queue.push(5.5);
//         queue.push(5.6);

//         EXPECT_EQ(*queue.front(), (float)5.1);
//         queue.pop();
//         EXPECT_EQ(*queue.front(), (float)5.2);
//         queue.pop();
//         EXPECT_EQ(*queue.front(), (float)5.3);
//         queue.pop();
//         EXPECT_EQ(*queue.front(), (float)5.4);
//         queue.pop();
//         EXPECT_EQ(*queue.front(), (float)5.5);
//         queue.pop();
//         EXPECT_EQ(*queue.front(), (float)5.6);
//     }
//     {
//         thunder::datastructures::Queue<std::string> queue;
//         queue.push("test1");
//         queue.push("test2");
//         queue.push("test3");
//         queue.push("test4");
//         queue.push("test5");
//         queue.push("test6");

//         EXPECT_EQ(*queue.front(), "test1");
//         queue.pop();
//         EXPECT_EQ(*queue.front(), "test2");
//         queue.pop();
//         EXPECT_EQ(*queue.front(), "test3");
//         queue.pop();
//         EXPECT_EQ(*queue.front(), "test4");
//         queue.pop();
//         EXPECT_EQ(*queue.front(), "test5");
//         queue.pop();
//         EXPECT_EQ(*queue.front(), "test6");
//     }
// }

// TEST(QueueBasicOperations, MoveConstructor) {
//     {
//         thunder::datastructures::Queue<int> queue;
//         queue.push(5);

//         thunder::datastructures::Queue<int> secondQueue(std::move(queue));
//         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
//         EXPECT_EQ(*secondQueue.front(), 5);
//     }
//     {
//         thunder::datastructures::Queue<char> queue;
//         queue.push('c');

//         thunder::datastructures::Queue<char> secondQueue(std::move(queue));
//         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
//         EXPECT_EQ(*secondQueue.front(), 'c');
//     }
//     {
//         thunder::datastructures::Queue<float> queue;
//         queue.push(5.1);
        
//         thunder::datastructures::Queue<float> secondQueue(std::move(queue));
//         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
//         EXPECT_EQ(*secondQueue.front(), (float)5.1);
//     }
//     {
//         thunder::datastructures::Queue<std::string> queue;
//         queue.push("foo");
        
//         thunder::datastructures::Queue<std::string> secondQueue(std::move(queue));
//         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
//         EXPECT_EQ(*secondQueue.front(), "foo");
//     }
// }

// TEST(QueueBasicOperations, MoveAssignsOperator) {
//     {
//         thunder::datastructures::Queue<int> queue;
//         queue.push(5);

//         thunder::datastructures::Queue<int> secondQueue = std::move(queue);
//         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
//         EXPECT_EQ(*secondQueue.front(), 5);
//     }
//     {
//         thunder::datastructures::Queue<char> queue;
//         queue.push('c');

//         thunder::datastructures::Queue<char> secondQueue = std::move(queue);
//         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
//         EXPECT_EQ(*secondQueue.front(), 'c');
//     }
//     {
//         thunder::datastructures::Queue<float> queue;
//         queue.push(5.1);
        
//         thunder::datastructures::Queue<float> secondQueue = std::move(queue);
//         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
//         EXPECT_EQ(*secondQueue.front(), (float)5.1);
//     }
//     {
//         thunder::datastructures::Queue<std::string> queue;
//         queue.push("foo");
        
//         thunder::datastructures::Queue<std::string> secondQueue = std::move(queue);
//         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
//         EXPECT_EQ(*secondQueue.front(), "foo");
//     }
// }