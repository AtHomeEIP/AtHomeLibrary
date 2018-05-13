#include "AtHomeConfig.h"
#if !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK)
# include "NetworkIPTypes.hpp"

namespace athome {
    namespace communication {
        namespace ip {
# ifndef __AVR__
        const char ip_format[] =        "%hhu.%hhu.%hhu.%hhu";
#  else
        const PROGMEM char ip_format[] =        "%hhu.%hhu.%hhu.%hhu";
# endif /* __AVR__ */
        }
    }
}

#endif /* !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) */