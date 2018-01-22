#ifndef WOODBOXMODULE_HPP
# define WOODBOXMODULE_HPP

# include <stdint.h>
# include "ARGBLed.hpp"
# include "ABaseModule.hpp"
# include "AWiFiCommunicator.hpp"

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

#ifdef __AVR__
                typedef PROGMEM char            moduleVendor[33];
                typedef PROGMEM char            moduleSerial[33];
#else
                typedef char            moduleVendor[33];
                typedef char            moduleSerial[33];
#endif
                typedef unsigned long   timestamp;

                WoodBoxModule(const WoodBoxModule &) = delete;
                WoodBoxModule &operator=(const WoodBoxModule &) = delete;
                ~WoodBoxModule() {}

            public:
                typedef void (*customCallback)();

                static WoodBoxModule *getInstance() {
                    if (_instance == nullptr) {
                        _instance = new WoodBoxModule<T, n>();
                        return _instance;
                    }
                    return _instance;
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

                moduleType          getType() const { return _type; }

                const moduleVendor* getVendor() const { return &_vendor; }

                const moduleSerial* getSerial() const { return &_serial; }

                void                setType(moduleType type) {
                    if (type != moduleType::UNKNOWN) {
                        _type = type;
                    }
                }

                void                setVendor(moduleVendor vendor) {
                    if (vendor != nullptr) {
                        _vendor = vendor;
                    }
                }

                void                setSerial(moduleSerial serial) {
                    if (serial != nullptr) {
                        _serial = serial;
                    }
                }

            private:
                static void _onSampleSensor() {
                    if (_instance != nullptr) {
                        _instance->onSampleSensor();
                    }
                }
                /* static void _onUpdateDisplay() {
                    if (_instance != nullptr) {
                        _instance->onUpdateDisplay();
                    }
                } */
                static void _onCommunicate() {
                    if (_instance != nullptr) {
                        _instance->onCommunicate();
                    }
                }
                static void _uploadData() {
                    if (_instance != nullptr) {
                        _instance->uploadData();
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
                    _nbMeasures(0) {
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
                void        uploadData() {}
                void        onReset() {}
                void        onStart() {}
                void        onStop() {}
                void        onPause() {}
                void        onResume() {}
                void        onBackupOnStorage() {}
                void        onRestoreFromStorage() {}
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
                void        onCommunicate() {}
            private:
                Scheduler                   *_scheduler;
                unsigned long               _sensorInterval;
                //unsigned long               _displayInterval;
                unsigned long               _communicationInterval;
                moduleType                  _type;
                size_t                      _nbMeasures;
                moduleVendor                _vendor;
                moduleSerial                _serial;
                T                           _measures[n];
                timestamp                   _timestamps[n];
                Task                        _uploadDataTask;
                Task                        _sensorTask;
                //Task                        _displayTask;
                Task                        _communicationTask;
                static WoodBoxModule<T, n> *_instance;
        };

        template <typename T, size_t n>
        WoodBoxModule<T, n> *WoodBoxModule<T, n>::_instance = nullptr;
    }
}
#endif /* WOODBOXMODULE_HPP */
