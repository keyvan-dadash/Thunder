#include <mutex>
#include <thread>
#include <thunder/synchronization/SpinLock.hpp>
#include <vector>

int main() {
  thunder::synchronization::SpinLock spinlock;
  std::vector<std::thread> threads;
  long total = 0;
  int count_per_thread = 50000;

  for (size_t i = 0; i < 16; i++) {
    std::thread t1([&]() {
      for (size_t j = 0; j < count_per_thread; j++) {
        spinlock.lock();
        total++;
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        spinlock.unlock();
      }
    });
    threads.push_back(std::move(t1));
  }

  for (int i = 0; i < 16; i++) {
    threads.at(i).join();
  }
}
