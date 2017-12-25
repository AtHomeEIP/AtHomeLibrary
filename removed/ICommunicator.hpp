#ifndef ICOMMUNICATOR_HPP
# define ICOMMUNICATOR_HPP

# include <stdint.h>

namespace woodBox {
    namespace communication {
        class ICommunicator {
            public:
                virtual ~ICommunicator() = 0;
				virtual uint8_t *read(size_t) = 0;
				virtual void write(const uint8_t &, size_t) = 0;
        };
    }
}

#endif /* ICOMMUNICATOR_HPP */
