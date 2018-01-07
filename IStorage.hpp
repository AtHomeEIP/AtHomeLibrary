#ifndef ISTORAGE_HPP
# define ISTORAGE_HPP

# include <stdint.h>

namespace woodBox {
    namespace storage {
        class IStorage {
            public:
                //virtual ~IStorage() = 0;
                virtual void open() = 0;
                virtual void close() = 0;
                virtual uint8_t& read(uint32_t);
                virtual void write(const uint8_t &, uint32_t);
        };
    }
}

#endif /* ISTORAGE_HPP */
