#include <thread>
#include <mutex>
#include <vector>

#include <thunder/synchronization/SpinLock.hpp>

int main()
{
  thunder::synchronization::CacheLineSpinLock spinlock;
  std::vector<std::thread> threads;
  long total = 0;
  int count_per_thread = 5000;

  for (size_t i = 0; i < 16; i++) {
    std::thread t1([&](){
        for (size_t j = 0; j < count_per_thread; j++) {
          spinlock.lock();
          total++;
          std::this_thread::sleep_for(std::chrono::milliseconds(1));
          spinlock.unlock();
        }
    });
    threads.push_back(std::move(t1));
  }

  for (int i = 0; i < 16; i++) {
    threads.at(i).join();
  }
}

