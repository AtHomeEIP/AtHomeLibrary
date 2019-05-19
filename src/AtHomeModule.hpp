#ifndef ATHOMEMODULE_HPP
#define ATHOMEMODULE_HPP

#define _TASK_INLINE  // Macro used by TaskScheduler library
#define _TASK_SLEEP_ON_IDLE_RUN

#include <TaskScheduler.h>
#include <alloca.h>
#include <stdint.h>
#include <string.h>
#include "ABaseModule.hpp"
#include "AEncryptedStream.hpp"
#include "ARGBLed.hpp"
#include "AWiFiCommunicator.hpp"
#include "AtHomeCommunicationProtocol.hpp"
#include "AtHomeConfig.h"
#include "AtHomeFlashCommon.h"
#include "AtHomeModuleStreamIO.hpp"
#include "CRC.hpp"
#include "ITime.hpp"
#include "Queue.hpp"

namespace athome {
namespace module {
/**
 * CustomCallback type is used to pass a callback to call during various events.
 *
 * Functions used as callback must have a compatible prototype to this:
 * \fn void name()
 */
using CustomCallback = void (*)();

#ifndef DISABLE_COMMUNICATION
/**
 * AtHomeCommandCommandPlugin callback is used to extend command interpreter and
 * called when an unknown command is the received.
 *
 * Functions used as callback receive a reference on a String holding the actual
 * command name and a reference on the Stream from which it has been received.
 *
 * These functions must have a prototype compatible to this one:
 * \fn void name(const String &, Stream &)
 */
using AtHomeCommandPlugin = void (*)(const char *, Stream &);

/**
 * Structure representing an AtHome callback, composed of two fields:
 *
 * - name: A storage optimized constant string representing the name of
 *   the command
 * - callback: The function called when a command is received corresponding
 *   to the `name` field
 */
struct Command {
  PGM_P name;
  AtHomeCommandPlugin callback;
};

/**
 * A table of command is an array of pointers on Command structures
 */
using CommandTable = const Command*[];

/**
 * A plugin list is a queue of Command structures
 */
using CommandPluginList = utility::Queue<Command>;

#endif /* DISABLE_COMMUNICATION */
#ifndef DISABLE_PERSISTENT_STORAGE
/**
 * AtHomeStoragePlugin callback is used to save or restore additional data from
 * actual storage.
 *
 * Functions used as callback will receive the current offset usable after the
 * module read or saved data in the storage, and a reference to this storage
 * interface.
 *
 * These functions must have a prototype compatible to this one:
 * \fn void name(size_t, athome::storage::IStorage &)
 */
typedef size_t (*AtHomeStoragePlugin)(size_t, storage::IStorage &);

using StoragePluginList = utility::Queue<const AtHomeStoragePlugin>;
#endif /* DISABLE_PERSISTENT_STORAGE */

/**
 * Template class implementing base AtHome Modules, taking the type representing
 * a reading from the intended sensor to use and the number of readings to save
 * in a buffer.
 */
template <typename T, size_t n>
class AtHomeModule : public ABaseModule, public AtHomeModuleStreamIO {
 public:
  /**
   * `moduleSerial` type represents a unique value used to identify a module
   * from other
   */
  typedef uint32_t moduleSerial;
#if !defined(DISABLE_PASSWORD) && !defined(DISABLE_COMMUNICATION)
  /**
   * `modulePassword` type represents a password value used to protect the
   * configuration of the module
   */
  typedef char modulePassword[17];
#endif /* !defined(DISABLE_PASSWORD) && !defined(DISABLE_COMMUNICATION) */
#ifndef DISABLE_TIME
  /**
   * `timestamp` type is used to represent sensor readings date
   */
  typedef time::ITime::DateTime t_timestamp;
#endif /* DISABLE_TIME */
  /**
   * AtHomeModule and derived classes are singletons, duplication is not allowed
   */
  AtHomeModule(const AtHomeModule &) = delete;
  /**
   * AtHomeModule and derived classes are singletons, as only one instance can
   * exist, copy is not possible
   */
  AtHomeModule &operator=(const AtHomeModule &) = delete;
  ~AtHomeModule() {}

  /**
   * Template function used to get the address of the current AtHomeModule or
   * derived class or instanciates it if there's still no instances existing.
   *
   * The template parameter takes the class type (and of course it's own
   * template parameters if it's a templated class).
   *
   * If no template parameter is given, it will create by default a AtHomeModule
   * instance.
   */
  template <typename U = AtHomeModule>
  static U *getInstance() {
    if (_instance == nullptr) {
      _instance = reinterpret_cast<void *>(new U());
    }
    return reinterpret_cast<U *>(_instance);
  }

  /**
   * Get a reference on the scheduler instance used to manage tasks used by the
   * module or for the user to add new custom tasks to execute.
   *
   * It's based on the TaskScheduler library, see here for the documentation:
   * https://github.com/arkhipenko/TaskScheduler/wiki
   *
   * [See there for the license of this library:
   * https://github.com/arkhipenko/TaskScheduler/blob/master/LICENSE.txt]
   * (https://github.com/arkhipenko/TaskScheduler/blob/master/LICENSE.txt)
   */
  Scheduler &getScheduler() { return _scheduler; }

  /**
   * Initialize (or reinitialize) Module
   */
  void setup() {
#ifndef DISABLE_PERSISTENT_STORAGE
    onRestoreFromStorage();
#endif /* DISABLE_PERSISTENT_STORAGE */
#ifndef DISABLE_SENSOR
    _sensorTask.set(_sensorInterval, TASK_FOREVER,
                    &AtHomeModule::_onSampleSensor);
    _scheduler.addTask(_sensorTask);
    _sensorTask.enableIfNot();
#endif /* DISABLE_SENSOR */
#ifndef DISABLE_COMMUNICATION
    _communicationTask.set(_communicationInterval, TASK_FOREVER,
                           &AtHomeModule::_onCommunicate);
    _uploadDataTask.set(_uploadDataInterval, TASK_FOREVER,
                        &AtHomeModule::_uploadData);
    _scheduler.addTask(_communicationTask);
    _scheduler.addTask(_uploadDataTask);
    _communicationTask.enableIfNot();
    _uploadDataTask.enableIfNot();
#endif /* DISABLE_COMMUNICATION */
  }

  /**
   * Main function used to execute the Module.
   *
   * It doesn't use an infinite loop to allow user to execute code around each
   * iterations of the module code, so it must be called in an infinite loop.
   */
  void run() { _scheduler.execute(); }

  /**
   * Stop the module features and deinitialize it.
   */
  void stop() {
#ifndef DISABLE_PERSISTENT_STORAGE
    onBackupOnStorage();
#endif /* DISABLE_PERSISTENT_STORAGE */
       // Todo: need to implement it
  }

#ifndef DISABLE_SENSOR
  /**
   * Set the interval between each sensor sampling in milliseconds.
   */
  void setSensorExecutionInterval(unsigned long ms) {
    if (ms && ms != _sensorInterval) {
      _sensorInterval = ms;
      _sensorTask.setInterval(_sensorInterval * TASK_MILLISECOND);
    }
  }

  /**
   * Get the interval between each time the module samples its sensor.
   */
  unsigned long getSensorExecutionInterval() const { return _sensorInterval; }

  /**
   * Set the callback called when the module sample a sensor.
   *
   * By default, the module has already a callback for this task, but the user
   * can change it by its own passed in parameter of this function.
   *
   * Calling this function passing no parameter or nullptr will restore the
   * default callback of this class.
   */
  void setSensorExecutionCallback(CustomCallback f = nullptr) {
    _sensorTask.setCallback((f == nullptr) ? &AtHomeModule::_onSampleSensor
                                           : f);
  }
#endif /* DISABLE_SENSOR */
#ifndef DISABLE_COMMUNICATION
  /**
   * Set the interval between each time the module listen for received commands
   * on its streams.
   */
  void setCommunicationExecutionInterval(unsigned long ms) {
    if (ms && ms != _communicationInterval) {
      _communicationInterval = ms;
      _communicationTask.setInterval(_communicationInterval * TASK_MILLISECOND);
    }
  }

#ifndef DISABLE_SENSOR
  /**
   * Set the interval between each time the module sends its stored sensor
   * readings over streams.
   */
  void setUploadDataExecutionInterval(unsigned long ms) {
    if (ms && ms != _uploadDataInterval) {
      _uploadDataInterval = ms;
      _uploadDataTask.setInterval(_uploadDataInterval * TASK_MILLISECOND);
    }
  }

  /**
   * Get the interval between each time the module sends its stored sensor
   * readings over its streams.
   */
  unsigned long getUploadDataExecutionInterval() const {
    return _uploadDataInterval;
  }

  /**
   * Set the callback called when the module sends its stored sensor readings
   * over its streams.
   *
   * By default, the module has already a callback for this task, but the user
   * can change it by its own passed in parameter of this function.
   *
   * Calling this function passing no parameter or nullptr will restore the
   * default callback of this class.
   */
  void setUploadDataExecutionCallback(CustomCallback f = nullptr) {
    _uploadDataTask.setCallback((f == nullptr) ? &AtHomeModule::_uploadData
                                               : f);
  }
#endif /* DISABLE_SENSOR */

  /**
   * Get the interval between each time the module listens for received commands
   * on its streams.
   */
  unsigned long getCommunicationExecutionInterval() const {
    return _communicationInterval;
  }

  /**
   * Set the callback called when the module listens for received inputs.
   *
   * By default, the module has already a callback for this task, but the user
   * can change it by its own passed in parameter of this function.
   *
   * Calling this function passing no parameter or nullptr will restore the
   * default callback of this class.
   */
  void setCommunicationExecutionCallback(CustomCallback f = nullptr) {
    _communicationTask.setCallback(
        (f == nullptr) ? &AtHomeModule::_onCommunicate : f);
  }

  /**
   * Set a callback called after default command interpreter executed and wasn't
   * able to execute received input, passing the command string and a reference
   * to the stream to the callback.
   */
  void setCommandPlugin(Command &command) {
    if (_communicationPlugin == nullptr) {
      _communicationPlugin = new CommandPluginList(command);
    } else {
      _communicationPlugin->push_back(command);
    }
  }

#ifndef DISABLE_PASSWORD
  /**
   * Set the password used to protect the module
   */
  void setPassword(const modulePassword &password) {
    strncpy(_password, password, sizeof(_password));
#ifndef DISABLE_PERSISTENT_STORAGE
    onBackupOnStorage();
#endif /* DISABLE_PERSISTENT_STORAGE */
  }

  /**
   * Return the current password used by the module
   */
  const modulePassword &getPassword() const { return _password; }
#endif /* DISABLE_PASSWORD */
#endif /* DISABLE_COMMUNICATION */
#ifndef DISABLE_PERSISTENT_STORAGE
  /**
   * Set a callback called after module backup was executed on storage, passing
   * the actual offset usable (after module owns data) and a reference to the
   * storage interface used.
   */
  void setOnBackupPlugin(AtHomeStoragePlugin plugin) {
    if (_onBackupPlugin == nullptr) {
      _onBackupPlugin = new StoragePluginList(plugin);
    } else {
      _onBackupPlugin->push_back(plugin);
    }
  }

  /**
   * Set a callback called after module data loading was executed on storage,
   * passing the actual offset usable (after module owns data) and a reference
   * to the storage interface used.
   */
  void setOnRestorePlugin(AtHomeStoragePlugin plugin) {
    if (_onRestorePlugin == nullptr) {
      _onRestorePlugin = new StoragePluginList(plugin);
    } else {
      _onRestorePlugin->push_back(plugin);
    }
  }
#endif /* DISABLE_PERSISTENT_STORAGE */

  /**
   * Return the unique serial of the module, used to identify it.
   */
  moduleSerial getSerial() const { return _serial; }

  /**
   * Set the unique serial used to identify the module.
   */
  void setSerial(moduleSerial serial) {
    _serial = serial;
#ifndef DISABLE_PERSISTENT_STORAGE
    onBackupOnStorage();
#endif /* DISABLE_PERSISTENT_STORAGE */
  }

  void setStreams(Stream **streams) {
    setClearStreams(streams);
  }

  Stream** getStreams() {
    return getClearStreams();
  }

 private:
#ifndef DISABLE_SENSOR
  static void _onSampleSensor() {
    AtHomeModule *instance = getInstance();
    if (instance != nullptr) {
      instance->onSampleSensor();
    }
  }
#endif /* DISABLE_SENSOR */
#ifndef DISABLE_COMMUNICATION
  static void _onCommunicate() {
    AtHomeModule *instance = getInstance();
    if (instance != nullptr) {
      instance->onCommunicate();
    }
  }
#ifndef DISABLE_SENSOR
  static void _uploadData() {
    AtHomeModule *instance = getInstance();
    if (instance != nullptr) {
      instance->uploadData();
    }
  }
#endif /* DISABLE_SENSOR */
#endif /* DISABLE_COMMUNICATION */
 protected:
#ifndef DISABLE_SENSOR
  struct AtHomeSensorMeasure {
    PGM_P label;
    T sample;
    utility::units::Unit unit;
    uint8_t estimate;
#ifndef DISABLE_TIME
    t_timestamp timestamp;
#endif /* DISABLE_TIME */
  };
#endif /* DISABLE_SENSOR */

  AtHomeModule()
      : ABaseModule(),
        AtHomeModuleStreamIO(),
#ifndef DISABLE_SENSOR
        _sensorInterval(DEFAULT_SENSOR_INTERVAL * TASK_MILLISECOND),
        _nbMeasures(0),
#endif /* DISABLE_SENSOR */
#ifndef DISABLE_COMMUNICATION
        _communicationInterval(DEFAULT_COMMUNICATION_INTERVAL *
                               TASK_MILLISECOND),
        _uploadDataInterval(DEFAULT_UPLOAD_DATA_INTERVAL * TASK_MILLISECOND),
        _communicationPlugin(nullptr),
#endif /* DISABLE_COMMUNICATION */
#ifndef DISABLE_PERSISTENT_STORAGE
        _onBackupPlugin(nullptr),
        _onRestorePlugin(nullptr),
#endif /* DISABLE_PERSISTENT_STORAGE */
        _serial(0) {
  }
#ifndef DISABLE_COMMUNICATION

  /**
   * Send an error alert to the host when a command fail
   */
  void send_command_error(Stream &stream, const char *command = nullptr) {
    printHelperLn(stream, FH(communication::commands::koReply));
    if (command != nullptr) {
      printHelper(stream, FH(command));
    }
    writeBytes<char>(stream, ATHOME_END_OF_COMMAND);
  }

  /**
   * Send an acknowledge to the host when a command success
   */
  void acknowledge_command(Stream &stream, const char *command = nullptr) {
    printHelperLn(stream, FH(communication::commands::okReply));
    if (command != nullptr) {
      printHelper(stream, FH(command));
    }
    writeBytes<char>(stream, ATHOME_END_OF_COMMAND);
  }

#ifndef DISABLE_SENSOR
  /**
   * Sends stored sensor readings over module streams.
   */
  void uploadData() {
    broadcastln(FH(communication::commands::uploadData));
    broadcast_varuint<moduleSerial>(_serial);
    broadcast_varuint<uint16_t>(time::absolute_year);
    broadcast_varuint<size_t>(_nbMeasures);
    for (size_t i = 0; i < _nbMeasures; i++) {
      uint8_t fields = 0;
      if (i) {
        if (_measures[i].label != _measures[i - 1].label) {
          fields |= 0x1;
        }
        if (_measures[i].unit.unit != _measures[i - 1].unit.unit) {
          fields |= 0x2;
        }
        if (_measures[i].unit.prefix != _measures[i - 1].unit.prefix) {
          fields |= 0x4;
        }
        if (_measures[i].estimate != _measures[i - 1].estimate) {
          fields |= 0x8;
        }
        if (_measures[i].sample != _measures[i - 1].sample) {
          fields |= 0x10;
        }
        if (_measures[i].timestamp != _measures[i - 1].timestamp) {
          fields |= 0x20;
        }
      } else {
        fields = 0x7F;
      }
      broadcast_varuint<uint8_t>(fields);
      if (fields & 0x1) {
        broadcast_raw_string(FH(_measures[i].label));
      }
      if (fields & 0x2) {
        raw_broadcast<uint8_t>(_measures[i].unit.unit);
      }
      if (fields & 0x4) {
        raw_broadcast<uint8_t>(_measures[i].unit.prefix);
      }
      if (fields & 0x8) {
        raw_broadcast<uint8_t>(_measures[i].estimate);
      }
      if (fields & 0x10) {
        broadcast_raw_string(_measures[i].sample);
      }
      if (fields & 0x20) {
        raw_broadcast<t_timestamp>(_measures[i].timestamp);
      }
    }
    broadcast(ATHOME_END_OF_COMMAND);
    _nbMeasures = 0;
#ifndef DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION
    flush_encryptedStreams();
#endif  // DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION
  }
#endif /* DISABLE_SENSOR */
#if !defined(DISABLE_PASSWORD) && !defined(DISABLE_COMMUNICATION)
  bool authenticate(Stream &stream) {
    modulePassword password;
    if (securedReadBytesUntil<modulePassword>(stream, password) < 1) {
      return false;
    }
    return !strncmp(_password, password, sizeof(_password));
  }
#endif /* !defined(DISABLE_PASSWORD) && !defined(DISABLE_COMMUNICATION) */
  /**
   * Called (or trigger if called) when a module listens for received commands.
   */
  void onCommunicate() {
    if (_streams == nullptr) {
      return;
    }
    for (size_t i = 0; _streams[i] != nullptr; i++) {
      _streams[i]->setTimeout(DEFAULT_STREAM_TIMEOUT);
      if (_streams[i]->available()) {
        char buffer[19];
        char buffer2;
        int len = 0;
        for (len = 0; len < 19; len++) {
          if (_streams[i]->readBytes(&buffer2, 1) < 0 ||
              buffer2 == ATHOME_NEW_LINE) {
            break;
          } else if (buffer2 == ATHOME_END_OF_COMMAND ||
                     (len == 17 && buffer2 != ATHOME_NEW_LINE)) {
            i--;
            len = 0;
            break;  // Means we found a end of command, so what was read until
                    // now was from a previous command
          } else if (buffer2 == ATHOME_END_OF_COMMUNICATION) {
            len = 0;
            break;
          } else if (buffer2 >= 'a' && buffer2 <= 'z') {
            buffer2 -= 32;  // Convert in upper case
          }
          buffer[len] = buffer2;
        }
        if (len < 1) {
          continue;  // Means the reading is invalid, so we pass to next stream
        }
        buffer[len] = '\0';
        bool found = false;
        for (size_t i = 0; _commands[i] != nullptr; i++) {
          if (!STRCMP(buffer, _commands[i]->name)) {
            _commands[i]->callback(buffer, *_streams[i]);
            found = true;
            break;
          }
        }
        if (!found && _communicationPlugin != nullptr) {
          CommandPluginList *list = _communicationPlugin;
          while (list != nullptr && !found) {
            Command *command = list->get();
            if (command != nullptr && !STRCMP(buffer, command->name)) {
              command->callback(buffer, *_streams[i]);
              found = true;
              break;
            }
            list = list->next();
          }
        }
        if (!found) {
          while (_streams[i]->read() != -1)
            ;
        }
      }
    }
  }

#endif /* DISABLE_COMMUNICATION */
#ifndef DISABLE_PERSISTENT_STORAGE
  /**
   * Called (or trigger if called) when module backup its data on a storage.
   */
  void onBackupOnStorage() {
    if (_storage != nullptr) {
      _storage->write(0, reinterpret_cast<const void *>(&_serial),
                      sizeof(_serial));
      size_t offset = sizeof(_serial);
#if !defined(DISABLE_PASSWORD) && !defined(DISABLE_COMMUNICATION)
      _storage->write(offset, reinterpret_cast<const void *>(_password),
                      sizeof(_password));
      offset += sizeof(_password);
#endif /* !defined(DISABLE_PASSWORD) && !defined(DISABLE_COMMUNICATION) */
#if !defined(DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION) && \
    !defined(DISABLE_COMMUNICATION)
      _storage->write(offset, reinterpret_cast<const void *>(_encryptionRawKey),
                      sizeof(_encryptionRawKey));
      offset += sizeof(_encryptionRawKey);
      _storage->write(offset, reinterpret_cast<const void *>(_encryptionRawIV),
                      sizeof(_encryptionRawIV));
      offset += sizeof(_encryptionRawIV);
#endif /* !defined(DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION) && \
          !defined(DISABLE_COMMUNICATION) */
      StoragePluginList *list = _onBackupPlugin;
      while (list != nullptr) {
        AtHomeStoragePlugin plugin = *(list->get());
        if (plugin != nullptr) {
          offset += plugin(offset, *_storage);
        }
        list = list->next();
      }
    }
  }

  /**
   * Called (or trigger if called) when module restore data from a storage.
   */
  void onRestoreFromStorage() {
    if (_storage != nullptr) {
      _storage->read(0, reinterpret_cast<void *>(&_serial), sizeof(_serial));
      if (!_serial) {
        return;  // The module is uninitialized
      }
      size_t offset = sizeof(_serial);
#if !defined(DISABLE_PASSWORD) && !defined(DISABLE_COMMUNICATION)
      _storage->read(offset, reinterpret_cast<void *>(_password),
                     sizeof(_password));
      offset += sizeof(_password);
#endif /* !defined(DISABLE_PASSWORD) && !defined(DISABLE_COMMUNICATION) */
#if !defined(DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION) && \
    !defined(DISABLE_COMMUNICATION)
      _storage->read(offset, reinterpret_cast<void *>(_encryptionRawKey),
                     sizeof(_encryptionRawKey));
      offset += sizeof(_encryptionRawKey);
      _storage->read(offset, reinterpret_cast<void *>(_encryptionRawIV),
                     sizeof(_encryptionRawIV));
      offset += sizeof(_encryptionRawIV);
#endif /* !defined(DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION) && \
          !defined(DISABLE_COMMUNICATION) */
      StoragePluginList *list = _onRestorePlugin;
      while (list != nullptr) {
        AtHomeStoragePlugin plugin = *(list->get());
        if (plugin != nullptr) {
          offset += plugin(offset, *_storage);
        }
        list = list->next();
      }
    }
  }
#endif /* DISABLE_PERSISTENT_STORAGE */
#ifndef DISABLE_SENSOR
  /**
   * Called (or trigger if called) when a module samples its sensor.
   */
  void onSampleSensor() {
    if (_nbMeasures >= n) {
      _nbMeasures = 0;
    }
    if (_sensor != nullptr && _nbMeasures < n) {
      const sensor::ISensor::ISensorValue &value = _sensor->getSample();
      _measures[_nbMeasures].unit = value.unit;
      _measures[_nbMeasures].estimate = value.estimate;
#ifndef DISABLE_TIME
      if (_clock != nullptr) {
        _measures[_nbMeasures].timestamp = _clock->getDateTime();
      }
#endif /* DISABLE_TIME */
      _measures[_nbMeasures].label = value.label;
      if (value.sampleRawPointer != nullptr) {
        memcpy(&(_measures[_nbMeasures].sample), value.sampleRawPointer,
               sizeof(T));
      } else {
        memset(&(_measures[_nbMeasures].sample), 0, sizeof(T));
      }
#ifndef DISABLE_DISPLAY
      onUpdateDisplay();
#endif /* DISABLE_DISPLAY */
      _nbMeasures++;
    }
  }
#ifndef DISABLE_DISPLAY
  /**
   * Called (or trigger if called) when a module update its display.
   */
  void onUpdateDisplay() {
    if (_display == nullptr || _sensor == nullptr) {
      return;
    }
    _display->setDisplayedEstimate(static_cast<sensor::ISensor::ISensorScale>(
        _measures[_nbMeasures].estimate));
    _display->update();
  }
#endif /* DISABLE_DISPLAY */
#endif /* DISABLE_SENSOR */
 private:
#ifndef DISABLE_COMMUNICATION
  int _setProfileSerial(Stream &stream) {
    moduleSerial serial;
    if (securedReadBytes<moduleSerial>(stream, serial, readBase64EncodedBytes) < 1) {
      return -1;
    }
    setSerial(serial);
    return 0;
  }
#ifndef DISABLE_PASSWORD
  int _setProfilePassword(Stream &stream) {
    modulePassword password;
    if (securedReadBytesUntil<modulePassword>(stream, password) < 1) {
      return -1;
    }
    setPassword(password);
    return 0;
  }
#endif /* DISABLE_PASSWORD */
#ifndef DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION
  int _setEncryptionKey(Stream &stream) {
    moduleEncryptionKey key;
    if (securedReadBytes<moduleEncryptionKey>(stream, key, readBase64EncodedBytes) < 1) {
      return -1;
    }
    setEncryptionKey(key);
    return 0;
  }

  int _setEncryptionIV(Stream &stream) {
    moduleEncryptionIV iv;
    if (securedReadBytes<moduleEncryptionIV>(stream, iv, readBase64EncodedBytes) < 1) {
      return -1;
    }
    setEncryptionIV(iv);
    return 0;
  }

  int _setEncryption(Stream &stream) {
    (
#ifndef DISABLE_PASSWORD
      authenticate(stream) ||
#endif /* DISABLE_PASSWORD */
      _setEncryptionKey(stream) ||
      _setEncryptionIV(stream)
    ) ?
      send_command_error(stream, communication::commands::setEncryption) :
      acknowledge_command(stream, communication::commands::setEncryption);
  }

  static void _setEncryptionCallback(const char *command, Stream &stream) {
    (void)command;
    AtHomeModule *instance = getInstance();
    if (instance != nullptr) {
      instance->_setEncryption(stream);
    }
  }
#endif /* DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION */

  void _setProfile(Stream &stream) {
#if !defined(DISABLE_PASSWORD)
    if (_setProfileSerial(stream) ||
#else
    if (_setProfileSerial(stream)
#endif /* !defined(DISABLE_PASSWORD) || \
          !defined(DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION) */
#if !defined(DISABLE_PASSWORD)
            _setProfilePassword(stream)
#endif /* DISABLE_PASSWORD */
    ) {
      send_command_error(stream, communication::commands::setProfile);
      return;
    }
    acknowledge_command(stream, communication::commands::setProfile);
  }

  static void _setProfileCallback(const char *command, Stream &stream) {
    (void)command;
    AtHomeModule *instance = getInstance();
    if (instance != nullptr) {
      instance->_setProfile(stream);
    }
  }
#ifndef DISABLE_TIME
  void _setDateTime(Stream &stream) {
#ifndef DISABLE_PASSWORD
    if (_clock == nullptr || !authenticate(stream)) {
#else
    if (_clock == nullptr) {
#endif /* DISABLE_PASSWORD */
      send_command_error(stream, communication::commands::setDateTime);
      return;
    }
    uint8_t buffer[7];
    if (securedReadBytes<uint8_t[7]>(stream, buffer, readBase64EncodedBytes) < 1) {
      send_command_error(stream, communication::commands::setDateTime);
      return;
    }
    time::ITime::DateTime time;
    time.second = buffer[0];
    time.minute = buffer[1];
    time.hour = buffer[2];
    time.day = buffer[3];
    time.month = buffer[4];
    time.year = 0;
    memcpy(&time::absolute_year, buffer + 5, 2);
    _clock->setCurrentDateTime(time);
    readBytes(stream, reinterpret_cast<char *>(buffer), 1);
    acknowledge_command(stream, communication::commands::setDateTime);
  }

  static void _setDateTimeCallback(const char *command, Stream &stream) {
    (void)command;
    AtHomeModule *instance = getInstance();
    if (instance != nullptr) {
      instance->_setDateTime(stream);
    }
  }
#endif /* DISABLE_TIME */
#ifndef DISABLE_SENSOR
  void _setSensorThresholds(Stream &stream) {
#ifndef DISABLE_PASSWORD
    if (_sensor == nullptr || !authenticate(stream)) {
#else
    if (_sensor == nullptr) {
#endif /* DISABLE_PASSWORD */
      while (stream.read() != ATHOME_END_OF_COMMAND)
        ;
      return;
    }
    sensor::ISensor::ISensorThresholds thresholds;
    {
      char buffer[2];
      if (securedReadBytes<char[2]>(stream, buffer, readBase64EncodedBytes) < 1 ||
          securedReadBytes<sensor::ISensor::SensorThreshold>(
              stream, thresholds.min, readBase64EncodedBytes) < 1 ||
          securedReadBytes<sensor::ISensor::SensorThreshold>(
              stream, thresholds.max) < 1) {
        send_command_error(stream,
                           communication::commands::setSensorThresholds);
        return;
      }
      thresholds.unit.unit = buffer[0];
      thresholds.unit.prefix = buffer[1];
    }
    _sensor->setThresholds(thresholds);
    uint8_t tmp;
    stream.readBytes(reinterpret_cast<char *>(&tmp), 1);
    acknowledge_command(stream, communication::commands::setSensorThresholds);
  }

  static void _setSensorThresholdsCallback(const char *command,
                                           Stream &stream) {
    (void)command;
    AtHomeModule *instance = getInstance();
    if (instance != nullptr) {
      instance->_setSensorThresholds(stream);
    }
  }
#endif /* DISABLE_SENSOR */
#endif /* DISABLE_COMMUNICATION */
 private:
#ifndef DISABLE_SENSOR
  unsigned long _sensorInterval;
  size_t _nbMeasures;
#endif /* DISABLE_SENSOR */
#ifndef DISABLE_COMMUNICATION
  unsigned long _communicationInterval;
  unsigned long _uploadDataInterval;
  CommandPluginList *_communicationPlugin;
#endif /* DISABLE_COMMUNICATION */
#ifndef DISABLE_PERSISTENT_STORAGE
  StoragePluginList *_onBackupPlugin;
  StoragePluginList *_onRestorePlugin;
#endif /* DISABLE_PERSISTENT_STORAGE */
  moduleSerial _serial;
  Scheduler _scheduler;
#ifndef DISABLE_SENSOR
  AtHomeSensorMeasure _measures[n];
#endif /* DISABLE_SENSOR */
#if !defined(DISABLE_PASSWORD) && !defined(DISABLE_COMMUNICATION)
  modulePassword _password;
#endif /* !defined(DISABLE_PASSWORD) && !defined(DISABLE_COMMUNICATION) */
#if !defined(DISABLE_SENSOR) && !defined(DISABLE_COMMUNICATION)
  Task _uploadDataTask;
#endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_COMMUNICATION) */
#ifndef DISABLE_SENSOR
  Task _sensorTask;
#endif /* DISABLE_SENSOR */
#ifndef DISABLE_COMMUNICATION
  Task _communicationTask;
#endif /* DISABLE_COMMUNICATION */
  static void *_instance;

 private:
#ifndef DISABLE_COMMUNICATION
  static const Command _commandSetProfile;
#ifndef DISABLE_TIME
  static const Command _commandSetDateTime;
#endif /* DISABLE_TIME */
#ifndef DISABLE_SENSOR
  static const Command _commandSetSensorThresholds;
#endif /* DISABLE_SENSOR */
#ifndef DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION
  static const Command _commandSetEncryption;
#endif /* DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION */
  static CommandTable _commands;
#endif /* DISABLE_COMMUNICATION */
};

template <typename T, size_t n>
void *AtHomeModule<T, n>::_instance = nullptr;
#ifndef DISABLE_COMMUNICATION
template <typename T, size_t n>
const Command AtHomeModule<T, n>::_commandSetProfile = {
    communication::commands::setProfile,
    AtHomeModule<T, n>::_setProfileCallback};
#ifndef DISABLE_TIME
template <typename T, size_t n>
const Command AtHomeModule<T, n>::_commandSetDateTime = {
    communication::commands::setDateTime,
    AtHomeModule<T, n>::_setDateTimeCallback};
#endif /* DISABLE_TIME */
#ifndef DISABLE_SENSOR
template <typename T, size_t n>
const Command AtHomeModule<T, n>::_commandSetSensorThresholds = {
    communication::commands::setSensorThresholds,
    AtHomeModule<T, n>::_setSensorThresholdsCallback};
#endif /* DISABLE_SENSOR */
#ifndef DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION
template <typename T, size_t n>
const Command AtHomeModule<T, n>::_commandSetEncryption = {
  communication::commands::setEncryption,
  AtHomeModule<T, n>::_setEncryptionCallback
};
#endif /* DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION */
template <typename T, size_t n>
CommandTable AtHomeModule<T, n>::_commands = {
    &AtHomeModule<T, n>::_commandSetProfile,
#ifndef DISABLE_TIME
    &AtHomeModule<T, n>::_commandSetDateTime,
#endif /* DISABLE_TIME */
#ifndef DISABLE_SENSOR
    &AtHomeModule<T, n>::_commandSetSensorThresholds,
#endif /* DISABLE_SENSOR */
#ifndef DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION
    &AtHomeModule<T, n>::_commandSetEncryption,
#endif /* DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION */
    nullptr};
#endif /* DISABLE_COMMUNICATION */
}  // namespace module
}  // namespace athome
#endif /* ATHOMEMODULE_HPP */
