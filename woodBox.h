#ifndef WOODBOX_H
# define WOODBOX_H

# include "WoodBoxWiFiModule.hpp"
# include "WoodBoxModule.hpp"
# include "network_ip_types.hpp"
# include "wifi_types.hpp"
# ifdef ARDUINO
#  include "GroveChainableLED.hpp"
# endif
# include "ESP8266WiFiCommunicator.hpp"
# include "AnalogSensor.hpp"
# include "CommonCathodeRGBLed.hpp"

# if defined(__AVR__) && defined(ARDUINO)
#  include "ArduinoEEPROM.hpp"
using woodBox::storage::ArduinoEEPROM;
# endif

using woodBox::module::WoodBoxModule;
using woodBox::module::WoodBoxWiFiModule;
using woodBox::communication::wifi::ESP8266WiFiCommunicator;
using woodBox::sensor::AnalogSensor;
using woodBox::display::CommonCathodeRGBLed;

# ifdef ARDUINO
using woodBox::display::GroveChainableLED;
# endif

#endif /* WOODBOX_H */
