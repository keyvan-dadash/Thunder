

#pragma once

#include <atomic>
#include <iostream>
#include <assert.h>
#include <memory>
#include <thread>
#include <vector>
#include <mutex>

#include "Epoch.hpp"

namespace thunder {


  namespace reclamation {

    
    template<typename T>
    Epoch<T>::Epoch()
    {
      this->registered_thread_epoch_.reserve(10);
      pthread_key_create(&(this->global_epoch_key_), [](void* param){
        free(param);
      });

      pthread_key_create(&(this->global_recycle_key_), [](void* param){
        free(param);
      });
    }

    template<typename T>
    Epoch<T>::~Epoch()
    {
      pthread_key_delete(this->global_epoch_key_);
      pthread_key_delete(this->global_recycle_key_);
    }

    template<typename T>
    void Epoch<T>::registerThisThread()
    {
      struct LocalEpoch* local_epoch;
      struct LocalRecycle<T>* local_recycle;

      local_epoch = static_cast<LocalEpoch*>(pthread_getspecific(this->global_epoch_key_));
      local_recycle = static_cast<LocalRecycle<T>*>(pthread_getspecific(this->global_recycle_key_));

      if (local_epoch == NULL && local_recycle == NULL)
      {
        std::lock_guard guard(this->mutex_);
        local_epoch = new LocalEpoch();
        local_recycle = new LocalRecycle<T>();

        printf("address is when register %p\n", (void *)local_epoch);

        local_epoch->local_epoch.store(this->currentGlobalEpoch(), std::memory_order_relaxed);

        pthread_setspecific(this->global_epoch_key_, local_epoch);
        pthread_setspecific(this->global_recycle_key_, local_recycle);

        this->registered_thread_epoch_.push_back(local_epoch);
        std::cout << this->registered_thread_epoch_.back()->local_epoch.load(std::memory_order_relaxed) << std::endl;
        std::cout << this->registered_thread_epoch_.front()->local_epoch.load(std::memory_order_relaxed) << std::endl;
        printf("address is when register vector %p\n", (void *)this->registered_thread_epoch_.back());
        this->registered_thread_recycle_.push_back(local_recycle);
      }
    }

    template<typename T>
    void Epoch<T>::unregistedThisThread()
    {
      
    }

    template<typename T>
    Epoch<T>::epoch_guard Epoch<T>::enterCriticalPath()
    {
      struct LocalEpoch* local_epoch = static_cast<LocalEpoch*>(pthread_getspecific(this->global_epoch_key_));

      printf("address is when critical %p\n", (void *)local_epoch);

      assert(local_epoch != NULL);

      std::cout << "before guard " << local_epoch->local_epoch.load(std::memory_order_relaxed) << std::endl;

      int64_t store_epoch = this->currentGlobalEpoch() | EPOCH_ACTIVE_FLAG;
      local_epoch->local_epoch.store(store_epoch, std::memory_order_relaxed);

      std::cout << "after guard spec" << local_epoch->local_epoch.load(std::memory_order_relaxed) << std::endl;

      return epoch_guard(local_epoch, [this](){return currentGlobalEpoch();});
      
    }

    template<typename T>
    void Epoch<T>::retireObj(std::remove_reference_t<T>* t)
    {
      struct LocalRecycle<T>* local_recycle = static_cast<LocalRecycle<T>*>(pthread_getspecific(this->global_recycle_key_));

      int64_t currentEpoch = this->currentGlobalEpoch();

      assert(local_recycle != NULL);

      std::pair<std::remove_reference_t<T>*, int64_t> item;
      item = std::make_pair(t, currentEpoch);

      std::cout << local_recycle->retire_list_.size() << std::endl;

      local_recycle->retire_list_.push_back(std::move(item));
      std::cout << this->registered_thread_recycle_.front()->retire_list_.size() << std::endl;
    }

    template<typename T>
    void Epoch<T>::recycleLocalThread()
    {
      struct LocalRecycle<T>* local_recycle = pthread_getspecific(this->global_recycle_key_);

      assert(local_recycle != NULL);

      int64_t currentGlobalEpoch = this->currentGlobalEpoch() + 1;

      std::vector<std::pair<std::remove_reference_t<T>, int64_t>> swap_list_;
      for (auto &[obj, epoch] : local_recycle->retire_list_) {
        if (epoch == currentGlobalEpoch) {
          delete obj;
        } else {
          swap_list_.push_back(std::make_pair<T*, int64_t>(obj, epoch));
        }
      }

      local_recycle->retire_list_ = std::move(swap_list_);
    }

    template<typename T>
    void Epoch<T>::sync()
    {
       struct LocalEpoch* local_epoch = static_cast<LocalEpoch*>(pthread_getspecific(this->global_epoch_key_));

       printf("address is %p\n", (void *)local_epoch);
       printf("address butt %p\n", (void *)this->registered_thread_epoch_.back());
       printf("address front %p\n", (void *)this->registered_thread_epoch_.front());

      assert(local_epoch != NULL);

      // std::cout << local_epoch->local_epoch.load(std::memory_order_relaxed) << std::endl;
      
      int64_t currentGlobalEpoch = this->currentGlobalEpoch();
      std::cout << this->registered_thread_epoch_.back()->local_epoch.load(std::memory_order_relaxed) << std::endl;
      std::cout << this->registered_thread_epoch_.front()->hoho << std::endl;
      for (auto thread_local_epoch: this->registered_thread_epoch_) {
        int64_t thread_epoch = thread_local_epoch->local_epoch.load(std::memory_order_relaxed);
        bool isActive = (thread_epoch & EPOCH_ACTIVE_FLAG) != 0;
        if (isActive && currentGlobalEpoch != thread_epoch) {
          std::cout << "fuck " << isActive << " " << currentGlobalEpoch << " " << thread_epoch << std::endl;
          return;
        }
      }

      std::cout << "lskdfjsldkjflksd" << std::endl;
      std::cout << this->registered_thread_recycle_.size() << std::endl;
      std::cout << this->registered_thread_recycle_.front()->retire_list_.size() << std::endl;

      this->global_epoch_.store((currentGlobalEpoch + 1) % 3, std::memory_order_relaxed);
      int64_t reclamationEpoch = (this->currentGlobalEpoch() + 1) % 3;
      
      for (auto &thread_local_recycle : this->registered_thread_recycle_) {
        std::vector<std::pair<std::remove_reference_t<T>*, int64_t>> swap_list_;
        for (auto &item : thread_local_recycle->retire_list_) {
          std::cout << thread_local_recycle->retire_list_.size() << std::endl;
          std::cout << "helooo " << item.second << std::endl;
          if (item.second == reclamationEpoch) {
            delete item.first;
          } else {
            std::pair<std::remove_reference_t<T>*, int64_t> not_ready_item;
            not_ready_item = std::make_pair(item.first, item.second);
            swap_list_.push_back(not_ready_item);
          }
        }

        thread_local_recycle->retire_list_ = std::move(swap_list_);
      }
    }

    template<typename T>
    int64_t Epoch<T>::currentGlobalEpoch()
    {
      return this->global_epoch_.load(std::memory_order_relaxed);
    }
  }
}