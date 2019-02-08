#include "AtHomeConfig.h"
#if !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) && !defined(DISABLE_WIFI) &&\
    !defined(DISABLE_ESP_AT_MODULE)
# include <Arduino.h>
# include <string.h>
# include <stdio.h>
# include "ESP8266WiFiCommunicator.hpp"
# include "AtHomeFlashCommon.h"

namespace athome {
    namespace communication {
        namespace wifi {
            namespace {
                const char at_nl =                                          '\n';
                const PROGMEM char at_eol[] =                               "\r\n";
                const PROGMEM char at_at[] =                                "AT";
                const PROGMEM char at_rst[] =                               "RST";
                const PROGMEM char at_gmr[] =                               "GMR";
                const PROGMEM char at_gslp[] =                              "GLSP";
                const PROGMEM char at_ate[] =                               "ATE";
                const PROGMEM char at_cwmode[] =                            "CWMODE";
                const PROGMEM char at_cwjap[] =                             "CWJAP";
                const PROGMEM char at_cwlap[] =                             "CWLAP";
                const PROGMEM char at_cwqap[] =                             "CWQAP";
                const PROGMEM char at_cwsap[] =                             "CWSAP";
                const PROGMEM char at_cwlif[] =                             "CWLIF";
                const PROGMEM char at_cwdhcp[] =                            "CWDHCP";
                const PROGMEM char at_cipstamac[] =                         "CIPSTAMAC";
                const PROGMEM char at_cipapmac[] =                          "CIPAPMAC";
                const PROGMEM char at_cipsta[] =                            "CIPSTA";
                const PROGMEM char at_ciap[] =                              "CIPAP";
                const PROGMEM char at_cipstatus[] =                         "CIPSTATUS";
                const PROGMEM char at_cipstart[] =                          "CIPSTART";
                const PROGMEM char at_cipsend[] =                           "CIPSEND";
                const PROGMEM char at_cipclose[] =                          "CIPCLOSE";
                const PROGMEM char at_cifsr[] =                             "CIFSR";
                const PROGMEM char at_cipmux[] =                            "CIPMUX";
                const PROGMEM char at_cipserver[] =                         "CIPSERVER";
                const PROGMEM char at_cipmode[] =                           "CIPMODE";
                const PROGMEM char at_cipsto[] =                            "CIPSTO";
                const PROGMEM char at_ciupdate[] =                          "CIUPDATE";
                const PROGMEM char at_ipd[] =                               "+IPD";
                const PROGMEM char at_ok[] =                                "OK";
                const PROGMEM char at_error[] =                             "ERROR";
                const PROGMEM char at_ready[] =                             "ready";
                const PROGMEM char at_listen[] =                            ">";
                const PROGMEM char channel_format[] =                       "%hhu";
                const PROGMEM char symbol_plus[] =                          "+";
                const PROGMEM char symbol_plus_x3[] =                       "+++";
                const PROGMEM char equal_1[] =                              "=1";
                const PROGMEM char equal_2[] =                              "=2";
                const PROGMEM char equal_double_quotes[] =                  "=\"";
                const PROGMEM char double_quotes[] =                        "\"";
                const PROGMEM char double_quotes_comma[] =                  "\",";
                const PROGMEM char double_quotes_comma_double_quotes[] =    "\",\"";
                const PROGMEM char comma_3[] =                              ",3";
                const PROGMEM char equal_tcp[] =                            "=\"TCP\",\"";
            }

            ESP8266WiFiCommunicator::ESP8266WiFiCommunicator(int enable_pin, int reset_pin):
                AWiFiCommunicator(),
                _enable_pin(enable_pin),
                _reset_pin(reset_pin),
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
                if (_connected_to_host) {
                    return _stream->available();
                }
                else {
                    return 0;
                }
            }

            int ESP8266WiFiCommunicator::read() {
                if (_connected_to_host) {
                    return _stream->read();
                }
                else {
                    return -1;
                }
            }

            int ESP8266WiFiCommunicator::peek() {
                if (_connected_to_host) {
                    return _stream->peek();
                }
                else {
                    return -1;
                }
            }

            size_t ESP8266WiFiCommunicator::write(uint8_t byte) {
                if (!_connected_to_host) {
                    return 0;
                }
                return _stream->write(byte);
            }

            void ESP8266WiFiCommunicator::enable() {
                _switch_on_esp();
            }

            void ESP8266WiFiCommunicator::disable() {
                if (_connected) {
                    disconnect();
                }
                _switch_off_esp();
            }

            int ESP8266WiFiCommunicator::connect() {
                if (!isAccessPointConfigured()) {
                    return -1;
                }
                if (_mode == ACCESS_POINT) {
                    return _create_ap();
                }
                else if (_mode == STATION) {
                    return _connect_ap();
                }
                return -1;
            }

            int ESP8266WiFiCommunicator::disconnect() {
                if (!isAccessPointConfigured()) {
                    return -3;
                }
                if (isConnected()) {
                    return _reset_esp();
                }
                return 0;
                // TODO: To complete
            }

            int ESP8266WiFiCommunicator::connectToHost() {
                // TODO: To implement
                if (!isHostConfigured()) {
                    return -4;
                }
                while (_connect_to_tcp_socket());
                while (_enable_transparent_mode());
                while (_go_to_send_mode());
                return 0;
            }

            int ESP8266WiFiCommunicator::disconnectFromHost() {
                // TODO: To implement
                if (!isHostConfigured()) {
                    return -4;
                }
                int check = _command_check();
                if (check) {
                    return check;
                }
                if (!_connected_to_host) {
                    return -3;
                }
                delay(50);
                _stream->print(FH(symbol_plus_x3));
                delay(50);
                return 0;
            }

            bool ESP8266WiFiCommunicator::isConnected() const {
                if (_mode == ACCESS_POINT) {
                    return _connected;
                }
                else if (_mode == STATION) {
                    return _connected_to_host;
                }
                return false;
            }

            void ESP8266WiFiCommunicator::_switch_on_esp() {
                // TODO: To implement
                if (_stream == nullptr) {
                    return;
                }
                digitalWrite(_enable_pin, LOW);
                while (_wait_for_esp_ready());
            }

            void ESP8266WiFiCommunicator::_switch_off_esp() {
                // TODO: To implement
                digitalWrite(_enable_pin, HIGH);
                _enabled = false;
            }

            int ESP8266WiFiCommunicator::_command_check() {
                if (_stream == nullptr) {
                    return -1;
                }
                if (!_enabled) {
                    enable(); // Try to enable the esp
                    if (!_enabled) {
                        return -2;
                    }
                }
                return 0;
            }

            int ESP8266WiFiCommunicator::_esp_answer_check() {
                while (!_stream->available());
                char buffer[16];
                _stream->setTimeout(15000);
                while (1) {
                    int len = _stream->readBytesUntil(at_nl, buffer, 15);
                    if (len < 1) {
                        break;
                    }
                    buffer[len] = '\0';
                    for (size_t i = 0; buffer[i] != '\0'; i++) {
                        if (buffer[i] == '\r' || buffer[i] == at_nl) {
                            buffer[i] = '\0';
                        }
                    }
                    if (!STRCMP(buffer, at_ok)) {
                        return 0;
                    }
                    if (!STRCMP(buffer, at_error)) {
                        return -1;
                    }
                }
                return -2;
            }

            int ESP8266WiFiCommunicator::_test_esp() {
                if (_stream == nullptr) {
                    return -1;
                }
                _stream->print(FH(at_at));
                _stream->print(FH(at_eol));
                return _esp_answer_check();
            }

            int ESP8266WiFiCommunicator::_set_esp_station_mode() {
                int check = _command_check();
                if (check) {
                    return check;
                }
                _stream->print(FH(at_at));
                _stream->print(FH(symbol_plus));
                _stream->print(FH(at_cwmode));
                _stream->print(FH(equal_1));
                _stream->print(FH(at_eol));
                return _esp_answer_check();
            }

            int ESP8266WiFiCommunicator::_set_esp_ap_mode() {
                int check = _command_check();
                if (check) {
                    return check;
                }
                _stream->print(FH(at_at));
                _stream->print(FH(symbol_plus));
                _stream->print(FH(at_cwmode));
                _stream->print(FH(equal_2));
                _stream->print(FH(at_eol));
                return _esp_answer_check();
            }

            int ESP8266WiFiCommunicator::_connect_ap() {
                if (!isAccessPointConfigured()) {
                    return -4;
                }
                int check = _command_check();
                if (check) {
                    return check;
                }
                while (_set_esp_station_mode());
                do {
                    _stream->print(FH(at_at));
                    _stream->print(FH(symbol_plus));
                    _stream->print(FH(at_cwjap));
                    _stream->print(FH(equal_double_quotes));
                    _stream->print(_ap.ssid);
                    _stream->print(FH(double_quotes_comma_double_quotes));
                    _stream->print(_ap.password);
                    _stream->print(FH(double_quotes));
                    _stream->print(FH(at_eol));
                } while (_esp_answer_check());
                Serial.println(F("Connected to AP"));
                _connected = true;
                return 0;
            }

            int ESP8266WiFiCommunicator::_create_ap() {
                if (!isAccessPointConfigured()) {
                    return -4;
                }
                int check = _command_check();
                if (check) {
                    return check;
                }
                while (_set_esp_ap_mode());
                char channel[3];
                SNPRINTF(channel, 2, channel_format, _ap.channel);
                _stream->print(FH(at_at));
                _stream->print(FH(symbol_plus));
                _stream->print(FH(at_cwsap));
                _stream->print(FH(equal_double_quotes));
                _stream->print(_ap.ssid);
                _stream->print(FH(double_quotes_comma_double_quotes));
                _stream->print(_ap.password);
                _stream->print(FH(double_quotes_comma));
                _stream->print(channel);
                _stream->print(FH(comma_3));
                _stream->print(FH(at_eol));
                return _esp_answer_check();
            }

            int ESP8266WiFiCommunicator::_reset_esp() {
                int check = _command_check();
                if (check) {
                    return check;
                }
                /*digitalWrite(_reset_pin, HIGH);
                delay(1);
                digitalWrite(_reset_pin, LOW);
                _stream->flush();
                flush();*/
                _stream->print(FH(at_at));
                _stream->print(FH(symbol_plus));
                _stream->print(FH(at_rst));
                _stream->print(FH(at_eol));
                if (_esp_answer_check()) {
                    return -3;
                }
                _enabled = false;
                _connected = false;
                _connected_to_host = false;
                _receiving_data = false;
                return _wait_for_esp_ready();
            }

            int ESP8266WiFiCommunicator::_wait_for_esp_ready() {
                if (_stream == nullptr) {
                    return -1;
                }
                char buffer[10];
                while (1) {
                    int len = _stream->readBytesUntil(at_nl, buffer, 9);
                    if (len > 0) {
                        buffer[len] = '\0';
                        for (size_t i = 0; buffer[i] != '\0'; i++) {
                            if (buffer[i] == '\r' || buffer[i] == at_nl) {
                                buffer[i] = '\0';
                            }
                        }
                        if (!STRCMP(buffer, at_ready)) {
                            _enabled = true;
                            return 0;
                        }
                    }
                    else {
                        if (!_test_esp()) {
                            _enabled = true;
                            return 0;
                        }
                        else {
                            _enabled = false;
                            return -1;
                        }
                    }
                }
            }

            int ESP8266WiFiCommunicator::_connect_to_tcp_socket() {
                if (!isHostConfigured() || !isAccessPointConfigured()) {
                    return -5;
                }
                if (!_host.hport) {
                    return -3;
                }
                int check = _command_check();
                if (check) {
                    return check;
                }
                if (!isConnected()) {
                    while (connect());
                }
                do {
                    char strIp[16];
                    SNPRINTF(strIp, 16, ip::ip_format, _host.ipv4[0], _host.ipv4[1], _host.ipv4[2], _host.ipv4[3]);
                    _stream->print(FH(at_at));
                    _stream->print(FH(symbol_plus));
                    _stream->print(FH(at_cipstart));
                    _stream->print(FH(equal_tcp));
                    _stream->print(strIp);
                    _stream->print(FH(double_quotes_comma));
                    _stream->print(_host.hport);
                    _stream->print(FH(at_eol));
                } while (_esp_answer_check());
                /* if (_esp_answer_check()) {
                    return -4;
                } */
                Serial.println(F("Connected to host"));
                _connected_to_host = true;
                return 0;
            }

            int ESP8266WiFiCommunicator::_enable_transparent_mode() {
                int check = _command_check_peer();
                if (check) {
                    return check;
                }
                _stream->print(FH(at_at));
                _stream->print(FH(symbol_plus));
                _stream->print(FH(at_cipmode));
                _stream->print(FH(equal_1));
                _stream->print(FH(at_eol));
                return _esp_answer_check();
            }

            int ESP8266WiFiCommunicator::_go_to_send_mode() {
                int check = _command_check_peer();
                if (check) {
                    return check;
                }
                _stream->print(FH(at_at));
                _stream->print(FH(symbol_plus));
                _stream->print(FH(at_cipsend));
                _stream->print(FH(at_eol));
                return (_esp_answer_check() == -1) ? -1 : 0;
            }

            int ESP8266WiFiCommunicator::_command_check_peer() {
                if (!isHostConfigured()) {
                    return -4;
                }
                int check = _command_check();
                if (check) {
                    return check;
                }
                if (isAccessPointConfigured() && !isConnected()) {
                    check = connect();
                    if (check) {
                        return check;
                    }
                    if (!isConnected()) {
                        return -1;
                    }
                    check = connectToHost();
                    if (check) {
                        return check;
                    }
                    if (!_connected_to_host) {
                        return -3;
                    }
                }
                return 0;
            }

            int ESP8266WiFiCommunicator::_set_echo(bool state) {
                int check = _command_check();
                if (check) {
                    return check;
                }
                _stream->print(FH(at_ate));
                _stream->print(state);
                _stream->print(FH(at_eol));
                return _esp_answer_check();
            }

            void ESP8266WiFiCommunicator::flush() {
                if (_stream != nullptr) {
                    _stream->flush();
                }
            }
        }
    }
}
#endif /* !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) && !defined(DISABLE_WIFI) && !defined(DISABLE_ESP_AT_MODULE) */