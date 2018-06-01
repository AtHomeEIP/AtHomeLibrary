#ifndef ATHOMECONFIG_H
# define ATHOMECONFIG_H

// Macros to define default interval values in ms
# define DEFAULT_SENSOR_INTERVAL        1000
# define DEFAULT_UPLOAD_DATA_INTERVAL   1000
# define DEFAULT_COMMUNICATION_INTERVAL 1

// Macros to uncomment to disable full part of modules
//# define DISABLE_SENSOR
//# define DISABLE_DISPLAY
//# define DISABLE_PERSISTENT_STORAGE
//# define DISABLE_COMMUNICATION
//# define DISABLE_POWER_MANAGEMENT
//# define DISABLE_HEARTBEAT
//# define DISABLE_TIME

// Macros to uncomment to disable network communications. DISABLE_NETWORK disable all types of networks
//# define DISABLE_NETWORK
//# define DISABLE_WIFI

// Macros to uncomment to disable evolved AtHomeModules
//# define DISABLE_ESP_AT_MODULE

// Macros to uncomment to disable families of sensors
//# define DISABLE_AIR_QUALITY_SENSOR
//# define DISABLE_TEMPERATURE_SENSOR
//# define DISABLE_HUMIDITY_SENSOR
//# define DISABLE_NOISE_SENSOR
//# define DISABLE_LUMINOSITY_SENSOR
//# define DISABLE_ANALOG_SENSOR

// Macros to uncomment to disable sensors
//# define DISABLE_DUMMY_SENSOR
//# define DISABLE_LDR_LUMINOSITY_SENSOR
//# define DISABLE_SOUND_SENSOR
//# define DISABLE_MQ2_SENSOR
# define DISABLE_GROVE_AIR_QUALITY_SENSOR
//# define DISABLE_GROVE_DIGITAL_LIGHT_SENSOR
//# define DISABLE_THERMISTOR_SENSOR
//# define DISABLE_TMP36GZ_TEMPERATURE_SENSOR

// Macros to uncomment to disable displays
//# define DISABLE_GROVE_CHAINABLE_LED
//# define DISABLE_COMMON_CATHODE_RGB_LED
//# define DISABLE_COMMON_ANODE_RGB_LED
//# define DISABLE_NEOPIXEL
//# define DISABLE_MONOCHROMATIC_LED
//# define DISABLE_PWM_LED

// Macros to uncomment to disable persisent storages
//# define DISABLE_AVR_EEPROM
//# define DISABLE_MSP430_FRAM

// Macros to uncomment to disable power management controllers
//# define DISABLE_AVR_POWER_MANAGEMENT

// Macros to uncomment to disable utilities
# define DISABLE_LIST
# define DISABLE_ITERABLE
//# define DISABLE_BUFFER
# define DISABLE_QUEUE

#endif /* ATHOMECONFIG_H */
