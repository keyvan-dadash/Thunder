/**
 * @file AbstractMutex.hpp
 * Contains AbstractMutex class which all other implementation of
 * mutex iherith from.
 */
#pragma once

namespace thunder {

namespace synchronization {

/**
 * \brief Abstract class for mutex.
 *
 * AbstractMutex is a abstract class which specify what api's
 * should mutex expose.
 */
class AbstractMutex {
 public:
  AbstractMutex() = default;

  /**
   * \brief lock the mutex.
   */
  virtual void lock() = 0;

  /**
   * \brief unlock the mutex.
   */
  virtual void unlock() = 0;

  /**
   * \brief check whatever the muted is Locked or not.
   * \return boolean which tell is the mutex locked or not.
   */
  virtual bool isLocked() = 0;
};
};  // namespace synchronization
}  // namespace thunder
