

// #include <type_traits>
// #include <atomic>
// #include <list>
// #include <vector>

// #include <thunder/datastructures/bucket.hpp>
// #include <thunder/synchronization/SpinLock.hpp>

// namespace thunder {
//   namespace datastructures {

//     const std::size_t DEFAULT_NUMBER_OF_SLOT_IN_BUCKET = 4;
//     const std::size_t DEFAULT_MAP_SIZE = (1U << 4) *
//     DEFAULT_NUMBER_OF_SLOT_IN_BUCKET; const std::size_t
//     MAXIMUM_NUMBER_OF_LOCKS = (1U << 16);

//     template<
//             class Key,
//             class Value,
//             std::size_t NUMBER_OF_SLOT_IN_BUCKET =
//             DEFAULT_NUMBER_OF_SLOT_IN_BUCKET, class KeyEqual =
//             std::equal_to<Key>, class Allocator =
//             std::allocator<std::pair<Key, Value>>, class Hash =
//             std::hash<Key>>
//     class ConcurrentMap
//     {
//       private:

//         class bucket_manager;

//         //bucket manager traits and allocators
//         using bucket_manager_traits_ = typename
//         std::allocator_traits<Allocator>::template
//         rebind_traits<bucket_manager>; using bucket_manager_allocator_type =
//         typename bucket_manager_traits_::allocator_type; using
//         bucket_manager_size_type = typename
//         bucket_manager_traits_::size_type;

//       public:

//         ConcurrentMap(Hash hasher = Hash(),
//                       Allocator allocator = Allocator(),
//                       KeyEqual key_equal = KeyEqual(),
//                       bucket_manager_size_type sizeMap = DEFAULT_MAP_SIZE) :
//                       all_locks_(allocator)
//         {
//           this->buckets_.reset(new bucket_manager(sizeMap, allocator));

//           this->all_locks_.emplace_back(std::min(this->buckets_.get()->size(),
//           MAXIMUM_NUMBER_OF_LOCKS));
//         }

//         //insert key and value inside map but not participate if map is full
//         and going to expand
//         //therefore it will return immediatly if map is under expanding or is
//         full bool insert(Key&& key, Value&& value)
//         {
//             return true;
//         }

//       private:

//         //in future maybe i extract this class
//         class bucket_manager
//         {
//           private:

//             friend class ConcurrentMap;
//             //bucket type for easy access
//             using bucket_t = bucket<Key, Value, Allocator,
//             DEFAULT_NUMBER_OF_SLOT_IN_BUCKET, KeyEqual>;

//           public:
//             using bucket_internal_traits = typename bucket_t::traits_;
//             using bucket_internal_size_type = typename bucket_t::size_type;
//             using bucket_internal_allocator = typename
//             bucket_t::allocator_type;

//             bucket_internal_allocator bucket_internal_allocator_;

//             //preaper allocator and traits
//             //honestly i dont understand this traits and stuff's at first
//             //but after research i findout what is going under the hood
//             //i will prepare resource here for those who want know more about
//             this(or maybe myself after while)
//             //resource:
//             //
//             https://stackoverflow.com/questions/14148756/what-does-template-rebind-do
//           private:
//             using bucket_traits = typename
//             std::allocator_traits<Allocator>::template
//             rebind_traits<bucket_t>;

//             bucket_allocator_type bucket_allocator_;

//             bucket_pointer buckets_;

//             std::atomic<bucket_internal_size_type> hashpower_;

//             bucket_internal_size_type sizeMap;

//           public:
//             using bucket_allocator_type = typename
//             bucket_traits::allocator_type; using bucket_const_pointer =
//             typename bucket_traits::const_pointer; using bucket_size_type =
//             typename bucket_traits::size_type; using bucket_pointer =
//             typename bucket_traits::pointer;

//           public:

//             bucket_manager(bucket_internal_size_type size, Allocator& alloc)
//             :
//                                                                     bucket_allocator_(alloc),
//                                                                     bucket_internal_allocator_(alloc)

//             {
//               hashpower(size);
//               this->buckets_ =
//               this->bucket_allocator_.allocate(this->size()); for
//               (bucket_internal_size_type i = 0; i < this->size(); i++) {
//                 bucket_internal_traits::construct(bucket_internal_allocator_,
//                 &buckets_[i]);
//               }

//             }

//             ~bucket_manager()
//             {
//               this->destroy();
//             }

//             template<typename K, typename V>
//             bool setKeyValue(bucket_internal_size_type index,
//             bucket_internal_size_type slot, K key, V value)
//             {

//               bucket_t& b = buckets_[index];
//               if (!b.isSlotEmpty(index)) {
//                 return false;
//               }

//               bucket_internal_traits::construct(bucket_internal_allocator_,
//                                                 std::addressof(b.returnChangeableKeyValue(slot)),
//                                                 std::piecewise_construct,
//                                                 std::forward_as_tuple(std::forward<K>(key)),
//                                                 std::forward_as_tuple(std::forward<V>(value)));

//               b->setSlotStatus(false);

//               return true;
//             }

//             bool earseKeyValue(bucket_internal_size_type index,
//             bucket_internal_size_type slot)
//             {

//               bucket_t& b = buckets_[index];
//               if (b.isSlotEmpty(index)) {
//                 return false;
//               }

//               bucket_internal_traits::destroy(bucket_internal_allocator_,
//                                                 std::addressof(b.returnChangeableKeyValue(slot)));

//               b.setSlotStatus(slot, true);

//               return true;
//             }

//             bool clear()
//             {
//               for (bucket_internal_size_type i = 0; i < this->size(); i++) {

//                 bucket_t& b = buckets_[i];
//                 for (int j = 0; j < NUMBER_OF_SLOT_IN_BUCKET; j++) {

//                   if (!b.isSlotEmpty(j)) {
//                     bool isCleaned = this->earseKeyValue(i, j);

//                     if (!isCleaned) {
//                       return false;
//                     }
//                   }
//                 }
//               }

//               return true;
//             }

//             void destroy()
//             {
//               if (this->buckets_ == nullptr) {
//                 return;
//               }

//               this->clear();
//               for (bucket_internal_size_type i = 0; i < this->size(); i++) {
//                 bucket_traits::destroy(bucket_allocator_,
//                                                 &buckets_[i]);
//               }

//               this->bucket_allocator_.deallocate(buckets_, this->size());
//               this->buckets_ = nullptr;
//             }

//             bucket_internal_size_type hashpower()
//             {
//               return hashpower_.load(std::memory_order_acquire);
//             }

//             void hashpower(bucket_internal_size_type newHashPower)
//             {
//               hashpower_.store(newHashPower, std::memory_order_release);
//             }

//             bucket_internal_size_type size()
//             {
//               return bucket_internal_size_type(1) << hashpower();
//             }

//             bucket_t& operator[](bucket_internal_size_type index)
//             {
//               return buckets_[index];
//             }

//         };

//         class BucketSpinLock {

//           public:
//             BucketSpinLock() : isMigrated_(true), numberOfElements_(0)
//             {
//               this->spinlock_.get()->clear();
//             }

//             BucketSpinLock(const BucketSpinLock& other)
//             {
//               this->isMigrated_(other.isMigrated_);
//               this->numberOfElements_(other.numberOfElements_);
//               this->spinlock_.get()->clear();
//             }

//             ~BucketSpinLock()
//             {
//               this->spinlock_.get()->clear();
//             }

//             void lock() noexcept
//             {
//               this->spinlock_.get()->lock();
//             }

//             void unlock() noexcept
//             {
//               this->spinlock_.get()->unlock();
//             }

//             bool trylock()
//             {
//               return this->spinlock_.get()->try_lock();
//             }

//             void clear()
//             {
//               this->spinlock_.get()->clear();
//             }

//             bool isMigrated()
//             {
//               return this->isMigrated_;
//             }

//             void setMigration(bool migrate)
//             {
//               this->isMigrated_ = migrate;
//             }

//           private:

//             std::unique_ptr<thunder::synchronization::SpinLock> spinlock_;
//             int64_t numberOfElements_;
//             bool isMigrated_;

//             char padding[thunder::synchronization::hardware_constructive_interference_size - \
//             (sizeof(spinlock_) + sizeof(int64_t) + sizeof(bool))];
//         };

//         static_assert(
//           sizeof(BucketSpinLock) ==
//           thunder::synchronization::hardware_destructive_interference_size,
//          "BucketSpinLock not in size cacheline");

//         template <typename T>
//         using rebind_T_Alloc = typename
//         std::allocator_traits<Allocator>::template rebind_alloc<T>;

//         std::unique_ptr<bucket_manager> buckets_ = nullptr;
//         std::unique_ptr<bucket_manager> old_buckets_ = nullptr;

//         using locks_t = typename std::vector<BucketSpinLock,
//         rebind_T_Alloc<BucketSpinLock>>; using all_locks_t = typename
//         std::list<locks_t,  rebind_T_Alloc<locks_t>>;

//         all_locks_t all_locks_;

//         struct UnlockOnDestruction {
//           void operator()(BucketSpinLock* lock) const
//           {
//             lock->unlock();
//           }
//         };

//         using SafeLock = std::unique_ptr<BucketSpinLock,
//         UnlockOnDestruction>;

//         using lock_all = std::integral_constant<bool, true>;
//         using normal_lock = std::integral_constant<bool, false>;

//         template<typename LOCK_MODE>
//         class TwoLockOnBucket {

//           public:
//             template<typename = std::enable_if_t<std::is_same<LOCK_MODE,
//             lock_all>::value, LOCK_MODE>>
//             TwoLockOnBucket(bucket_manager_size_type lock1,
//             bucket_manager_size_type lock2) : first_lock_index_(lock1),
//                                                                                               second_lock_index_(lock2)
//             {}

//             template<typename = std::enable_if_t<std::is_same<LOCK_MODE,
//             normal_lock>::value, LOCK_MODE>> TwoLockOnBucket(lock_all& locks,
//             bucket_manager_size_type lock1, bucket_manager_size_type lock2) :
//                           first_lock_index_(lock1),
//                           second_lock_index_(lock2),
//                           firstLock_(get_locks(lock1)),
//                           secondLock_(get_locks(lock1) != get_locks(lock2) ?
//                           get_locks(lock2) : nullptr)

//             {
//             }

//             unlock()
//             {
//               this->firstLock_.reset();
//               this->secondLock_.reset();
//             }

//           size_t first_lock_index_, second_lock_index_;

//           private:
//             SafeLock firstLock_, secondLock_;

//         };

//         struct UnlockTwoLockOnBucket {
//           void operator()(TwoLockOnBucket* twoLock) const
//           {
//             twoLock->unlock();
//           }
//         }

//         using SafeTwoLockOnBucket =
//         std::unique_ptr<TwoLockOnBucket,UnlockTwoLockOnBucket>;

//         template<typename LOCK_MODE, typename =
//         std::enable_if_t<std::is_same<LOCK_MODE, lock_all>::value,
//         LOCK_MODE>> SafeLock lock_one_bucket(bucket_manager_size_type hp,
//         bucket_manager_size_type lock1, lock_all)
//         {
//           return SafeLock();
//         }

//         template<typename LOCK_MODE, typename =
//         std::enable_if_t<std::is_same<LOCK_MODE, normal_lock>::value,
//         LOCK_MODE>> SafeLock
//         lock_one_bucket(bucket_manager::bucket_internal_size_type hp,
//         bucket_manager_size_type lock1, normal_lock)
//         {
//           BucketSpinLock& spinlock = get_locks(lock1);
//           spinlock->lock();
//           if (this->buckets_.get()->hashpower() != hp) {
//             spinlock.unlock();
//             return SafeLock();
//           }
//           return SafeLock(spinlock);

//         }

//         template<typename LOCK_MODE, typename =
//         std::enable_if_t<std::is_same<LOCK_MODE, lock_all>::value,
//         LOCK_MODE>> SafeLock
//         lock_two_bucket(bucket_manager::bucket_internal_size_type hp,
//         bucket_manager_size_type lock1, bucket_manager_size_type lock2,
//         lock_all)
//         {
//           return SafeTwoLockOnBucket(current_locks, lock1, lock2,
//           lock_all());
//         }

//         template<typename LOCK_MODE, typename =
//         std::enable_if_t<std::is_same<LOCK_MODE, normal_lock>::value,
//         LOCK_MODE>> SafeLock
//         lock_two_bucket(bucket_manager::bucket_internal_size_type hp,
//         bucket_manager_size_type lock1, bucket_manager_size_type lock2,
//         normal_lock)
//         {
//           lock_all current_locks = get_current_locks();
//           BucketSpinLock& spinlock1 = get_locks(lock1);
//           BucketSpinLock& spinlock2 = get_locks(lock2);
//           if (lock1 > lock2) {
//             std::swap(lock1, lock2);
//           }
//           spinlock1->lock();
//           if (this->buckets_.get()->hashpower() != hp) {
//             spinlock1.unlock();
//             return SafeTwoLockOnBucket(current_locks, lock1, lock2,
//             lock_all()); //TODO: we should return error
//           }
//           if (lock1 != lock2) {
//             spinlock2->lock();
//           }
//           return SafeTwoLockOnBucket(current_locks, lock1, lock2,
//           normal_lock());
//         }

//     };

//   }
// }