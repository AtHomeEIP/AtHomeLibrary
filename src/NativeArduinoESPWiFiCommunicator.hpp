#ifndef NATIVEARDUINOESPWIFICOMMUNICATOR_HPP
# define NATIVEARDUINOESPWIFICOMMUNICATOR_HPP
# include "AtHomeConfig.h"
# if !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) && !defined(DISABLE_WIFI) &&\
     !defined(DISABLE_NATIVE_ESP_COM) && defined(ESP8266) && defined(ARDUINO)
# include <ESP8266WiFi.h>
#  include "AWiFiCommunicator.hpp"

namespace athome {
    namespace communication {
        namespace wifi {
            class NativeArduinoESPWiFiCommunicator : public AWiFiCommunicator {
            public:
                NativeArduinoESPWiFiCommunicator();
                NativeArduinoESPWiFiCommunicator(const NativeArduinoESPWiFiCommunicator &) = delete;
                NativeArduinoESPWiFiCommunicator &operator=(const NativeArduinoESPWiFiCommunicator &) = delete;
                ~NativeArduinoESPWiFiCommunicator();

                /* Methods inherited from Stream */
                virtual int available();
                virtual int read();
                virtual int peek();
                virtual size_t write(uint8_t);
                virtual void flush();
                /* Methods inherited from ANetworkCommunicator */
                virtual int connectToHost();
                virtual int disconnectFromHost();
                /* Methods inherited from AWiFiCommunicator */
                virtual int connect();
                virtual int disconnect();
                virtual bool isConnected() const;
                void setStreamToChipset(Stream *) = delete;

            private:
                WiFiClient    _client;
            };
        }
    }
}

# endif /* !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) && !defined(DISABLE_WIFI) &&\
           !defined(DISABLE_NATIVE_ESP_COM) && defined(ESP8266) && defined(ARDUINO) */
#endif /* NATIVEARDUINOESPWIFICOMMUNICATOR_HPP */