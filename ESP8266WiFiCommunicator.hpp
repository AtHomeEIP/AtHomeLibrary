#ifndef ESP8266WIFICOMMUNICATOR_HPP
# define ESP8266WIFICOMMUNICATOR_HPP

# include "AWiFiCommunicator.hpp"

namespace woodBox {
    namespace communication {
        namespace wifi {
            class ESP8266WiFiCommunicator : public AWiFiCommunicator {
            public:
                ESP8266WiFiCommunicator();
                ESP8266WiFiCommunicator(const ESP8266WiFiCommunicator &) = delete;
                ESP8266WiFiCommunicator &operator=(const ESP8266WiFiCommunicator &) = delete;
                virtual int available();
                virtual int read();
                virtual int peek();
                virtual size_t write(uint8_t);
                virtual void flush();
                virtual void open();
                virtual void close();
                virtual void connect();
                virtual void disconnect();
                virtual void connectToHost();
                virtual void disconnectFromHost();
                virtual bool isConnected();
            /* private:
                virtual ~ESP8266WiFiCommunicator(); */
            private:
                bool _connected;
            };
        }
    }
}

#endif /* ESP8266WIFICOMMUNICATOR_HPP */
