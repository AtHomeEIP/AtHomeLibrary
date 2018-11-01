#ifndef ATHOMEWIFIMODULE_HPP
#define ATHOMEWIFIMODULE_HPP

#include "AtHomeConfig.h"
#if !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) && \
    !defined(DISABLE_WIFI)
#include <stdio.h>
#include "AtHomeFlashCommon.h"
#include "AtHomeNetworkModule.hpp"

namespace athome {
namespace module {
/**
 * Template class derived from athome::module::AtHomeModule, implementing AtHome
 * WiFi extension.
 *
 * The template parameters are the same than athome::module::AtHomeModule class,
 * taking the type representing in memory a sample from the sensor and the
 * number of samples to buffer.
 *
 * Example:
 *
 * \code{.cpp}
 * #include <AtHome.h>
 *
 * AtHomeWiFiModule<uint16_t, 15> *module = AtHomeModule<uint16_t,
 * 15>::getInstance<AtHomeWiFiModule<uint16_t, 15> >(); ESP8266WiFiCommunicator
 * esp8266(2, 3); // CH_ED (enable) pin of the ESP8266 is imagined to be
 * connected to pin 2 of the host, and RST (reset) pin is imagined to be
 * connected to pin 3 of the host
 *
 * void setup() {
 *   Serial1.begin(115200);
 *   esp8266.setStreamToChipset(&Serial1); // If you're using an Arduino UNO or
 * any other board with only one or no UART, use the SoftwareSerial library to
 * create a new Serial instance to pass to this function
 *   module->setWiFiCommunicator(esp8266);
 * }
 *
 * void loop() {
 *   // Put your code here
 * }
 * \endcode
 */
template <typename T, size_t n>
class AtHomeWiFiModule : public AtHomeNetworkModule<T, n> {
  friend class AtHomeModule<T, n>;

 public:
  AtHomeWiFiModule(const AtHomeWiFiModule &) = delete;
  AtHomeWiFiModule &operator=(const AtHomeWiFiModule &) = delete;
  ~AtHomeWiFiModule() {}

  /**
   * Set the WiFi object used for wireless communication by passing it as
   * reference.
   *
   * See class example for usage of this method.
   */
  void setWiFiCommunicator(communication::wifi::AWiFiCommunicator &wifi) {
    if (_wifi != nullptr) {
      _wifi->disconnect();
    }
    _wifi = &wifi;
    if (AtHomeModule<T, n>::getSerial()) {
      _wifi->connect();
    }
    AtHomeNetworkModule<T, n>::setNetworkCommunicator(wifi);
    // this->onRestoreFromStorage(); // Restore wifi parameters from storage
  }

 protected:
  AtHomeWiFiModule() : AtHomeNetworkModule<T, n>(), _wifi(nullptr) {
    AtHomeModule<T, n>::setCommandPlugin(_wifiCommands);
#ifndef DISABLE_PERSISTENT_STORAGE
    AtHomeModule<T, n>::setOnBackupPlugin(
        AtHomeWiFiModule::_saveWiFiParameters);
    AtHomeModule<T, n>::setOnRestorePlugin(
        AtHomeWiFiModule::_restoreWiFiParameters);
#endif /* DISABLE_PERSISTENT_STORAGE */
  }

 private:
  void setWiFi(Stream &communicator) {
    communication::wifi::WiFi_ap ap;
    char buffer;
    int len = communicator.readBytesUntil('\0', ap.ssid, sizeof(ap.ssid) - 1);
    ap.ssid[len] = '\0';
    len =
        communicator.readBytesUntil('\0', ap.password, sizeof(ap.password) - 1);
    ap.password[len] = '\0';
    if (_wifi != nullptr) {
      _wifi->disconnect();
      _wifi->setAccessPoint(ap);
      _wifi->connect();
    }
#ifndef DISABLE_PERSISTENT_STORAGE
    this->onBackupOnStorage();
#endif /* DISABLE_PERSISTENT_STORAGE */
  }
  static void _setWiFi(const char *command, Stream &stream) {
    (void)command;
    AtHomeWiFiModule *instance =
        reinterpret_cast<AtHomeWiFiModule *>(AtHomeModule<T, n>::getInstance());
    if (instance != nullptr) {
      instance->setWiFi(stream);
    }
  }
#ifndef DISABLE_PERSISTENT_STORAGE
  size_t saveWiFiParameters(size_t offset, storage::IStorage &storage) {
    if (_wifi == nullptr) {
      return 0;
    }
    size_t delta = 0;
    const communication::wifi::WiFi_ap &ap = _wifi->getAccessPoint();
    storage.write(offset, reinterpret_cast<const void *>(ap.ssid),
                  sizeof(communication::wifi::wifi_ssid));
    delta += sizeof(communication::wifi::wifi_ssid);
    storage.write(offset + delta, reinterpret_cast<const void *>(ap.password),
                  sizeof(communication::wifi::wifi_password));
    delta += sizeof(communication::wifi::wifi_password);
    return delta;
  }

  size_t restoreWiFiParameters(size_t offset, storage::IStorage &storage) {
    if (_wifi == nullptr) {
      return 0;
    }
    size_t delta = 0;
    communication::wifi::WiFi_ap ap;
    storage.read(offset, reinterpret_cast<void *>(ap.ssid),
                 sizeof(communication::wifi::wifi_ssid));
    delta += sizeof(communication::wifi::wifi_ssid);
    storage.read(offset + delta, reinterpret_cast<void *>(ap.password),
                 sizeof(communication::wifi::wifi_password));
    delta += sizeof(communication::wifi::wifi_password);
    _wifi->setAccessPoint(ap);
    return delta;
  }
#endif /* DISABLE_PERSISTENT_STORAGE */
 private:
#ifndef DISABLE_PERSISTENT_STORAGE
  static size_t _saveWiFiParameters(size_t offset, storage::IStorage &storage) {
    AtHomeWiFiModule *instance =
        reinterpret_cast<AtHomeWiFiModule *>(AtHomeModule<T, n>::getInstance());
    if (instance != nullptr) {
      return instance->saveWiFiParameters(offset, storage);
    }
    return 0;
  }

  static size_t _restoreWiFiParameters(size_t offset,
                                       storage::IStorage &storage) {
    AtHomeWiFiModule *instance =
        reinterpret_cast<AtHomeWiFiModule *>(AtHomeModule<T, n>::getInstance());
    if (instance != nullptr) {
      return instance->restoreWiFiParameters(offset, storage);
    }
    return 0;
  }
#endif /* DISABLE_PERSISTENT_STORAGE */
 private:
  communication::wifi::AWiFiCommunicator *_wifi;

 private:
  static const Command _setWiFiCommand;
  static CommandTable _wifiCommands;
};

template <typename T, size_t n>
const Command AtHomeWiFiModule<T, n>::_setWiFiCommand = {
    communication::commands::setWiFi, AtHomeWiFiModule<T, n>::_setWiFi};

template <typename T, size_t n>
CommandTable AtHomeWiFiModule<T, n>::_wifiCommands = {
    &AtHomeWiFiModule::_setWiFiCommand, nullptr};
}  // namespace module
}  // namespace athome

#endif /* !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) && \
          !defined(DISABLE_WIFI) */
#endif /* ATHOMEWIFIMODULE_HPP */
