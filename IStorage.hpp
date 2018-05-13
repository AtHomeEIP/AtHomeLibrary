#ifndef ISTORAGE_HPP
# define ISTORAGE_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_PERSISTENT_STORAGE)
#  include <stddef.h>

namespace athome {
    namespace storage {
        /**
         * Interface used to be able to store and read raw data from any source, by transferring memory buffers
         */
        class IStorage {
            public:
                //virtual ~IStorage() = 0;
                /**
                 * Copy n bytes (3rd parameter) in dest (2nd parameter) from offset x (1st parameter)
                 *
                 * Example:
                 *
                 * \code{.cpp}
                 * void my_function_reading_something(IStorage &storage) {
                 *   char my_string[33];
                 *   int my_number;
                 *
                 *   storage.read(0, my_string, sizeof(my_string)); // Reading a string (array of 33 char) at offset 0 of storage instance into my_string
                 *   storage.read(sizeof(my_string), &my_number, sizeof(my_number)); // Reading an integer at offset 33 (after content put into my_string) from storage instance into my_number
                 * }
                 * \endcode
                 */
                virtual void read(size_t, void *, size_t) = 0;
                /**
                 * Write n bytes (3rd parameter) in offset x (1st parameter) from src (2nd parameter)
                 *
                 * Example:
                 *
                 * \code{.cpp}
                 * void my_function_writing_something(IStorage &storage) {
                 *   char my_string[] = "Hello, World!";
                 *   int my_number = 42;
                 *
                 *   storage.write(0, my_string, sizeof(my_string)); // Writing the string (array of 14 char) my_string at offset 0 of storage instance
                 *   storage.write(sizeof(my_string), &my_number, sizeof(my_number)); // Writing my_number at offset 14 (after my_string) of storage instance
                 * }
                 * \endcode
                 */
                virtual void write(size_t, const void *, size_t) = 0;
        };
    }
}

# endif /* DISABLE_PERSISTENT_STORAGE */
#endif /* ISTORAGE_HPP */
