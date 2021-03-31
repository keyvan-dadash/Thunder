


#include <thread>
#include <chrono>

#include <gtest/gtest.h>

#include <thunder/synchronization/SpinLock.hpp>



TEST(SpinLockBasicOpreation, Create)
{
    EXPECT_NO_THROW({
        thunder::synchronization::SpinLock spinlock;
    });
}

TEST(SpinLockBasicOpreation, Lock)
{
    thunder::synchronization::SpinLock spinlock;

    EXPECT_NO_THROW(spinlock.lock());
}

TEST(SpinLockBasicOpreation, TrylockNoThrow)
{
    thunder::synchronization::SpinLock spinlock;

    EXPECT_NO_THROW(spinlock.try_lock());
}

TEST(SpinLockBasicOpreation, TrylockBool)
{
    thunder::synchronization::SpinLock spinlock;

    EXPECT_TRUE(spinlock.try_lock());
}

TEST(SpinLockBasicOpreation, Unlock)
{
    thunder::synchronization::SpinLock spinlock;

    EXPECT_NO_THROW(spinlock.unlock());
}

TEST(SpinLockBasicOpreation, CheckThreadSafty)
{
    thunder::synchronization::SpinLock spinlock;

    std::vector<std::thread> threads;

    int total = 0;

    int count_per_thread = 5000;

    for (size_t i = 0; i < 16; i++) {
        
        std::thread t1([&](){

            for (size_t j = 0; j < count_per_thread; j++)
            {
                spinlock.lock();
                total++;
                spinlock.unlock();
            }
            
        });

        threads.push_back(std::move(t1));

    }

    for (int i = 0; i < 16; i++) {
        threads.at(i).join();
    }

    EXPECT_EQ(total, count_per_thread*16);
    
    
}



TEST(CacheLineSpinLockBasicOpreation, Create)
{
    EXPECT_NO_THROW({
        thunder::synchronization::SpinLock spinlock;
    });
}

TEST(CacheLineSpinLockBasicOpreation, Lock)
{
    thunder::synchronization::SpinLock spinlock;

    EXPECT_NO_THROW(spinlock.lock());
}

TEST(CacheLineSpinLockBasicOpreation, TrylockNoThrow)
{
    thunder::synchronization::SpinLock spinlock;

    EXPECT_NO_THROW(spinlock.try_lock());
}

TEST(CacheLineSpinLockBasicOpreation, TrylockBool)
{
    thunder::synchronization::SpinLock spinlock;

    EXPECT_TRUE(spinlock.try_lock());
}

TEST(CacheLineSpinLockBasicOpreation, Unlock)
{
    thunder::synchronization::CacheLineSpinLock spinlock;

    EXPECT_NO_THROW(spinlock.unlock());
}

TEST(CacheLineSpinLockBasicOpreation, CheckThreadSafty)
{
    thunder::synchronization::CacheLineSpinLock spinlock;

    std::vector<std::thread> threads;

    int total = 0;

    int count_per_thread = 5000;

    for (size_t i = 0; i < 16; i++) {
        
        std::thread t1([&](){

            for (size_t j = 0; j < count_per_thread; j++)
            {
                spinlock.lock();
                total++;
                spinlock.unlock();
            }
            
        });

        threads.push_back(std::move(t1));

    }

    for (int i = 0; i < 16; i++) {
        threads.at(i).join();
    }

    EXPECT_EQ(total, count_per_thread*16);
    
    
}