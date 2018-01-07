#ifndef COMMANDSETWIFI_HPP
# define COMMANDSETWIFI_HPP

# include "ICommunicatorEventReceiver.hpp"
# include "wifi_types.hpp"
# include "AWiFiCommunicator.hpp"

namespace woodBox {
    namespace communication {
        namespace commands {
            class CommandSetWiFi : public ICommunicatorCommandReceiver {
            public:
                CommandSetWiFi(AWiFiCommunicator &);
                CommandSetWiFi(const CommandSetWiFi &) = delete;
                CommandSetWiFi &operator=(const CommandSetWiFi &) = delete;
                ~CommandSetWiFi();
                virtual void parse(ICommunicator &);
                virtual void execute();
                virtual void reply(ICommunicator &);
            private:
                bool                            _ok;
                AWiFiCommunicator               &_com;
                WiFi_ap                         _ap;
                tcp_host                        _host;
            };
        }
    }
}

#endif /* COMMANDSETWIFI_HPP */
