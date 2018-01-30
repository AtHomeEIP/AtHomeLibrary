#include "AtHomeCommunicationProtocol.hpp"

namespace athome {
    namespace communication {
        namespace commands {
            const char spacer = '\t';
            const char end_of_command = '\x03'; // It's the end of text character, ie Ctrl^D
            const char end_of_communication = '\x04'; // It's the end of transmission character, ie Ctrl^C
#ifndef __AVR__
            const char end_of_line[] = "\r\n";
            const char part_separator[] = "================================================================================";

            const char enumerate[] = "Enumerate";
            const char uploadData[] = "UploadData";
            const char syncTime[] = "SyncTime";
            const char setWiFi[] = "SetWiFi";
            const char setEndPoint[] = "SetEndPoint";
            const char setProfile[] = "SetProfile";

            const char ssid_key[] = "ssid";
            const char password_key[] = "password";
            const char ip_key[] = "ip";
            const char port_key[] = "port";


#else
# include <avr/pgmspace.h>
            const PROGMEM char end_of_line[] = "\r\n";
            const PROGMEM char part_separator[] = "================================================================================";

            const PROGMEM char enumerate[] = "Enumerate";
            const PROGMEM char uploadData[] = "UploadData";
            const PROGMEM char syncTime[] = "SyncTime";
            const PROGMEM char setWiFi[] = "SetWiFi";
            const PROGMEM char setEndPoint[] = "SetEndPoint";
            const PROGMEM char setProfile[] = "SetProfile";

            const PROGMEM char ssid_key[] = "ssid";
            const PROGMEM char password_key[] = "password";
            const PROGMEM char ip_key[] = "ip";
            const PROGMEM char port_key[] = "port";


#endif
        }
    }
}
