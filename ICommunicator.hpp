#ifndef ICOMMUNICATOR_HPP
# define ICOMMUNICATOR_HPP

# include <stdint.h>

# include <Stream.h>

namespace woodBox {
    namespace communication {
        class ICommunicator : public Stream {
            public:
                //virtual ~ICommunicator() = 0;
                virtual void open() = 0;
                virtual void close() = 0;
        };
    }
}

#endif /* ICOMMUNICATOR_HPP */
