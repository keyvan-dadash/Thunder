/**
 * @file Semaphore.hpp
 * Contains Semaphore class.
 */

#pragma once

#include <semaphore.h>

namespace thunder {
namespace synchronization {

/**
 * \brief Wrapper around libc semaphore.
 *
 * Semaphore is use for limiting access to priticualr resouce.
 */
class Semaphore {
 public:

  /**
   * \brief Constructor of semaphore class.
   * \param counter is the initial vlaue of semaphore and default is 1.
   */
  Semaphore(int counter = 1) { sem_init(&semaphore, 0, counter); }

  ~Semaphore() { sem_close(&semaphore); }

  /**
   * \brief Release aquired semaphore and increase counter by 1.
   */
  void release() noexcept { sem_post(&semaphore); }

  /**
   * \brief Aquire semaphore and decrease counter by 1.
   */
  void acquire() noexcept { sem_wait(&semaphore); }

 private:
  sem_t semaphore; /// semaphore struct in libc.
};
}  // namespace synchronization

}  // namespace thunder
