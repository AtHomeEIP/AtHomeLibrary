#ifndef ACOMMUNICATIVEMODULE_HPP
# define ACOMMUNICATIVEMODULE_HPP

//# include <Stream.h>
# include "IModule.hpp"
# include "ICommunicator.hpp"

# define END_OF_LINE            "\r\n"
# define SEPARATOR              "================================================================================"
# define HEADER_SPACER          "\t"
# define END_OF_COMMAND         "\X03"
# define END_OF_COMMUNICATION   "\X04"

# define ENUMERATE              "Enumerate"
# define UPLOAD_DATA            "UploadData"
# define SYNC_TIME              "SyncTime"
# define SET_WIFI               "SetWiFi"
# define SET_ENDPOINT           "SetEndpoint"
# define SET_PROFILE            "SetProfile"

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
                //virtual const communication::ICommunicator **getCommunicators();
                virtual void setCommunicators(communication::ICommunicator **);
            protected:
                communication::ICommunicator    **_streams;
        };
    }
}

#endif /* ACOMMUNICATIVEMODULE_HPP */
