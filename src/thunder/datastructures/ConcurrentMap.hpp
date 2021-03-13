

#include <thunder/datastructures/bucket.hpp>



namespace thunder {
  namespace datastructures {


    template<
            class Key,
            class Value,
            std::size_t NUMBER_OF_SLOT_IN_BUCKET,
            class KeyEqual = std::equal_to<Key>,
            class Allocator = std::allocator<std::pair<Key, Value>>,
            class Hash = std::hash<Key>>
    class ConcurrentMap
    {

    };


  }
}