
#pragma once

#include <thunder/datastructures/BaseConcurrentQueue.hpp>
#include <thunder/datastructures/ConcurrentQueue.hpp>
#include <type_traits>

namespace thunder {
namespace datastructures {

template <typename Element, std::size_t QueueSize,
          template <typename _Tp, std::size_t _QueueSize>
          typename ChannelBackend>
class Channel {
 public:
  Channel();

  ~Channel() = default;

  Element recvElement();

  template <typename T>
  void sendElement(T&& t);

 private:
  static const std::size_t kQueueSize = QueueSize;

  ChannelBackend<Element, QueueSize> channel_backend_;

  static_assert(std::is_base_of_v<BaseConcurrentQueue<Element>,
                                  ChannelBackend<Element, QueueSize>>,
                "ChannelBackend should inherith from BaseConcurrentQueue");
};
}  // namespace datastructures
}  // namespace thunder
