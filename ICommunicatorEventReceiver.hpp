#ifndef ICOMMUNICATOR_EVENT_RECEIVER_HPP
# define ICOMMUNICATOR_EVENT_RECEIVER_HPP

# include "ICommunicator.hpp"

namespace woodBox {
    namespace communication {
        namespace command {
            class ICommunicatorCommandReceiver {
            public:
                // Parse command input -> parameters + body
                virtual void parse(ICommunicator *) = 0;
                // Execute code related to this command
                virtual void execute() = 0;
                // Write a reply, if needed
                virtual void reply(ICommunicator *) = 0;
            };
        }
    }
}

#endif /* ICOMMUNICATOR_EVENT_RECEIVER_HPP */
