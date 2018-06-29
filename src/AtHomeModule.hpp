#ifndef ATHOMEMODULE_HPP
# define ATHOMEMODULE_HPP

# define _TASK_INLINE // Macro used by TaskScheduler library
//# define _TASK_SLEEP_ON_IDLE_RUN

# include "AtHomeConfig.h"
# include <TaskScheduler.h>
# include <stdint.h>
# include <alloca.h>
# include "ARGBLed.hpp"
# include "ABaseModule.hpp"
# include "AWiFiCommunicator.hpp"
# include "AtHomeCommunicationProtocol.hpp"
# include "AtHomeFlashCommon.h"
# include "ITime.hpp"

namespace athome {
    namespace module {
        /**
         * Template class implementing base AtHome Modules, taking the type representing a reading from the intended sensor to use and the number of readings to save in a buffer.
         */
        template <typename T, size_t n>
        class AtHomeModule : public ABaseModule {
            public:
                /**
                 * `moduleSerial` type represents a unique value used to identify a module from other
                 */
                typedef uint32_t                moduleSerial;
# ifndef DISABLE_TIME
                /**
                 * `timestamp` type is used to represent sensor readings date
                 */
                typedef time::ITime::DateTime   t_timestamp;
# endif /* DISABLE_TIME */
                /**
                 * AtHomeModule and derived classes are singletons, duplication is not allowed
                 */
                AtHomeModule(const AtHomeModule &) = delete;
                /**
                 * AtHomeModule and derived classes are singletons, as only one instance can exist, copy is not possible
                 */
                AtHomeModule &operator=(const AtHomeModule &) = delete;
                ~AtHomeModule() {}

            public:
                /**
                 * customCallback type is used to pass a callback to call during various events.
                 *
                 * Functions used as callback must have a compatible prototype to this:
                 * \fn void name()
                 */
                typedef void (*customCallback)();

# ifndef DISABLE_COMMUNICATION
                /**
                 * AtHomeCommandCommandPlugin callback is used to extend command interpreter and called when an unknown command is the received.
                 *
                 * Functions used as callback receive a reference on a String holding the actual command name and a reference on the Stream from which it has been received.
                 *
                 * These functions must have a prototype compatible to this one:
                 * \fn void name(const String &, Stream &)
                 */
                typedef void (*AtHomeCommandPlugin)(const char *, Stream &);
# endif /* DISABLE_COMMUNICATION */
# ifndef DISABLE_PERSISTENT_STORAGE
                /**
                 * AtHomeStoragePlugin callback is used to save or restore additional data from actual storage.
                 *
                 * Functions used as callback will receive the current offset usable after the module read or saved data in the storage, and a reference to this storage interface.
                 *
                 * These functions must have a prototype compatible to this one:
                 * \fn void name(size_t, athome::storage::IStorage &)
                 */
                typedef void (*AtHomeStoragePlugin)(size_t, storage::IStorage &);
# endif /* DISABLE_PERSISTENT_STORAGE */

                /**
                 * Template function used to get the address of the current AtHomeModule or derived class or instanciates it if there's still no instances existing.
                 *
                 * The template parameter takes the class type (and of course it's own template parameters if it's a templated class).
                 *
                 * If no template parameter is given, it will create by default a AtHomeModule instance.
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
                 * Get a reference on the scheduler instance used to manage tasks used by the module or for the user to add new custom tasks to execute.
                 *
                 * It's based on the TaskScheduler library, see here for the documentation: https://github.com/arkhipenko/TaskScheduler/wiki
                 *
                 * [See there for the license of this library: https://github.com/arkhipenko/TaskScheduler/blob/master/LICENSE.txt] (https://github.com/arkhipenko/TaskScheduler/blob/master/LICENSE.txt)
                 */
                Scheduler &getScheduler() {
                    return _scheduler;
                }

                /**
                 * Initialize (or reinitialize) Module
                 */
                void setup() {
# ifndef DISABLE_PERSISTENT_STORAGE
                    onRestoreFromStorage();
# endif /* DISABLE_PERSISTENT_STORAGE */
# ifndef DISABLE_SENSOR
                    _sensorTask.set(_sensorInterval, TASK_FOREVER, &AtHomeModule::_onSampleSensor);
                    _scheduler.addTask(_sensorTask);
                    _sensorTask.enableIfNot();
# endif /* DISABLE_SENSOR */
# ifndef DISABLE_COMMUNICATION
                    _communicationTask.set(_communicationInterval, TASK_FOREVER, &AtHomeModule::_onCommunicate);
                    _uploadDataTask.set(_uploadDataInterval, TASK_FOREVER, &AtHomeModule::_uploadData);
                    _scheduler.addTask(_communicationTask);
                    _scheduler.addTask(_uploadDataTask);
                    _communicationTask.enableIfNot();
                    _uploadDataTask.enableIfNot();
# endif /* DISABLE_COMMUNICATION */
                }

                /**
                 * Main function used to execute the Module.
                 *
                 * It doesn't use an infinite loop to allow user to execute code around each iterations of the module code,
                 * so it must be called in an infinite loop.
                 */
                void run() {
                    _scheduler.execute();
                }

                /**
                 * Stop the module features and deinitialize it.
                 */
                void stop() {
# ifndef DISABLE_PERSISTENT_STORAGE
                    onBackupOnStorage();
# endif /* DISABLE_PERSISTENT_STORAGE */
                    // Todo: need to implement it
                }

# ifndef DISABLE_SENSOR
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
                unsigned long getSensorExecutionInterval() const {
                    return _sensorInterval;
                }

                /**
                 * Set the callback called when the module sample a sensor.
                 *
                 * By default, the module has already a callback for this task, but the user can change it by its own passed in parameter of this function.
                 *
                 * Calling this function passing no parameter or nullptr will restore the default callback of this class.
                 */
                virtual void setSensorExecutionCallback(customCallback f = nullptr) {
                    _sensorTask.setCallback((f == nullptr) ? &AtHomeModule::_onSampleSensor : f);
                }
# endif /* DISABLE_SENSOR */
# ifndef DISABLE_COMMUNICATION
                /**
                 * Set the interval between each time the module listen for received commands on its streams.
                 */
                void setCommunicationExecutionInterval(unsigned long ms) {
                    if (ms && ms != _communicationInterval) {
                        _communicationInterval = ms;
                        _communicationTask.setInterval(_communicationInterval * TASK_MILLISECOND);
                    }
                }

#  ifndef DISABLE_SENSOR
                /**
                 * Set the interval between each time the module sends its stored sensor readings over streams.
                 */
                void setUploadDataExecutionInterval(unsigned long ms) {
                    if (ms && ms != _uploadDataInterval) {
                        _uploadDataInterval = ms;
                        _uploadDataTask.setInterval(_uploadDataInterval * TASK_MILLISECOND);
                    }
                }

                /**
                 * Get the interval between each time the module sends its stored sensor readings over its streams.
                 */
                unsigned long getUploadDataExecutionInterval() const {
                    return _uploadDataInterval;
                }

                /**
                 * Set the callback called when the module sends its stored sensor readings over its streams.
                 *
                 * By default, the module has already a callback for this task, but the user can change it by its own passed in parameter of this function.
                 *
                 * Calling this function passing no parameter or nullptr will restore the default callback of this class.
                 */
                virtual void setUploadDataExecutionCallback(customCallback f = nullptr) {
                    _uploadDataTask.setCallback((f == nullptr) ? &AtHomeModule::_uploadData : f);
                }
#  endif /* DISABLE_SENSOR */

                /**
                 * Get the interval between each time the module listens for received commands on its streams.
                 */
                unsigned long getCommunicationExecutionInterval() const {
                    return _communicationInterval;
                }

                /**
                 * Set the callback called when the module listens for received inputs.
                 *
                 * By default, the module has already a callback for this task, but the user can change it by its own passed in parameter of this function.
                 *
                 * Calling this function passing no parameter or nullptr will restore the default callback of this class.
                 */
                virtual void setCommunicationExecutionCallback(customCallback f = nullptr) {
                    _communicationTask.setCallback((f == nullptr) ? &AtHomeModule::_onCommunicate : f);
                }

                /**
                 * Set a callback called after default command interpreter executed and wasn't able to execute received input, passing the command string and a reference to the stream to the callback.
                 */
                virtual void setCommandPlugin(AtHomeCommandPlugin plugin) {
                    _communicationPlugin = plugin;
                }
# endif /* DISABLE_COMMUNICATION */
# ifndef DISABLE_PERSISTENT_STORAGE
                /**
                 * Set a callback called after module backup was executed on storage, passing the actual offset usable (after module owns data) and a reference to the storage interface used.
                 */
                virtual void setOnBackupPlugin(AtHomeStoragePlugin plugin) {
                    _onBackupPlugin = plugin;
                }

                /**
                 * Set a callback called after module data loading was executed on storage, passing the actual offset usable (after module owns data) and a reference to the storage interface used.
                 */
                virtual void setOnRestorePlugin(AtHomeStoragePlugin plugin) {
                    _onRestorePlugin = plugin;
                }
# endif /* DISABLE_PERSISTENT_STORAGE */

                /**
                 * Return the unique serial of the module, used to identify it.
                 */
                const moduleSerial getSerial() const { return _serial; }

                /**
                 * Set the unique serial used to identify the module.
                 */
                void setSerial(moduleSerial serial) {
                    _serial = serial;
# ifndef DISABLE_PERSISTENT_STORAGE
                    onBackupOnStorage();
# endif /* DISABLE_PERSISTENT_STORAGE */
                }

            private:
# ifndef DISABLE_SENSOR
                static void _onSampleSensor() {
                    AtHomeModule *instance = getInstance();
                    if (instance != nullptr) {
                        instance->onSampleSensor();
                    }
                }
# endif /* DISABLE_SENSOR */
# ifndef DISABLE_COMMUNICATION
                static void _onCommunicate() {
                    AtHomeModule *instance = getInstance();
                    if (instance != nullptr) {
                        instance->onCommunicate();
                    }
                }
#  ifndef DISABLE_SENSOR
                static void _uploadData() {
                    AtHomeModule *instance = getInstance();
                    if (instance != nullptr) {
                        instance->uploadData();
                    }
                }
#  endif /* DISABLE_SENSOR */
# endif /* DISABLE_COMMUNICATION */
            protected:
# ifndef DISABLE_SENSOR
                struct AtHomeSensorMeasure {
                    T                               sample;
# ifndef DISABLE_TIME
                    t_timestamp                     timestamp;
# endif /* DISABLE_TIME */
                    uint8_t                         estimate;
                    utility::units::Unit            unit;
                    PGM_P                           label;
                };
# endif /* DISABLE_SENSOR */

                AtHomeModule():
                    ABaseModule(),
# ifndef DISABLE_SENSOR
                    _sensorInterval(DEFAULT_SENSOR_INTERVAL * TASK_MILLISECOND),
                    _nbMeasures(0),
# endif /* DISABLE_SENSOR */
# ifndef DISABLE_COMMUNICATION
                    _communicationInterval(DEFAULT_COMMUNICATION_INTERVAL * TASK_MILLISECOND),
                    _uploadDataInterval(DEFAULT_UPLOAD_DATA_INTERVAL * TASK_MILLISECOND),
                    _communicationPlugin(nullptr),
# endif /* DISABLE_COMMUNICATION */
# ifndef DISABLE_PERSISTENT_STORAGE
                    _onBackupPlugin(nullptr),
                    _onRestorePlugin(nullptr),
# endif /* DISABLE_PERSISTENT_STORAGE */
                    _serial(0) {}
# ifndef DISABLE_COMMUNICATION
                /**
                 * Broadcast the data passed as parameter over all module streams.
                 */
                template <typename U>
                void        broadcast(const U &data) {
                    if (_streams == nullptr) {
                        return;
                    }
                    for (size_t i = 0; _streams[i] != nullptr; i++) {
                        _streams[i]->print(data);
                    }
                }

                /**
                 * Broadcast the data passed as parameter over all module streams, followed by line return string "\r\n".
                 */
                template <typename U>
                void        broadcastln(const U &data) {
                    broadcast(data);
                    broadcast(FH(ATHOME_NEW_LINE));
                }

                void        raw_broadcast(const uint8_t *data, size_t len) {
                    if (_streams == nullptr) {
                        return;
                    }
                    for (size_t i = 0; _streams[i] != nullptr; i++) {
                        _streams[i]->write(data, len);
                    }
                }
#  ifndef DISABLE_SENSOR
                /**
                 * Sends stored sensor readings over module streams.
                 */
                void        uploadData() {
                    // Forward version of uploadData
                    broadcastln(FH(communication::commands::uploadData));
                    raw_broadcast(reinterpret_cast<uint8_t *>(&_serial), sizeof(_serial));
#   ifndef DISABLE_SENSOR
                    for (size_t i = 0; i < _nbMeasures; i++) {
                        raw_broadcast(reinterpret_cast<uint8_t *>(&(_measures[i])), sizeof(AtHomeSensorMeasure));
                    }
#   endif /* DISABLE_SENSOR */
                    broadcast(ATHOME_END_OF_COMMAND);
#   ifndef DISABLE_SENSOR
                    _nbMeasures = 0;
#   endif /* DISABLE_SENSOR */
                }
#  endif /* DISABLE_SENSOR */
                /**
                 * Called (or trigger if called) when a module listens for received commands.
                 */
                void        onCommunicate() {
                    if (_streams == nullptr) {
                        return;
                    }
                    for (size_t i = 0; _streams[i] != nullptr; i++) {
                        if (_streams[i]->available()) {
                            char buffer[18];
                            char buffer2;
                            int len = 0;
                            for (len = 0; len < 18; len++) {
                                if (_streams[i]->readBytes(&buffer2, 1) < 0 || buffer2 == ATHOME_NEW_LINE) {
                                    break;
                                }
                                else if (buffer2 == ATHOME_END_OF_COMMAND ||
                                         (len == 17 && buffer2 !=  ATHOME_NEW_LINE)) {
                                    i--;
                                    len = 0;
                                    break; // Means we found a end of command, so what was read until now was from a previous command
                                }
                                else if (buffer2 == ATHOME_END_OF_COMMUNICATION) {
                                    len = 0;
                                    break;
                                }
                                else if (buffer2 >= 'a' && buffer2 <= 'z') {
                                    buffer2 -= 32; // Convert in upper case
                                }
                                buffer[len] = buffer2;
                            }
                            if (len < 1) {
                                continue; // Means the reading is invalid, so we pass to next stream
                            }
                            buffer[len] = '\0';
                            if (!STRCMP(buffer, communication::commands::setProfile)) {
                                _setProfile(*_streams[i]);
                            }
#  ifndef DISABLE_TIME
                            else if (!STRCMP(buffer, communication::commands::setDateTime)) {
                                _setDateTime(*_streams[i]);
                            }
#  endif /* DISABLE_TIME */
#  ifndef DISABLE_SENSOR
                            else if (!STRCMP(buffer, communication::commands::setSensorThresholds)) {
                                _setSensorThresholds(*_streams[i]);
                            }
#  endif /* DISABLE_SENSOR */
                            else if (_communicationPlugin != nullptr) {
                                _communicationPlugin(buffer, *_streams[i]);
                            }
                            else {
                                //_streams[i]->flush(); // Would also remove output buffers
                                while (_streams[i]->read() != -1);
                            }
                            //while (_streams[i]->read() != -1); // Remove the rest of the input
                        }
                    }
                }

                /**
                 * Flush all streams used by the module.
                 */
                void        flushStreams() {
                    if (_streams == nullptr) {
                        return;
                    }
                    for (size_t i = 0; _streams[i] != nullptr; i++) {
                        _streams[i]->flush();
                    }
                }
# endif /* DISABLE_COMMUNICATION */
# ifndef DISABLE_PERSISTENT_STORAGE
                /**
                 * Called (or trigger if called) when module backup its data on a storage.
                 */
                void        onBackupOnStorage() {
                    if (_storage != nullptr) {
                        _storage->write(0, reinterpret_cast<const void *>(&_serial), sizeof(moduleSerial));
                        if (_onBackupPlugin != nullptr) {
                            _onBackupPlugin(sizeof(moduleSerial), *_storage);
                        }
                    }
                }

                /**
                 * Called (or trigger if called) when module restore data from a storage.
                 */
                void        onRestoreFromStorage() {
                    if (_storage != nullptr) {
                        _storage->read(0, reinterpret_cast<void *>(&_serial), sizeof(moduleSerial));
                        if (!_serial) {
                            return; // The module is uninitialized
                        }
                        if (_onRestorePlugin != nullptr) {
                            _onRestorePlugin(sizeof(moduleSerial), *_storage);
                        }
                    }
                }
# endif /* DISABLE_PERSISTENT_STORAGE */
# ifndef DISABLE_SENSOR
                /**
                 * Called (or trigger if called) when a module samples its sensor.
                 */
                void        onSampleSensor() {
                    if (_nbMeasures >= n) {
                        _nbMeasures = 0;
                    }
                    if (_sensor != nullptr && _nbMeasures < n) {
                        const sensor::ISensor::ISensorValue &value = _sensor->getSample();
                        _measures[_nbMeasures].unit = value.unit;
                        _measures[_nbMeasures].estimate = value.estimate;
                        // TODO: need a time interface
# ifndef DISABLE_TIME
                        if (_clock != nullptr) {
                            _measures[_nbMeasures].timestamp = _clock->getDateTime();
                        }
# endif /* DISABLE_TIME */
                        _measures[_nbMeasures].label = value.label;
                        if (value.sampleRawPointer != nullptr) {
                            memcpy(&(_measures[_nbMeasures].sample), value.sampleRawPointer, sizeof(T));
                        }
                        else {
                            memset(&(_measures[_nbMeasures].sample), 0, sizeof(T));
                        }
#  ifndef DISABLE_DISPLAY
                        onUpdateDisplay();
#  endif /* DISABLE_DISPLAY */
                        _nbMeasures++;
                    }
                }
#  ifndef DISABLE_DISPLAY
                /**
                 * Called (or trigger if called) when a module update its display.
                 */
                void        onUpdateDisplay() {
                    if (_display == nullptr || _sensor == nullptr) {
                        return;
                    }
                    _display->setDisplayedEstimate(
                            static_cast<sensor::ISensor::ISensorScale>(_measures[_nbMeasures].estimate));
                    _display->update();
                }
#  endif /* DISABLE_DISPLAY */
# endif /* DISABLE_SENSOR */
            private:
# ifndef DISABLE_COMMUNICATION
                void        _setProfile(Stream &stream) {
                    moduleSerial serial;
                    if (stream.readBytesUntil(ATHOME_END_OF_COMMAND,
                                              reinterpret_cast<char *>(&serial), sizeof(moduleSerial)) < 1) {
                        return;
                    }
                    setSerial(serial);
                    stream.readBytesUntil(ATHOME_END_OF_COMMAND, reinterpret_cast<char *>(&serial),
                                          1);
                }
#  ifndef DISABLE_TIME
                void        _setDateTime(Stream &stream) {
                    if (_clock == nullptr) {
                        while (stream.read() != ATHOME_END_OF_COMMAND);
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
#  endif /* DISABLE_TIME */
#  ifndef DISABLE_SENSOR
                void        _setSensorThresholds(Stream &stream) {
                    if (_sensor == nullptr) {
                        while (stream.read() != ATHOME_END_OF_COMMAND);
                        return;
                    }
                    sensor::ISensor::ISensorThresholds thresholds;
                    {
                        char buffer[2];
                        if (stream.readBytes(buffer, 2) < 1 ||
                            stream.readBytes(reinterpret_cast<char *>(&thresholds.min), sizeof(thresholds.min)) < 1 ||
                            stream.readBytes(reinterpret_cast<char *>(&thresholds.max), sizeof(thresholds.max)) < 1) {
                            return;
                        }
                        thresholds.unit.unit = buffer[0];
                        thresholds.unit.prefix = buffer[1];
                    }
                    _sensor->setThresholds(thresholds);
                    uint8_t tmp;
                    stream.readBytes(reinterpret_cast<char *>(&tmp), 1);
                }
#  endif /* DISABLE_SENSOR */
# endif /* DISABLE_COMMUNICATION */
            private:
# ifndef DISABLE_SENSOR
                unsigned long               _sensorInterval;
                size_t                      _nbMeasures;
# endif /* DISABLE_SENSOR */
# ifndef DISABLE_COMMUNICATION
                unsigned long               _communicationInterval;
                unsigned long               _uploadDataInterval;
                AtHomeCommandPlugin        _communicationPlugin;
# endif /* DISABLE_COMMUNICATION */
# ifndef DISABLE_PERSISTENT_STORAGE
                AtHomeStoragePlugin        _onBackupPlugin;
                AtHomeStoragePlugin        _onRestorePlugin;
# endif /* DISABLE_PERSISTENT_STORAGE */
                moduleSerial                _serial;
                Scheduler                   _scheduler;
# ifndef DISABLE_SENSOR
                AtHomeSensorMeasure         _measures[n];
# endif /* DISABLE_SENSOR */
# if !defined(DISABLE_SENSOR) && !defined(DISABLE_COMMUNICATION)
                Task                        _uploadDataTask;
# endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_COMMUNICATION) */
# ifndef DISABLE_SENSOR
                Task                        _sensorTask;
# endif /* DISABLE_SENSOR */
# ifndef DISABLE_COMMUNICATION
                Task                        _communicationTask;
# endif /* DISABLE_COMMUNICATION */
                static void                 *_instance;
        };

        template <typename T, size_t n>
        void *AtHomeModule<T, n>::_instance = nullptr;
    }
}
#endif /* ATHOMEMODULE_HPP */
