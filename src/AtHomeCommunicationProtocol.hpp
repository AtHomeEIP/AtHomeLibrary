#ifndef ATHOMECOMMUNICATIONPROTOCOL_HPP
# define ATHOMECOMMUNICATIONPROTOCOL_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_COMMUNICATION)
#  include "AtHomeFlashCommon.h"
#  define ATHOME_NEW_LINE               '\n'   // Line feed
#  define ATHOME_SPACER                 '\t'   // Tabulation
#  define ATHOME_END_OF_COMMAND         '\x03' // End of Text / Ctrl^C
#  define ATHOME_END_OF_COMMUNICATION   '\x04' // End of Transmission / Ctrl^D

namespace athome {
    namespace communication {
        namespace commands {
#  if !defined(DISABLE_WIFI)
#   if !defined(DISABLE_SENSOR)
            extern const PROGMEM char setSensorThresholds[];
#   endif /* !defined(DISABLE_SENSOR) */
            extern const PROGMEM char uploadData[];
#   if !defined(DISABLE_NETWORK)
#    if !defined(DISABLE_WIFI)
            extern const PROGMEM char setWiFi[];
#    endif /* !defined(DISABLE_WIFI) */
            extern const PROGMEM char setEndPoint[];
#   endif /* !defined(DISABLE_NETWORK) */
            extern const PROGMEM char setProfile[];

#   if !defined(DISABLE_NETWORK)
#    if !defined(DISABLE_WIFI)
            extern const PROGMEM char ssid_key[];
            extern const PROGMEM char password_key[];
#    endif /* !defined(DISABLE_WIFI) */
            extern const PROGMEM char ip_key[];
            extern const PROGMEM char port_key[];
#   endif /* !defined(DISABLE_NETWORK) */

#  if !defined(DISABLE_TIME)
            extern const PROGMEM char setDateTime[];
#  endif /* !defined(DISABLE_TIME) */

/*
 * Thinking communication wisely
 * - Shall we have only two communication interface? Sync + AtHome? Two ICommunicators with dedicated methods?
 * - Shall we have a variable number of interchangeable communication interface?
 * - Shall we have an interface of already defined methods to implement?
 * - Shall we have a dictionary of methods to use when receiving a message?
 *
 * Communication protocol:
 * - End of line: \r\n string
 * - Spacer: \t
 * - End of command: end of text character
 * - End of communication: end of transmission character
 *
 * - Line 1 should be method name
 * - There should be an array of parameters available to use for requests
 * - Request body should be able to transmit on a character mode interface, thus using encoding such as Base64
 *
 * Communication protocol format:
 * [aZ-09]+[EOL] // First line should be method name, insensible to format
 * ([aZ-09][Spacer]*)+[EOL] // N following lines is parameters array
 * [Separator][EOL] // Indicates end of parameters and beginning of body
 * [Stream][EOC][EOT]
 *
 * Communication methods
 * - Enumerate: Give module information, describe itself. Maybe should have a better name
 * - UploadData: Module sensor samples sent over interface. How to chose which interface if we don't specialize them? Configuration?
 * - SyncTime: Update clock of module
 * - SetWiFi: Set module WiFi configuration. How does a module know a ICommunicator is a WiFi interface?
 * - SetEndPoint: Set TCP socket endpoint (ip + port). How does a module know a ICommunicator implements a TCP/IP stack?
 * - SetProfile: Set a configuration profile to the module to store internally. What is supposed to be inside? What the module is supposed to do with it?
 *
 * Request object
 * - Should be derived from Printable?
 * - Attributes
 *   - Method name -> defined by constructor? Setter? Builder class?
 *   - Parameters -> defined by dictionary? Attributes in a class?
 *   - Body -> Blob
 *
 * Proposition:
 * - ICommunicators classes are discovered through dynamic_cast, eg.: A SetWiFi command is received,
 *   the corresponding receiver method iterate through all ICommunicators and if some are AWiFiCommunicator,
 *   they update the settings. But this exclude having several communicators of same type connected to several different pairs
 * - Event receivers are specialized classes implementing an interface, and are associated in a map to the corresponding event
 * - SendingCommand objects are a helper class used to send a command. Predefined command could have specialized classes implementing their action
 * */
        }

        namespace json {
            extern const PROGMEM char jsonDictBegin[];
            extern const PROGMEM char jsonDictEnd[];
            extern const PROGMEM char jsonListBegin[];
            extern const PROGMEM char jsonListEnd[];
            extern const PROGMEM char jsonItemSeparator[];
            extern const PROGMEM char jsonKeySeparator[];
            extern const PROGMEM char jsonStringQuote[];
            extern const PROGMEM char jsonStringDoubleQuotes[];
            extern const PROGMEM char jsonStringNestedQuote[];
            extern const PROGMEM char jsonStringNestedDoubleQuotes[];
            extern const PROGMEM char jsonListDictStart[];
            extern const PROGMEM char jsonListDictEnd[];
            extern const PROGMEM char jsonDictListStart[];
            extern const PROGMEM char jsonDictListEnd[];

            namespace uploadData {
                extern const PROGMEM char uploadDataStart[];
                extern const PROGMEM char uploadDataListStart[];
                extern const PROGMEM char uploadDataValueStart[];
                extern const PROGMEM char uploadDataTimestamp[];
                extern const PROGMEM char uploadDataEstimate[];
                extern const PROGMEM char uploadDataUnit[];
                extern const PROGMEM char uploadDataPrefix[];
                extern const PROGMEM char uploadDataLabel[];
            }

            namespace iso8601 {
                extern const PROGMEM char paddingDigit[];
                extern const PROGMEM char timeSeparator[];
                extern const PROGMEM char itemSeparator[];
            }
        }
#  endif /* DISABLE_WIFI */
    }
}

# endif /* if !defined(DISABLE_COMMUNICATION) */
#endif /* ATHOMECOMMUNICATIONPROTOCOL */
