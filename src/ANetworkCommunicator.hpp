#ifndef ANETWORKCOMMUNICATOR_HPP
# define ANETWORKCOMMUNICATOR_HPP
# include "AtHomeConfig.h"
# if !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK)
#  include <Stream.h>
#  include "NetworkIPTypes.hpp"

namespace athome {
    namespace communication {
        class ANetworkCommunicator : public Stream {
        public:
            ANetworkCommunicator(ip::tcp_host * = nullptr);
            ANetworkCommunicator(const ANetworkCommunicator &) = delete;
            ANetworkCommunicator &operator=(const ANetworkCommunicator &) = delete;
            ~ANetworkCommunicator();
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
            bool isHostConfigured();
        private:
            bool                _hostConfigured;
        protected:
            ip::tcp_host	    _host;
        };
    }
}

# endif /* !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) */
#endif /* ANETWORKCOMMUNICATOR_HPP */