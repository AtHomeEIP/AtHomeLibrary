#include <ArduinoJson.h>
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
 * 'password': 'blabla2A',
 * 'channel': 10,
 * 'ip': '192.168.0.1',
 * 'port': 65535
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
                const char ip_key[] = "ip";
                const char port_key[] = "port";
#else
#include <avr/pgmspace.h>
                const PROGMEM char ssid_key[] = "ssid";
                const PROGMEM char password_key[] = "password";
                const PROGMEM char ip_key[] = "ip";
                const PROGMEM char port_key[] = "port";
#endif
            }

            CommandSetWiFi::CommandSetWiFi(wifi::AWiFiCommunicator &com):
                    _ok(false),
                    _com(com) {
                memset(&_ap, 0, sizeof(WiFi_ap));
                memset(&_host, 0, sizeof(tcp_host));
            }

            CommandSetWiFi::~CommandSetWiFi() {}

            void CommandSetWiFi::parse(ICommunicator &communicator) {
                char buffer[130];
                size_t len = 0;
                if ((len = communicator.readBytesUntil(end_of_command, buffer, 130))) {
                    buffer[len] = '\0';
                    StaticJsonBuffer<100> json;
                    JsonObject &root = json.parseObject(buffer);
                    const char *ssid = root.get<const char*>(ssid_key);
                    const char *password = root.get<const char *>(password_key);
                    if (ssid != nullptr && password != nullptr) {
                        // C casting is ugly, but it's a hack to allow some C standard libs having different definitions of char to compile
                        strncpy((char *)_ap.ssid, ssid, 32);
                        strncpy((char *)_ap.password, password, 32);
                        _ap.ssid[32] = '\0';
                        _ap.password[32] = '\0';
                        const char *ip = root.get<const char *>(ip_key);
                        port p = root.get<port>(port_key);
                        if (ip != nullptr) {
                            sscanf(ip, "%hhu.%hhu.%hhu.%hhu",
                                   &(_host.ipv4[0]),
                                   &(_host.ipv4[1]),
                                   &(_host.ipv4[2]),
                                   &(_host.ipv4[3]));
                            _host.hport = p;
                        }
                        _ok = true;
                    }
                }
            }

            void CommandSetWiFi::execute() {
                _com.setAccessPoint(_ap);
                _com.connectToHost(&_host);
            }

            void CommandSetWiFi::reply(ICommunicator &communicator) {
                // Nothing to reply?
            }
        }
    }
}
