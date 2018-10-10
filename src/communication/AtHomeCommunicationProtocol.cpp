#include "AtHomeConfig.h"
#if !defined(DISABLE_COMMUNICATION)
#include "AtHomeCommunicationProtocol.hpp"

namespace athome {
namespace communication {
namespace commands {
#if !defined(DISABLE_SENSOR)
const PROGMEM char setSensorThresholds[] = "SET_THRESHOLDS";
#endif /* !defined(DISABLE_SENSOR) */
const PROGMEM char uploadData[] = "UPLOAD_DATA";
#if !defined(DISABLE_NETWORK)
#if !defined(DISABLE_WIFI)
const PROGMEM char setWiFi[] = "SET_WIFI";
#endif /* !defined(DISABLE_WIFI) */
const PROGMEM char setEndPoint[] = "SET_END_POINT";
#endif /* !defined(DISABLE_NETWORK) */
const PROGMEM char setProfile[] = "SET_PROFILE";

#if !defined(DISABLE_NETWORK)
#if !defined(DISABLE_WIFI)
const PROGMEM char ssid_key[] = "ssid";
const PROGMEM char password_key[] = "password";
#endif /* !defined(DISABLE_WIFI) */
const PROGMEM char ip_key[] = "ip";
const PROGMEM char port_key[] = "port";
#endif /* !defined(DISABLE_NETWORK) */
#if !defined(DISABLE_TIME)
const PROGMEM char setDateTime[] = "SET_DATE_TIME";
#endif /* !defined(DISABLE_TIME) */
}  // namespace commands
}  // namespace communication
}  // namespace athome
#endif /* !defined(DISABLE_COMMUNICATION) */