#ifndef ATHOME_H
# define ATHOME_H

# include "AtHomeModule.hpp"
# include "NetworkIPTypes.hpp"
# include "WiFiTypes.hpp"
# ifdef ARDUINO
#  include "GroveChainableLED.hpp"
#  include "GroveDigitalLightSensor.hpp"
#  include "SoundSensor.hpp"
#  include "MQ2GasSensor.hpp"
# endif /* ARDUINO */
# ifndef __MSP430__
# include "AtHomeWiFiModule.hpp"
# include "ESP8266WiFiCommunicator.hpp"
# endif /* __MSP430__ */
# include "AnalogSensor.hpp"
# include "CommonCathodeRGBLed.hpp"
# include "CommonAnodeRGBLed.hpp"
# if defined(__AVR__) && defined(ARDUINO)
#  include "ArduinoEEPROM.hpp"
#  include "GroveAirQualitySensor.hpp"
# endif /* __AVR__ && ARDUINO */
# if !defined(__MSP430__) && !defined(TARGET_IS_MSP432P4XX) && !defined(__PIC32MX__)
#  include "NeoPixel.hpp"
# endif /* List of incompatible architectures */
# include "AHumiditySensor.hpp"
# include "ATemperatureSensor.hpp"
# include "ALuminositySensor.hpp"
# include "ANoiseSensor.hpp"
# include "AAirQualitySensor.hpp"
# include "DummySensor.hpp"
# include "MonochromaticLED.hpp"
# include "PWMLED.hpp"
# include "Thermistor.hpp"
# include "Maths.hpp"
# ifdef __MSP430__
#  include "MSP430FRAM.hpp"
# endif

using athome::module::AtHomeModule;
# ifndef __MSP430__
using athome::module::AtHomeWiFiModule;
using athome::communication::wifi::ESP8266WiFiCommunicator;
# endif /* __MSP430__ */
using athome::sensor::AnalogSensor;
using athome::display::CommonCathodeRGBLed;
using athome::display::CommonAnodeRGBLed;
using athome::sensor::AHumiditySensor;
using athome::sensor::ATemperatureSensor;
using athome::sensor::ALuminositySensor;
using athome::sensor::ANoiseSensor;
using athome::sensor::AAirQualitySensor;
using athome::sensor::DummySensor;
using athome::display::MonochromaticLED;
using athome::display::PWMLED;
using athome::sensor::Thermistor;
# ifdef ARDUINO
using athome::sensor::SoundSensor;
using athome::display::GroveChainableLED;
using athome::sensor::GroveDigitalLightSensor;
using athome::sensor::MQ2GasSensor;
# endif /* ARDUINO */
# if !defined(__MSP430__) && !defined(TARGET_IS_MSP432P4XX) && !defined(__PIC32MX__)
using athome::display::NeoPixel;
# endif /* List of incompatible architectures */
# if defined(__AVR__) && defined(ARDUINO)
using athome::storage::ArduinoEEPROM;
using athome::sensor::GroveAirQualitySensor;
# endif /* __AVR__ && ARDUINO */
# ifdef __MSP430__
using athome::storage::MSP430FRAM;
# endif

#endif /* ATHOME_H */
