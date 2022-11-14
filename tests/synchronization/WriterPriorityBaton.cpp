

#include <gtest/gtest.h>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <thunder/synchronization/Baton.hpp>

TEST(WriterBatonBasicCreation, Create) {
  EXPECT_NO_THROW({ thunder::synchronization::WriterPriorityBaton baton; });
}

TEST(WriterBatonBasicOpreation, CheckThreadSafty) {
  thunder::synchronization::WriterPriorityBaton baton;

  std::vector<std::thread> threads;

  int total = 0;

  int count_per_thread = 7000;

  for (size_t i = 0; i < 18; i++) {
    std::thread t1([&]() {
      for (size_t j = 0; j < count_per_thread; j++) {
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

  EXPECT_EQ(total, count_per_thread * 18);
}

std::string gen_random_writebaton(const int len) {
  std::string tmp_s;
  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

  srand((unsigned)time(NULL) * getpid());

  tmp_s.reserve(len);

  for (int i = 0; i < len; ++i)
    tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];

  return tmp_s;
}

TEST(WriterBatonBasicOpreation, CheckThreadSaftyReaderWriter) {
  thunder::synchronization::WriterPriorityBaton baton;
  thunder::synchronization::WriterPriorityBaton resBaton;

  std::vector<std::thread> threads;
  std::vector<std::thread::id> writerThreadsID;
  std::vector<std::thread::id> readerThreadsID;

  std::vector<std::tuple<int, std::thread::id, std::string, std::thread::id>>
      result;
  std::vector<std::tuple<int, std::thread::id, std::string>> queue;

  int total = 0;

  int count_per_thread = 2000;

  for (size_t i = 0; i < 8; i++) {
    std::thread t1([&]() {
      baton.write_access_start();
      writerThreadsID.push_back(std::this_thread::get_id());
      baton.write_access_end();

      for (size_t j = 0; j < count_per_thread; j++) {
        baton.write_access_start();
        queue.push_back(std::tuple<int, std::thread::id, std::string>(
            j, std::this_thread::get_id(), gen_random_writebaton(j % 30)));
        std::this_thread::sleep_for(std::chrono::microseconds(40));
        total++;
        baton.write_access_end();
      }
    });
    threads.push_back(std::move(t1));
  }

  for (size_t i = 0; i < 8; i++) {
    std::thread t1([&]() {
      baton.write_access_start();
      readerThreadsID.push_back(std::this_thread::get_id());
      baton.write_access_end();

      for (size_t j = 0; j < count_per_thread; j++) {
        if (queue.empty()) {
          j--;
          std::this_thread::sleep_for(std::chrono::microseconds(20));
          continue;
        }

        baton.read_access_start();
        resBaton.write_access_start();
        if (queue.empty()) {
          j--;
          std::this_thread::sleep_for(std::chrono::microseconds(20));
          resBaton.write_access_end();
          baton.read_access_end();
          continue;
        }
        auto s = queue.back();
        queue.pop_back();
        std::sort(std::get<2>(s).begin(), std::get<2>(s).end());
        result.push_back(
            std::tuple<int, std::thread::id, std::string, std::thread::id>(
                j, std::get<1>(s), std::get<2>(s), std::this_thread::get_id()));
        resBaton.write_access_end();
        std::this_thread::sleep_for(std::chrono::microseconds(20));
        baton.read_access_end();
      }
    });
    threads.push_back(std::move(t1));
  }

  for (int i = 0; i < 16; i++) {
    threads.at(i).join();
  }

  std::map<std::thread::id, int> resultCounter;
  std::map<std::thread::id, int> processedCounter;

  EXPECT_EQ(total, count_per_thread * 8);
  EXPECT_EQ(result.size(), count_per_thread * 8);

  for (auto& wirterThreads : writerThreadsID) {
    resultCounter[wirterThreads] = 0;
  }

  for (auto& readerThreads : readerThreadsID) {
    processedCounter[readerThreads] = 0;
  }

  for (auto& res : result) {
    resultCounter[std::get<1>(res)] = resultCounter[std::get<1>(res)] + 1;
    processedCounter[std::get<3>(res)] = processedCounter[std::get<3>(res)] + 1;
  }

  for (const auto& [key, value] : resultCounter) {
    EXPECT_EQ(value, count_per_thread);
  }

  for (const auto& [key, value] : processedCounter) {
    EXPECT_EQ(value, count_per_thread);
  }
}

TEST(WriterBatonBasicOpreation, CheckReaderFairness) {
  thunder::synchronization::WriterPriorityBaton baton;
  thunder::synchronization::WriterPriorityBaton resBaton;

  std::vector<std::thread> threads;
  std::vector<std::thread::id> writerThreadsID;
  std::vector<std::thread::id> readerThreadsID;

  std::vector<std::tuple<int, std::thread::id, std::string, std::thread::id>>
      result;
  std::vector<std::tuple<int, std::thread::id, std::string>> queue;
  std::vector<std::tuple<int, std::thread::id, std::string>> persistantQueue;

  int total = 0;

  int finishedThread = 0;

  int count_per_thread = 2000;

  for (size_t i = 0; i < 8; i++) {
    std::thread t1([&]() {
      baton.write_access_start();
      writerThreadsID.push_back(std::this_thread::get_id());
      baton.write_access_end();

      for (size_t j = 0; j < count_per_thread; j++) {
        baton.write_access_start();
        queue.push_back(std::tuple<int, std::thread::id, std::string>(
            j, std::this_thread::get_id(), gen_random_writebaton(j % 30)));
        persistantQueue.push_back(std::tuple<int, std::thread::id, std::string>(
            j, std::this_thread::get_id(), gen_random_writebaton(j % 30)));
        std::this_thread::sleep_for(std::chrono::microseconds(40));
        total++;
        baton.write_access_end();
      }
      baton.write_access_start();
      finishedThread++;
      baton.write_access_end();
    });
    threads.push_back(std::move(t1));
  }

  for (size_t i = 0; i < 8; i++) {
    std::thread t1([&]() {
      baton.write_access_start();
      readerThreadsID.push_back(std::this_thread::get_id());
      baton.write_access_end();

      while (finishedThread != 8) {
        if (queue.empty()) {
          std::this_thread::sleep_for(std::chrono::microseconds(20));
          continue;
        }

        baton.read_access_start();
        resBaton.write_access_start();
        if (queue.empty()) {
          std::this_thread::sleep_for(std::chrono::microseconds(20));
          resBaton.write_access_end();
          baton.read_access_end();
          continue;
        }
        auto s = queue.back();
        queue.pop_back();
        std::sort(std::get<2>(s).begin(), std::get<2>(s).end());
        result.push_back(
            std::tuple<int, std::thread::id, std::string, std::thread::id>(
                std::get<0>(s), std::get<1>(s), std::get<2>(s),
                std::this_thread::get_id()));
        resBaton.write_access_end();
        std::this_thread::sleep_for(std::chrono::microseconds(20));
        baton.read_access_end();
      }
    });
    threads.push_back(std::move(t1));
  }

  for (int i = 0; i < 16; i++) {
    threads.at(i).join();
  }

  std::map<std::thread::id, int> resultCounter;
  std::map<std::thread::id, int> processedCounter;

  EXPECT_EQ(total, count_per_thread * 8);
  EXPECT_EQ(persistantQueue.size(), count_per_thread * 8);

  for (auto& wirterThreads : writerThreadsID) {
    resultCounter[wirterThreads] = 0;
  }

  for (auto& readerThreads : readerThreadsID) {
    processedCounter[readerThreads] = 0;
  }

  for (auto& item : persistantQueue) {
    resultCounter[std::get<1>(item)] = resultCounter[std::get<1>(item)] + 1;
  }

  for (const auto& [key, value] : resultCounter) {
    EXPECT_EQ(value, count_per_thread);
  }
}