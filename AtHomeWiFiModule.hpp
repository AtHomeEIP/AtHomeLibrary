#ifndef ATHOMEWIFIMODULE_HPP
# define ATHOMEWIFIMODULE_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) && !defined(DISABLE_WIFI)
#  include <stdio.h>
#  include <ArduinoJson.h>
#  include "AtHomeNetworkModule.hpp"
#  include "AtHomeFlashCommon.h"

namespace athome {
    namespace module {
        /**
         * Template class derived from athome::module::AtHomeModule, implementing AtHome WiFi extension.
         *
         * The template parameters are the same than athome::module::AtHomeModule class, taking the type representing in memory a sample from the sensor and the number of samples to buffer.
         *
         * Example:
         *
         * \code{.cpp}
         * #include <AtHome.h>
         *
         * AtHomeWiFiModule<uint16_t, 15> *module = AtHomeModule<uint16_t, 15>::getInstance<AtHomeWiFiModule<uint16_t, 15> >();
         * ESP8266WiFiCommunicator esp8266(2, 3); // CH_ED (enable) pin of the ESP8266 is imagined to be connected to pin 2 of the host, and RST (reset) pin is imagined to be connected to pin 3 of the host
         *
         * void setup() {
         *   Serial1.begin(115200);
         *   esp8266.setStreamToChipset(&Serial1); // If you're using an Arduino UNO or any other board with only one or no UART, use the SoftwareSerial library to create a new Serial instance to pass to this function
         *   module->setWiFiCommunicator(esp8266);
         * }
         *
         * void loop() {
         *   // Put your code here
         * }
         * \endcode
         */
        template <typename T, size_t n>
        class AtHomeWiFiModule : public AtHomeNetworkModule<T, n> {
        friend class AtHomeModule<T, n>;
        public:
            AtHomeWiFiModule(const AtHomeWiFiModule &) = delete;
            AtHomeWiFiModule &operator=(const AtHomeWiFiModule &) = delete;
            ~AtHomeWiFiModule() {}

            /**
             * Set the WiFi object used for wireless communication by passing it as reference.
             *
             * See class example for usage of this method.
             */
            void setWiFiCommunicator(communication::wifi::AWiFiCommunicator &wifi) {
                if (_wifi != nullptr) {
                    _wifi->disconnect();
                }
                _wifi = &wifi;
                this->onRestoreFromStorage(); // Restore wifi parameters from storage
            }

        protected:
            AtHomeWiFiModule():
                AtHomeNetworkModule<T, n>(),
                _wifi(nullptr) {
                AtHomeModule<T, n>::setCommandPlugin(&AtHomeWiFiModule::executeWiFiCommands);
                AtHomeModule<T, n>::setOnBackupPlugin(&AtHomeWiFiModule::_saveWiFiParameters);
                AtHomeModule<T, n>::setOnRestorePlugin(&AtHomeWiFiModule::_restoreWiFiParameters);
            }

        private:
            void setWiFiCommand(Stream &communicator) {
                {
                    char buffer[101];
                    size_t len = 0;
                    if ((len = communicator.readBytesUntil(communication::commands::end_of_command, buffer, 100))) {
                        buffer[len] = '\0';
                        StaticJsonBuffer<100> json;
                        JsonObject &root = json.parseObject(buffer);
                        const char *ssid = root[FH(communication::commands::ssid_key)];
                        const char *password = root[FH(communication::commands::password_key)];
                        if (ssid != nullptr && password != nullptr) {
                            communication::wifi::WiFi_ap ap;
                            strncpy(ap.ssid, ssid, 32);
                            strncpy(ap.password, password, 32);
                            ap.ssid[32] = '\0';
                            ap.password[32] = '\0';
                            _wifi->disconnect();
                            _wifi->setAccessPoint(ap);
                            _wifi->connect();
                        }
                    }
                }
                this->onBackupOnStorage(); // Backup wifi parameters in storage
            }

            void saveWiFiParameters(size_t offset, storage::IStorage &storage) {
                if (_wifi == nullptr) {
                    return;
                }
                const communication::wifi::WiFi_ap &ap = _wifi->getAccessPoint();
                const communication::ip::tcp_host &host = _wifi->getHost();
                storage.write(offset, reinterpret_cast<const void *>(ap.ssid), sizeof(communication::wifi::wifi_ssid));
                offset += sizeof(communication::wifi::wifi_ssid);
                storage.write(offset, reinterpret_cast<const void *>(ap.password), sizeof(communication::wifi::wifi_password));
                offset += sizeof(communication::wifi::wifi_password);
                storage.write(offset, reinterpret_cast<const void *>(host.ipv4), sizeof(communication::ip::ipv4_address));
                offset += sizeof(communication::ip::ipv4_address);
                storage.write(offset, reinterpret_cast<const void *>(&(host.hport)), sizeof(communication::ip::port));
            }

            void restoreWiFiParameters(size_t offset, storage::IStorage &storage) {
                if (_wifi == nullptr) {
                    return;
                }
                communication::wifi::WiFi_ap ap;
                communication::ip::tcp_host host;
                storage.read(offset, reinterpret_cast<void *>(ap.ssid), sizeof(communication::wifi::wifi_ssid));
                offset += sizeof(communication::wifi::wifi_ssid);
                storage.read(offset, reinterpret_cast<void *>(ap.password), sizeof(communication::wifi::wifi_password));
                offset += sizeof(communication::wifi::wifi_password);
                storage.read(offset, reinterpret_cast<void *>(host.ipv4), sizeof(communication::ip::ipv4_address));
                offset += sizeof(communication::ip::ipv4_address);
                storage.read(offset, reinterpret_cast<void *>(&(host.hport)), sizeof(communication::ip::port));
                _wifi->setAccessPoint(ap);
                _wifi->setHost(host);
            }

        private:
            static void executeWiFiCommands(const String &command, Stream &stream) {
                AtHomeWiFiModule *instance = AtHomeModule<T, n>::template getInstance<AtHomeWiFiModule<T, n> >();
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

            static void _saveWiFiParameters(size_t offset, storage::IStorage &storage) {
                AtHomeWiFiModule *instance = AtHomeModule<T, n>::template getInstance<AtHomeWiFiModule<T, n> >();
                if (instance != nullptr) {
                    instance->saveWiFiParameters(offset, storage);
                }
            }

            static void _restoreWiFiParameters(size_t offset, storage::IStorage &storage) {
                AtHomeWiFiModule *instance = AtHomeModule<T, n>::template getInstance<AtHomeWiFiModule<T, n> >();
                if (instance != nullptr) {
                    instance->restoreWiFiParameters(offset, storage);
                }
            }

        private:
            communication::wifi::AWiFiCommunicator   *_wifi;
        };
    }
}

# endif /* !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) && !defined(DISABLE_WIFI) */
#endif /* ATHOMEWIFIMODULE_HPP */
