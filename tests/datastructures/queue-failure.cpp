

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

// TEST(QueueFailureOperations, GetFrontElementFromEmptyQueue) {
//     {
//         thunder::datastructures::Queue<int> queue;
//         EXPECT_EQ(*queue.front(), 0);
//     }
//     {
//         thunder::datastructures::Queue<char> queue;
//         EXPECT_EQ(*queue.front(), 0);
//     }
//     {
//         thunder::datastructures::Queue<float> queue;
//         EXPECT_EQ(*queue.front(), (float)0);
//     }
//     {
//         thunder::datastructures::Queue<std::string> queue;
//         EXPECT_EQ(*queue.front(), "");
//     }
// }

// TEST(QueueFailureOperations, PopElementFromQueue) {
//     {
//         thunder::datastructures::Queue<int> queue;
//         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
//     }
//     {
//         thunder::datastructures::Queue<char> queue;
//         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
//     }
//     {
//         thunder::datastructures::Queue<float> queue;
//         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
//     }
//     {
//         thunder::datastructures::Queue<std::string> queue;
//         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
//     }
// }

// TEST(QueueFailureOperations, GetFrontElementAndChangeShouldNotChangeOriginalValue) {
//     {
//         thunder::datastructures::Queue<int> queue;
//         queue.push(5);

//         auto isReadOnly = queue.front();
//         *isReadOnly.get() = 6;
//         EXPECT_EQ(*queue.front(), 5);

//         isReadOnly = queue.front();
//         isReadOnly.reset(new int(10));
//         EXPECT_EQ(*queue.front(), 5);
//     }
//     {
//         thunder::datastructures::Queue<char> queue;
//         queue.push('c');
        
//         auto isReadOnly = queue.front();
//         *isReadOnly.get() = 'A';
//         EXPECT_EQ(*queue.front(), 'c');

//         isReadOnly = queue.front();
//         isReadOnly.reset(new char('T'));
//         EXPECT_EQ(*queue.front(), 'c');
//     }
//     {
//         thunder::datastructures::Queue<float> queue;
//         queue.push((float)5.1);
        
//         auto isReadOnly = queue.front();
//         *isReadOnly.get() = (float)5.3;
//         EXPECT_EQ(*queue.front(), (float)5.1);

//         isReadOnly = queue.front();
//         isReadOnly.reset(new float(5.2));
//         EXPECT_EQ(*queue.front(), (float)5.1);
//     }
//     {
//         thunder::datastructures::Queue<std::string> queue;
//         queue.push("foo");
        
//         auto isReadOnly = queue.front();
//         *isReadOnly.get() = "too";
//         EXPECT_EQ(*queue.front(), "foo");

//         isReadOnly = queue.front();
//         isReadOnly.reset(new std::string("zoo"));
//         EXPECT_EQ(*queue.front(), "foo");
//     }
// }