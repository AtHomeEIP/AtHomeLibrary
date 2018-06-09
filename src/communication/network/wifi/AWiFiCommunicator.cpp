#include "AtHomeConfig.h"
#if !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) && !defined(DISABLE_WIFI)
# include <string.h>
# include "AWiFiCommunicator.hpp"

namespace athome {
    namespace communication {
        namespace wifi {
            AWiFiCommunicator::AWiFiCommunicator(const WiFi_ap *ap, const WiFi_client *client, const wifi_mode mode, Stream *stream):
                    _mode(mode),
                    _stream(stream)
            {
                (ap != nullptr) ? memcpy(&_ap, ap, sizeof(WiFi_ap)) : memset(&_ap, 0, sizeof(WiFi_ap));
                (client != nullptr) ? memcpy(&_me, client, sizeof(WiFi_client)) : memset(&_me, 0, sizeof(WiFi_client));
            }

            const WiFi_ap &AWiFiCommunicator::getAccessPoint() const {
                return _ap;
            }

            const WiFi_client &AWiFiCommunicator::getConnectionAddresses() const {
                return _me;
            }

            wifi_mode AWiFiCommunicator::getWiFiMode() const {
                return _mode;
            }

            void AWiFiCommunicator::setAccessPoint(const WiFi_ap &ap) {
                disconnect();
                memcpy(&_ap, &ap, sizeof(WiFi_ap));
            }

            void AWiFiCommunicator::setConnectionAddresses(const WiFi_client &client) {
                disconnect();
                memcpy(&_me, &client, sizeof(WiFi_client));
            }

            void AWiFiCommunicator::setWiFiMode(wifi_mode mode) {
                disconnect();
                _mode = mode;
            }

            void AWiFiCommunicator::setStreamToChipset(Stream *stream) {
                _stream = stream;
            }
        }
    }
}
#endif /* !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) && !defined(DISABLE_WIFI) */