/**
 * @file Channel
 * Contains Channel class
 */
#pragma once

#include <thunder/datastructures/BaseConcurrentQueue.hpp>
#include <thunder/datastructures/ConcurrentQueue.hpp>
#include <type_traits>

namespace thunder {
namespace datastructures {

/**
 * \brief Channel user for communication between threads.
 * \tparam Element which channel send or recv.
 * \tparam Maximum size of channel.
 * \tparam ChannelBackend is a class which should inherith from BaseConcurrentQueue,
 * and this class use as backend for transmit or recive element to or from threads in safe manner.
 *
 * Channel is a simple type which can help diffrent threads communicate with each other without
 * losing elements or any other race condition. Setting right ChannelBackend for right circumstance
 * results in highperformance and ease api usage for multithread envirment.
 */
template <typename Element, std::size_t QueueSize,
          template <typename _Tp, std::size_t _QueueSize>
          typename ChannelBackend>
class Channel {
 public:
  Channel();

  ~Channel() = default;

  /**
   * \brief Recieve element from channel.
   * \return Recieveed element from channel
   */
  Element recvElement();

  /**
   * \brief Send element inside channel.
   * \tparam Type of element for seding in channel.
   */
  template <typename T>
  void sendElement(T&& t);

 private:
  static const std::size_t kQueueSize = QueueSize; /// Maximum size of channel.

  ChannelBackend<Element, QueueSize> channel_backend_; /// Underlaying ChannelBackend for stroing elements.

  static_assert(std::is_base_of_v<BaseConcurrentQueue<Element>,
                                  ChannelBackend<Element, QueueSize>>,
                "ChannelBackend should inherith from BaseConcurrentQueue");
};
}  // namespace datastructures
}  // namespace thunder
