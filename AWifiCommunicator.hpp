#ifndef AWIFICOMMUNICATOR_HPP
# define AWIFICOMMUNICATOR_HPP

# ifdef ARDUINO

#  include <Arduino.h>

# endif /* ARDUINO */

//# include <Stream.h>
# include "ICommunicator.hpp"
# include "wifi_types.hpp"

namespace woodBox {
	namespace communication {
		class AWiFiCommunicator : public ICommunicator {
			public:
				AWiFiCommunicator(const WiFi_ap * = nullptr, const WiFi_client * = nullptr, const wifi_mode = STATION, ICommunicator * = nullptr);
				AWiFiCommunicator(const AWiFiCommunicator &) = delete;
				AWiFiCommunicator &operator=(const AWiFiCommunicator &) = delete;
				virtual int available() = 0;
				virtual int read() = 0;
				virtual int peek() = 0;
				virtual size_t write(uint8_t) = 0;
				/* Methods from ICommunicator:
				virtual uint8_t &read(uint32_t) = 0; // Read n bytes
				virtual void write(const uint8_t &, uint32_t) = 0; // Write n bytes */
				virtual void connect(const WiFi_ap * = nullptr) = 0;
				virtual void disconnect() = 0;
				virtual void connectToHost(const tcp_host * = nullptr) = 0;
				virtual void disconnectFromHost() = 0;
				virtual bool isConnected() = 0;

				virtual const WiFi_ap &getAccessPoint() const;
				virtual const WiFi_client &getConnectionAddresses() const;
				//virtual const tcp_host &getHost() const;
				virtual wifi_mode getWiFiMode() const;
				//virtual const WiFi_client[] &getConnectedClients() const;
				
				virtual void setAccessPoint(const WiFi_ap &);
				virtual void setConnectionAddresses(const WiFi_client &);
				//virtual void setHost(const tcp_host &);
				virtual void setWiFiMode(wifi_mode);
				virtual void setICommunicatorToChipset(ICommunicator *);
			protected:
				wifi_mode	_mode;
				ICommunicator		*_streams;
				WiFi_ap		_ap;
				WiFi_client	_me;
				//tcp_host	_host;
				//WiFi_client	*_clients;
		};
	}
}

#endif /* AWIFICOMMUNICATOR_HPP */
