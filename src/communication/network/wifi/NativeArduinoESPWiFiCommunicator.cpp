#include "AtHomeConfig.h"
#if !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) && !defined(DISABLE_WIFI) &&\
    !defined(DISABLE_NATIVE_ESP_COM) && defined(ESP8266) && defined(ARDUINO)
# include <ESP8266WiFi.h>
# include "NativeArduinoESPWiFiCommunicator.hpp"

namespace athome {
    namespace communication {
        namespace wifi {
            NativeArduinoESPWiFiCommunicator::NativeArduinoESPWiFiCommunicator():AWiFiCommunicator() {
                _mode = STATION;
                _stream = nullptr;
            }

            NativeArduinoESPWiFiCommunicator::~NativeArduinoESPWiFiCommunicator() {}

            int NativeArduinoESPWiFiCommunicator::available() {
                if (!_client.connected()) {
                    connectToHost();
                }
                return _client.available();
            }

            int NativeArduinoESPWiFiCommunicator::read() {
                if (!_client.connected()) {
                    connectToHost();
                }
                return _client.read();
            }

            int NativeArduinoESPWiFiCommunicator::peek() {
                if (!_client.connected()) {
                    connectToHost();
                }
                return _client.peek();
            }

            size_t NativeArduinoESPWiFiCommunicator::write(uint8_t c) {
                if (!_client.connected()) {
                    connectToHost();
                }
                return _client.write(c);
            }

            void NativeArduinoESPWiFiCommunicator::flush() {
                return _client.flush();
            }

            int NativeArduinoESPWiFiCommunicator::connectToHost() {
                IPAddress address(_host.ipv4);
                _client.connect(address, _host.hport);
            }

            int NativeArduinoESPWiFiCommunicator::disconnectFromHost() {
                _client.stop();
            }

            int NativeArduinoESPWiFiCommunicator::connect() {
                if (_mode == STATION) {
                    WiFi.mode(WIFI_STA);
                    WiFi.begin(_ap.ssid, _ap.password);
                    auto result = WiFi.waitForConnectResult();
                    while (result == WL_IDLE_STATUS) {
                        result = WiFi.waitForConnectResult();
                    }
                    if (result != WL_CONNECTED) {
                        return -1;
                    }
                    WiFi.setAutoReconnect(true);
                    IPAddress my_ip = WiFi.localIP();
                    for (uint8_t i = 0; i < 4; i++) {
                        _me.ipv4[i] = my_ip[i];
                    }
                    WiFi.macAddress(_me.mac);
                }
            }

            int NativeArduinoESPWiFiCommunicator::disconnect() {
                disconnectFromHost();
                WiFi.disconnect(true);
            }

            bool NativeArduinoESPWiFiCommunicator::isConnected() const {
                return WiFi.isConnected();
            }
        }
    }
}

#endif /* !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) && !defined(DISABLE_WIFI) &&\
          !defined(DISABLE_NATIVE_ESP_COM) && defined(ESP8266) && defined(ARDUINO) */