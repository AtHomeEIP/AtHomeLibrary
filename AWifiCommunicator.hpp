#ifndef AWIFICOMMUNICATOR_HPP
# define AWIFICOMMUNICATOR_HPP

# ifdef ARDUINO

#  include <Arduino.h>

# endif /* ARDUINO */

# include "ICommunicator.hpp"
# include "wifi_types.hpp"

namespace woodBox {
	namespace communication {
		class AWiFiCommunicator : public ICommunicator {
			public:
				AWiFiCommunicator(const WiFi_ap * = nullptr, const WiFi_client * = nullptr);
				AWiFiCommunicator(const AWiFiCommunicator &) = delete;
				virtual AWiFiCommunicator &operator=(const AWiFiCommunicator &) = delete;
				virtual ~AWiFiCommunicator() = 0;
				virtual uint8_t &read(uint32_t) = 0; // Read n bytes
				virtual void write(const uint8_t &, uint32_t) = 0; // Write n bytes
				virtual void connect(const WiFi_ap * = nullptr) = 0;
				virtual void disconnect() = 0;
				virtual void connectToHost(const tcp_host * = nullptr) = 0;
				virtual void disconnectFromHost() = 0;
				virtual const WiFi_ap &getAccessPoint();
				virtual const WiFi_client &getConnectionAddresses();
				virtual const tcp_host &getHost();
				virtual void setAccessPoint(const WiFi_ap &);
				virtual void setConnectionAddresses(const WiFi_client &);
				virtual void setHost(const tcp_host &);
			protected:
				WiFi_ap		_ap;
				WiFi_client	_me;
				tcp_host	_host;
		};
	}
}

#endif /* AWIFICOMMUNICATOR_HPP */