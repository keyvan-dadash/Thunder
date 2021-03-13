

#include <thunder/datastructures/bucket.hpp>



namespace thunder {
  namespace datastructures {

    const std::size_t DEFAULT_NUMBER_OF_SLOT_IN_BUCKET = 4;

    template<
            class Key,
            class Value,
            std::size_t NUMBER_OF_SLOT_IN_BUCKET = DEFAULT_NUMBER_OF_SLOT_IN_BUCKET,
            class KeyEqual = std::equal_to<Key>,
            class Allocator = std::allocator<std::pair<Key, Value>>,
            class Hash = std::hash<Key>>
    class ConcurrentMap
    {
      public:


        bool insert(Key&& key, Value&& value)
        {
            return true;
        }


    };


  }
}