#ifndef AWIFICOMMUNICATOR_HPP
# define AWIFICOMMUNICATOR_HPP

# ifdef ARDUINO

#  include <Arduino.h>

# endif /* ARDUINO */

# include <Stream.h>
# include "ICommunicator.hpp"
# include "wifi_types.hpp"

namespace woodBox {
	namespace communication {
		class AWiFiCommunicator : public Stream {
			public:
				AWiFiCommunicator(const WiFi_ap * = nullptr, const WiFi_client * = nullptr, const wifi_mode = STATION);
				AWiFiCommunicator(const AWiFiCommunicator &) = delete;
				AWiFiCommunicator &operator=(const AWiFiCommunicator &) = delete;
				virtual ~AWiFiCommunicator();
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

				virtual const WiFi_ap &getAccessPoint();
				virtual const WiFi_client &getConnectionAddresses();
				virtual const tcp_host &getHost();
				virtual wifi_mode getWiFiMode();
				virtual const WiFi_client[] &getConnectedClients();
				
				virtual void setAccessPoint(const WiFi_ap &);
				virtual void setConnectionAddresses(const WiFi_client &);
				virtual void setHost(const tcp_host &);
				virtual void setWiFiMode(wifi_mode);
				virtual void setStreamToChipset(Stream *);
			protected:
				WiFi_ap		_ap;
				WiFi_client	_me;
				wifi_mode	_mode;
				Stream		*_stream;
				tcp_host	_host;
				WiFi_client	*_clients;
		};
	}
}

#endif /* AWIFICOMMUNICATOR_HPP */