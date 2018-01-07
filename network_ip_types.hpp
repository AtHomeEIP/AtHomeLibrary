#ifndef NETWORK_IP_TYPES_HPP
# define NETWORK_IP_TYPES_HPP

# include <stdint.h>

typedef uint8_t                    byte;
typedef uint16_t                port;

typedef byte                    mac_address[6];
typedef byte                    ipv4_address[4];
typedef byte                    ipv6_address[16];
typedef byte                    sha256_hash[32];

struct s_host {
    union {
        ipv6_address    ipv6;
        ipv4_address    ipv4;
    };
    port                hport;
};

typedef s_host                    tcp_host;
typedef s_host                    udp_host;

#endif /* NETWORK_IP_TYPES_HPP */