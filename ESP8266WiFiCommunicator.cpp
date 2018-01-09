#include <Arduino.h>
#include <string.h>
#include "ESP8266WiFiCommunicator.hpp"

namespace woodBox {
    namespace communication {
        namespace wifi {
            namespace {
                const char at_nl =          '\n';
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
                const char at_ok[] =        "OK";
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
                const PROGMEM char at_ok[] =        "OK";
#endif
            }

            ESP8266WiFiCommunicator::ESP8266WiFiCommunicator(int enable_pin):
                AWiFiCommunicator(),
                _enable_pin(enable_pin),
                _enabled(false),
                _connected(false),
                _connected_to_host(false),
                _receiving_data(false),
                _receiving_len(0) {
                pinMode(_enable_pin, OUTPUT);
                digitalWrite(_enable_pin, (_enabled) ? LOW : HIGH); // esp needs a pull-up on enabled pin, logic is reversed
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

            int ESP8266WiFiCommunicator::connect() {
                if (_mode == ACCESS_POINT) {
                    return _create_ap();
                }
                else if (_mode == STATION) {
                    return _connect_ap();
                }
                return -1;
            }

            int ESP8266WiFiCommunicator::disconnect() {
                return _reset_esp();
                // TODO: To complete
            }

            int ESP8266WiFiCommunicator::connectToHost() {
                // TODO: To implement
                return -1;
            }

            int ESP8266WiFiCommunicator::disconnectFromHost() {
                // TODO: To implement
                return -1;
            }

            bool ESP8266WiFiCommunicator::isConnected() {
                if (_mode == ACCESS_POINT) {
                    return _connected;
                }
                else if (_mode == STATION) {
                    return _connected_to_host;
                }
                return false;
            }

            void ESP8266WiFiCommunicator::_interpret() {
                // TODO: To implement
            }

            void ESP8266WiFiCommunicator::_read() {
                // TODO: To implement
            }

            void ESP8266WiFiCommunicator::_flush_input() {
                // TODO: To implement
                _input_buffer.flush();
            }

            void ESP8266WiFiCommunicator::_flush_output() {
                // TODO: To implement : need to send buffer data over a socket and flush it after
                _output_buffer.flush();
            }

            void ESP8266WiFiCommunicator::_switch_on_esp() {
                // TODO: To implement
                digitalWrite(_enable_pin, LOW);
            }

            void ESP8266WiFiCommunicator::_switch_off_esp() {
                // TODO: To implement
                digitalWrite(_enable_pin, HIGH);
            }

            int ESP8266WiFiCommunicator::_command_check() {
                if (_stream == nullptr) {
                    return -1;
                }
                if (!_enabled) {
                    return -2;
                }
            }

            int ESP8266WiFiCommunicator::_esp_answer_check() {
                while (!_stream->available()) {
                    delay(10);
                }
                String result = _stream->readStringUntil(at_nl);
                result.replace("\r", "");
                return (!strcmp(result.c_str(), at_ok)) ? 0 : -3;
            }

            int ESP8266WiFiCommunicator::_test_esp() {
                int check = _command_check();
                if (check) {
                    return check;
                }
                _stream->print(at_at);
                _stream->print(at_eol);
                return _esp_answer_check();
            }

            int ESP8266WiFiCommunicator::_set_esp_station_mode() {
                int check = _command_check();
                if (check) {
                    return check;
                }
                _stream->print(at_cwmode);
                _stream->print(F("=1"));
                _stream->print(at_eol);
                return _esp_answer_check();
            }

            int ESP8266WiFiCommunicator::_set_esp_ap_mode() {
                int check = _command_check();
                if (check) {
                    return check;
                }
                _stream->print(at_cwmode);
                _stream->print(F("=2"));
                _stream->print(at_eol);
                return _esp_answer_check();
            }

            int ESP8266WiFiCommunicator::_connect_ap() {
                int check = _command_check();
                if (check) {
                    return check;
                }
                else if (_set_esp_ap_mode()) {
                    return -3;
                }
                _stream->print(at_cwjap);
                _stream->print(F("=\""));
                _stream->print(reinterpret_cast<const char *>(_ap.ssid));
                _stream->print(F("\",\""));
                _stream->print(reinterpret_cast<const char *>(_ap.password));
                _stream->print(F("\""));
                _stream->print(at_eol);
                return _esp_answer_check();
            }

            int ESP8266WiFiCommunicator::_create_ap() {
                // Todo: Need to implement
                return -1;
            }

            int ESP8266WiFiCommunicator::_reset_esp() {
                // Todo: Need to implement
                return -1;
            }
        }
    }
}
