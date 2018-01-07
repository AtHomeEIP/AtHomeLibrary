namespace woodBox {
    namespace communication {
        namespace commands {
            const char *end_of_line = "\r\n";
            const char *part_separator = "================================================================================";
            const char spacer = '\t';
            const char end_of_command = '\x03'; // It's the end of text character, ie Ctrl^D
            const char end_of_communication = '\x04'; // It's the end of transmission character, ie Ctrl^C

            const char *enumerate = "Enumerate";
            const char *uploadData = "UploadData";
            const char *syncTime = "SyncTime";
            const char *setWiFi = "SetWiFi";
            const char *setEndPoint = "SetEndPoint";
            const char *setProfile = "SetProfile";

/*
 * Thinking communication wisely
 * - Shall we have only two communication interface? Sync + WoodBox? Two ICommunicators with dedicated methods?
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
    }
}
