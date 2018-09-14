#ifndef ATHOMEMODULE_HPP
#define ATHOMEMODULE_HPP

#define _TASK_INLINE  // Macro used by TaskScheduler library
//# define _TASK_SLEEP_ON_IDLE_RUN

#include <TaskScheduler.h>
#include <alloca.h>
#include <stdint.h>
#include <string.h>
#include "ABaseModule.hpp"
#include "ARGBLed.hpp"
#include "AWiFiCommunicator.hpp"
#include "AtHomeCommunicationProtocol.hpp"
#include "AtHomeConfig.h"
#include "AtHomeFlashCommon.h"
#include "ITime.hpp"
#include "Queue.hpp"

namespace athome {
namespace module {
/**
 * customCallback type is used to pass a callback to call during various events.
 *
 * Functions used as callback must have a compatible prototype to this:
 * \fn void name()
 */
typedef void (*customCallback)();

typedef void (*t_callback)(void *, ...);
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
typedef void (*AtHomeCommandPlugin)(const char *, Stream &);

struct Command {
  PGM_P name;
  AtHomeCommandPlugin callback;
};

typedef const Command *CommandTable[];
typedef const Command **C_CommandTable;

using CommandPluginList = utility::ConstQueue<C_CommandTable>;
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
class AtHomeModule : public ABaseModule {
 public:
  /**
   * `moduleSerial` type represents a unique value used to identify a module
   * from other
   */
  typedef uint32_t moduleSerial;
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
      U *ptr = new U();
      _instance = reinterpret_cast<void *>(ptr);
      return ptr;
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
  void setSensorExecutionCallback(customCallback f = nullptr) {
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
  void setUploadDataExecutionCallback(customCallback f = nullptr) {
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
  void setCommunicationExecutionCallback(customCallback f = nullptr) {
    _communicationTask.setCallback(
        (f == nullptr) ? &AtHomeModule::_onCommunicate : f);
  }

  /**
   * Set a callback called after default command interpreter executed and wasn't
   * able to execute received input, passing the command string and a reference
   * to the stream to the callback.
   */
  void setCommandPlugin(CommandTable table) {
    if (_communicationPlugin == nullptr) {
      _communicationPlugin = new CommandPluginList(table);
    } else {
      _communicationPlugin->push_back(table);
    }
  }
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
  const moduleSerial getSerial() const { return _serial; }

  /**
   * Set the unique serial used to identify the module.
   */
  void setSerial(moduleSerial serial) {
    _serial = serial;
#ifndef DISABLE_PERSISTENT_STORAGE
    onBackupOnStorage();
#endif /* DISABLE_PERSISTENT_STORAGE */
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
   * Broadcast the data passed as parameter over all module streams.
   */
  template <typename U>
  inline void broadcast(const U &data) {
    if (_streams == nullptr) {
      return;
    }
    for (size_t i = 0; _streams[i] != nullptr; i++) {
      _streams[i]->print(data);
    }
  }

  /**
   * Broadcast the data passed as parameter over all module streams, followed by
   * line return string "\r\n".
   */
  template <typename U>
  inline void broadcastln(const U &data) {
    broadcast(data);
    broadcast(FH(ATHOME_NEW_LINE));
  }

  inline void raw_broadcast(const uint8_t *data, size_t len) {
    if (_streams == nullptr) {
      return;
    }
    for (size_t i = 0; _streams[i] != nullptr; i++) {
      _streams[i]->write(data, len);
    }
  }

  inline void raw_broadcast_empty() { broadcast('\0'); }

  template <typename U>
  inline void broadcast_string(const U &data) {
    broadcast(data);
    raw_broadcast_empty();
  }

  inline bool is_non_zero(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
      if (data[i] & 0xFF) {
        return true;
      }
    }
    return false;
  }

  inline void broadcast_varuint(uint64_t data) {
    if (!is_non_zero(reinterpret_cast<uint8_t *>(&data), sizeof(uint64_t))) {
      broadcast('\0');
      return;
    }
    while (is_non_zero(reinterpret_cast<uint8_t *>(&data), sizeof(uint64_t))) {
      uint8_t buffer = 0;
      buffer |= (data & 0b01111111);
      data >>= 7;
      if (is_non_zero(reinterpret_cast<uint8_t *>(&data), sizeof(uint64_t))) {
        buffer |= 0b10000000;
      }
      raw_broadcast(&buffer, 1);
    }
  }
#ifndef DISABLE_SENSOR
  /**
   * Sends stored sensor readings over module streams.
   */
  void uploadData() {
    broadcastln(FH(communication::commands::uploadData));
    broadcast_varuint(_serial);
    broadcast_varuint(time::absolute_year);
    broadcast_varuint(_nbMeasures);
#ifndef DISABLE_SENSOR
    for (size_t i = 0; i < _nbMeasures; i++) {
      if (!i) {
        broadcast_string(FH(_measures[i].label));
        raw_broadcast(reinterpret_cast<uint8_t *>(&_measures[i].unit.unit), 1);
        raw_broadcast(reinterpret_cast<uint8_t *>(&_measures[i].unit.prefix),
                      1);
        raw_broadcast(reinterpret_cast<uint8_t *>(&_measures[i].estimate), 1);
        broadcast_string(_measures[i].sample);
        raw_broadcast(reinterpret_cast<uint8_t *>(&_measures[i].timestamp), 1);
      } else {
        if (_measures[i].label == _measures[i - 1].label) {
          raw_broadcast_empty();
        } else {
          broadcast_string(FH(_measures[i].label));
        }
        if (_measures[i].unit.unit == _measures[i - 1].unit.unit) {
          raw_broadcast_empty();
        } else {
          raw_broadcast(reinterpret_cast<uint8_t *>(&_measures[i].unit.unit),
                        1);
        }
        if (_measures[i].unit.prefix == _measures[i - 1].unit.prefix) {
          raw_broadcast_empty();
        } else {
          raw_broadcast(reinterpret_cast<uint8_t *>(&_measures[i].unit.prefix),
                        1);
        }
        if (_measures[i].estimate == _measures[i - 1].estimate) {
          raw_broadcast_empty();
        } else {
          raw_broadcast(reinterpret_cast<uint8_t *>(&_measures[i].estimate), 1);
        }
        if (!memcmp(&_measures[i].sample, &_measures[i - 1].sample,
                    sizeof(T))) {
          raw_broadcast_empty();
        } else {
          broadcast_string(_measures[i].sample);
        }
        if (!memcmp(&_measures[i].timestamp, &_measures[i - 1].timestamp,
                    sizeof(t_timestamp))) {
          raw_broadcast_empty();
        } else {
          raw_broadcast(reinterpret_cast<uint8_t *>(&_measures[i].timestamp),
                        1);
        }
      }
      // raw_broadcast(reinterpret_cast<uint8_t *>(&_measures[i]),
      // sizeof(AtHomeSensorMeasure));
    }
#endif /* DISABLE_SENSOR */
    broadcast(ATHOME_END_OF_COMMAND);
#ifndef DISABLE_SENSOR
    _nbMeasures = 0;
#endif /* DISABLE_SENSOR */
  }
#endif /* DISABLE_SENSOR */
  /**
   * Called (or trigger if called) when a module listens for received commands.
   */
  void onCommunicate() {
    if (_streams == nullptr) {
      return;
    }
    for (size_t i = 0; _streams[i] != nullptr; i++) {
      if (_streams[i]->available()) {
        char buffer[18];
        char buffer2;
        int len = 0;
        for (len = 0; len < 18; len++) {
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
            C_CommandTable const &table = list->get();
            for (size_t j = 0; table[j] != nullptr; j++) {
              if (!STRCMP(buffer, table[j]->name)) {
                table[j]->callback(buffer, *_streams[i]);
                found = true;
                break;
              }
            }
            list = list->next();
          }
        }
        if (!found) {
          //_streams[i]->flush(); // Would also remove output buffers
          while (_streams[i]->read() != -1)
            ;
        }
        // while (_streams[i]->read() != -1); // Remove the rest of the input
      }
    }
  }

  /**
   * Flush all streams used by the module.
   */
  void flushStreams() {
    if (_streams == nullptr) {
      return;
    }
    for (size_t i = 0; _streams[i] != nullptr; i++) {
      _streams[i]->flush();
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
                      sizeof(moduleSerial));
      size_t offset = sizeof(moduleSerial);
      StoragePluginList *list = _onBackupPlugin;
      while (list != nullptr) {
        AtHomeStoragePlugin plugin = list->get();
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
      _storage->read(0, reinterpret_cast<void *>(&_serial),
                     sizeof(moduleSerial));
      if (!_serial) {
        return;  // The module is uninitialized
      }
      size_t offset = sizeof(moduleSerial);
      StoragePluginList *list = _onRestorePlugin;
      while (list != nullptr) {
        AtHomeStoragePlugin plugin = list->get();
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
      // TODO: need a time interface
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
  void _setProfile(Stream &stream) {
    moduleSerial serial;
    if (stream.readBytesUntil(ATHOME_END_OF_COMMAND,
                              reinterpret_cast<char *>(&serial),
                              sizeof(moduleSerial)) < 1) {
      return;
    }
    setSerial(serial);
    stream.readBytesUntil(ATHOME_END_OF_COMMAND,
                          reinterpret_cast<char *>(&serial), 1);
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
    if (_clock == nullptr) {
      while (stream.read() != ATHOME_END_OF_COMMAND)
        ;
      return;
    }
    char buffer[8];
    int len = stream.readBytesUntil(ATHOME_END_OF_COMMAND, buffer, 8);
    if (len < 1) {
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
    if (_sensor == nullptr) {
      while (stream.read() != ATHOME_END_OF_COMMAND)
        ;
      return;
    }
    sensor::ISensor::ISensorThresholds thresholds;
    {
      char buffer[2];
      if (stream.readBytes(buffer, 2) < 1 ||
          stream.readBytes(reinterpret_cast<char *>(&thresholds.min),
                           sizeof(thresholds.min)) < 1 ||
          stream.readBytes(reinterpret_cast<char *>(&thresholds.max),
                           sizeof(thresholds.max)) < 1) {
        return;
      }
      thresholds.unit.unit = buffer[0];
      thresholds.unit.prefix = buffer[1];
    }
    _sensor->setThresholds(thresholds);
    uint8_t tmp;
    stream.readBytes(reinterpret_cast<char *>(&tmp), 1);
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
  static CommandTable _commands;
#endif /* DISABLE_COMMUNICATION */
};

template <typename T, size_t n>
void *AtHomeModule<T, n>::_instance = nullptr;
#ifndef DISABLE_COMMUNICATION
template <typename T, size_t n>
const Command AtHomeModule<T, n>::_commandSetProfile = {
    communication::commands::uploadData,
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
template <typename T, size_t n>
CommandTable AtHomeModule<T, n>::_commands = {
    &AtHomeModule<T, n>::_commandSetProfile,
#ifndef DISABLE_TIME
    &AtHomeModule<T, n>::_commandSetDateTime,
#endif /* DISABLE_TIME */
#ifndef DISABLE_SENSOR
    &AtHomeModule<T, n>::_commandSetSensorThresholds,
#endif /* DISABLE_SENSOR */
    nullptr};
#endif /* DISABLE_COMMUNICATION */
}  // namespace module
}  // namespace athome
#endif /* ATHOMEMODULE_HPP */
