#ifndef AWIFICOMMUNICATOR_HPP
# define AWIFICOMMUNICATOR_HPP

# include <Arduino.h>
# include <Stream.h>
# include "wifi_types.hpp"

namespace woodBox {
	namespace communication {
	    namespace wifi {
	        /**
	         * Abstract class derived from Stream, declaring some new WiFi common interface methods, and implementing some utility methods for these `Stream`s
	         *
	         * Here's a simple example of implementing a UART to WiFi chipset pre-configured to connect a WiFi network and transparently bridge the serial connection always connected to Serial Stream:
	         *
	         * \code{.cpp}
	         * #include <AWiFiCommunicator.hpp>
	         *
	         * using woodBox::communication::wifi::AWiFiCommunicator;
	         *
	         * class MyUARTBridge : public AWiFiCommunicator {
	         *   public:
	         *     MyUARTBridge():AWiFiCommunicator() {
	         *       _stream = &Serial;
	         *     }
	         *     ~MyUARTBridge() {}
	         *
	         *     int available() { return _stream->available(); }
	         *     int read() { return _stream->return(); }
	         *     int peek() { return _stream->peek(); }
	         *     size_t write(uint8_t data) { return _stream->write(data); }
	         *     int connect() { return 0; }
	         *     int disconnect() { return 0; }
	         *     int connectToHost() { return 0; }
	         *     int disconnectFromHost() { return 0; }
	         *     bool isConnected() { return true; }
	         * }
	         * \endcode
	         */
		    class AWiFiCommunicator : public Stream {
			    public:
		            /**
		             * Base constructor taking optional parameters to initialize instance attributes.
		             */
				    AWiFiCommunicator(const WiFi_ap * = nullptr, const WiFi_client * = nullptr, const wifi_mode = STATION, Stream * = nullptr);
				    AWiFiCommunicator(const AWiFiCommunicator &) = delete;
				    AWiFiCommunicator &operator=(const AWiFiCommunicator &) = delete;
				    /**
				     * Virtual method inherited from Stream interface. [See documentation on Arduino reference.](https://www.arduino.cc/reference/en/language/functions/communication/stream/streamavailable/)
				     */
				    virtual int available() = 0;
				    /**
				     * Virtual method inherited from Stream interface. [See documentation on Arduino reference.](https://www.arduino.cc/reference/en/language/functions/communication/stream/streamread/)
				     */
				    virtual int read() = 0;
				    /**
				     * Virtual method inherited from Stream interface. [See documentation on Arduino reference.](https://www.arduino.cc/reference/en/language/functions/communication/stream/streampeek/)
				     */
				    virtual int peek() = 0;
				    /**
				     * Virtual method inherited from Stream interface. [See documentation on Arduino reference.](https://www.arduino.cc/en/Serial/Write)
				     */
				    virtual size_t write(uint8_t) = 0;
				    /**
				     * Virtual method inherited from Stream interface. [See documentation on Arduino reference.](https://www.arduino.cc/reference/en/language/functions/communication/stream/streamflush/)
				     */
				    virtual void flush() = 0;
				    /**
				     * Virtual method used to implement connection to WiFi network through the adapter.
				     *
				     * A return value of 0 is expected to indicate the connection was successful, otherwise all other values are available for error codes.
				     *
				     * Example:
				     *
				     * \code{.cpp}
				     * #include <string.h>
				     * #include <woodBox.h>
				     *
				     * using woodBox::communication::wifi::WiFi_ap; // Use this to not have to write the full namespace names further
				     * using woodBox::communication::wifi::AWiFiCommunicator;
				     *
				     * void my_function_connecting_wifi_to_network_called_foobar(AWiFiCommunicator &wifi) {
				     *   WiFi_ap network; // Declare a WiFi_ap structure
				     *   strcpy(network.ssid, "foobar"); // Copy "foobar" in the ssid field of the structure, as the SSID of a WiFi network is the name shown to humans
				     *   strcpy(network.password, "12345678"); // Copy "12345678" in the password field, to set the password used by the WiFi network
				     *   wifi.setAccessPoint(network); // Set the WiFi network parameters we created
				     *   if (!wifi.connect()) { // Connect to this network
				     *     Serial.println("Successfully connected to WiFi network");
				     *   } else {
				     *     Serial.println("Connection to WiFi network failed");
				     *   }
				     * }
				     * \endcode
				     */
				    virtual int connect() = 0;
				    /**
				     * Virtual method used to implement disconnection of the current WiFi network through the adapter.
				     *
				     * A return value of 0 is expected to indicate success, otherwise all other values are available for error codes.
				     *
				     * Usage is as simple as this:
				     *
				     * \code{.cpp}
				     * #include <AWiFiCommunicator.hpp>
				     *
				     * using woodBox::communication::wifi::AWiFiCommunicator;
				     *
				     * void my_function_disconnecting_wifi_adapters(AWiFiCommuncator &wifi) {
				     *   if (!wifi.disconnect()) {
				     *     Serial.println("Successfully disconnected from WiFi network");
				     *   } else {
				     *     Serial.println("Failed to disconnect from WiFi network"); // Is that really possible? In case we're stuck, it's a problem where the "have you tried turning it off and on again?" rocks ;)
				     * }
				     * \endcode
				     */
				    virtual int disconnect() = 0;
				    /**
				     * Virtual method used to connect adapter to a socket on a host.
				     *
				     * A return value of 0 is expected to indicate success, otherwise all other values are available for error codes.
				     *
				     * Example:
				     *
				     * \code{.cpp}
				     * #include <woodBox.h>
				     *
				     * using woodBox::communication::ip::tcp_host; // Use this to not have to write the full namespace names further
				     * using woodBox::communication::wifi::WiFi_client;
				     * using woodBox::communication::wifi::AWiFiCommunicator;
				     *
				     * void my_function_connecting_to_http_port_on_router(AWiFiCommunicator &wifi) {
				     *   const WiFi_client &my_ip = wifi.getConnectionAddresses(); // Get our actual MAC and IP (IPv4 or IPv6) addresses
				     *   tcp_host router = {{my_ip.ipv4[0], my_ip.ipv4[1], my_ip.ipv4[2], 1}, 80}; // Use our current IPv4 address to deduce the one of the router, which often is the same as ours but ending with the value 1
				     *   wifi.setHost(router); // Set the new host to connect on, our structure tcp_host is a couple IP + port
				     *   if (!wifi.connectToHost()) { // Connect to the socket on the host
				     *     Serial.println("Successfully connected to host");
				     *   } else {
				     *     Serial.println("Failed to connect to host");
				     *   }
				     * }
				     * \endcode
				     */
				    virtual int connectToHost() = 0;
				    /**
				     * Virtual method to disconnect from the current host, closing the socket.
				     *
				     * A return value of 0 is expected to indicate success, otherwise all other values are available for error codes.
				     *
				     * Example:
				     *
				     * \code{.cpp}
				     * #include <AWiFiCommunicator.hpp>
				     *
				     * using woodBox::communication::wifi::AWiFiCommunicator;
				     *
				     * void my_function_disconnecting_an_adapter_from_host(AWiFiCommunicator &wifi) {
				     *   if (!wifi.disconnectFromHost()) {
				     *     Serial.println("Successfully disconnected from host");
				     *   } else {
				     *     Serial.println("Failed to disconnect from host"); // If that happens, see the same scenario on disconnect method to have the solution ;)
				     *   }
				     * }
				     * \endcode
				     */
				    virtual int disconnectFromHost() = 0;
				    /**
				     * Virtual method to know if the adapter is connected to a WiFi network.
				     *
				     * Return true if the adapter is connected, otherwise return false;
				     *
				     * Example:
				     *
				     * \code{.cpp}
				     * #include <AWiFiCommunicator.hpp>
				     *
				     * using woodBox::communication::wifi::AWiFiCommunicator;
				     *
				     * void my_function_printing_if_adapter_is_connected_to_network_or_not(AWiFiCommunicator &wifi) {
				     *   Serial.println((wifi.isConnected) ? "Adapter is connected to WiFi network" : "Adapter isn't connected to WiFi network");
				     * }
				     * \endcode
				     */
				    virtual bool isConnected() = 0;
				    /**
				     * Method returning a reference on the current configured access point into the instance of AWiFiCommunicator.
				     *
				     * See woodBox::communication::wifi::WiFi_ap for more information on the content of this structure.
				     *
				     * Example:
				     *
				     * \code{.cpp}
				     * #include <woodBox.h>
				     *
				     * using woodBox::communication::wifi::WiFi_ap;
				     * using woodBox::communication::wifi::AWiFiCommunicator;
				     *
				     * void my_function_printing_current_access_point_info(AWiFiCommunicator &wifi) {
				     *   const WiFi_ap &access_point = wifi.getAccessPoint;
				     *   Serial.print("Network name: ");
				     *   Serial.println(access_point.ssid);
				     *   Serial.print("Password: ");
				     *   Serial.println(access_point.password);
				     * }
				     * \endcode
				     */
				    const WiFi_ap &getAccessPoint() const;
				    const WiFi_client &getConnectionAddresses() const;
				    const ip::tcp_host &getHost() const;
				    wifi_mode getWiFiMode() const;
				
				    void setAccessPoint(const WiFi_ap &);
				    void setConnectionAddresses(const WiFi_client &);
				    void setHost(const ip::tcp_host &);
				    void setWiFiMode(wifi_mode);
				    void setStreamToChipset(Stream *);
			    protected:
				    wifi_mode	        _mode;
				    Stream		        *_stream;
				    WiFi_ap		        _ap;
				    WiFi_client	        _me;
				    ip::tcp_host	    _host;
				    //WiFi_client	    *_clients;
		    };
	    }
	}
}

#endif /* AWIFICOMMUNICATOR_HPP */
