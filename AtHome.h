#ifndef ATHOME_H
# define ATHOME_H

# ifdef __MSP430__
#  include <stdlib.h>
#  define nullptr NULL
# endif

# include "AtHomeModule.hpp"
# include "NetworkIPTypes.hpp"
# include "WiFiTypes.hpp"
# ifdef ARDUINO
#  include "GroveChainableLED.hpp"
# endif /* ARDUINO */
# ifndef __MSP430__
# include "AtHomeWiFiModule.hpp"
# include "ESP8266WiFiCommunicator.hpp"
# endif /* __MSP430__ */
# include "AnalogSensor.hpp"
# include "CommonCathodeRGBLed.hpp"
#if defined(AVR_PRO) || defined(AVR_UNO_WIFI_DEV_ED) || defined(AVR_NG) || \
        defined(AVR_LILYPAD) || defined(AVR_BT) || defined(AVR_FIO) || \
        defined(AVR_ETHERNET) || defined(AVR_MINI) || defined(AVR_NANO) || \
        defined(AVR_DUEMILANOVE) || defined(AVR_UNO)
// The Grove air quality sensor library uses AVR registers, and is not portable to other platforms
#  include "GroveAirQualitySensor.hpp"
# endif /* List of compatible Arduino boards */
# if defined(__AVR__) && defined(ARDUINO)
#  include "ArduinoEEPROM.hpp"
using athome::storage::ArduinoEEPROM;
# endif /* __AVR__ && ARDUINO */
# if !defined(__MSP430__) && !defined(TARGET_IS_MSP432P4XX) && !defined(__PIC32MX__)
#  include "NeoPixel.hpp"
# endif /* List of incompatible architectures */

using athome::module::AtHomeModule;
# ifndef __MSP430__
using athome::module::AtHomeWiFiModule;
using athome::communication::wifi::ESP8266WiFiCommunicator;
# endif /* __MSP430__ */
using athome::sensor::AnalogSensor;
using athome::display::CommonCathodeRGBLed;
# ifdef ARDUINO
using athome::display::GroveChainableLED;
# endif /* ARDUINO */
#if defined(AVR_PRO) || defined(AVR_UNO_WIFI_DEV_ED) || defined(AVR_NG) || \
        defined(AVR_LILYPAD) || defined(AVR_BT) || defined(AVR_FIO) || \
        defined(AVR_ETHERNET) || defined(AVR_MINI) || defined(AVR_NANO) || \
        defined(AVR_DUEMILANOVE) || defined(AVR_UNO)
using athome::sensor::GroveAirQualitySensor;
# endif /* List of compatible Arduino boards */
# if !defined(__MSP430__) && !defined(TARGET_IS_MSP432P4XX) && !defined(__PIC32MX__)
using athome::display::NeoPixel;
# endif /* List of incompatible architectures */

#endif /* ATHOME_H */
