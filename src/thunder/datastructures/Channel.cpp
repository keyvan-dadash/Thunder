

#include <type_traits>

#include <thunder/datastructures/Channel.hpp>
#include <thunder/datastructures/BaseConcurrentQueue.hpp>
#include <thunder/datastructures/ConcurrentQueue.hpp>

namespace thunder
{
  namespace datastructures 
  {

    template<typename Element, std::size_t QueueSize, template <typename _Tp, std::size_t _QueueSize> typename ChannelBackend>
    Element Channel<Element, QueueSize, ChannelBackend>::recvElement()
    {
      Element elem;
      channel_backend_.pop(elem);
      return elem;
    }

    template<typename Element, std::size_t QueueSize, template <typename _Tp, std::size_t _QueueSize> typename ChannelBackend>
    template<typename T>
    void Channel<Element, QueueSize, ChannelBackend>::sendElement(T&& t)
    {
      //TODO: we should block or maybe design api's
      channel_backend_.tryPush(std::forward<T>(t), kQueueSize);
    }
  }
}
