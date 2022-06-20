#include <thread>

#include <benchmark/benchmark.h>

#include <thunder/synchronization/SpinLock.hpp>

static void BE_SpinLock(benchmark::State& state) {
    // state.PauseTiming();

    for (auto _ : state) {
    thunder::synchronization::SpinLock spinlock;

    std::vector<std::thread> threads;

    int total = 0;

    int count_per_thread = 5000;

    // state.ResumeTiming();
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
    }
}
BENCHMARK(BE_SpinLock)->Range(8, 8<<16)->UseRealTime();

static void BE_CachelineSpinLock(benchmark::State& state) {
    // state.PauseTiming();

    for (auto _ : state) {
    thunder::synchronization::CacheLineSpinLock spinlock;

    std::vector<std::thread> threads;

    int total = 0;

    int count_per_thread = 5000;

    // state.ResumeTiming();
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
    }
}
BENCHMARK(BE_CachelineSpinLock)->Range(8, 8<<16)->UseRealTime();

BENCHMARK_MAIN();
