#define CATCH_CONFIG_MAIN

#include <vector>
#include <thread>
#include <iostream>
#include <mutex>
#include <cstdint>
#include <cassert>
#include <random>
#include <iostream>
#include <random>
#include <condition_variable>

#include <gtest/gtest.h>

#include <thunder/reclamation/Epoch.hpp>


// struct Data {
//     uint64_t p;
//     uint64_t q;
//     Data(){
//         p = 0;
//         q = 0;
//     }
// };

// TEST(EpochBasicTest, BasicEpoch) {

//     constexpr int num_data = 2;
//     std::vector<std::atomic<Data*>> arr_data(num_data);
    
//     for(int i=0;i<num_data;++i){
//         auto d = new Data();
//         d->p = i;
//         arr_data[i].store( d );
//     }
    
//     int numthread = std::thread::hardware_concurrency();

//     std::vector<std::thread> th;    

//     std::mutex mtx_write;

//     unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
//     std::default_random_engine gen(seed);

//     int sync_count = 0;

//     std::mutex m_sync;
//     std::condition_variable cv;

//     std::map<int, int> removed_count;
    
//     thunder::reclamation::Epoch<Data> ebr;

//     auto f = [&](int id){
//         ebr.registerThisThread();
//         {
//             std::scoped_lock<std::mutex> lock(m_sync);
//             ++sync_count;
//             cv.notify_all();
//         }
        
//         {
//             std::unique_lock<std::mutex> lock(m_sync);
//             cv.wait(lock, [&](){return sync_count>=numthread;});
//         }
        
//         std::uniform_int_distribution<int> distrib(0,3);
//         std::uniform_int_distribution<int> distrib_arr(0,num_data-1);
    
//         int count_read = 0;
//         int count_removed = 0;
    
//         std::vector<int> read;
//         std::vector<int> removed;

//         int chunk = num_data * 800000;
//         for(int i=0;i<chunk;++i){

//             int num = distrib(gen);
//             int arr_idx = distrib_arr(gen);

//             auto guard = ebr.enterCriticalPath();

//             if(num<=1){
//                 Data * ptr = arr_data[arr_idx].load(std::memory_order_acquire);
//                 if(ptr){
//                     auto val = ptr->p;
//                     read.push_back(val);
//                     ++count_read;
//                 }
//             }else{
//                 auto d_new = new Data();
//                 d_new->p = num_data + id * chunk + i;
                
//                 Data * ptr = arr_data[arr_idx].load(std::memory_order_relaxed);
//                 if(ptr && arr_data[arr_idx].compare_exchange_strong(ptr, d_new)){
//                     removed.push_back(ptr->p);
//                     ebr.retireObj(ptr);
//                     ++count_removed;
//                 }else{
//                     delete d_new;
//                 }
//             }
//         }
        
//         {
//             std::scoped_lock<std::mutex> lock(m_sync);
//             ++sync_count;
//             cv.notify_all();
//         }
        
//         {
//             std::unique_lock<std::mutex> lock(m_sync);
//             cv.wait(lock, [&](){return sync_count==numthread*2;});
//         }
        
//         {
//             std::lock_guard<std::mutex> lock(mtx_write);
//             std::cout << "count_read: " << count_read << ", count removed: " << count_removed << std::endl;
//         }

//         {
//             std::scoped_lock<std::mutex> lock(m_sync);
//             for(auto i:removed){
//                 removed_count[i]++;
//             }
//         }

//         // ebr.sync();
//     };
    
//     for( int i = 0; i < numthread; ++i ){
//         th.push_back( std::thread(f, i) );
//     }
    
//     for( auto & i: th ){
//         i.join();
//     }
        
//     int count_remain = 0;
//     for(auto &i: arr_data){
//         auto d = i.load();
//         if(d){
//             ++count_remain;
//             delete d;
//         }
//     }
//     ebr.sync();

//     std::cout << "count final remain: " << count_remain << std::endl;
//     for(auto [key,count]: removed_count){
//         if(count!=1) std::cout <<"count: " << count << std::endl;
//         EXPECT_EQ(count, 1);
//     }
// }


TEST(EpochBasicTest, EpochRelamation)
{
  struct Data
  {
    int32_t p;
    ~Data()
    {
      std::cout << "des " << this->p << std::endl;
    }
  } data_t;

  std::mutex m_;
  std::condition_variable cv;

  bool isReady = false;

  int32_t number_of_data = 100;

  thunder::reclamation::Epoch<Data> ebr;

  std::vector<std::atomic<Data*>> arr_data(number_of_data);;

  auto reader_fn = [&]()
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ebr.registerThisThread();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    isReady = true;
    cv.notify_one();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));


    for(int i=0;i < number_of_data;++i){
      ebr.sync();
      auto guard = ebr.enterCriticalPath();
        auto data = arr_data[i].load(std::memory_order_acquire);
        (*data).p = 43543;
    }

  };

  auto writer_fn = [&]()
  {
    ebr.registerThisThread();
    for(int i=0;i < number_of_data;++i){
        auto d = new Data();
        d->p = i;
        arr_data[i].store( d );
    }

    std::unique_lock<std::mutex> lock(m_);
    cv.wait(lock, [&](){return isReady;});

    for(int i=0;i < number_of_data;++i){
        auto data = arr_data[i].load(std::memory_order_acquire);
        ebr.retireObj(data);
        // delete data;
        // auto d = new Data();
        // d->p = i * 2;
        // arr_data[i].store( d );
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));

  };

  std::thread reader_thread(reader_fn);
  std::thread writer_thread(writer_fn);


  reader_thread.join();
  writer_thread.join();


  // ebr.sync();
  for(int i=0;i < number_of_data;++i){
        auto data = arr_data[i].load(std::memory_order_acquire);
        std::cout << "call " << (*data).p << std::endl;
        (*data).p = 4345343;
    }

  //     for( int i = 0; i < numthread; ++i ){
//         th.push_back( std::thread(f, i) );
//     }
    
//     for( auto & i: th ){
//         i.join();
//     }
}