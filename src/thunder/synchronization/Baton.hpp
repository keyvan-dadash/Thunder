

#include <atomic>
#include <functional>
#include <semaphore>
#include <iostream>


namespace thunder {


  namespace synchronization {

    template <bool MayBlock = true, template <typename> class Atom = std::atomic>
    class Baton
    {
      public:
        Baton() 
        {
            this->priority_function_ = [](int, int){return true;};
        }

        Baton(std::function<bool(int, int)> isReaderHavingHigherPriority)
        {
            this->priority_function_ = isReaderHavingHigherPriority;
        }

        ~Baton()
        {

        }

        void read_access_start()
        {
            this->entry_signal.acquire();
            if (this->number_of_writer_ > 0)
            {
                this->delayed_reader_ += 1;
                this->entry_signal.release();
                this->reader_signal.acquire();
            }
            this->number_of_reader_++;
            if (this->delayed_reader_ > 0)
            {
                this->delayed_reader_--;
                this->reader_signal.release();
            }
            else
            {
                this->entry_signal.release();
            }
        }

        void read_access_end()
        {
            this->entry_signal.acquire();
            this->number_of_reader_--;
            if (this->number_of_reader_ == 0 && this->delayed_writer_ > 0)
            {
                this->delayed_writer_--;
                this->writer_signal.release();
            }
            else
            {
                this->entry_signal.release();
            }
        }

        void write_access_start()
        {
            this->entry_signal.acquire();
            if (this->number_of_reader_ > 0 || this->number_of_writer_ > 0)
            {
                this->delayed_writer_++;
                this->entry_signal.release();
                this->writer_signal.acquire();
            }

            this->number_of_writer_++;
            this->entry_signal.release();
        }

        void write_access_end()
        {
            this->entry_signal.acquire();
            this->number_of_writer_--;
            if (this->delayed_reader_ > 0)
            {
                this->delayed_reader_--;
                this->reader_signal.release();
            }
            else if (this->delayed_writer_ > 0)
            {
                this->delayed_writer_--;
                this->writer_signal.release();
            }
            else
            {
                this->entry_signal.release();
            }
        }

      private:
        int number_of_reader_= 0;
        int number_of_writer_= 0;
        int delayed_reader_= 0;
        int delayed_writer_= 0;
        std::function<bool(int, int)> priority_function_ = NULL;
        std::binary_semaphore entry_signal{1};
        std::binary_semaphore reader_signal{0};
        std::binary_semaphore writer_signal{0};
    };



    }
}