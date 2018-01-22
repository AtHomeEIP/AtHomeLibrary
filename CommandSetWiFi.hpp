#ifndef COMMANDSETWIFI_HPP
# define COMMANDSETWIFI_HPP

# include "wifi_types.hpp"
# include "AWiFiCommunicator.hpp"

namespace woodBox {
    namespace communication {
        namespace commands {
            class CommandSetWiFi {
            public:
                CommandSetWiFi(wifi::AWiFiCommunicator &);
                CommandSetWiFi(const CommandSetWiFi &) = delete;
                CommandSetWiFi &operator=(const CommandSetWiFi &) = delete;
                ~CommandSetWiFi();
                void parse(Stream &);
            private:
                wifi::AWiFiCommunicator         &_com;
                WiFi_ap                         _ap;
                //tcp_host                        _host;
            };
        }
    }
}

#endif /* COMMANDSETWIFI_HPP */
