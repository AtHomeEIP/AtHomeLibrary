#ifndef WOODBOXWIFIMODULE_HPP
# define WOODBOXWIFIMODULE_HPP

# include <stdio.h>
# include <ArduinoJson.h>
# include "WoodBoxModule.hpp"

namespace woodBox {
    namespace module {
        /**
         * Template class derived from woodBox::module::WoodBoxModule, implementing WoodBox WiFi extension.
         *
         * The template parameters are the same than woodBox::module::WoodBoxModule class, taking the type representing in memory a sample from the sensor and the number of samples to buffer.
         *
         * Example:
         *
         * \code{.cpp}
         * #include <woodBox.h>
         *
         * WoodBoxWiFiModule<uint16_t, 15> *module = WoodBoxModule<uint16_t, 15>::getInstance<WoodBoxWiFiModule<uint16_t, 15> >();
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
        class WoodBoxWiFiModule : public WoodBoxModule<T, n> {
        friend class WoodBoxModule<T, n>;
        public:
            WoodBoxWiFiModule(const WoodBoxWiFiModule &) = delete;
            WoodBoxWiFiModule &operator=(const WoodBoxWiFiModule &) = delete;
            ~WoodBoxWiFiModule() {}

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
            WoodBoxWiFiModule():
                WoodBoxModule<T, n>(),
                _wifi(nullptr) {
                WoodBoxModule<T, n>::setCommandPlugin(&WoodBoxWiFiModule::executeWiFiCommands);
                WoodBoxModule<T, n>::setOnBackupPlugin(&WoodBoxWiFiModule::_saveWiFiParameters);
                WoodBoxModule<T, n>::setOnRestorePlugin(&WoodBoxWiFiModule::_restoreWiFiParameters);
            }

        private:
            void setWiFiCommand(Stream &communicator) {
                bool backup = false;
                {
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
                            _wifi->disconnect();
                            _wifi->setAccessPoint(ap);
                            _wifi->connect();
                        }
                    }
                }
                if (backup) {
                    this->onBackupOnStorage(); // Backup wifi parameters in storage
                }
            }

            void setEndPointCommand(Stream &communicator) {
                bool backup = false;
                {
                    char buffer[42];
                    size_t len = 0;
                    if ((len = communicator.readBytesUntil(communication::commands::end_of_command, buffer, 42))) {
                        buffer[len] = '\0';
                        StaticJsonBuffer<42> json;
                        JsonObject &root = json.parseObject(buffer);
                        const char *ip = root.get<const char *>(communication::commands::ip_key);
                        communication::ip::port p = root.get<communication::ip::port>(communication::commands::port_key);
                        if (ip != nullptr) {
                            communication::ip::s_host host;
                            SSCANF(ip, communication::ip::ip_format,
                                   &(host.ipv4[0]),
                                   &(host.ipv4[1]),
                                   &(host.ipv4[2]),
                                   &(host.ipv4[3]));
                            host.hport = p;
                            _wifi->disconnectFromHost();
                            _wifi->setHost(host);
                            _wifi->connectToHost();
                            backup = true;
                        }
                    }
                }
                if (backup) {
                    this->onBackupOnStorage(); // Backup wifi parameters in storage
                }
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

            static void _saveWiFiParameters(size_t offset, storage::IStorage &storage) {
                WoodBoxWiFiModule *instance = WoodBoxModule<T, n>::template getInstance<WoodBoxWiFiModule<T, n> >();
                if (instance != nullptr) {
                    instance->saveWiFiParameters(offset, storage);
                }
            }

            static void _restoreWiFiParameters(size_t offset, storage::IStorage &storage) {
                WoodBoxWiFiModule *instance = WoodBoxModule<T, n>::template getInstance<WoodBoxWiFiModule<T, n> >();
                if (instance != nullptr) {
                    instance->restoreWiFiParameters(offset, storage);
                }
            }

        private:
            communication::wifi::AWiFiCommunicator   *_wifi;
        };
    }
}

#endif /* WOODBOXWIFIMODULE_HPP */
