#ifndef AWIFICOMMUNICATOR_HPP
# define AWIFICOMMUNICATOR_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) && !defined(DISABLE_WIFI)
#  include <Arduino.h>
#  include <Stream.h>
#  include "WiFiTypes.hpp"

namespace athome {
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
	         * using athome::communication::wifi::AWiFiCommunicator;
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
				     * #include <AtHome.h>
				     *
				     * using athome::communication::wifi::WiFi_ap; // Use this to not have to write the full namespace names further
				     * using athome::communication::wifi::AWiFiCommunicator;
				     *
				     * void my_function_connecting_wifi_to_network_called_foobar(AWiFiCommunicator &wifi) {
				     *   WiFi_ap network; // Declare a WiFi_ap structure
				     *   strncpy(network.ssid, "foobar", sizeof(network.ssid)); // Copy "foobar" in the ssid field of the structure, as the SSID of a WiFi network is the name shown to humans
				     *   strncpy(network.password, "12345678", sizeof(network.password)); // Copy "12345678" in the password field, to set the password used by the WiFi network
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
				     * using athome::communication::wifi::AWiFiCommunicator;
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
				     * #include <AtHome.h>
				     *
				     * using athome::communication::ip::tcp_host; // Use this to not have to write the full namespace names further
				     * using athome::communication::wifi::WiFi_client;
				     * using athome::communication::wifi::AWiFiCommunicator;
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
				     * using athome::communication::wifi::AWiFiCommunicator;
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
				     * using athome::communication::wifi::AWiFiCommunicator;
				     *
				     * void my_function_printing_if_adapter_is_connected_to_network_or_not(const AWiFiCommunicator &wifi) {
				     *   Serial.println((wifi.isConnected) ? "Adapter is connected to WiFi network" : "Adapter isn't connected to WiFi network");
				     * }
				     * \endcode
				     */
				    virtual bool isConnected() const = 0;
				    /**
				     * Method returning a reference on the current configured access point into the instance of AWiFiCommunicator.
				     *
				     * See athome::communication::wifi::WiFi_ap for more information on the content of this structure.
				     *
				     * Example:
				     *
				     * \code{.cpp}
				     * #include <AtHome.h>
				     *
				     * using athome::communication::wifi::WiFi_ap;
				     * using athome::communication::wifi::AWiFiCommunicator;
				     *
				     * void my_function_printing_current_access_point_info(const AWiFiCommunicator &wifi) {
				     *   const WiFi_ap &access_point = wifi.getAccessPoint;
				     *   Serial.print("Network name: ");
				     *   Serial.println(access_point.ssid);
				     *   Serial.print("Password: ");
				     *   Serial.println(access_point.password);
				     * }
				     * \endcode
				     */
				    const WiFi_ap &getAccessPoint() const;
				    /**
				     * Method returning a reference on the current (or last if the module is currently disconnected) IP and MAC addresses of this instance of AWiFiCommunicator.
				     *
				     * See athome::communication::wifi::WiFi_client for more information on the content of this structure.
				     *
				     * Example:
				     *
				     * \code{.cpp}
				     * #include <stdio.h>
				     * #include <AtHome.h>
				     *
				     * using athome::communication::wifi::WiFi_client;
				     * using athome::communication::wifi::AWiFiCommunicator;
				     *
				     * void my_function_printing_current_ip_and_mac_addresses(const AWiFiCommunicator &wifi) {
				     *   const WiFi_client &addresses = wifi.getConnectionAddresses();
				     *   char my_ipv4[16];
				     *   char my_mac[18];
				     *   snprintf(my_ipv4, 16, "%hhu.%hhu.%hhu.%hhu", addresses.ipv4[0], addresses.ipv4[1], addresses.ipv4[2], addresses.ipv4[3]);
				     *   // Note: For MAC address conversion, if you're compiler is broken and doesn't promote char's to int, see this StackOverflow for fix:
				     *   // https://stackoverflow.com/questions/12344814/how-to-print-unsigned-char-as-2-digit-hex-value-in-c
				     *   snprintf(my_mac, 18, "%02x-%02x-%02x-%02x-%02x-%02x",
				     *            addresses.mac[0], addresses.mac[1], addresses.mac[2], addresses.mac[3], addresses.mac[4], addresses.mac[5]);
				     *   Serial.print("IPv4: ");
				     *   Serial.println(my_ipv4);
				     *   Serial.print("MAC: ");
				     *   Serial.println(my_mac);
				     * }
				     * \endcode
				     */
				    const WiFi_client &getConnectionAddresses() const;
				    /**
				     * Method returning a reference on the current (or last if the module is currently disconnected) connection to a host of this instance of AWiFiCommunicator.
				     *
				     * See athome::communication::ip::tcp_host for more information on the content of this structure.
				     *
				     * Example:
				     *
				     * \code{.cpp}
				     * #include <stdio.h>
				     * #include <AtHome.h>
				     *
				     * using athome::communication::ip::tcp_host;
				     * using athome::communication::wifi::AWiFiCommunicator;
				     *
				     * void my_function_printing_current_connection_to_a_host(const AWiFiCommunicator &wifi) {
				     *   const tcp_host &my_host = wifi.getHost();
				     *   char host_ipv4[16];
				     *   snprintf(host_ipv4, 16, "%hhu.%hhu.%hhu.%hhu", my_host.ipv4[0], my_host.ipv4[1], my_host.ipv4[2], my_host.ipv4[3]);
				     *   Serial.print("Host IPv4: ");
				     *   Serial.println(host_ipv4);
				     *   Serial.print("Host port: ");
				     *   Serial.println(my_host.hport);
				     * }
				     * \endcode
				     */
				    const ip::tcp_host &getHost() const;
				    /**
				     * Method returning a value of athome::communication::ip::wifi_mode enumeration, being STATION if the adapter connects to a WiFi network,
				     * or ACCESS_POINT if it creates one.
				     *
				     * See athome::communication::ip::wifi_mode for more information about this enumeration.
				     *
				     * Example:
				     *
				     * \code{.cpp}
				     * #include <AtHome.h>
				     *
				     * using athome::communication::wifi::wifi_mode;
				     * using athome::communication::wifi::AWiFiCommunicator;
				     *
				     * void my_function_printing_current_wifi_mode(const AWiFiCommunicator &wifi) {
				     *   wifi_mode mode = wifi.getWiFiMode();
				     *   switch (mode) {
				     *     case wifi_mode::STATION:
				     *       Serial.println("The WiFi adapter connects to a Wireless network");
				     *       break;
				     *     case wifi_mode::ACCESS_POINT:
				     *       Serial.println("The WiFi adapter creates a new Access Point (network)");
				     *       break;
				     *     default:
				     *       Serial.println("The WiFi adapter use a less common mode. See athome::communication::ip::wifi_mode for more information");
				     *       Serial.println(mode);
				     *       break;
				     *   }
				     * }
				     * \endcode
				     */
				    wifi_mode getWiFiMode() const;
				    /**
				     * Method setting the new access point parameters (to connect or create, depending on adapter mode).
				     *
				     * Calling this method will disconnect the adapter from current network to update adapter configuration, but will not reconnect it automatically.
				     *
				     * A call to athome::communication::wifi::AWiFiCommunicator::connect is required to connect to the new network after calling this method.
				     *
				     * Example:
				     *
				     * \code{.cpp}
				     * #include <string.h>
				     * #include <AtHome.h>
				     *
				     * using athome::communication::wifi::WiFi_ap;
				     *
				     * // ESP8266WiFiCommunicator is an implementation of AWiFiCommunicator for ESP8266 using default AT firmware
				     * // Note: using global variables is normally a dirty way of doing this,
				     * // but on Arduino it will allow you to see at compile time how much RAM is used by this part of your sketch before moving it in a function
				     * ESP8266WiFiCommunicator wifi(2, 3); // ESP8266WiFiCommunicator need to know on which pins of the board are connected the CH_PD and RST pins of the ESP8266
				     * WiFi_ap my_network;
				     *
				     * void setup() {
				     *   // We're opening the Stream used to communicate with the adapter
				     *   Serial.begin(115200);
				     *   // We set the Stream used to communicate with the adapter
				     *   wifi.setStreamToChipset(&Serial);
				     *   // We're configuring our WiFi adapter to connect to a network called "foobar" with the password "12345678"
				     *   strncpy(my_network.ssid, "foobar", sizeof(my_network.ssid));
				     *   strncpy(my_network.password, "12345678", sizeof(my_network.password));
				     *   wifi.setAccessPoint(my_network);
				     *   wifi.connect(); // You need to call this method for the adapter to actually connect to the new network
				     * }
				     *
				     * void loop() {
				     * }
				     * \endcode
				     */
				    void setAccessPoint(const WiFi_ap &);
				    /**
				     * Method setting addresses used by the WiFi network if a static IP is assigned to your adapter.
				     *
				     * Do not use this method if your adapter has its IP address through DHCP!
				     *
				     * If you're not sure about how your WiFi network is configured, don't use this method. Most of WiFi networks attributes IP addresses to devices automatically through DHCP.
				     *
				     * See athome::communication::wifi::WiFi_client for more information on the content of this structure.
				     *
				     * Example:
				     *
				     * \code{.cpp}
				     * #include <AtHome.h>
				     *
				     * using athome::communication::wifi::WiFi_client;
				     *
				     * ESP8266WiFiCommunicator wifi(2, 3); // ESP8266WiFiCommunicator need to know on which pins of the board are connected the CH_PD and RST pins of the ESP8266
				     * WiFi_client addresses = {{0x5C, 0xCF, 0x7F, 0x00, 0x00, 0x00}, {127, 0, 0, 1}}; // Users of this method should know why setting this IP address will be pretty useless
				     *
				     * void setup() {
				     *   // We're opening the Stream used to communicate with the adapter
                     *   Serial.begin(115200);
                     *   // We set the Stream used to communicate with the adapter
                     *   wifi.setStreamToChipset(&Serial);
				     *   ESP8266WiFiCommunicator.setConnectionAddresses();
				     *   ESP8266WiFiCommunicator.connect(); // You need to call it, even if you were previously already connected to a WiFi network as calling the setConnectionAddresses method will disconnect you from any current network
				     * }
				     *
				     * void loop() {
				     * }
				     * \endcode
				     */
				    void setConnectionAddresses(const WiFi_client &);
				    /**
				     * Method used to set on which host and port the adapter should connect by passing a reference on a athome::communication::ip::tcp_host as parameter.
				     *
				     * You should note that calling this method will close any existing connection to a previous host and that connection to the new host is not automatic,
				     * thus, the user must call the method athome::communication::wifi::AWiFiCommunicator::connectToHost when he wants to connect to the host.
				     *
				     * See athome::communication::ip::tcp_host for more information on the content of this structure.
				     *
				     * Example:
				     *
				     * \code{.cpp}
				     * #include <AtHome.h>
				     *
				     * using athome::communication::ip::tcp_host;
				     * using athome::communication::wifi::WiFi_client;
				     * using athome::module::ABaseModule;
				     *
				     * void my_function_connecting_an_adapter_to_web_server_on_router(ABaseModule &wifi) {
				     *   if (!wifi.isConnected()) {
				     *     return; // It's useless to try to connect on a server while not being connected to any network
				     *   }
				     *   const WiFi_client &my_addresses = wifi.getConnectionAddresses();
				     *   const tcp_host host = {{my_addresses.ipv4[0], my_addresses.ipv4[1], my_addresses.ipv4[2], 1}, 80};
				     *   wifi.setHost(host); // Will close any previous opened connection to an host
				     *   wifi.connectToHost(); // You need to call it after setting the new host when you want to connect it, connection is not automatic
				     * }
				     * \endcode
				     */
				    void setHost(const ip::tcp_host &);
				    /**
				     * Method used to define if the WiFi adapter is used to connect to an existing network, or to create a new access point.
				     *
				     * Note: this method will not call automatically the athome::communication::wifi::AWiFiCommunicator::connect method to create or to connect to the set up network.
				     *
				     * See athome::communication::wifi::wifi_mode for more information about values of this enumeration.
				     *
				     * Example to create a new access point named "foobar" protected by the password "12345678":
				     *
				     * \code{.cpp}
				     * #include <string.h>
				     * #include <AtHome.h>
				     *
				     * using athome::communication::wifi::WiFi_ap;
				     * using athome::communication::wifi::wifi_mode;
				     *
				     * ESP8266WiFiCommunicator wifi(2, 3); // ESP8266WiFiCommunicator need to know on which pins of the board are connected the CH_PD and RST pins of the ESP8266
				     * WiFi_ap ap;
				     *
				     * void setup() {
				     *   // We're opening the Stream used to communicate with the adapter
                     *   Serial.begin(115200);
                     *   // We set the Stream used to communicate with the adapter
                     *   wifi.setStreamToChipset(&Serial);
                     *   // We set the parameters to define the network we want to create:
				     *   strncpy(ap.ssid, "foobar", sizeof(ap.ssid));
				     *   strncpy(ap.password, "12345678", sizeof(ap.password));
				     *   wifi.setAccessPoint(ap);
				     *   wifi.setWiFiMode(wifi_mode::ACCESS_POINT);
				     *   // Create the access point:
				     *   wifi.connect();
				     * }
				     *
				     * void loop() {
				     * }
				     * \endcode
				     *
				     * Example to connect to an existing network called "foobar" protected by the password "12345678":
				     *
				     * \code{.cpp}
                     * #include <string.h>
                     * #include <AtHome.h>
                     *
                     * using athome::communication::wifi::WiFi_ap;
                     * using athome::communication::wifi::wifi_mode;
                     *
                     * ESP8266WiFiCommunicator wifi(2, 3); // ESP8266WiFiCommunicator need to know on which pins of the board are connected the CH_PD and RST pins of the ESP8266
                     * WiFi_ap ap;
                     *
                     * void setup() {
                     *   // We're opening the Stream used to communicate with the adapter
                     *   Serial.begin(115200);
                     *   // We set the Stream used to communicate with the adapter
                     *   wifi.setStreamToChipset(&Serial);
                     *   // We set the parameters to define the network we want to connect to:
                     *   strncpy(ap.ssid, "foobar", sizeof(ap.ssid));
                     *   strncpy(ap.password, "12345678", sizeof(ap.password));
                     *   wifi.setAccessPoint(ap);
                     *   wifi.setWiFiMode(wifi_mode::STATION);
                     *   // Connect to the network:
                     *   wifi.connect();
                     * }
                     *
                     * void loop() {
                     * }
                     * \endcode
				     */
				    void setWiFiMode(wifi_mode);
				    /**
				     * Method used to set the Stream used to communicate with the WiFi adapter (for example Serial on Arduino) by passing a pointer on it.
				     *
				     * A `nullptr` value will just indicate the instance of this class doesn't have a user defined Stream to communicate with the adapter.
				     *
				     * Example for an ESP8266 connected to the serial port of an Arduino (pins 0 and 1):
				     *
				     * \code{.cpp}
				     * #include <AtHome.h>
				     *
				     * ESP8266WiFiCommunicator wifi(2, 3);
				     *
				     * void setup() {
				     *   // We initialize the Stream so that the instance is able to use it. Here we start a serial connection at 115200 bauds:
				     *   Serial.begin(115200);
				     *   // We set the stream used to communicate with the ESP8266 adapter:
				     *   wifi.setStreamToChipset(&Serial);
				     * }
				     *
				     * void loop() {
				     * }
				     * \endcode
				     */
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

# endif /* !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) && !defined(DISABLE_WIFI) */
#endif /* AWIFICOMMUNICATOR_HPP */
