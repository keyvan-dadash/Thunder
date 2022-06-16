#include <thread>
#include <mutex>
#include <vector>

int main()
{
  std::mutex mutex;
  std::vector<std::thread> threads;
  long total = 0;
  int count_per_thread = 5000;

  for (size_t i = 0; i < 16; i++) {
    std::thread t1([&](){
        for (size_t j = 0; j < count_per_thread; j++) {
          mutex.lock();
          total++;
          std::this_thread::sleep_for(std::chrono::milliseconds(1));
          mutex.unlock();
        }
    });
    threads.push_back(std::move(t1));
  }

  for (int i = 0; i < 16; i++) {
    threads.at(i).join();
  }
}

