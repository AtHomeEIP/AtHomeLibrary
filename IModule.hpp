#ifndef IMODULE_HPP
# define IMODULE_HPP

namespace woodBox {
    namespace module {
        class IModule {
            public:
                virtual ~IModule() = 0;
                virtual void run() = 0; // Start or resume the module execution
                virtual void stop() = 0; // Stop the module execution
        };
    }
}

#endif /* IMODULE_HPP */
