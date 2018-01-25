#ifndef WIFI_TYPES_HPP
# define WIFI_TYPES_HPP

# include "network_ip_types.hpp"

namespace woodBox {
    namespace communication {
        namespace wifi {

            typedef char                    ascii_char;

/* Common types between all architectures */

            enum e_wifi_frequency_band {
                TWO_POINT_FOUR_GHZ,
                FIVE_GHZ
            };

            enum e_wifi_norm {
                WIFI_A,
                WIFI_AC,
                WIFI_AD,
                WIFI_AH,
                WIFI_AX,
                WIFI_B,
                WIFI_C,
                WIFI_D,
                WIFI_E,
                WIFI_F,
                WIFI_G,
                WIFI_H,
                WIFI_I,
                WIFI_IR,
                WIFI_J,
                WIFI_N,
                WIFI_R,
                WIFI_S,
                WIFI_U,
                WIFI_V
            };

            enum e_wifi_mode {
                AD_HOC,
                ACCESS_POINT,
                STATION,
                ACCESS_POINT_STATION
            };

            typedef e_wifi_frequency_band   wifi_frequency_band;
            typedef e_wifi_norm             wifi_norm;
            typedef ip::byte                wifi_channel;
            typedef e_wifi_mode             wifi_mode;
            typedef ascii_char              wifi_ssid[33];
            typedef ascii_char              wifi_password[33];

            struct s_wifi_client {
                ip::mac_address     mac;
                union {
                    ip::ipv4_address    ipv4;
                    ip::ipv6_address    ipv6;
                };
            };

            typedef s_wifi_client           WiFi_client;

            struct s_wifi_access_point {
                wifi_ssid           ssid;
                wifi_password       password;
                wifi_channel        channel;
                wifi_norm           norm;
                wifi_frequency_band band;
            };

            typedef s_wifi_access_point     WiFi_ap;

        }
    }
}

#endif /* WIFI_TYPES_HPP */
