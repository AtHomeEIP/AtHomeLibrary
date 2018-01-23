#include "network_ip_types.hpp"

namespace woodBox {
    namespace communication {
        namespace ip {
# ifndef __AVR__
        const char ip_format[] =        "%hhu.%hhu.%hhu.%hhu";
# else
        const PROGMEM char ip_format[] =        "%hhu.%hhu.%hhu.%hhu";
# endif
        }
    }
}
