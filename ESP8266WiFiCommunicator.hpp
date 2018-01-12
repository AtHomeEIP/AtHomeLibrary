#ifndef ESP8266WIFICOMMUNICATOR_HPP
# define ESP8266WIFICOMMUNICATOR_HPP

# include <stdint.h>
# include "AWiFiCommunicator.hpp"
# include "Buffer.hpp"

# ifndef ESP8266_BUFFER_SIZE
#  define ESP8266_BUFFER_SIZE 64
# endif

# ifndef ESP8266_COMMAND_BUFFER_SIZE
#  define ESP8266_COMMAND_BUFFER_SIZE 4
# endif

namespace woodBox {
    namespace communication {
        namespace wifi {
            class ESP8266WiFiCommunicator : public AWiFiCommunicator {
            public:
                ESP8266WiFiCommunicator(int, int);
                ESP8266WiFiCommunicator(const ESP8266WiFiCommunicator &) = delete;
                ESP8266WiFiCommunicator &operator=(const ESP8266WiFiCommunicator &) = delete;
                virtual int available();
                virtual int read();
                virtual int peek();
                virtual size_t write(uint8_t);
                virtual void flush();
                virtual void open();
                virtual void close();
                virtual int connect();
                virtual int disconnect();
                virtual int connectToHost();
                virtual int disconnectFromHost();
                virtual bool isConnected();
            /* private:
                virtual ~ESP8266WiFiCommunicator(); */
            private:
                void _interpret();
                void _read();
                void _write();
                void _flush_input();
                void _flush_output();
                void _switch_on_esp();
                void _switch_off_esp();
            private:
                int _command_check();
                int _esp_answer_check();
                int _test_esp();
                int _set_esp_station_mode();
                int _set_esp_ap_mode();
                int _connect_ap();
                int _create_ap();
                int _reset_esp();
                int _wait_for_esp_ready();
                int _connect_to_tcp_socket();
                int _enable_transparent_mode();
                int _go_to_send_mode();
            private:
                int                                                                                 _enable_pin;
                int                                                                                 _reset_pin;
                bool                                                                                _enabled;
                bool                                                                                _connected;
                bool                                                                                _connected_to_host;
                bool                                                                                _receiving_data;
                size_t                                                                              _receiving_len;
                utility::Buffer<int, ESP8266_BUFFER_SIZE>                                           _input_buffer;
                utility::Buffer<int, ESP8266_BUFFER_SIZE>                                           _output_buffer;
                utility::Buffer<int (ESP8266WiFiCommunicator::*)(), ESP8266_COMMAND_BUFFER_SIZE>    _command_buffer;
            };
        }
    }
}

#endif /* ESP8266WIFICOMMUNICATOR_HPP */
