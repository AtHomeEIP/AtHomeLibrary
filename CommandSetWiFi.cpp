#include <ArduinoJson.h>
#include <string.h>
#include <stdint.h>
#include "CommandSetWiFi.hpp"
#include "woodbox_communication_protocol.hpp"
#include "AWiFiCommunicator.hpp"

/*
 * Example of setWiFiJson:
 * {
 * 'ssid': 'ElominpBox',
 * 'password': 'blabla2A',
 * 'channel': 10
 * }
 *
 * Note: esp8266 supports only 2.4ghz band, so this parameter is useless (as for now), and the norm is WiFi n.
 * Channel is optional.
 * Memory is preallocated for minimized json. Using useless characters could break parsing
 */

namespace woodBox {
    namespace communication {
        namespace commands {
            CommandSetWiFi::CommandSetWiFi(AWiFiCommunicator &com):
                    _ok(false),
                    _com(com) {
                memset(&_ap, 0, sizeof(WiFi_ap));
            }

            CommandSetWiFi::~CommandSetWiFi() {}

            void CommandSetWiFi::parse(ICommunicator &communicator) {
                char buffer[100];
                size_t len = 0;
                if ((len = communicator.readBytesUntil(end_of_command, buffer, 100))) {
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
                        _ok = true;
                    }
                }
            }

            void CommandSetWiFi::execute() {
                _com.setAccessPoint(_ap);
            }

            void CommandSetWiFi::reply(ICommunicator &communicator) {
                // Nothing to reply?
            }
        }
    }
}
