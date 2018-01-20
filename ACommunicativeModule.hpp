#ifndef ACOMMUNICATIVEMODULE_HPP
# define ACOMMUNICATIVEMODULE_HPP

# include <Stream.h>
# include "IModule.hpp"

namespace woodBox {
    namespace module {
        class ACommunicativeModule : public IModule {
            public:
                ACommunicativeModule(Stream **);
                ACommunicativeModule(ACommunicativeModule &);
                virtual ACommunicativeModule &operator=(ACommunicativeModule &);
                virtual ~ACommunicativeModule();
                virtual void run() = 0;
                virtual void stop() = 0;
                virtual Stream **getStreams();
                virtual void setStreams(Stream **);
            protected:
                Stream  **_streams;
        };
    }
}

#endif /* ACOMMUNICATIVEMODULE_HPP */
