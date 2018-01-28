#ifndef WOODBOXMODULE_HPP
# define WOODBOXMODULE_HPP

# include <stdint.h>
# include "ARGBLed.hpp"
# include "ABaseModule.hpp"
# include "AWiFiCommunicator.hpp"
# include "woodbox_communication_protocol.hpp"

namespace woodBox {
    namespace module {
        /**
         * Template class implementing base WoodBox Modules, taking the type representing a reading from the intended sensor to use and the number of readings to save in a buffer.
         */
        template <typename T, size_t n>
        class WoodBoxModule : public ABaseModule {
            public:
                /**
                 * Enumeration representing the type of a module
                 */
                enum moduleType {
                    UNKNOWN,
                    AIR_QUALITY, // Dust, smoke, ...etc
                    TEMPERATURE,
                    HUMIDITY,
                    LUMINOSITY,
                    NOISE,
                    CARBON, // Monoxyde, Dioxyde, Methane, Butane, Propane, ...etc
                    CARBON_MONOXYDE,
                    ELECTROMAGNETIC,
                    RADIOACTIVITY // Geiger counter
                };

                /**
                 * `moduleVendor` type represents a type holding the vendor name of a module ("WoodBox" when they are built by WoodBox team)
                 */
                typedef char            moduleVendor[33];
                /**
                 * `moduleSerial` type represents a unique value used to identify a module from other
                 */
                typedef char            moduleSerial[33];
                /**
                 * `timestamp` type is used to represent sensor readings date
                 */
                typedef unsigned long   timestamp;

                /**
                 * WoodBoxModule and derived classes are singletons, duplication is not allowed
                 */
                WoodBoxModule(const WoodBoxModule &) = delete;
                /**
                 * WoodBoxModule and derived classes are singletons, as only one instance can exist, copy is not possible
                 */
                WoodBoxModule &operator=(const WoodBoxModule &) = delete;
                ~WoodBoxModule() {}

            public:
                /**
                 * customCallback type is used to pass a callback to call during various events.
                 *
                 * Functions used as callback must have a compatible prototype to this:
                 * \fn void name()
                 */
                typedef void (*customCallback)();
                /**
                 * WoodBoxCommandCommandPlugin callback is used to extend command interpreter and called when an unknown command is the received.
                 *
                 * Functions used as callback receive a reference on a String holding the actual command name and a reference on the Stream from which it has been received.
                 *
                 * These functions must have a prototype compatible to this one:
                 * \fn void name(const String &, Stream &)
                 */
                typedef void (*WoodBoxCommandPlugin)(const String &, Stream &);
                /**
                 * WoodBoxStoragePlugin callback is used to save or restore additional data from actual storage.
                 *
                 * Functions used as callback will receive the current offset usable after the module read or saved data in the storage, and a reference to this storage interface.
                 *
                 * These functions must have a prototype compatible to this one:
                 * \fn void name(size_t, woodBox::storage::IStorage &)
                 */
                typedef void (*WoodBoxStoragePlugin)(size_t, storage::IStorage &);

                /**
                 * Template function used to get the address of the current WoodBoxModule or derived class or instanciates it if there's still no instances existing.
                 *
                 * The template parameter takes the class type (and of course it's own template parameters if it's a templated class).
                 *
                 * If no template parameter is given, it will create by default a WoodBoxModule instance.
                 */
                template <typename U = WoodBoxModule>
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
                    onRestoreFromStorage();
                    _sensorTask.set(_sensorInterval, TASK_FOREVER, &WoodBoxModule::_onSampleSensor);
                    _communicationTask.set(_communicationInterval, TASK_FOREVER, &WoodBoxModule::_onCommunicate);
                    _uploadDataTask.set(_uploadDataInterval, TASK_FOREVER, &WoodBoxModule::_uploadData);
                    _scheduler.addTask(_sensorTask);
                    _scheduler.addTask(_communicationTask);
                    _scheduler.addTask(_uploadDataTask);
                    _sensorTask.enableIfNot();
                    _communicationTask.enableIfNot();
                    _uploadDataTask.enableIfNot();
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
                    onBackupOnStorage();
                    // Todo: need to implement it
                }

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
                 * Set the interval between each time the module listen for received commands on its streams.
                 */
                void setCommunicationExecutionInterval(unsigned long ms) {
                    if (ms && ms != _communicationInterval) {
                        _communicationInterval = ms;
                        _communicationTask.setInterval(_communicationInterval * TASK_MILLISECOND);
                    }
                }

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
                 * Get the interval between each time the module samples its sensor.
                 */
                unsigned long getSensorExecutionInterval() const {
                    return _sensorInterval;
                }

                /**
                 * Get the interval between each time the module listens for received commands on its streams.
                 */
                unsigned long getCommunicationExecutionInterval() const {
                    return _communicationInterval;
                }

                /**
                 * Get the interval between each time the module sends its stored sensor readings over its streams.
                 */
                unsigned long getUploadDataExecutionInterval() const {
                    return _uploadDataInterval;
                }

                /**
                 * Set the callback called when the module sample a sensor.
                 *
                 * By default, the module has already a callback for this task, but the user can change it by its own passed in parameter of this function.
                 *
                 * Calling this function passing no parameter or nullptr will restore the default callback of this class.
                 */
                void setSensorExecutionCallback(customCallback f = nullptr) {
                    _sensorTask.setCallback((f == nullptr) ? &WoodBoxModule::_onSampleSensor : f);
                }

                /**
                 * Set the callback called when the module listens for received inputs.
                 *
                 * By default, the module has already a callback for this task, but the user can change it by its own passed in parameter of this function.
                 *
                 * Calling this function passing no parameter or nullptr will restore the default callback of this class.
                 */
                void setCommunicationExecutionCallback(customCallback f = nullptr) {
                    _communicationTask.setCallback((f == nullptr) ? &WoodBoxModule::_onCommunicate : f);
                }

                /**
                 * Set the callback called when the module sends its stored sensor readings over its streams.
                 *
                 * By default, the module has already a callback for this task, but the user can change it by its own passed in parameter of this function.
                 *
                 * Calling this function passing no parameter or nullptr will restore the default callback of this class.
                 */
                void setUploadDataExecutionCallback(customCallback f = nullptr) {
                    _uploadDataTask.setCallback((f == nullptr) ? &WoodBoxModule::_uploadData() : f);
                }

                /**
                 * Set a callback called after default command interpreter executed and wasn't able to execute received input, passing the command string and a reference to the stream to the callback.
                 */
                void setCommandPlugin(WoodBoxCommandPlugin plugin) {
                    _communicationPlugin = plugin;
                }

                /**
                 * Set a callback called after module backup was executed on storage, passing the actual offset usable (after module owns data) and a reference to the storage interface used.
                 */
                void setOnBackupPlugin(WoodBoxStoragePlugin plugin) {
                    _onBackupPlugin = plugin;
                }

                /**
                 * Set a callback called after module data loading was executed on storage, passing the actual offset usable (after module owns data) and a reference to the storage interface used.
                 */
                void setOnRestorePlugin(WoodBoxStoragePlugin plugin) {
                    _onRestorePlugin = plugin;
                }

                /**
                 * Return the type of the module, using woodBox::module::WoodBoxModule::moduleType enumeration.
                 */
                moduleType          getType() const { return _type; }

                /**
                 * Return a reference to the vendor ID of the module ("WoodBox" for WoodBox modules).
                 */
                const moduleVendor &getVendor() const { return _vendor; }

                /**
                 * Return the unique serial of the module, used to identify it.
                 */
                const moduleSerial &getSerial() const { return _serial; }

                /**
                 * Set the type of the module, passing a woodBox::module::WoodBoxModule::moduleType enumeration value as parameter.
                 */
                void                setType(moduleType type) {
                    if (type != moduleType::UNKNOWN) {
                        _type = type;
                    }
                }

                /**
                 * Set the vendor ID of the module.
                 */
                void                setVendor(const moduleVendor &vendor) {
                    if (vendor != nullptr) {
                        memcpy(_vendor, &vendor, sizeof(moduleVendor));
                    }
                }

                /**
                 * Set the unique serial used to identify the module.
                 */
                void                setSerial(const moduleSerial &serial) {
                    if (serial != nullptr) {
                        memcpy(_serial, &serial, sizeof(moduleSerial));
                    }
                }

            private:
                static void _onSampleSensor() {
                    WoodBoxModule *instance = getInstance();
                    if (instance != nullptr) {
                        instance->onSampleSensor();
                    }
                }

                static void _onCommunicate() {
                    WoodBoxModule *instance = getInstance();
                    if (instance != nullptr) {
                        instance->onCommunicate();
                    }
                }
                static void _uploadData() {
                    WoodBoxModule *instance = getInstance();
                    if (instance != nullptr) {
                        instance->uploadData();
                    }
                }

            protected:
                WoodBoxModule():
                    ABaseModule(),
                    _sensorInterval(TASK_SECOND),
                    _communicationInterval(TASK_MILLISECOND * 10),
                    _uploadDataInterval(TASK_SECOND * 15),
                    _type(UNKNOWN),
                    _nbMeasures(0),
                    _refTimestamp(0),
                    _communicationPlugin(nullptr),
                    _onBackupPlugin(nullptr),
                    _onRestorePlugin(nullptr) {
                    memset(_vendor, 0, sizeof(moduleVendor));
                    memset(_serial, 0, sizeof(moduleSerial));
                    memset(_measures, 0, sizeof(T) * n);
                    memset(_timestamps, 0, sizeof(T) * n);
                }

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
                    broadcast(communication::commands::end_of_line);
                }

                /**
                 * Sends stored sensor readings over module streams.
                 */
                void        uploadData() {
                    // Forward version of uploadData
                    // Todo: implement a more resource efficient and generic version
                    broadcastln(reinterpret_cast<const __FlashStringHelper *>(communication::commands::uploadData));
                    broadcastln(reinterpret_cast<const __FlashStringHelper *>(communication::commands::part_separator));
                    broadcast(F("{\"Serial\":\""));
                    broadcast(_serial);
                    broadcast(F("\",\"Data\":["));
                    for (size_t i = 0; i <= _nbMeasures; i++) {
                        broadcast(F("{\"Measure\":\""));
                        broadcast(_measures[i]);
                        broadcast(F("\",\"Timestamp\":\""));
                        broadcast(_timestamps[i]);
                        broadcast(F("\"}"));
                        if (i < (n - 1)) {
                            broadcast(F(","));
                        }
                    }
                    broadcastln(F("]}"));
                    broadcast(reinterpret_cast<const __FlashStringHelper *>(communication::commands::end_of_command));
                    _nbMeasures = 0;
                }

                /**
                 * Called (or trigger if called) when module backup its data on a storage.
                 */
                void        onBackupOnStorage() {
                    if (_storage != nullptr) {
                        _storage->write(0, reinterpret_cast<const void *>(_vendor), sizeof(moduleVendor));
                        _storage->write(sizeof(moduleVendor), reinterpret_cast<const void *>(_serial), sizeof(moduleSerial));
                        _storage->write(sizeof(moduleVendor) + sizeof(moduleSerial), reinterpret_cast<const void *>(&_type), sizeof(moduleType));
                        if (_onBackupPlugin != nullptr) {
                            _onBackupPlugin(sizeof(moduleVendor) + sizeof(moduleSerial) + sizeof(moduleType), *_storage);
                        }
                    }
                }

                /**
                 * Called (or trigger if called) when module restore data from a storage.
                 */
                void        onRestoreFromStorage() {
                    if (_storage != nullptr) {
                        _storage->read(0, reinterpret_cast<void *>(_vendor), sizeof(moduleVendor));
                        _storage->read(sizeof(moduleVendor), reinterpret_cast<void *>(_serial), sizeof(moduleSerial));
                        _storage->read(sizeof(moduleVendor) + sizeof(moduleSerial), reinterpret_cast<void *>(&_type), sizeof(moduleType));
                        if (_onRestorePlugin != nullptr) {
                            _onRestorePlugin(sizeof(moduleVendor) + sizeof(moduleSerial) + sizeof(moduleType), *_storage);
                        }
                    }
                }

                /**
                 * Called (or trigger if called) when a module samples its sensor.
                 */
                void        onSampleSensor() {
                    if (_sensor != nullptr && _nbMeasures < n) {
                        uint8_t* rawData = _sensor->getSample();
                        if (rawData != nullptr) {
                            memcpy(&(_measures[_nbMeasures]), rawData, sizeof(T));
                        }
                        else {
                            memset(&(_measures[_nbMeasures]), 0, sizeof(T));
                        }
                        _timestamps[_nbMeasures] = _refTimestamp + millis();
                        _nbMeasures++;
                        onUpdateDisplay();
                    }
                }

                /**
                 * Called (or trigger if called) when a module update its display.
                 */
                void        onUpdateDisplay() {
                    if (_display == nullptr || _sensor == nullptr) {
                        return;
                    }
                    display::ARGBLed *display = (display::ARGBLed *)_display;
                    display::ARGBLed::Color color;
                    if (_sensor->getEstimate() >= sensor::ISensor::ISensorScale::FIVE) {
                        color = {0, 5, 0};
                    }
                    else {
                        color = {255, 0, 0};
                    }
                    display->setColor(color);
                    display->update();
                }

                /**
                 * Called (or trigger if called) when a module listens for received commands.
                 */
                void        onCommunicate() {
                    if (_streams == nullptr) {
                        return;
                    }
                    for (size_t i = 0; _streams[i] != nullptr; i++) {
                        if (_streams[i]->available()) {
                            String commandName = _streams[i]->readStringUntil('\n');
                            commandName.replace('\r', '\0');
                            if (!STRCMP(commandName.c_str(), communication::commands::setProfile)) {
                                // TODO: need to implement set profile command -> set conf of module itself (vendor, serial, ...etc)
                                _streams[i]->println(F("Not implemented yet"));
                            }
                            else if (!STRCMP(commandName.c_str(), communication::commands::enumerate)) {
                                // TODO: need to implement enumerate command
                                _streams[i]->println(F("Not implemented yet"));
                            }
                            else if (!STRCMP(commandName.c_str(), communication::commands::syncTime)) {
                                // TODO: need to implement syncTime command
                                _streams[i]->println(F("Not implemented yet"));
                            }
                            else if (_communicationPlugin != nullptr) {
                                _communicationPlugin(commandName, *_streams[i]);
                            }
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
            private:
                unsigned long               _sensorInterval;
                unsigned long               _communicationInterval;
                unsigned long               _uploadDataInterval;
                moduleType                  _type;
                size_t                      _nbMeasures;
                timestamp                   _refTimestamp;
                WoodBoxCommandPlugin        _communicationPlugin;
                WoodBoxStoragePlugin        _onBackupPlugin;
                WoodBoxStoragePlugin        _onRestorePlugin;
                Scheduler                   _scheduler;
                moduleVendor                _vendor;
                moduleSerial                _serial;
                T                           _measures[n];
                timestamp                   _timestamps[n];
                Task                        _uploadDataTask;
                Task                        _sensorTask;
                Task                        _communicationTask;
                static void                 *_instance;
        };

        template <typename T, size_t n>
        void *WoodBoxModule<T, n>::_instance = nullptr;
    }
}
#endif /* WOODBOXMODULE_HPP */
