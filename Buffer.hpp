#ifndef BUFFER_HPP
# define BUFFER_HPP

# include <stdint.h>
# include <string.h>
# include <Stream.h>

namespace woodBox {
    namespace utility {
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

            T read() {
                if (_read_idx == _write_idx && _buffer[_read_idx] == 0) {
                    return -1; // Empty buffer
                }
                T data = _buffer[_read_idx];
                _buffer[_read_idx] = 0; // Free
                _read_idx = (_read_idx == (size - 1)) ? 0 : _read_idx + 1; // Circular pointer
                return data;
            }

            int available() {
                if (_read_idx == _write_idx) {
                    return (_buffer[_read_idx] != 0) ? 1 : 0;
                }
                else {
                    return (_write_idx > _read_idx) ? _write_idx - _read_idx : size - _read_idx + _write_idx;
                }
            }

            T peek() {
                if (_read_idx == _write_idx && _buffer[_read_idx] == 0) {
                    return -1;
                }
                return _buffer[_read_idx];
            }

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

        template <size_t size>
        class StreamBuffer : virtual public Stream, virtual public Buffer<int, size> {};
    }
}

#endif /* BUFFER_HPP */
