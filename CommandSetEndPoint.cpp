#ifndef __msp430
#include <ArduinoJson.h>
#endif
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "CommandSetEndPoint.hpp"
#include "woodbox_communication_protocol.hpp"
#include "AWiFiCommunicator.hpp"

namespace woodBox {
    namespace communication {
        namespace commands {
            namespace {
#ifndef __AVR__
                const char ip_key[] = "ip";
                const char port_key[] = "port";
#else
#include <avr/pgmspace.h>
                const PROGMEM char ip_key[] = "ip";
                const PROGMEM char port_key[] = "port";
#endif
            }

            CommandSetEndPoint::CommandSetEndPoint(wifi::AWiFiCommunicator &com):
                    _ok(false),
                    _com(com) {
                memset(&_host, 0, sizeof(tcp_host));
            }

            CommandSetEndPoint::~CommandSetEndPoint() {}

            void CommandSetEndPoint::parse(ICommunicator &communicator) {
                char buffer[42];
                size_t len = 0;
                if ((len = communicator.readBytesUntil(end_of_command, buffer, 42))) {
                    buffer[len] = '\0';
#ifndef __msp430
                    StaticJsonBuffer<42> json;
                    JsonObject &root = json.parseObject(buffer);
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
#endif
                }
                _ok = false;
            }

            void CommandSetEndPoint::execute() {
                if (_ok) {
                    _com.setHost(_host);
                    _com.connectToHost();
                }
            }

            void CommandSetEndPoint::reply(ICommunicator &communicator) {
                // Nothing to reply?
            }
        }
    }
}
