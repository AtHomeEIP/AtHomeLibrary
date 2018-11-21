#ifndef ATHOMECOMMUNICATIONPROTOCOL_HPP
#define ATHOMECOMMUNICATIONPROTOCOL_HPP

#include "AtHomeConfig.h"
#if !defined(DISABLE_COMMUNICATION)
#include "AtHomeFlashCommon.h"
#define ATHOME_NEW_LINE '\n'                // Line feed
#define ATHOME_SPACER '\t'                  // Tabulation
#define ATHOME_END_OF_COMMAND '\x03'        // End of Text / Ctrl^C
#define ATHOME_END_OF_COMMUNICATION '\x04'  // End of Transmission / Ctrl^D

namespace athome {
namespace communication {
namespace commands {
#if !defined(DISABLE_SENSOR)
extern const PROGMEM char setSensorThresholds[];
#endif /* !defined(DISABLE_SENSOR) */
extern const PROGMEM char uploadData[];
#if !defined(DISABLE_NETWORK)
#if !defined(DISABLE_WIFI)
extern const PROGMEM char setWiFi[];
#endif /* !defined(DISABLE_WIFI) */
extern const PROGMEM char setEndPoint[];
#endif /* !defined(DISABLE_NETWORK) */
extern const PROGMEM char setProfile[];
#if !defined(DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION)
extern const PROGMEM char setEncryption[];
#endif /* !defined(DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION) */

#if !defined(DISABLE_NETWORK)
#if !defined(DISABLE_WIFI)
extern const PROGMEM char ssid_key[];
extern const PROGMEM char password_key[];
#endif /* !defined(DISABLE_WIFI) */
extern const PROGMEM char ip_key[];
extern const PROGMEM char port_key[];
#endif /* !defined(DISABLE_NETWORK) */

#if !defined(DISABLE_TIME)
extern const PROGMEM char setDateTime[];
#endif /* !defined(DISABLE_TIME) */

extern const PROGMEM char okReply[];
extern const PROGMEM char koReply[];
}  // namespace commands
}  // namespace communication
}  // namespace athome

#endif /* if !defined(DISABLE_COMMUNICATION) */
#endif /* ATHOMECOMMUNICATIONPROTOCOL */
