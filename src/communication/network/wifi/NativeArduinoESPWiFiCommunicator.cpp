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
                if (!WiFi.isConnected()) {
                    return 0;
                }
                if (!_client.connected()) {
                    connectToHost();
                }
                return _client.available();
            }

            int NativeArduinoESPWiFiCommunicator::read() {
                if (!WiFi.isConnected()) {
                    return -1;
                }
                if (!_client.connected()) {
                    connectToHost();
                }
                return _client.read();
            }

            int NativeArduinoESPWiFiCommunicator::peek() {
                if (!WiFi.isConnected()) {
                    return -1;
                }
                if (!_client.connected()) {
                    connectToHost();
                }
                return _client.peek();
            }

            size_t NativeArduinoESPWiFiCommunicator::write(uint8_t c) {
                if (!WiFi.isConnected()) {
                    return 0;
                }
                if (!_client.connected() && connectToHost()) {
                    return 0;
                }
                return _client.write(c);
            }

            void NativeArduinoESPWiFiCommunicator::flush() {
                if (!WiFi.isConnected()) {
                    return;
                }
                return _client.flush();
            }

            int NativeArduinoESPWiFiCommunicator::connectToHost() {
                if (!WiFi.isConnected()) {
                    return -1;
                }
                IPAddress address(_host.ipv4);
                return !_client.connect(address, _host.hport);
            }

            int NativeArduinoESPWiFiCommunicator::disconnectFromHost() {
                if (!WiFi.isConnected() || !_client.connected()) {
                    return 0;
                }
                _client.stop();
                return 0;
            }

            int NativeArduinoESPWiFiCommunicator::connect() {
                if (_mode == STATION) {
                    WiFi.mode(WIFI_STA);
                    WiFi.begin(_ap.ssid, _ap.password);
                    auto result = WiFi.waitForConnectResult();
                    while (result == WL_IDLE_STATUS) {
                        result = WiFi.waitForConnectResult();
                        yield();
                    }
                    WiFi.setAutoReconnect(true);
                    if (result != WL_CONNECTED) {
                        return -1;
                    }
                    IPAddress my_ip = WiFi.localIP();
                    for (uint8_t i = 0; i < 4; i++) {
                        _me.ipv4[i] = my_ip[i];
                    }
                    WiFi.macAddress(_me.mac);
                    return 0;
                }
                return -1;
            }

            int NativeArduinoESPWiFiCommunicator::disconnect() {
                if (!WiFi.isConnected()) {
                    return 0;
                }
                disconnectFromHost();
                return WiFi.disconnect(true);
            }

            bool NativeArduinoESPWiFiCommunicator::isConnected() const {
                return WiFi.isConnected();
            }
        }
    }
}

#endif /* !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) && !defined(DISABLE_WIFI) &&
          !defined(DISABLE_NATIVE_ESP_COM) && defined(ESP8266) && defined(ARDUINO) */