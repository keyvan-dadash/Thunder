

// #include <string>


// #include <gtest/gtest.h>

// #include <thunder/datastructures/ConcurrentMap.hpp>

// TEST(MapBasicOperations, CreateMap) {
//     //we should test this way with lambda expression bcs we got error when using normal mode EXPECT_NO_THROW
//     //see https://stackoverflow.com/questions/41681243/compilation-error-using-googletests-expect-no-throw-with-stdarray
//     EXPECT_NO_THROW((
//         []{thunder::datastructures::ConcurrentMap<int, int> map;}()
//     ));
//     EXPECT_NO_THROW((
//         []{thunder::datastructures::ConcurrentMap<char, char> map;}()
//     ));
//     EXPECT_NO_THROW((
//         []{thunder::datastructures::ConcurrentMap<float, float> map;}()
//     ));
//     EXPECT_NO_THROW((
//         []{thunder::datastructures::ConcurrentMap<std::string, std::string> map;}()
//     ));
// }

// TEST(MapBasicOperations, InsertInMap) {
//     {
//         thunder::datastructures::ConcurrentMap<int, int> map;
//         EXPECT_TRUE(map.insert(5, 6));
//     }
//     {
//         thunder::datastructures::ConcurrentMap<int, char> map;
//         EXPECT_TRUE(map.insert(5, 'c'));
//     }
//     {
//         thunder::datastructures::ConcurrentMap<int, float> map;
//         EXPECT_TRUE(map.insert(5, (float)5.6));
//     }
//     {
//         thunder::datastructures::ConcurrentMap<int, std::string> map;
//         EXPECT_TRUE(map.insert(5, "foo"));
//         EXPECT_TRUE(map.insert(6, std::string("bar")));
//     }

//     {
//         thunder::datastructures::ConcurrentMap<char, int> map;
//         EXPECT_TRUE(map.insert('c', 6));
//     }
//     {
//         thunder::datastructures::ConcurrentMap<char, char> map;
//         EXPECT_TRUE(map.insert('f', 'o'));
//     }
//     {
//         thunder::datastructures::ConcurrentMap<char, float> map;
//         EXPECT_TRUE(map.insert('f', (float)5.6));
//     }
//     {
//         thunder::datastructures::ConcurrentMap<char, std::string> map;
//         EXPECT_TRUE(map.insert('c', "foo"));
//         EXPECT_TRUE(map.insert('b', std::string("bar")));
//     }

//     {
//         thunder::datastructures::ConcurrentMap<float, int> map;
//         EXPECT_TRUE(map.insert((float)5.6, 6));
//     }
//     {
//         thunder::datastructures::ConcurrentMap<float, char> map;
//         EXPECT_TRUE(map.insert((float)5.6, 'c'));
//     }
//     {
//         thunder::datastructures::ConcurrentMap<float, float> map;
//         EXPECT_TRUE(map.insert((float)5.6, (float)5.7));
//     }
//     {
//         thunder::datastructures::ConcurrentMap<float, std::string> map;
//         EXPECT_TRUE(map.insert((float)5.6, "foo"));
//         EXPECT_TRUE(map.insert((float)5.7, std::string("bar")));
//     }

//     {
//         thunder::datastructures::ConcurrentMap<std::string, int> map;
//         EXPECT_TRUE(map.insert("foo", 6));
//         EXPECT_TRUE(map.insert(std::string("bar"), 7));
//     }
//     {
//         thunder::datastructures::ConcurrentMap<std::string, char> map;
//         EXPECT_TRUE(map.insert("foo", 'f'));
//         EXPECT_TRUE(map.insert(std::string("bar"), 'b'));
//     }
//     {
//         thunder::datastructures::ConcurrentMap<std::string, float> map;
//         EXPECT_TRUE(map.insert("foo", 6));
//         EXPECT_TRUE(map.insert(std::string("bar"), (float)5.6));
//     }
//     {
//         thunder::datastructures::ConcurrentMap<std::string, std::string> map;
//         EXPECT_TRUE(map.insert("foo", "foo1"));
//         EXPECT_TRUE(map.insert(std::string("bar"), "bar1"));
//     }
// }

// // TEST(QueueBasicOperations, EmptyQueue) {
// //     {
// //         thunder::datastructures::Queue<int> queue;
// //         EXPECT_EQ(queue.isEmpty(), true);
// //     }
// //     {
// //         thunder::datastructures::Queue<char> queue;
// //         EXPECT_EQ(queue.isEmpty(), true);
// //     }
// //     {
// //         thunder::datastructures::Queue<float> queue;
// //         EXPECT_EQ(queue.isEmpty(), true);
// //     }
// //     {
// //         thunder::datastructures::Queue<std::string> queue;
// //         EXPECT_EQ(queue.isEmpty(), true);
// //     }
// // }

// // TEST(QueueBasicOperations, PushElementToQueue) {
// //     {
// //         thunder::datastructures::Queue<int> queue;
// //         EXPECT_EQ(queue.push(5), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
// //     }
// //     {
// //         thunder::datastructures::Queue<char> queue;
// //         EXPECT_EQ(queue.push('c'), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
// //     }
// //     {
// //         thunder::datastructures::Queue<float> queue;
// //         EXPECT_EQ(queue.push(5.1), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
// //     }
// //     {
// //         thunder::datastructures::Queue<std::string> queue;
// //         EXPECT_EQ(queue.push("foo"), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
// //         EXPECT_EQ(queue.push(std::string("bar")), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
// //     }
// // }

// // TEST(QueueBasicOperations, TryPushElementToQueue) {
// //     {
// //         thunder::datastructures::Queue<int> queue;
// //         EXPECT_EQ(queue.tryPush(5, 10), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
// //     }
// //     {
// //         thunder::datastructures::Queue<char> queue;
// //         EXPECT_EQ(queue.tryPush('c', 10), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
// //     }
// //     {
// //         thunder::datastructures::Queue<float> queue;
// //         EXPECT_EQ(queue.tryPush(5.1, 10), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
// //     }
// //     {
// //         thunder::datastructures::Queue<std::string> queue;
// //         EXPECT_EQ(queue.tryPush("foo", 10), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
// //         EXPECT_EQ(queue.tryPush(std::string("bar"), 10), thunder::datastructures::QueueOperationStatus::ELEMENT_PUSHED_SUCCESSFULLY);
// //     }
// // }

// // TEST(QueueBasicOperations, GetFrontElement) {
// //     {
// //         thunder::datastructures::Queue<int> queue;
// //         queue.push(5);
// //         EXPECT_EQ(*queue.front(), 5);
// //     }
// //     {
// //         thunder::datastructures::Queue<char> queue;
// //         queue.push('c');
// //         EXPECT_EQ(*queue.front(), 'c');
// //     }
// //     {
// //         thunder::datastructures::Queue<float> queue;
// //         queue.push((float)5.1);
// //         EXPECT_EQ(*queue.front(), (float)5.1);
// //     }
// //     {
// //         thunder::datastructures::Queue<std::string> queue;
// //         queue.push("foo");
// //         EXPECT_EQ(*queue.front(), "foo");
// //         queue.push(std::string("bar"));
// //         EXPECT_EQ(*queue.front(), "foo");
// //     }
// // }

// // TEST(QueueBasicOperations, PopElementFromQueue) {
// //     {
// //         thunder::datastructures::Queue<int> queue;
// //         queue.push(5);
// //         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
// //         EXPECT_EQ(queue.isEmpty(), true);
// //     }
// //     {
// //         thunder::datastructures::Queue<char> queue;
// //         queue.push('c');
// //         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
// //         EXPECT_EQ(queue.isEmpty(), true);
// //     }
// //     {
// //         thunder::datastructures::Queue<float> queue;
// //         queue.push(5.1);
// //         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
// //         EXPECT_EQ(queue.isEmpty(), true);
// //     }
// //     {
// //         thunder::datastructures::Queue<std::string> queue;
// //         queue.push("foo");
// //         queue.push(std::string("bar"));
// //         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
// //         EXPECT_EQ(queue.isEmpty(), false);
// //         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::ELEMENT_POPED_SUCCESSFULLY);
// //         EXPECT_EQ(queue.isEmpty(), true);
// //     }
// // }

// // TEST(QueueBasicOperations, PopElementFromQueueAndCheckFront) {
// //     {
// //         thunder::datastructures::Queue<int> queue;
// //         queue.push(5);
// //         queue.push(6);
// //         queue.push(7);
// //         queue.push(8);
// //         queue.push(9);

// //         EXPECT_EQ(*queue.front(), 5);
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), 6);
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), 7);
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), 8);
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), 9);
// //     }
// //     {
// //         thunder::datastructures::Queue<char> queue;
// //         queue.push('a');
// //         queue.push('b');
// //         queue.push('c');
// //         queue.push('d');
// //         queue.push('e');
// //         queue.push('f');

// //         EXPECT_EQ(*queue.front(), 'a');
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), 'b');
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), 'c');
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), 'd');
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), 'e');
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), 'f');
// //     }
// //     {
// //         thunder::datastructures::Queue<float> queue;
// //         queue.push(5.1);
// //         queue.push(5.2);
// //         queue.push(5.3);
// //         queue.push(5.4);
// //         queue.push(5.5);
// //         queue.push(5.6);

// //         EXPECT_EQ(*queue.front(), (float)5.1);
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), (float)5.2);
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), (float)5.3);
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), (float)5.4);
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), (float)5.5);
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), (float)5.6);
// //     }
// //     {
// //         thunder::datastructures::Queue<std::string> queue;
// //         queue.push("test1");
// //         queue.push("test2");
// //         queue.push("test3");
// //         queue.push("test4");
// //         queue.push("test5");
// //         queue.push("test6");

// //         EXPECT_EQ(*queue.front(), "test1");
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), "test2");
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), "test3");
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), "test4");
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), "test5");
// //         queue.pop();
// //         EXPECT_EQ(*queue.front(), "test6");
// //     }
// // }

// // TEST(QueueBasicOperations, MoveConstructor) {
// //     {
// //         thunder::datastructures::Queue<int> queue;
// //         queue.push(5);

// //         thunder::datastructures::Queue<int> secondQueue(std::move(queue));
// //         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
// //         EXPECT_EQ(*secondQueue.front(), 5);
// //     }
// //     {
// //         thunder::datastructures::Queue<char> queue;
// //         queue.push('c');

// //         thunder::datastructures::Queue<char> secondQueue(std::move(queue));
// //         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
// //         EXPECT_EQ(*secondQueue.front(), 'c');
// //     }
// //     {
// //         thunder::datastructures::Queue<float> queue;
// //         queue.push(5.1);
        
// //         thunder::datastructures::Queue<float> secondQueue(std::move(queue));
// //         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
// //         EXPECT_EQ(*secondQueue.front(), (float)5.1);
// //     }
// //     {
// //         thunder::datastructures::Queue<std::string> queue;
// //         queue.push("foo");
        
// //         thunder::datastructures::Queue<std::string> secondQueue(std::move(queue));
// //         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
// //         EXPECT_EQ(*secondQueue.front(), "foo");
// //     }
// // }

// // TEST(QueueBasicOperations, MoveAssignsOperator) {
// //     {
// //         thunder::datastructures::Queue<int> queue;
// //         queue.push(5);

// //         thunder::datastructures::Queue<int> secondQueue = std::move(queue);
// //         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
// //         EXPECT_EQ(*secondQueue.front(), 5);
// //     }
// //     {
// //         thunder::datastructures::Queue<char> queue;
// //         queue.push('c');

// //         thunder::datastructures::Queue<char> secondQueue = std::move(queue);
// //         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
// //         EXPECT_EQ(*secondQueue.front(), 'c');
// //     }
// //     {
// //         thunder::datastructures::Queue<float> queue;
// //         queue.push(5.1);
        
// //         thunder::datastructures::Queue<float> secondQueue = std::move(queue);
// //         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
// //         EXPECT_EQ(*secondQueue.front(), (float)5.1);
// //     }
// //     {
// //         thunder::datastructures::Queue<std::string> queue;
// //         queue.push("foo");
        
// //         thunder::datastructures::Queue<std::string> secondQueue = std::move(queue);
// //         EXPECT_EQ(queue.pop(), thunder::datastructures::QueueOperationStatus::OPERATION_CANNOT_PERMIT_QUEUE_IS_EMPTY);
// //         EXPECT_EQ(*secondQueue.front(), "foo");
// //     }
// // }