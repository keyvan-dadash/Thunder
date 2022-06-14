#include <thread>
#include <chrono>

#include <gtest/gtest.h>

#include <thunder/synchronization/SmartLock.hpp>

TEST(SmartLockBasicOpreation, Create)
{
    EXPECT_NO_THROW({
        thunder::synchronization::SmartLock lock;
    });
}

TEST(SmartLockBasicOpreation, Lock)
{
    thunder::synchronization::SmartLock lock;

    EXPECT_NO_THROW(lock.lock());
}

TEST(SmartLockBasicOpreation, Unlock)
{
    thunder::synchronization::SmartLock lock;
    lock.lock();
    
    EXPECT_NO_THROW(lock.unlock());
}

TEST(SmartLockBasicOpreation, CheckThreadSafty)
{
    thunder::synchronization::SmartLock lock;

    std::vector<std::thread> threads;

    int total = 0;

    int count_per_thread = 5000;
    
    const int number_of_threads = 16;

    for (size_t i = 0; i < number_of_threads; i++) {
        
        std::thread t1([&](){

            for (size_t j = 0; j < count_per_thread; j++)
            {
                lock.lock();
                total++;
                lock.unlock();
            }
            
        });

        threads.push_back(std::move(t1));

    }

    for (int i = 0; i < number_of_threads; i++) {
        threads.at(i).join();
    }

    EXPECT_EQ(total, count_per_thread * number_of_threads); 
}

TEST(SmartLockBasicOpreation, CheckVectorSize)
{
    thunder::synchronization::SmartLock lock;

    std::vector<std::thread> threads;

    int total = 0;

    int count_per_thread = 100;

    std::vector<int> all_numbers;

    const int number_of_threads = 16;

    for (size_t i = 0; i < number_of_threads; i++) {
        
        std::thread t1([&](){

            for (size_t j = 0; j < count_per_thread; j++)
            {
                lock.lock();
                all_numbers.push_back(j);
                lock.unlock();
            }
            
        });

        threads.push_back(std::move(t1));

    }

    for (int i = 0; i < number_of_threads; i++) {
        threads.at(i).join();
    }

    int numbers_cnt[100];
    for (int i = 0; i < 100; i++) {
      numbers_cnt[i] = 0;
    }

    for (auto& number : all_numbers) {
      numbers_cnt[number]++;
    }

    for (int i = 0; i < 100; i++) {
      EXPECT_EQ(numbers_cnt[i], number_of_threads); 
    }
}

