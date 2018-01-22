#ifndef COMMANDSETENDPOINT_HPP
# define COMMANDSETENDPOINT_HPP

# include "wifi_types.hpp"
# include "AWiFiCommunicator.hpp"

namespace woodBox {
    namespace communication {
        namespace commands {
            class CommandSetEndPoint {
            public:
                CommandSetEndPoint(wifi::AWiFiCommunicator &);
                CommandSetEndPoint(const CommandSetEndPoint &) = delete;
                CommandSetEndPoint &operator=(const CommandSetEndPoint &) = delete;
                ~CommandSetEndPoint();
                void parse(Stream &);
            private:
                wifi::AWiFiCommunicator         &_com;
                tcp_host                        _host;
            };
        }
    }
}

#endif /* COMMANDSETENDPOINT_HPP */
