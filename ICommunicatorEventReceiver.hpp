#ifndef ICOMMUNICATOR_EVENT_RECEIVER_HPP
# define ICOMMUNICATOR_EVENT_RECEIVER_HPP

# include <Stream.h>

namespace woodBox {
    namespace communication {
        namespace commands {
            class ICommunicatorCommandReceiver {
            public:
                //virtual ~ICommunicatorCommandReceiver() = 0;
                // Parse command input -> parameters + body
                virtual void parse(Stream &) = 0;
                // Execute code related to this command
                virtual void execute() = 0;
                // Write a reply, if needed
                virtual void reply(Stream &) = 0;
            };
        }
    }
}

#endif /* ICOMMUNICATOR_EVENT_RECEIVER_HPP */
