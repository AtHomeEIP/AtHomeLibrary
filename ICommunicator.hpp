#ifndef ICOMMUNICATOR_HPP
# define ICOMMUNICATOR_HPP

# include <stdint.h>

namespace woodBox {
    namespace communication {
        class ICommunicator {
            public:
                virtual ~ICommunicator() = 0;
                virtual uint8_t &read(uint32_t) = 0; // Read n bytes
                virtual void write(const uint8_t &, uint32_t) = 0; // Write n bytes
        };
    }
}

#endif /* ICOMMUNICATOR_HPP */
