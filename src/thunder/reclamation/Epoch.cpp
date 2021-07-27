

#pragma once

#include <atomic>
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

        pthread_setspecific(this->global_epoch_key_, local_epoch);
        pthread_setspecific(this->global_recycle_key_, local_recycle);

        this->registered_thread_epoch_.push_back(local_epoch);
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

      assert(local_epoch != NULL);

      int64_t store_epoch = this->currentGlobalEpoch() | EPOCH_ACTIVE_FLAG;
      local_epoch->local_epoch.store(store_epoch, std::memory_order_relaxed);

      return epoch_guard(local_epoch);
      
    }

    template<typename T>
    void Epoch<T>::retireObj(std::remove_reference_t<T>* t)
    {
      struct LocalRecycle<T>* local_recycle = static_cast<LocalRecycle<T>*>(pthread_getspecific(this->global_recycle_key_));

      int64_t currentEpoch = this->currentGlobalEpoch();

      assert(local_recycle != NULL);

      std::pair<std::remove_reference_t<T>*, int64_t> item;
      item = std::make_pair(t, currentEpoch);

      local_recycle->retire_list_.push_back(item);
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
      int64_t currentGlobalEpoch = this->currentGlobalEpoch();
      for (auto &thread_local_epoch: this->registered_thread_epoch_) {
        int64_t thread_epoch = thread_local_epoch->local_epoch.load(std::memory_order_relaxed);
        bool isActive = (thread_epoch & EPOCH_ACTIVE_FLAG) != 0;
        if (isActive && currentGlobalEpoch != thread_epoch) {
          return;
        }
      }

      this->global_epoch_.store((currentGlobalEpoch + 1) % 3, std::memory_order_relaxed);
      int64_t reclamationEpoch = (this->currentGlobalEpoch() + 1) % 3;
      
      for (auto &thread_local_recycle : this->registered_thread_recycle_) {
        std::vector<std::pair<std::remove_reference_t<T>*, int64_t>> swap_list_;
        for (auto &[obj, epoch] : thread_local_recycle->retire_list_) {
          if (epoch == reclamationEpoch) {
            delete obj;
          } else {
            std::pair<std::remove_reference_t<T>*, int64_t> item;
            item = std::make_pair(obj, epoch);
            swap_list_.push_back(item);
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