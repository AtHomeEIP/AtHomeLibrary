#include "AtHomeConfig.h"
#if !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK)
# include <string.h>
# include "ANetworkCommunicator.hpp"

namespace athome {
    namespace communication {
        ANetworkCommunicator::ANetworkCommunicator(ip::tcp_host *host) {
            (host == nullptr) ? memset(&_host, 0, sizeof(_host)) : memcpy(&_host, host, sizeof(_host));
        }

        ANetworkCommunicator::~ANetworkCommunicator() {}

        const ip::tcp_host &ANetworkCommunicator::getHost() const {
            return _host;
        }

        void ANetworkCommunicator::setHost(const ip::tcp_host &host) {
            memcpy(&_host, &host, sizeof(_host));
        }
    }
}

#endif /* !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) */