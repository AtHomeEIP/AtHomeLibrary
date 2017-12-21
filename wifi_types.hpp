#ifndef WIFI_TYPES_HPP
# define WIFI_TYPES_HPP

# include "network_ip_types.hpp"

typedef int8_t					ascii_char;

/* Common types between all architectures */

enum e_wifi_frequency_band {
	TWO_POINT_FOUR_GHZ,
	FIVE_GHZ
};

enum e_wifi_norm {
	A,
	AC,
	AD,
	AH,
	AX,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	IR,
	J,
	N,
	R,
	S,
	U,
	V
};

enum e_wifi_mode {
	AD_HOC,
	ACCESS_POINT,
	STATION,
	ACCESS_POINT_STATION
};

typedef e_wifi_frequency_band	wifi_frequency_band;
typedef e_wifi_norm				wifi_norm;
typedef byte					wifi_channel;
typedef e_wifi_mode				wifi_mode;
typedef ascii_char				wifi_ssid[33];
typedef ascii_char				wifi_password[33];

struct s_wifi_client {
	mac_address		mac;
	ipv4_address	ipv4;
	ipv6_address	ipv6;
};

typedef s_wifi_client			wifi_client;

struct s_wifi_access_point {
	wifi_ssid		ssid;
	wifi_password	password;
	wifi_channel	channel;
};

typedef s_wifi_access_point		wifi_ap;

struct s_wifi_configuration {
	wifi_ap				ap;
	wifi_norm			norm;
	wifi_frequency_band	band;
	wifi_channel		channel;
};

typedef s_wifi_configuration	wifi_configuration;

#endif /* WIFI_TYPES_HPP */