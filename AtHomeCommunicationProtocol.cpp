#include "AtHomeConfig.h"
#if !defined(DISABLE_COMMUNICATION)
# include "AtHomeCommunicationProtocol.hpp"

namespace athome {
    namespace communication {
        namespace commands {
            const char spacer = '\t';
            const char end_of_command = '\x03'; // It's the end of text character, ie Ctrl^D
            const char end_of_communication = '\x04'; // It's the end of transmission character, ie Ctrl^C

            const PROGMEM char end_of_line[] = "\r\n";
            const PROGMEM char part_separator[] = "================================================================================";

            const PROGMEM char enumerate[] = "Enumerate";
            const PROGMEM char uploadData[] = "UploadData";
# if !defined(DISABLE_NETWORK)
#  if !defined(DISABLE_WIFI)
            const PROGMEM char setWiFi[] = "SetWiFi";
#  endif /* !defined(DISABLE_WIFI) */
            const PROGMEM char setEndPoint[] = "SetEndPoint";
# endif /* !defined(DISABLE_NETWORK) */
            const PROGMEM char setProfile[] = "SetProfile";

# if !defined(DISABLE_NETWORK)
#  if !defined(DISABLE_WIFI)
            const PROGMEM char ssid_key[] = "ssid";
            const PROGMEM char password_key[] = "password";
#  endif /* !defined(DISABLE_WIFI) */
            const PROGMEM char ip_key[] = "ip";
            const PROGMEM char port_key[] = "port";
# endif /* !defined(DISABLE_NETWORK) */
# if !defined(DISABLE_TIME)
            const PROGMEM char setDateTime[] = "SetDateTime";
# endif /* !defined(DISABLE_TIME) */
        }
    }
}
#endif /* !defined(DISABLE_COMMUNICATION) */