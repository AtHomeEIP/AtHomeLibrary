#ifndef ATHOME_H
# define ATHOME_H

# include "AtHomeModule.hpp"
# include "NetworkIPTypes.hpp"
# include "WiFiTypes.hpp"
# ifdef ARDUINO
#  include "GroveChainableLED.hpp"
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
# endif /* __AVR__ && ARDUINO */
# ifdef ARDUINO_AVR_UNO
#  include "GroveAirQualitySensor.hpp"
# endif /* ARDUINO_AVR_UNO */
# if defined(ARDUINO) && (defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__) ||\
     defined(__AVR_ATmega2560__))
#  include "GroveDigitalLightSensor.hpp"
# endif /* defined(ARDUINO) && (defined(__ATmega328__) || defined(__ATmega32U4__) || defined(__ATmega2560__)) */
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
# include "TMP36GZTemperatureSensor.hpp"
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
using TMP36GZTemperatureSensor5V = athome::sensor::TMP36GZTemperatureSensor<5000000, 10>;
using TMP36GZTemperatureSensor3V3 = athome::sensor::TMP36GZTemperatureSensor<3300000, 10>;
# ifdef ARDUINO
using athome::sensor::SoundSensor;
using athome::display::GroveChainableLED;
using athome::sensor::MQ2GasSensor;
# endif /* ARDUINO */
# if defined(ARDUINO) && (defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__) ||\
     defined(__AVR_ATmega2560__))
using athome::sensor::GroveDigitalLightSensor;
# endif /* defined(ARDUINO) && (defined(__ATmega328__) || defined(__ATmega32U4__) || defined(__ATmega2560__)) */
# if !defined(__MSP430__) && !defined(TARGET_IS_MSP432P4XX) && !defined(__PIC32MX__)
using athome::display::NeoPixel;
# endif /* List of incompatible architectures */
# if defined(__AVR__) && defined(ARDUINO)
using athome::storage::ArduinoEEPROM;
# endif /* __AVR__ && ARDUINO */
# ifdef ARDUINO_AVR_UNO
using athome::sensor::GroveAirQualitySensor;
# endif /* ARDUINO_AVR_UNO */
# ifdef __MSP430__
using athome::storage::MSP430FRAM;
# endif

#endif /* ATHOME_H */
