#ifndef WOODBOXWIFIMODULE_HPP
# define WOODBOXWIFIMODULE_HPP

# include <stdio.h>
# include <ArduinoJson.h>
# include "WoodBoxModule.hpp"

namespace woodBox {
    namespace module {
        template <typename T, size_t n>
        class WoodBoxWiFiModule : public WoodBoxModule<T, n> {
        friend class WoodBoxModule<T, n>;
        public:
            WoodBoxWiFiModule(const WoodBoxWiFiModule &) = delete;
            WoodBoxWiFiModule &operator=(const WoodBoxWiFiModule &) = delete;
            ~WoodBoxWiFiModule() {}

            void setWiFiCommunicator(communication::wifi::AWiFiCommunicator &wifi) {
                if (_wifi != nullptr) {
                    _wifi->disconnect();
                }
                _wifi = &wifi;
                this->onRestoreFromStorage(); // Restore wifi parameters from storage
            }

        protected:
            WoodBoxWiFiModule():
                WoodBoxModule<T, n>(),
                _wifi(nullptr) {
                WoodBoxModule<T, n>::setCommandPlugin(&WoodBoxWiFiModule::executeWiFiCommands);
            }

        private:
            void setWiFiCommand(Stream &communicator) {
                char buffer[101];
                size_t len = 0;
                if ((len = communicator.readBytesUntil(communication::commands::end_of_command, buffer, 100))) {
                    buffer[len] = '\0';
                    StaticJsonBuffer<100> json;
                    JsonObject &root = json.parseObject(buffer);
                    const char *ssid = root.get<const char*>(communication::commands::ssid_key);
                    const char *password = root.get<const char *>(communication::commands::password_key);
                    if (ssid != nullptr && password != nullptr) {
                        communication::wifi::WiFi_ap ap;
                        strncpy(ap.ssid, ssid, 32);
                        strncpy(ap.password, password, 32);
                        ap.ssid[32] = '\0';
                        ap.password[32] = '\0';
                        this->onBackupOnStorage(); // Backup wifi parameters in storage
                        _wifi->disconnect();
                        _wifi->setAccessPoint(ap);
                        _wifi->connect();
                    }
                }
            }

            void setEndPointCommand(Stream &communicator) {
                char buffer[42];
                size_t len = 0;
                if ((len = communicator.readBytesUntil(communication::commands::end_of_command, buffer, 42))) {
                    buffer[len] = '\0';
                    communication::ip::s_host host;
                    StaticJsonBuffer<42> json;
                    JsonObject &root = json.parseObject(buffer);
                    const char *ip = root.get<const char *>(communication::commands::ip_key);
                    communication::ip::port p = root.get<communication::ip::port>(communication::commands::port_key);
                    if (ip != nullptr) {
                        SSCANF(ip, communication::ip::ip_format,
                               &(host.ipv4[0]),
                               &(host.ipv4[1]),
                               &(host.ipv4[2]),
                               &(host.ipv4[3]));
                        host.hport = p;
                    }
                    this->onBackupOnStorage(); // Backup wifi parameters in storage
                    _wifi->disconnectFromHost();
                    _wifi->setHost(host);
                    _wifi->connectToHost();
                }
            }

        private:
            static void executeWiFiCommands(const String &command, Stream &stream) {
                WoodBoxWiFiModule *instance = WoodBoxModule<T, n>::template getInstance<WoodBoxWiFiModule<T, n> >();
                if (instance == nullptr || instance->_wifi == nullptr) {
                    return;
                }
                if (!STRCMP(command.c_str(), communication::commands::setWiFi)) {
                    instance->setWiFiCommand(stream);
                }
                else if (!STRCMP(command.c_str(), communication::commands::setEndPoint)) {
                    instance->setEndPointCommand(stream);
                }
            }

        private:
            communication::wifi::AWiFiCommunicator   *_wifi;
        };
    }
}

#endif /* WOODBOXWIFIMODULE_HPP */
