
#pragma once



#include <array>
#include <algorithm>
#include <type_traits>


namespace thunder {

  namespace datastructures {


    template<
            typename Key,
            typename Value,
            typename Allocator,
            std::size_t NUMBER_OF_SLOT_IN_BUCKET,
            typename KeyEqual = std::equal_to<Key>>
    class bucket
    {
      public:

        //combination of key value
        using key = Key;
        using value = Value;
        using key_value_item = std::pair<key, value>;
        using key_const_value_item = std::pair<const key, value>;
        using key_const_value_const_item = std::pair<const key, const value>;

        //utility for easy access to allocator member functions
        using traits_ = typename std::allocator_traits<Allocator>::template rebind_traits<key_value_item>;
        using allocator_type = typename traits_::allocator_type;
        using size_type = typename traits_::size_type; 

        //below array indicate that is there any element in slot or not
        std::array<bool, NUMBER_OF_SLOT_IN_BUCKET> slotsStatus;

        //below array is used for store items in slots and preallocated
        std::array<
            std::aligned_storage_t<sizeof(key_value_item), alignof(key_value_item)>,
            NUMBER_OF_SLOT_IN_BUCKET> slots;
      public:
        bucket()
        {

        }

        bucket(bucket& other) = delete;

        bucket operator=(bucket& other) = delete;

        ~bucket()
        {
            
        }

        bool setKeyValueOnIndex(Key& key, Value& value, size_type ind)
        {
            if (!this->isSlotEmpty(ind)) {
                return false;
            }
            this->slots[ind] = std::make_pair(key, value);
            this->slotsStatus[ind] = true;
            return true;
        }

        key_const_value_item& returnKeyValue(size_type ind)
        {
            return reinterpret_cast<key_const_value_item&>(this->slots[ind]);
        }

        const key_const_value_item& returnKeyValue(size_type ind) const
        {
            return reinterpret_cast<const key_const_value_item&>(this->slots[ind]);
        }

        key_value_item& returnChangeableKeyValue(size_type ind)
        {
            return reinterpret_cast<key_value_item&>(this->slots[ind]);
        }

        key& returnKey(size_type ind)
        {
            return reinterpret_cast<key&>(this->slots[ind].first);
        }

        const key& returnKey(size_type ind) const
        {
            return reinterpret_cast<const key&>(this->slots[ind].first);
        }

        value& returnValue(size_type ind)
        {
            return reinterpret_cast<value&>(this->slots[ind].second);
        }

        const value& returnValue(size_type ind) const
        {
            return reinterpret_cast<const value&>(this->slots[ind].second);
        }

        bool isSlotEmpty(size_type ind)
        {
            return !this->slotsStatus[ind];
        }

        void setSlotStatus(size_type ind, bool status)
        {
            this->slotsStatus[ind] = status;
        }
    };

  }
}