

#include <string>


#include <gtest/gtest.h>

#include <thunder/datastructures/ConcurrentQueue.hpp>

TEST(ConcurrentQueueBasicOperations, CreateQueue) {
    EXPECT_NO_THROW({
        thunder::datastructures::ConcurrentQueue<int> queue;
    });
    EXPECT_NO_THROW({
        thunder::datastructures::ConcurrentQueue<char> queue;
    });
    EXPECT_NO_THROW({
        thunder::datastructures::ConcurrentQueue<float> queue;
    });
    EXPECT_NO_THROW({
        thunder::datastructures::ConcurrentQueue<std::string> queue;
    });
}

TEST(ConcurrentQueueBasicOperations, EmptyQueue) {
    {
        thunder::datastructures::ConcurrentQueue<int> queue;
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::ConcurrentQueue<char> queue;
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::ConcurrentQueue<float> queue;
        EXPECT_EQ(queue.isEmpty(), true);
    }
    {
        thunder::datastructures::ConcurrentQueue<std::string> queue;
        EXPECT_EQ(queue.isEmpty(), true);
    }
}

TEST(ConcurrentQueueBasicOperations, PushElementToQueue) {
    {
        thunder::datastructures::ConcurrentQueue<int> queue;
        EXPECT_EQ(queue.forcePush(5), thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::ConcurrentQueue<char> queue;
        EXPECT_EQ(queue.forcePush('c'), thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::ConcurrentQueue<float> queue;
        EXPECT_EQ(queue.forcePush(5.1), thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::ConcurrentQueue<std::string> queue;
        EXPECT_EQ(queue.forcePush("foo"), thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
        EXPECT_EQ(queue.forcePush(std::string("bar")), thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
}

TEST(ConcurrentQueueBasicOperations, TryPushElementToQueue) {
    {
        thunder::datastructures::ConcurrentQueue<int> queue;
        EXPECT_EQ(queue.tryPush(5), thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::ConcurrentQueue<char> queue;
        EXPECT_EQ(queue.tryPush('c'), thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::ConcurrentQueue<float> queue;
        EXPECT_EQ(queue.tryPush(5.1), thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
    {
        thunder::datastructures::ConcurrentQueue<std::string> queue;
        EXPECT_EQ(queue.tryPush("foo"), thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
        EXPECT_EQ(queue.tryPush(std::string("bar")), thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
    }
}

// TEST(ConcurrentQueueBasicOperations, GetFrontElement) {
//     {
//         thunder::datastructures::ConcurrentQueue<int> queue;
//         queue.push(5);
//         EXPECT_EQ(*queue.front(), 5);
//     }
//     {
//         thunder::datastructures::ConcurrentQueue<char> queue;
//         queue.push('c');
//         EXPECT_EQ(*queue.front(), 'c');
//     }
//     {
//         thunder::datastructures::ConcurrentQueue<float> queue;
//         queue.push((float)5.1);
//         EXPECT_EQ(*queue.front(), (float)5.1);
//     }
//     {
//         thunder::datastructures::ConcurrentQueue<std::string> queue;
//         queue.push("foo");
//         EXPECT_EQ(*queue.front(), "foo");
//         queue.push(std::string("bar"));
//         EXPECT_EQ(*queue.front(), "foo");
//     }
// }

// TEST(ConcurrentQueueBasicOperations, PopElementFromQueue) {
//     {
//         thunder::datastructures::ConcurrentQueue<int> queue;
//         queue.push(5);
//         EXPECT_EQ(queue.pop(), thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
//         EXPECT_EQ(queue.isEmpty(), true);
//     }
//     {
//         thunder::datastructures::ConcurrentQueue<char> queue;
//         queue.push('c');
//         EXPECT_EQ(queue.pop(), thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
//         EXPECT_EQ(queue.isEmpty(), true);
//     }
//     {
//         thunder::datastructures::ConcurrentQueue<float> queue;
//         queue.push(5.1);
//         EXPECT_EQ(queue.pop(), thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
//         EXPECT_EQ(queue.isEmpty(), true);
//     }
//     {
//         thunder::datastructures::ConcurrentQueue<std::string> queue;
//         queue.push("foo");
//         queue.push(std::string("bar"));
//         EXPECT_EQ(queue.pop(), thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
//         EXPECT_EQ(queue.isEmpty(), false);
//         EXPECT_EQ(queue.pop(), thunder::datastructures::ConcurrentQueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
//         EXPECT_EQ(queue.isEmpty(), true);
//     }
// }

// TEST(ConcurrentQueueBasicOperations, PopElementFromQueueAndCheckFront) {
//     {
//         thunder::datastructures::ConcurrentQueue<int> queue;
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
//         thunder::datastructures::ConcurrentQueue<char> queue;
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
//         thunder::datastructures::ConcurrentQueue<float> queue;
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
//         thunder::datastructures::ConcurrentQueue<std::string> queue;
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
