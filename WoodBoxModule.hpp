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
                typedef void (*WoodBoxCommandPlugin)(const String &, Stream *);

                template <typename U = WoodBoxModule>
                static U *getInstance() {
                    if (_instance == nullptr) {
                        U *ptr = new U();
                        _instance = reinterpret_cast<void *>(ptr);
                        return ptr;
                    }
                    return reinterpret_cast<U *>(_instance);
                }

                void setScheduler(Scheduler &scheduler) {
                    if (_scheduler != nullptr) {
                        _scheduler->deleteTask(_sensorTask);
                        //_scheduler->deleteTask(_displayTask);
                        _scheduler->deleteTask(_communicationTask);
                    }
                    _scheduler = &scheduler;
                    _scheduler->addTask(_sensorTask);
                    //_scheduler->addTask(_displayTask);
                    _scheduler->addTask(_communicationTask);
                }

                Scheduler *getScheduler() {
                    return _scheduler;
                }

                void run() {
                    _scheduler->execute();
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

                unsigned long getSensorExecutionInterval() const {
                    return _sensorInterval;
                }

                /* unsigned long getDisplayExecutionInterval() const {
                    return _displayInterval;
                } */

                unsigned long getCommunicationExecutionInterval() const {
                    return _communicationInterval;
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

                void setCommandPlugin(WoodBoxCommandPlugin plugin) {
                    _communicationPlugin = plugin;
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
                    WoodBoxModule<T, n> *instance = getInstance();
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
                    WoodBoxModule<T, n> *instance = getInstance();
                    if (instance != nullptr) {
                        instance->onCommunicate();
                    }
                }
                static void _uploadData() {
                    WoodBoxModule<T, n> *instance = getInstance();
                    if (instance != nullptr) {
                        instance->uploadData();
                    }
                }

                void setup() {
                    onRestoreFromStorage();
                    onStart();
                    _sensorTask.enableIfNot();
                    //_displayTask.enableIfNot();
                    _communicationTask.enableIfNot();
                    _uploadDataTask.enableIfNot();
                }

            protected:

                WoodBoxModule():
                    ABaseModule(),
                    _scheduler(nullptr),
                    _sensorInterval(TASK_SECOND),
                    //_displayInterval(TASK_SECOND),
                    _communicationInterval(TASK_MILLISECOND),
                    _type(UNKNOWN),
                    _nbMeasures(0),
                    _communicationPlugin(nullptr) {
                    memset(_vendor, 0, sizeof(moduleVendor));
                    memset(_serial, 0, sizeof(moduleSerial));
                    memset(_measures, 0, sizeof(T) * n);
                    memset(_timestamps, 0, sizeof(T) * n);
                    _uploadDataTask.set(TASK_SECOND * 30, TASK_FOREVER, &WoodBoxModule::_uploadData);
                    _sensorTask.set(_sensorInterval, TASK_FOREVER, &WoodBoxModule::_onSampleSensor);
                    //_displayTask.set(_displayInterval, TASK_FOREVER, &WoodBoxModule::_onUpdateDisplay);
                    _communicationTask.set(_communicationInterval, TASK_FOREVER, &WoodBoxModule::_onCommunicate);
                    setup();
                }
                void        uploadData() {
                    //Todo: Need to implement it
                }
                void        onReset() {}
                void        onStart() {}
                void        onStop() {}
                void        onPause() {}
                void        onResume() {}
                void        onBackupOnStorage() {
                    if (_storage != nullptr) {
                        _storage->write(0, reinterpret_cast<const void *>(_vendor), sizeof(moduleVendor));
                        _storage->write(sizeof(moduleVendor), reinterpret_cast<const void *>(_serial), sizeof(moduleSerial));
                        _storage->write(sizeof(moduleVendor) + sizeof(moduleSerial), reinterpret_cast<const void *>(&_type), sizeof(moduleType));
                    }
                }
                void        onRestoreFromStorage() {
                    if (_storage != nullptr) {
                        _storage->read(0, reinterpret_cast<void *>(_vendor), sizeof(moduleVendor));
                        _storage->read(sizeof(moduleVendor), reinterpret_cast<void *>(_serial), sizeof(moduleSerial));
                        _storage->read(sizeof(moduleVendor) + sizeof(moduleSerial), reinterpret_cast<void *>(&_type), sizeof(moduleType));
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
                    for (Stream *stream = *_streams; stream != nullptr; stream++) {
                        if (stream->available()) {
                            String commandName = stream->readStringUntil('\n');
                            commandName.replace('\r', '\0');
                            if (STRCMP(commandName.c_str(), communication::commands::setProfile)) {
                                // TODO: need to implement set profile command -> set conf of module itself (vendor, serial, ...etc)
                                stream->println(F("Not implemented yet"));
                            }
                            else if (STRCMP(commandName.c_str(), communication::commands::enumerate)) {
                                // TODO: need to implement enumerate command
                                stream->println(F("Not implemented yet"));
                            }
                            else if (STRCMP(commandName.c_str(), communication::commands::syncTime)) {
                                // TODO: need to implement syncTime command
                                stream->println(F("Not implemented yet"));
                            }
                            else if (_communicationPlugin != nullptr) {
                                _communicationPlugin(commandName, stream);
                            }
                        }
                    }
                }
            private:
                Scheduler                   *_scheduler;
                unsigned long               _sensorInterval;
                //unsigned long               _displayInterval;
                unsigned long               _communicationInterval;
                moduleType                  _type;
                size_t                      _nbMeasures;
                WoodBoxCommandPlugin        _communicationPlugin;
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
