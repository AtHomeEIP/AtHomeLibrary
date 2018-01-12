#ifndef COMMANDSETENDPOINT_HPP
# define COMMANDSETENDPOINT_HPP

# include "ICommunicatorEventReceiver.hpp"
# include "wifi_types.hpp"
# include "AWiFiCommunicator.hpp"

namespace woodBox {
    namespace communication {
        namespace commands {
            class CommandSetEndPoint : public ICommunicatorCommandReceiver {
            public:
                CommandSetEndPoint(wifi::AWiFiCommunicator &);
                CommandSetEndPoint(const CommandSetEndPoint &) = delete;
                CommandSetEndPoint &operator=(const CommandSetEndPoint &) = delete;
                ~CommandSetEndPoint();
                virtual void parse(ICommunicator &);
                virtual void execute();
                virtual void reply(ICommunicator &);
            private:
                bool                            _ok;
                wifi::AWiFiCommunicator         &_com;
                tcp_host                        _host;
            };
        }
    }
}

#endif /* COMMANDSETENDPOINT_HPP */
