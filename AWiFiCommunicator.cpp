#include <string.h>
#include "AWiFiCommunicator.hpp"

namespace woodBox {
	namespace communication {
		AWiFiCommunicator::AWiFiCommunicator(const WiFi_ap *ap, const WiFi_client *client, const wifi_mode mode, ICommunicator *stream):
			/* _ap({
				"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",
				"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",
				0,
				N,
				TWO_POINT_FOUR_GHZ
			}),
			_me({
				{0,0,0,0,0,0},
				{0,0,0,0},
				{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
			}), */
			_mode(mode),
			_stream(stream)
			//_host({{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 0}),
			//_clients(nullptr)
		{
			(ap != nullptr) ? memcpy(&_ap, ap, sizeof(WiFi_ap)) : memset(&_ap, 0, sizeof(WiFi_ap));
			(client != nullptr) ? memcpy(&_me, client, sizeof(WiFi_client)) : memset(&_me, 0, sizeof(WiFi_client));
		}

		//AWiFiCommunicator::~AWiFiCommunicator() {}

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
			disconnectFromHost();
			disconnect();
			memcpy(&_ap, &ap, sizeof(WiFi_ap));
		}

		void AWiFiCommunicator::setConnectionAddresses(const WiFi_client &client) {
			disconnectFromHost();
			disconnect();
			memcpy(&_me, &client, sizeof(WiFi_client));
		}

		void AWiFiCommunicator::setWiFiMode(wifi_mode mode) {
			disconnectFromHost();
			disconnect();
			_mode = mode;
		}

		void AWiFiCommunicator::setICommunicatorToChipset(ICommunicator *stream) {
			_stream = stream;
		}
	}
}
