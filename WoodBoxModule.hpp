#ifndef WOODBOXMODULE_HPP
# define WOODBOXMODULE_HPP

# include <stdint.h>
# include "ARGBLed.hpp"
# include "ABaseModule.hpp"
# include "AWiFiCommunicator.hpp"
# include "woodbox_communication_protocol.hpp"

namespace woodBox {
    namespace module {
        template <typename T, size_t n>
        class WoodBoxModule : public ABaseModule {
            public:
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

                typedef char            moduleVendor[33];
                typedef char            moduleSerial[33];
                typedef unsigned long   timestamp;

                WoodBoxModule(const WoodBoxModule &) = delete;
                WoodBoxModule &operator=(const WoodBoxModule &) = delete;
                ~WoodBoxModule() {}

            public:
                typedef void (*customCallback)();
                typedef void (*WoodBoxCommandPlugin)(const String &, Stream &);
                typedef void (*WoodBoxStoragePlugin)(size_t, storage::IStorage &);

                template <typename U = WoodBoxModule>
                static U *getInstance() {
                    if (_instance == nullptr) {
                        U *ptr = new U();
                        _instance = reinterpret_cast<void *>(ptr);
                        return ptr;
                    }
                    return reinterpret_cast<U *>(_instance);
                }

                /*void setScheduler(Scheduler &scheduler) {
                    if (_scheduler != nullptr) {
                        _scheduler.deleteTask(_sensorTask);
                        //_scheduler.deleteTask(_displayTask);
                        _scheduler.deleteTask(_communicationTask);
                        _scheduler.deleteTask(_uploadDataTask);
                    }
                    _scheduler = &scheduler;
                    _scheduler.addTask(_sensorTask);
                    //_scheduler.addTask(_displayTask);
                    _scheduler.addTask(_communicationTask);
                    _scheduler.addTask(_uploadDataTask);
                    _sensorTask.enableIfNot();
                    _communicationTask.enableIfNot();
                    _uploadDataTask.enableIfNot();
                }*/

                Scheduler &getScheduler() {
                    return _scheduler;
                }

                void setup() {
                    onRestoreFromStorage();
                    onStart();
                    _sensorTask.set(_sensorInterval, TASK_FOREVER, &WoodBoxModule::_onSampleSensor);
                    //_displayTask.set(_displayInterval, TASK_FOREVER, &WoodBoxModule::_onUpdateDisplay);
                    _communicationTask.set(_communicationInterval, TASK_FOREVER, &WoodBoxModule::_onCommunicate);
                    _uploadDataTask.set(_uploadDataInterval, TASK_FOREVER, &WoodBoxModule::_uploadData);
                    _scheduler.addTask(_sensorTask);
                    _scheduler.addTask(_communicationTask);
                    _scheduler.addTask(_uploadDataTask);
                    _sensorTask.enableIfNot();
                    _communicationTask.enableIfNot();
                    _uploadDataTask.enableIfNot();
                }

                void run() {
                    _scheduler.execute();
                }

                void stop() {
                    onBackupOnStorage();
                    onStop();
                }

                void setSensorExecutionInterval(unsigned long ms) {
                    if (ms && ms != _sensorInterval) {
                        _sensorInterval = ms;
                        _sensorTask.setInterval(_sensorInterval * TASK_MILLISECOND);
                    }
                }

                /* void setDisplayExecutionInterval(unsigned long ms) {
                    if (ms && ms != _displayInterval) {
                        _displayInterval = ms;
                        _displayTask.setInterval(_displayInterval * TASK_MILLISECOND);
                    }
                } */

                void setCommunicationExecutionInterval(unsigned long ms) {
                    if (ms && ms != _communicationInterval) {
                        _communicationInterval = ms;
                        _communicationTask.setInterval(_communicationInterval * TASK_MILLISECOND);
                    }
                }

                void setUploadDataExecutionInterval(unsigned long ms) {
                    if (ms && ms != _uploadDataInterval) {
                        _uploadDataInterval = ms;
                        _uploadDataTask.setInterval(_uploadDataInterval * TASK_MILLISECOND);
                    }
                }

                unsigned long getSensorExecutionInterval() const {
                    return _sensorInterval;
                }

                /* unsigned long getDisplayExecutionInterval() const {
                    return _displayInterval;
                } */

                unsigned long getCommunicationExecutionInterval() const {
                    return _communicationInterval;
                }

                unsigned long getUploadDataExecutionInterval() const {
                    return _uploadDataInterval;
                }

                void setSensorExecutionCallback(customCallback f) {
                    _sensorTask.setCallback((f == nullptr) ? &WoodBoxModule::_onSampleSensor : f);
                }

                /* void setDisplayExecutionCallback(customCallback f) {
                    _displayTask.setCallback((f == nullptr) ? &WoodBoxModule::_onUpdateDisplay : f);
                } */

                void setCommunicationExecutionCallback(customCallback f) {
                    _communicationTask.setCallback((f == nullptr) ? &WoodBoxModule::_onCommunicate : f);
                }

                void setUploadDataExecutionCallback(customCallback f) {
                    _uploadDataTask.setCallback((f == nullptr) ? &WoodBoxModule::_uploadData() : f);
                }

                void setCommandPlugin(WoodBoxCommandPlugin plugin) {
                    _communicationPlugin = plugin;
                }

                void setOnBackupPlugin(WoodBoxStoragePlugin plugin) {
                    _onBackupPlugin = plugin;
                }

                void setOnRestorePlugin(WoodBoxStoragePlugin plugin) {
                    _onRestorePlugin = plugin;
                }

                moduleType          getType() const { return _type; }

                const moduleVendor &getVendor() const { return _vendor; }

                const moduleSerial &getSerial() const { return _serial; }

                void                setType(moduleType type) {
                    if (type != moduleType::UNKNOWN) {
                        _type = type;
                    }
                }

                void                setVendor(const moduleVendor &vendor) {
                    if (vendor != nullptr) {
                        memcpy(_vendor, &vendor, sizeof(moduleVendor));
                    }
                }

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
                /* static void _onUpdateDisplay() {
                    if (_instance != nullptr) {
                        _instance->onUpdateDisplay();
                    }
                } */
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
                    //_displayInterval(TASK_SECOND),
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

                template <typename U>
                void        broadcast(const U &data) {
                    if (_streams == nullptr) {
                        return;
                    }
                    for (size_t i = 0; _streams[i] != nullptr; i++) {
                        _streams[i]->print(data);
                    }
                }

                template <typename U>
                void        broadcastln(const U &data) {
                    broadcast(data);
                    broadcast(communication::commands::end_of_line);
                }

            public:
                void        uploadData() {
                    // Forward version of uploadData
                    // Todo: implement a more resource efficient and generic version
                    broadcastln(communication::commands::uploadData);
                    broadcastln(communication::commands::part_separator);
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
                    broadcast(communication::commands::end_of_command);
                    _nbMeasures = 0;
                }
            protected:

                void        onReset() {
                    // Todo: need to implement
                }

                void        onStart() {
                    // Todo: need to implement
                }

                void        onStop() {
                    // Todo: need to implement
                }

                void        onPause() {
                    // Todo: need to implement
                }

                void        onResume() {
                    // Todo: need to implement
                }

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
                //unsigned long               _displayInterval;
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
                //Task                        _displayTask;
                Task                        _communicationTask;
                static void                 *_instance;
        };

        template <typename T, size_t n>
        void *WoodBoxModule<T, n>::_instance = nullptr;
    }
}
#endif /* WOODBOXMODULE_HPP */
