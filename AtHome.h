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
#if defined(AVR_PRO) || defined(AVR_UNO_WIFI_DEV_ED) || defined(AVR_NG) || \
        defined(AVR_LILYPAD) || defined(AVR_BT) || defined(AVR_FIO) || \
        defined(AVR_ETHERNET) || defined(AVR_MINI) || defined(AVR_NANO) || \
        defined(AVR_DUEMILANOVE) || defined(AVR_UNO)
// The Grove air quality sensor library uses AVR registers, and is not portable to other platforms
#  include "GroveAirQualitySensor.hpp"
# endif /* List of compatible Arduino boards */
# if defined(__AVR__) && defined(ARDUINO)
#  include "ArduinoEEPROM.hpp"
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
# include "Thermistor.hpp"
# include "StaticUtils.hpp"
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
using athome::sensor::Thermistor;
# ifdef ARDUINO
using athome::sensor::SoundSensor;
using athome::display::GroveChainableLED;
using athome::sensor::GroveDigitalLightSensor;
using athome::sensor::MQ2GasSensor;
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
# if defined(__AVR__) && defined(ARDUINO)
using athome::storage::ArduinoEEPROM;
# endif /* __AVR__ && ARDUINO */
# ifdef __MSP430__
using athome::storage::MSP430FRAM;
# endif

#endif /* ATHOME_H */
