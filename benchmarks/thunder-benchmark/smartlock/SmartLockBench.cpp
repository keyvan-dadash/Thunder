#include <thread>
#include <vector>

#include <benchmark/benchmark.h>

#include <thunder/synchronization/SmartLock.hpp>

static void BE_SmartLock(benchmark::State& state) {
    // state.PauseTiming();

    for (auto _ : state) {
    thunder::synchronization::SmartLock smartlock;

    std::vector<std::thread> threads;

    int total = 0;

    int count_per_thread = 5000;

    // state.ResumeTiming();
    for (size_t i = 0; i < 16; i++) {
        
        std::thread t1([&](){

            for (size_t j = 0; j < count_per_thread; j++)
            {
                smartlock.lock();
                total++;
                smartlock.unlock();
            }
            
        });

        threads.push_back(std::move(t1));

    }

    for (int i = 0; i < 16; i++) {
        threads.at(i).join();
    }
    }
}
BENCHMARK(BE_SmartLock)->Range(8, 8<<16)->UseRealTime();

static void BE_Mutex(benchmark::State& state) {
    // state.PauseTiming();

    for (auto _ : state) {
    std::mutex lock;

    std::vector<std::thread> threads;

    int total = 0;

    int count_per_thread = 5000;

    // state.ResumeTiming();
    for (size_t i = 0; i < 16; i++) {
        
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

    for (int i = 0; i < 16; i++) {
        threads.at(i).join();
    }
    }
}
BENCHMARK(BE_Mutex)->Range(8, 8<<16)->UseRealTime();

BENCHMARK_MAIN();
