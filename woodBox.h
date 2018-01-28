#ifndef WOODBOX_H
# define WOODBOX_H

# ifdef __MSP430__
#  include <stdlib.h>
#  define nullptr NULL
# endif

# include "WoodBoxModule.hpp"
# include "network_ip_types.hpp"
# include "wifi_types.hpp"
# ifdef ARDUINO
#  include "GroveChainableLED.hpp"
# endif /* ARDUINO */
# ifndef __MSP430__
# include "WoodBoxWiFiModule.hpp"
# include "ESP8266WiFiCommunicator.hpp"
# endif /* __MSP430__ */
# include "AnalogSensor.hpp"
# include "CommonCathodeRGBLed.hpp"
# ifdef __AVR__
// The Grove air quality sensor library uses AVR registers, and is not portable to other platforms
#  include "GroveAirQualitySensor.hpp"
# endif /* __AVR__ */
# if defined(__AVR__) && defined(ARDUINO)
#  include "ArduinoEEPROM.hpp"
using woodBox::storage::ArduinoEEPROM;
# endif /* __AVR__ && ARDUINO */

using woodBox::module::WoodBoxModule;
# ifndef __MSP430__
using woodBox::module::WoodBoxWiFiModule;
using woodBox::communication::wifi::ESP8266WiFiCommunicator;
# endif /* __MSP430__ */
using woodBox::sensor::AnalogSensor;
using woodBox::display::CommonCathodeRGBLed;
# ifdef ARDUINO
using woodBox::display::GroveChainableLED;
# endif /* ARDUINO */
# ifdef __AVR__
using woodBox::sensor::GroveAirQualitySensor;
# endif /* __AVR__ */

#endif /* WOODBOX_H */
