#ifndef __msp430
#include <ArduinoJson.h>
#endif
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "CommandSetWiFi.hpp"
#include "woodbox_communication_protocol.hpp"
#include "AWiFiCommunicator.hpp"

/*
 * Example of setWiFiJson:
 * {
 * 'ssid': 'ElominpBox',
 * 'password': 'blabla2A'
 * }
 *
 * Note: esp8266 supports only 2.4ghz band, so this parameter is useless (as for now), and the norm is WiFi n.
 * Channel is optional.
 * Memory is preallocated for minimized json. Using useless characters could break parsing
 */

namespace woodBox {
    namespace communication {
        namespace commands {
            namespace {
#ifndef __AVR__
                const char ssid_key[] = "ssid";
                const char password_key[] = "password";
#else
#include <avr/pgmspace.h>
                const PROGMEM char ssid_key[] = "ssid";
                const PROGMEM char password_key[] = "password";
#endif
            }

            CommandSetWiFi::CommandSetWiFi(wifi::AWiFiCommunicator &com):
                    _ok(false),
                    _com(com) {
                memset(&_ap, 0, sizeof(WiFi_ap));
            }

            CommandSetWiFi::~CommandSetWiFi() {}

            void CommandSetWiFi::parse(Stream &communicator) {
                char buffer[101];
                size_t len = 0;
                _ok = false;
                if ((len = communicator.readBytesUntil(end_of_command, buffer, 100))) {
                    buffer[len] = '\0';
                    StaticJsonBuffer<100> json;
                    JsonObject &root = json.parseObject(buffer);
                    const char *ssid = root.get<const char*>(ssid_key);
                    const char *password = root.get<const char *>(password_key);
                    if (ssid != nullptr && password != nullptr) {
                        strncpy(_ap.ssid, ssid, 32);
                        strncpy(_ap.password, password, 32);
                        _ap.ssid[32] = '\0';
                        _ap.password[32] = '\0';
                        _ok = true;
                    }
                }
            }

            void CommandSetWiFi::execute() {
                if (_ok) {
                    _com.setAccessPoint(_ap);
                }
            }

            void CommandSetWiFi::reply(Stream &communicator) {
                // Nothing to reply?
            }
        }
    }
}
