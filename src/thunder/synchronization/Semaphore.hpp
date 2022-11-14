

#include <semaphore.h>

namespace thunder {
namespace synchronization {

class Semaphore {
 public:
  Semaphore(int counter = 1) { sem_init(&semaphore, 0, counter); }

  ~Semaphore() { sem_close(&semaphore); }

  void release() noexcept { sem_post(&semaphore); }

  void acquire() noexcept { sem_wait(&semaphore); }

 private:
  sem_t semaphore;
};
}  // namespace synchronization

}  // namespace thunder