#ifndef ATHOME_H
# define ATHOME_H

# ifndef ARDUINO
#  error Bare metal is not yet supported
# endif

# include "AtHomeConfig.h"
# include "AtHomeModule.hpp"
# include "Maths.hpp"

using athome::module::AtHomeModule;

# ifndef DISABLE_SENSOR
#  ifndef DISABLE_HUMIDITY_SENSOR
#   include "AHumiditySensor.hpp"
using athome::sensor::AHumiditySensor;
#  endif /* DISABLE_HUMIDITY_SENSOR */
#  ifndef DISABLE_TEMPERATURE_SENSOR
#   include "ATemperatureSensor.hpp"
using athome::sensor::ATemperatureSensor;
#   ifndef DISABLE_THERMISTOR_SENSOR
#    include "Thermistor.hpp"
using athome::sensor::Thermistor;
#   endif /* DISABLE_THERMISTOR_SENSOR */
#   ifndef DISABLE_TMP36GZ_TEMPERATURE_SENSOR
#    include "TMP36GZTemperatureSensor.hpp"
using TMP36GZTemperatureSensor5V = athome::sensor::TMP36GZTemperatureSensor<5000000, 10>;
using TMP36GZTemperatureSensor3V3 = athome::sensor::TMP36GZTemperatureSensor<3300000, 10>;
#   endif /* DISABLE_TMP36GZ_TEMPERATURE_SENSOR */
#  endif /* DISABLE_TEMPERATURE_SENSOR */
#  ifndef DISABLE_LUMINOSITY_SENSOR
#   include "ALuminositySensor.hpp"
using athome::sensor::ALuminositySensor;
#   if !defined(DISABLE_LDR_LUMINOSITY_SENSOR)
#    include "LDRLuminositySensor.hpp"
using LDRLuminositySensor5V = athome::sensor::LDRLuminositySensor<5000000, 10, 10000>;
using LDRLuminositySensor3V3 = athome::sensor::LDRLuminositySensor<3300000, 10, 10000>;
#   endif /* DISABLE_LDR_LUMINOSITY_SENSOR */
#   if (defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__) ||\
       defined(__AVR_ATmega2560__)) && !defined(DISABLE_GROVE_DIGITAL_LIGHT_SENSOR)
#    include "GroveDigitalLightSensor.hpp"
        using athome::sensor::GroveDigitalLightSensor;
#   endif /* defined(ARDUINO) && (defined(__ATmega328__) || defined(__ATmega32U4__) || defined(__ATmega2560__)) &&
             !defined(DISABLE_GROVE_DIGITAL_LIGHT_SENSOR) */
#  endif /* DISABLE_LUMINOSITY_SENSOR */
#  ifndef DISABLE_NOISE_SENSOR
#   include "ANoiseSensor.hpp"
using athome::sensor::ANoiseSensor;
#   ifndef DISABLE_SOUND_SENSOR
#    include "SoundSensor.hpp"
using athome::sensor::SoundSensor;
#   endif /* DISABLE_SOUND_SENSOR */
#  endif /* DISABLE_NOISE_SENSOR */
#  ifndef DISABLE_AIR_QUALITY_SENSOR
#   if defined(ARDUINO_AVR_UNO) && !defined(DISABLE_GROVE_AIR_QUALITY_SENSOR)
#    include "GroveAirQualitySensor.hpp"
using athome::sensor::GroveAirQualitySensor;
#   endif /* ARDUINO_AVR_UNO && !DISABLE_GROVE_AIR_QUALITY_SENSOR */
#   if !defined(DISABLE_MQ2_SENSOR)
#    include "MQ2GasSensor.hpp"
using athome::sensor::MQ2GasSensor;
#   endif /* DISABLE_MQ2_SENSOR */
#  endif /* DISABLE_AIR_QUALITY_SENSOR */
#  ifndef DISABLE_DUMMY_SENSOR
#   include "DummySensor.hpp"
using athome::sensor::DummySensor;
#  endif /* DISABLE_DUMMY_SENSOR */
#  if !defined(DISABLE_ANALOG_SENSOR)
#   include "AnalogSensor.hpp"
using athome::sensor::AnalogSensor;
#  endif /* DISABLE_ANALOG_SENSOR */
# endif /* DISABLE_SENSOR */
# ifndef DISABLE_DISPLAY
#  ifndef DISABLE_MONOCHROMATIC_LED
#   include "MonochromaticLED.hpp"
using athome::display::MonochromaticLED;
#  endif /* DISABLE_MONOCHROMATIC_LED */
#  ifndef DISABLE_PWM_LED
#   include "PWMLED.hpp"
using athome::display::PWMLED;
#  endif /* DISABLE_PWD_LED */
#  if !defined(__MSP430__) && !defined(TARGET_IS_MSP432P4XX) && !defined(__PIC32MX__) && !defined(DISABLE_NEOPIXEL)
#   include "NeoPixel.hpp"
using athome::display::NeoPixel;
#  endif /* List of incompatible architectures */
#  if !defined(DISABLE_COMMON_ANODE_RGB_LED)
#   include "CommonAnodeRGBLed.hpp"
using athome::display::CommonAnodeRGBLed;
#  endif /* DISABLE_COMMON_ANODE_RGB_LED */
#  if !defined(DISABLE_COMMON_CATHODE_RGB_LED)
#   include "CommonCathodeRGBLed.hpp"
using athome::display::CommonCathodeRGBLed;
#  endif /* DISABLE_COMMON_CATHODE_RGB_LED */
#  if !defined(DISABLE_GROVE_CHAINABLE_LED)
#   include "GroveChainableLED.hpp"
using athome::display::GroveChainableLED;
#  endif /* DISABLE_GROVE_CHAINABLE_LED */
# endif /* DISABLE_DISPLAY */
# ifndef DISABLE_COMMUNICATION
#  ifndef DISABLE_NETWORK
#   include "NetworkIPTypes.hpp"
#   ifndef DISABLE_WIFI
#    include "WiFiTypes.hpp"
#    if !defined(__MSP430__) && !defined(DISABLE_ESP_AT_MODULE)
#     include "AtHomeWiFiModule.hpp"
#     include "ESP8266WiFiCommunicator.hpp"
using athome::module::AtHomeWiFiModule;
using athome::communication::wifi::ESP8266WiFiCommunicator;
#    endif /* !defined(__MSP430__) && !defined(DISABLE_ESP_AT_MODULE) */
#   endif /* DISABLE_WIFI */
#  endif /* DISABLE_NETWORK */
# endif /* DISABLE_COMMUNICATION */
# ifndef DISABLE_PERSISTENT_STORAGE
#  if defined(__MSP430__) && !defined(DISABLE_MSP430_FRAM)
#   include "MSP430FRAM.hpp"
using athome::storage::MSP430FRAM;
#  endif /* defined(__MSP430__) && !defined(DISABLE_MSP430_FRAM) */
#  if defined(__AVR__) && !defined(DISABLE_AVR_EEPROM)
#   include "ArduinoEEPROM.hpp"
using athome::storage::ArduinoEEPROM;
#  endif /* __AVR__ && !DISABLE_AVR_EEPROM */
# endif /* DISABLE_PERSISTENT_STORAGE */

#endif /* ATHOME_H */
