#ifndef COMMANDSETWIFI_HPP
# define COMMANDSETWIFI_HPP

# include "ICommunicatorEventReceiver.hpp"
# include "wifi_types.hpp"

namespace woodBox {
    namespace communication {
        namespace commands {
            class CommandSetWiFi : public ICommunicatorCommandReceiver {
            public:
                CommandSetWiFi();
                CommandSetWiFi(const CommandSetWiFi &) = delete;
                CommandSetWiFi &operator=(const CommandSetWiFi &) = delete;
                ~CommandSetWiFi();
                virtual void parse(ICommunicator &);
                virtual void execute();
                virtual void reply(ICommunicator &);
            private:
                bool    _ok;
                WiFi_ap _ap;
            };

            const char *ssid_key = "ssid";
            const char *password_key = "password";
        }
    }
}

#endif /* COMMANDSETWIFI_HPP */
