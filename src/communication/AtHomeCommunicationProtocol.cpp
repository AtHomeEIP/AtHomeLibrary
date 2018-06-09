#include "AtHomeConfig.h"
#if !defined(DISABLE_COMMUNICATION)
# include "AtHomeCommunicationProtocol.hpp"

namespace athome {
    namespace communication {
        namespace commands {
# if !defined(DISABLE_COMMUNICATION)
            const char spacer = '\t';
            const char end_of_command = '\x03'; // It's the end of text character, ie Ctrl^D
            const char end_of_communication = '\x04'; // It's the end of transmission character, ie Ctrl^C

            const PROGMEM char end_of_line[] = "\r\n";
            const PROGMEM char part_separator[] = "================================================================================";

#  if !defined(DISABLE_SENSOR)
            const PROGMEM char setSensorThresholds[] = "SetThresholds";
#  endif /* !defined(DISABLE_SENSOR) */
            const PROGMEM char uploadData[] = "UploadData";
#  if !defined(DISABLE_NETWORK)
#   if !defined(DISABLE_WIFI)
            const PROGMEM char setWiFi[] = "SetWiFi";
#   endif /* !defined(DISABLE_WIFI) */
            const PROGMEM char setEndPoint[] = "SetEndPoint";
#  endif /* !defined(DISABLE_NETWORK) */
            const PROGMEM char setProfile[] = "SetProfile";

#  if !defined(DISABLE_NETWORK)
#   if !defined(DISABLE_WIFI)
            const PROGMEM char ssid_key[] = "ssid";
            const PROGMEM char password_key[] = "password";
#   endif /* !defined(DISABLE_WIFI) */
            const PROGMEM char ip_key[] = "ip";
            const PROGMEM char port_key[] = "port";
#  endif /* !defined(DISABLE_NETWORK) */
#  if !defined(DISABLE_TIME)
            const PROGMEM char setDateTime[] = "SetDateTime";
#  endif /* !defined(DISABLE_TIME) */
        }

        namespace json {
            const PROGMEM char jsonDictBegin[] = "{";
            const PROGMEM char jsonDictEnd[] = "}";
            const PROGMEM char jsonListBegin[] = "[";
            const PROGMEM char jsonListEnd[] = "]";
            const PROGMEM char jsonItemSeparator[] = ",";
            const PROGMEM char jsonKeySeparator[] = ":";
            const PROGMEM char jsonStringQuote[] = "'";
            const PROGMEM char jsonStringDoubleQuotes[] = "\"";
            const PROGMEM char jsonStringNestedQuote[] = "\\\'";
            const PROGMEM char jsonStringNestedDoubleQuotes[] = "\\\"";
            const PROGMEM char jsonListDictStart[] = "[{";
            const PROGMEM char jsonListDictEnd[] = "]}";
            const PROGMEM char jsonDictListStart[] = "{[";
            const PROGMEM char jsonDictListEnd[] = "}]";

            namespace uploadData {
                const PROGMEM char uploadDataStart[] = "{\"Serial\":";
                const PROGMEM char uploadDataListStart[] = ",\"Data\":[";
                const PROGMEM char uploadDataValueStart[] = "{\"Value\":";
                const PROGMEM char uploadDataTimestamp[] = ",\"Timestamp\":\"";
                const PROGMEM char uploadDataEstimate[] = ",\"Estimate\":";
                const PROGMEM char uploadDataUnit[] = ",\"Unit\":";
                const PROGMEM char uploadDataPrefix[] = ",\"Prefix\":";
                const PROGMEM char uploadDataLabel[] = ",\"Label\":\"";
            }

            namespace iso8601 {
                const PROGMEM char paddingDigit[] = "0";
                const PROGMEM char timeSeparator[] = "T";
                const PROGMEM char itemSeparator[] = "-";
            }
        }
# endif /* DISABLE_WIFI */
    }
}
#endif /* !defined(DISABLE_COMMUNICATION) */