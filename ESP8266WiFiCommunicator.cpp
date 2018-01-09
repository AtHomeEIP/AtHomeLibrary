#include "ESP8266WiFiCommunicator.hpp"

namespace woodBox {
    namespace communication {
        namespace wifi {
            namespace {
#ifndef __AVR__
                const char at_eol[] =       "\r\n";
                const char at_at[] =        "AT";
                const char at_rst[] =       "RST";
                const char at_gmr[] =       "GMR";
                const char at_gslp[] =      "GLSP";
                const char at_ate[] =       "ATE";
                const char at_cwmode[] =    "CWMODE";
                const char at_cwjap[] =     "CWJAP";
                const char at_cwlap[] =     "CWLAP";
                const char at_cwqap[] =     "CWQAP";
                const char at_cwsap[] =     "CWSAP";
                const char at_cwlif[] =     "CWLIF";
                const char at_cwdhcp[] =    "CWDHCP";
                const char at_cipstamac[] = "CIPSTAMAC";
                const char at_cipapmac[] =  "CIPAPMAC";
                const char at_cipsta[] =    "CIPSTA";
                const char at_ciap[] =      "CIPAP";
                const char at_cipstatus[] = "CIPSTATUS";
                const char at_cipstart[] =  "CIPSTART";
                const char at_cipsend[] =   "CIPSEND";
                const char at_cipclose[] =  "CIPCLOSE";
                const char at_cifsr[] =     "CIFSR";
                const char at_cipmux[] =    "CIPMUX";
                const char at_cipserver[] = "CIPSERVER";
                const char at_cipmode[] =   "CIPMODE";
                const char at_cipsto[] =    "CIPSTO";
                const char at_ciupdate[] =  "CIUPDATE";
                const char at_ipd[] =       "+IPD";
#else
# include <avr/pgmspace.h>
                const PROGMEM char at_eol[] =       "\r\n";
                const PROGMEM char at_at[] =        "AT";
                const PROGMEM char at_rst[] =       "RST";
                const PROGMEM char at_gmr[] =       "GMR";
                const PROGMEM char at_gslp[] =      "GLSP";
                const PROGMEM char at_ate[] =       "ATE";
                const PROGMEM char at_cwmode[] =    "CWMODE";
                const PROGMEM char at_cwjap[] =     "CWJAP";
                const PROGMEM char at_cwlap[] =     "CWLAP";
                const PROGMEM char at_cwqap[] =     "CWQAP";
                const PROGMEM char at_cwsap[] =     "CWSAP";
                const PROGMEM char at_cwlif[] =     "CWLIF";
                const PROGMEM char at_cwdhcp[] =    "CWDHCP";
                const PROGMEM char at_cipstamac[] = "CIPSTAMAC";
                const PROGMEM char at_cipapmac[] =  "CIPAPMAC";
                const PROGMEM char at_cipsta[] =    "CIPSTA";
                const PROGMEM char at_ciap[] =      "CIPAP";
                const PROGMEM char at_cipstatus[] = "CIPSTATUS";
                const PROGMEM char at_cipstart[] =  "CIPSTART";
                const PROGMEM char at_cipsend[] =   "CIPSEND";
                const PROGMEM char at_cipclose[] =  "CIPCLOSE";
                const PROGMEM char at_cifsr[] =     "CIFSR";
                const PROGMEM char at_cipmux[] =    "CIPMUX";
                const PROGMEM char at_cipserver[] = "CIPSERVER";
                const PROGMEM char at_cipmode[] =   "CIPMODE";
                const PROGMEM char at_cipsto[] =    "CIPSTO";
                const PROGMEM char at_ciupdate[] =  "CIUPDATE";
                const PROGMEM char at_ipd[] =       "+IPD";
#endif
            }

            ESP8266WiFiCommunicator::ESP8266WiFiCommunicator():
                AWiFiCommunicator(),
                _connected(false),
                _connected_to_host(false),
                _receiving_data(false),
                _receiving_len(0) {
            }

            //ESP8266WiFiCommunicator::~ESP8266WiFiCommunicator() {}

            int ESP8266WiFiCommunicator::available() {
                return _input_buffer.available();
            }

            int ESP8266WiFiCommunicator::read() {
                return _input_buffer.read();
            }

            int ESP8266WiFiCommunicator::peek() {
                return _input_buffer.peek();
            }

            size_t ESP8266WiFiCommunicator::write(uint8_t byte) {
                if (!_connected_to_host) {
                    return 0;
                }
                _output_buffer.write(byte);
                if (_output_buffer.available() == ESP8266_BUFFER_SIZE) {
                    _flush_output();
                }
                return 1;
            }

            void ESP8266WiFiCommunicator::flush() {
                _flush_input();
                _flush_output();
            }

            void ESP8266WiFiCommunicator::open() {
                _switch_on_esp();
            }

            void ESP8266WiFiCommunicator::close() {
                if (_connected) {
                    disconnect();
                }
                _switch_off_esp();
            }

            void ESP8266WiFiCommunicator::connect() {
                // TODO: To implement
            }

            void ESP8266WiFiCommunicator::disconnect() {
                if (_connected_to_host) {
                    disconnectFromHost();
                }
                // TODO: To complete
            }

            void ESP8266WiFiCommunicator::connectToHost() {
                // TODO: To implement
            }

            void ESP8266WiFiCommunicator::disconnectFromHost() {
                // TODO: To implement
            }

            bool ESP8266WiFiCommunicator::isConnected() {
                return _connected_to_host;
            }

            void ESP8266WiFiCommunicator::_interpret() {
                // TODO: To implement
            }

            void ESP8266WiFiCommunicator::_read() {
                // TODO: To implement
            }

            void ESP8266WiFiCommunicator::_flush_input() {
                // TODO: To implement
            }

            void ESP8266WiFiCommunicator::_flush_output() {
                // TODO: To implement
            }

            void ESP8266WiFiCommunicator::_switch_on_esp() {
                // TODO: To implement
            }

            void ESP8266WiFiCommunicator::_switch_off_esp() {
                // TODO: To implement
            }
        }
    }
}
