#ifndef AWIFICOMMUNICATOR_HPP
# define AWIFICOMMUNICATOR_HPP

# include <Arduino.h>
# include <Stream.h>
# include "wifi_types.hpp"

namespace woodBox {
	namespace communication {
	    namespace wifi {
		    class AWiFiCommunicator : public Stream {
			    public:
				    AWiFiCommunicator(const WiFi_ap * = nullptr, const WiFi_client * = nullptr, const wifi_mode = STATION, Stream * = nullptr);
				    AWiFiCommunicator(const AWiFiCommunicator &) = delete;
				    AWiFiCommunicator &operator=(const AWiFiCommunicator &) = delete;
				    virtual int available() = 0;
				    virtual int read() = 0;
				    virtual int peek() = 0;
				    virtual size_t write(uint8_t) = 0;
				    virtual void flush() = 0;
				    /* Methods from ICommunicator:
				    virtual uint8_t &read(uint32_t) = 0; // Read n bytes
				    virtual void write(const uint8_t &, uint32_t) = 0; // Write n bytes
				    virtual void open() = 0;
				    virtual void close() = 0; */
				    virtual int connect() = 0;
				    virtual int disconnect() = 0;
				    virtual int connectToHost() = 0;
				    virtual int disconnectFromHost() = 0;
				    virtual bool isConnected() = 0;

				    const WiFi_ap &getAccessPoint() const;
				    const WiFi_client &getConnectionAddresses() const;
				    const tcp_host &getHost() const;
				    wifi_mode getWiFiMode() const;
				    //virtual const WiFi_client[] &getConnectedClients() const;
				
				    void setAccessPoint(const WiFi_ap &);
				    void setConnectionAddresses(const WiFi_client &);
				    void setHost(const tcp_host &);
				    void setWiFiMode(wifi_mode);
				    void setStreamToChipset(Stream *);
			    protected:
				    wifi_mode	        _mode;
				    Stream		        *_stream;
				    WiFi_ap		        _ap;
				    WiFi_client	        _me;
				    tcp_host	        _host;
				    //WiFi_client	    *_clients;
		    };
	    }
	}
}

#endif /* AWIFICOMMUNICATOR_HPP */
