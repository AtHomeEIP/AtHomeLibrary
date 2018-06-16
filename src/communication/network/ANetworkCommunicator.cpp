#include "AtHomeConfig.h"
#if !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK)
# include <string.h>
# include "ANetworkCommunicator.hpp"

namespace athome {
    namespace communication {
        ANetworkCommunicator::ANetworkCommunicator(ip::tcp_host *host):_hostConfigured(false) {
            (host == nullptr) ? memset(&_host, 0, sizeof(_host)) : memcpy(&_host, host, sizeof(_host));
        }

        ANetworkCommunicator::~ANetworkCommunicator() {}

        const ip::tcp_host &ANetworkCommunicator::getHost() const {
            return _host;
        }

        void ANetworkCommunicator::setHost(const ip::tcp_host &host) {
            memcpy(&_host, &host, sizeof(_host));
            _hostConfigured = true;
        }

        bool ANetworkCommunicator::isHostConfigured() {
            return _hostConfigured;
        }
    }
}

#endif /* !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) */