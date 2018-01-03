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

/*
 * Thinking communication wisely
 * - Shall we have only two communication interface? Sync + WoodBox? Two ICommunicators with dedicated methods?
 * - Shall we have a variable number of interchangeable communication interface?
 * - Shall we have an interface of already defined methods to implement?
 * - Shall we have a dictionary of methods to use when receiving a message?
 *
 * Communication protocol:
 * - End of line: \r\n string
 * - Spacer: \t
 * - End of command: end of text character
 * - End of communication: end of transmission character
 *
 * - Line 1 should be method name
 * - There should be an array of parameters available to use for requests
 * - Request body should be able to transmit on a character mode interface, thus using encoding such as Base64
 *
 * Communication protocol format:
 * [aZ-09]+[EOL] // First line should be method name, insensible to format
 * ([aZ-09][Spacer]*)+[EOL] // N following lines is parameters array
 * [Separator][EOL] // Indicates end of parameters and beginning of body
 * [Stream][EOC][EOT]
 *
 * Communication methods
 * - Enumerate: Give module information, describe itself. Maybe should have a better name
 * - UploadData: Module sensor samples sent over interface. How to chose which interface if we don't specialize them? Configuration?
 * - SyncTime: Update clock of module
 * - SetWiFi: Set module WiFi configuration. How does a module know a ICommunicator is a WiFi interface?
 * - SetEndPoint: Set TCP socket endpoint (ip + port). How does a module know a ICommunicator implements a TCP/IP stack?
 * - SetProfile: Set a configuration profile to the module to store internally. What is supposed to be inside? What the module is supposed to do with it?
 *
 * Request object
 * - Should be derived from Printable?
 * - Attributes
 *   - Method name -> defined by constructor? Setter? Builder class?
 *   - Parameters -> defined by dictionary? Attributes in a class?
 *   - Body -> Blob
 * */

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
