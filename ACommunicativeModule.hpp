#ifndef ACOMMUNICATIVEMODULE_HPP
# define ACOMMUNICATIVEMODULE_HPP

//# include <Stream.h>
# include "IModule.hpp"
# include "ICommunicator.hpp"

namespace woodBox {
    namespace module {
        class ACommunicativeModule : public IModule {
            public:
                ACommunicativeModule(communication::ICommunicator **);
                ACommunicativeModule(ACommunicativeModule &);
                virtual ACommunicativeModule &operator=(ACommunicativeModule &);
                virtual ~ACommunicativeModule();
                virtual void run() = 0;
                virtual void stop() = 0;
                virtual communication::ICommunicator **getCommunicators();
                virtual void setCommunicators(communication::ICommunicator **);
            protected:
                communication::ICommunicator    **_streams;
        };
    }
}

#endif /* ACOMMUNICATIVEMODULE_HPP */
