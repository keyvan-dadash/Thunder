

#include <type_traits>
#include <atomic>


#include <thunder/datastructures/bucket.hpp>



namespace thunder {
  namespace datastructures {

    const std::size_t DEFAULT_NUMBER_OF_SLOT_IN_BUCKET = 4;
    const std::size_t DEFAULT_MAP_SIZE = (1U << 16) * DEFAULT_NUMBER_OF_SLOT_IN_BUCKET;

    template<
            class Key,
            class Value,
            std::size_t NUMBER_OF_SLOT_IN_BUCKET = DEFAULT_NUMBER_OF_SLOT_IN_BUCKET,
            class KeyEqual = std::equal_to<Key>,
            class Allocator = std::allocator<std::pair<Key, Value>>,
            class Hash = std::hash<Key>>
    class ConcurrentMap
    {
      private:

        class bucket_manager;

        //bucket manager traits and allocators
        using bucket_manager_traits_ = typename std::allocator_traits<Allocator>::template rebind_traits<bucket_manager>;
        using bucket_manager_allocator_type = typename bucket_manager_traits_::allocator_type;
        using bucket_manager_size_type = typename bucket_manager_traits_::size_type;


      public:


        ConcurrentMap(Hash hasher = Hash(),
                      Allocator allocator = Allocator(),
                      KeyEqual key_equal = KeyEqual(),
                      bucket_manager_size_type sizeMap = DEFAULT_MAP_SIZE) : buckets_(sizeMap, allocator)
        {

        }


        //insert key and value inside map but not participate if map is full and going to expand
        //therefore it will return immediatly if map is under expanding or is full 
        bool insert(Key&& key, Value&& value)
        {
            return true;
        }









      private:

        //in future maybe i extract this class
        class bucket_manager
        {
          private:

            friend class ConcurrentMap;
            //bucket type for easy access
            using bucket_t = bucket<Key, Value, Allocator, DEFAULT_NUMBER_OF_SLOT_IN_BUCKET, KeyEqual>;
            using bucket_internal_traits = typename bucket_t::traits_;
            using bucket_internal_size_type = typename bucket_t::size_type;
            using bucket_internal_allocator = typename bucket_t::allocator_type;

            bucket_internal_allocator bucket_internal_allocator_;

            //preaper allocator and traits 
            //honestly i dont understand this traits and stuff's at first
            //but after research i findout what is going under the hood
            //i will prepare resource here for those who want know more about this(or maybe myself after while)
            //resource:
            //  https://stackoverflow.com/questions/14148756/what-does-template-rebind-do
            using bucket_traits = typename std::allocator_traits<Allocator>::template rebind_traits<bucket_t>;
            using bucket_allocator_type = typename bucket_traits::allocator_type;
            using bucket_const_pointer = typename bucket_traits::const_pointer;
            using bucket_size_type = typename bucket_traits::size_type;
            using bucket_pointer = typename bucket_traits::pointer;


            bucket_allocator_type bucket_allocator_;

            bucket_pointer buckets_;

            std::atomic<bucket_internal_size_type> hashpower_;

            bucket_internal_size_type sizeMap;


          public:

            bucket_manager(bucket_internal_size_type size, Allocator& alloc) : 
                                                                    bucket_allocator_(alloc), 
                                                                    bucket_internal_allocator_(alloc),
                                                                    buckets_(bucket_allocator_.allocate(this->size()))
                                                                    
            {
              hashpower(size);
              for (bucket_internal_size_type i = 0; i < this->size(); i++) {
                  bucket_internal_traits::construct(bucket_internal_allocator_, &buckets_[i]);
              }

            }

            ~bucket_manager() 
            {
              this->destroy();
            }



            template<typename K, typename V>
            bool setKeyValue(bucket_internal_size_type index, bucket_internal_size_type slot, K key, V value) 
            {

              bucket_t& b = buckets_[index];
              if (!b.isSlotEmpty(index)) {
                return false;
              }

              bucket_internal_traits::construct(bucket_internal_allocator_, 
                                                std::addressof(b.returnChangeableKeyValue(slot)),
                                                std::piecewise_construct,
                                                std::forward_as_tuple(std::forward<K>(key)),
                                                std::forward_as_tuple(std::forward<V>(value)));

              b->setSlotStatus(false);

              return true;
            }

            bool earseKeyValue(bucket_internal_size_type index, bucket_internal_size_type slot) 
            {

              bucket_t& b = buckets_[index];
              if (b.isSlotEmpty(index)) {
                return false;
              }

              bucket_internal_traits::destroy(bucket_internal_allocator_, 
                                                std::addressof(b.returnChangeableKeyValue(slot)));

              b->setSlotStatus(true);

              return true;
            }

            bool clear() 
            {
              for (bucket_internal_size_type i = 0; i < this->size(); i++) {

                bucket_t& b = buckets_[i];
                for (int j = 0; j < NUMBER_OF_SLOT_IN_BUCKET; j++) {

                  if (!b.isSlotEmpty(j)) {
                    bool isCleaned = this->earseKeyValue(i, j);

                    if (!isCleaned) {
                      return false
                    }
                  }
                }
              }

              return true
            }


            void destroy()
            {
              if (this->buckets_ == nullptr) {
                return;
              }

              this->clear();
              for (bucket_internal_size_type i = 0; i < this->size(); i++) {
                bucket_internal_traits::destroy(bucket_internal_allocator_, 
                                                &buckets_[i]);
              }

              this->bucket_allocator_.deallocate(buckets_, this->size());
              this->bucket_ = nullptr;
            }



            bucket_internal_size_type hashpower()
            {
              return hashpower_.load(std::memory_order_acquire);
            }

            void hashpower(bucket_internal_size_type newHashPower)
            {
              hashpower_.store(std::memory_order_release);
            }

            bucket_internal_size_type size()
            {
              return bucket_internal_size_type(1) << hashpower();
            }

            bucket_t& operator[](bucket_internal_size_type index)
            {
              return buckets_[index];
            }

        };


        bucket_manager buckets_;


    };


  }
}