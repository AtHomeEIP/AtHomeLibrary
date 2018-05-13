#ifndef BUFFER_HPP
# define BUFFER_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_BUFFER)
#  include <stdint.h>
#  include <string.h>
#  include <Stream.h>

namespace athome {
    namespace utility {
        /**
         * Templated buffer, taking as template parameters the type of hold data and their numbers.
         *
         * Example of use:
         *
         * \code{.cpp}
         * #include <Buffer.hpp>
         * #include <String.h>
         *
         * using athome::utility::Buffer;
         *
         * String my_string1("Hello, World!");
         * String my_string2("foobar");
         * String my_string3("Philipe ! Je sais ou tu te caches !"); // Sorry for non-french readers :)
         *
         * void my_function_using_a_buffer() {
         *   Buffer<const String &, 2> my_buffer; // Create a buffer able to store 2 references on constant String objects
         *   my_buffer.write(my_string1); // Add a reference on constant my_string1 to the buffer
         *   my_buffer.write(my_string2); // Add a reference on constant my_string2 to the buffer
         *   my_buffer.write(my_string3); // Won't work! The buffer is full
         *   Serial.println(my_buffer.available()); // Will print 2
         *   const String &re_string1 = my_buffer.read(); // Return a reference on constant String my_string1 and free a space in the buffer
         *   Serial.println(my_buffer.available()); // Will print 1
         *   const String &re_string2 = my_buffer.peek(); // Return a reference on constant String my_string2, don't modify the buffer
         *   Serial.println(my_buffer.available()); // Will print 1
         *   my_buffer.write(my_string3); // Add a reference on constant my_string3 to the buffer, storing now references on my_string2 and my_string3
         *   Serial.println(my_buffer.available()); // Will print 2
         *   my_buffer.flush(); // Clear the buffer
         *   Serial.println(my_buffer.available()); // Will print 0
         * }
         * \endcode
         */
        template <typename T, size_t size>
        class Buffer {
        public:
            Buffer():
                _read_idx(0),
                _write_idx(0) {
                memset(_buffer, 0, sizeof(T) * size);
            }

            Buffer(const Buffer &other):
                _read_idx(other._read_idx),
                _write_idx(other._write_idx) {
                memcpy(_buffer, other._buffer, sizeof(T) * size);
            }

            Buffer &operator=(const Buffer &other) {
                _read_idx = other._read_idx;
                _write_idx = other._write_idx;
                memcpy(_buffer, other._buffer, sizeof(T) * size);
            }

            ~Buffer() {}
            /**
             * This method store the data passed as parameter into the buffer.
             *
             * Note: if the buffer is already full, this method does nothing.
             *
             * Check with the method athome::utility::Buffer::available() if you're unsure if the buffer has free space.
             *
             * If the buffer has free space, the method athome::utility::Buffer::available() should return an integer inferior to the size of the buffer.
             *
             * See class detailed documentation for example of usage.
             */
            void write(T data) {
                if (_buffer[_write_idx] != 0) {
                    return;
                }
                else {
                    _buffer[_write_idx] = data;
                    _write_idx++;
                }
                if (_write_idx == size) {
                    _write_idx = 0; // Circular pointer
                }
            }
            /**
             * This method return the oldest stored element in the buffer and remove it from the buffer.
             *
             * See class detailed documentation for example of usage.
             */
            T read() {
                if (_read_idx == _write_idx && _buffer[_read_idx] == 0) {
                    return -1; // Empty buffer
                }
                T data = _buffer[_read_idx];
                _buffer[_read_idx] = 0; // Free
                _read_idx = (_read_idx == (size - 1)) ? 0 : _read_idx + 1; // Circular pointer
                return data;
            }
            /**
             * This method return the number of elements stored in the buffer.
             *
             * See class detailed documentation for example of usage.
             */
            int available() {
                if (_read_idx == _write_idx) {
                    return (_buffer[_read_idx] != 0) ? 1 : 0;
                }
                else {
                    return (_write_idx > _read_idx) ? _write_idx - _read_idx : size - _read_idx + _write_idx;
                }
            }
            /**
             * This method return the oldest stored element in the buffer and doesn't remove it from the buffer.
             *
             * See class detailed documentation for example of usage.
             */
            T peek() {
                if (_read_idx == _write_idx && _buffer[_read_idx] == 0) {
                    return -1;
                }
                return _buffer[_read_idx];
            }
            /**
             * This method remove all elements from the buffer.
             *
             * See class detailed documentation for example of usage.
             */
            void flush() {
                _read_idx = 0;
                _write_idx = 0;
                memset(_buffer, 0, sizeof(T) * size);
            }

        private:
            size_t   _read_idx;
            size_t   _write_idx;
            T        _buffer[size];
        };
    }
}

# endif /* DISABLE_BUFFER */
#endif /* BUFFER_HPP */
