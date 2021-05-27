

#include <string>
#include <thread>
#include <chrono>
#include <iostream>

#include <gtest/gtest.h>

#include <thunder/synchronization/Baton.hpp>





TEST(BatonBasicCreation, Create)
{
    EXPECT_NO_THROW({
        thunder::synchronization::Baton baton;
    });
}

TEST(BatonBasicOpreation, CheckThreadSafty)
{
    thunder::synchronization::Baton baton;

    std::vector<std::thread> threads;

    int total = 0;

    int count_per_thread = 7000;

    for (size_t i = 0; i < 18; i++) {
        
        std::thread t1([&](){

            for (size_t j = 0; j < count_per_thread; j++)
            {
                baton.write_access_start();
                total++;
                baton.write_access_end();
            }
            
        });

        threads.push_back(std::move(t1));

    }

    for (int i = 0; i < 18; i++) {
        threads.at(i).join();
    }

    EXPECT_EQ(total, count_per_thread*18);
    
    
}

std::string gen_random(const int len) {
    
    std::string tmp_s;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    srand( (unsigned) time(NULL) * getpid());

    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) 
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    
    
    return tmp_s;
    
}

TEST(BatonBasicOpreation, CheckThreadSaftyReaderWriter)
{
    thunder::synchronization::Baton baton;

    std::vector<std::thread> threads;

    std::vector<std::tuple<int, std::thread::id, std::string, std::thread::id>> result;
    std::vector<std::tuple<int, std::thread::id, std::string>> queue;

    int total = 0;

    int count_per_thread = 2000;

    std::cout << 1 << std::endl;

    for (size_t i = 0; i < 8; i++) {
        
        std::thread t1([&](){

            for (size_t j = 0; j < count_per_thread; j++)
            {
                std::cout << 2 << std::endl;
                baton.write_access_start();
                queue.push_back(std::tuple<int, std::thread::id, std::string>(j, std::this_thread::get_id(), gen_random(j % 30)));
                std::this_thread::sleep_for(std::chrono::microseconds(40));
                baton.write_access_end();
            }
            
        });
        threads.push_back(std::move(t1));
    }

    for (size_t i = 0; i < 8; i++) {
        
        std::thread t1([&](){

            for (size_t j = 0; j < count_per_thread; j++)
            {
                if (queue.empty()) {
                    j--;
                    std::cout << 3 << std::endl;
                    std::this_thread::sleep_for(std::chrono::microseconds(20));
                    continue;
                }

                std::cout << 4 << std::endl;

                baton.read_access_start();
                if (queue.empty())
                {
                    j--;
                    std::cout << 5 << std::endl;
                    std::this_thread::sleep_for(std::chrono::microseconds(20));
                    baton.read_access_end();
                    continue;
                }
                auto s = queue.back();
                queue.pop_back();
                std::cout << 6 << std::endl;
                std::sort(std::get<2>(s).begin(), std::get<2>(s).end());
                std::cout << 7 << std::endl;
                result.push_back(std::tuple<int, std::thread::id, std::string, std::thread::id>(std::get<0>(s), std::get<1>(s), std::get<2>(s), std::this_thread::get_id()));
                std::cout << 8 << std::endl;
                baton.read_access_end();
            }
            
        });
        threads.push_back(std::move(t1));
    }

    for (int i = 0; i < 16; i++) {
        threads.at(i).join();
    }

    EXPECT_EQ(total, count_per_thread*16);
    
    
}