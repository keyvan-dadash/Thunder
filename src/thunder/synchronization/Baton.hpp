

#include <atomic>
#include <functional>

namespace thunder {


  namespace synchronization {

    template <bool MayBlock = true, template <typename> class Atom = std::atomic>
    class Baton
    {
      public:
        Baton() 
        {
            this->priority_function_ = [](int, int){return true;}
        }

        Baton(std::function<bool(int, int)> isReaderHavingHigherPriority)
        {
            this->priority_function_ = isReaderHavingHigherPriority;
        }

        void r_signal()
        {
            this->number_of_reader_--;
            if (this->number_of_reader_ == 0 && this->delayed_writer_ > 0)
            {
                this->delayed_writer_--;
                //mechanism for wakeup writer
            } 
            else 
            {
                //mechanism for release process    
            }
        }

        void r_wait()
        {

        }

        void w_signal()
        {
            this->number_of_writer_--;
            if (this->delayed_reader_ > 0)
            {
                this->delayed_reader_--;
                //mechanism for wakeup reader
            }
            else if (this->delayed_writer_ > 0)
            {
                this->delayed_writer_--;
                //mechanism for wakeup writer
            }
            else
            {
                //mechanism for release process
            }
        }

        void w_wait()
        {

        }

        ~Baton()
        {

        }

      private:
        std::function<bool(int, int)> priority_function_;
        int number_of_reader_;
        int number_of_writer_;
        int delayed_reader_;
        int delayed_writer_;

    }



    }
}