#ifndef AWOODBOXMODULE_HPP
# define AWOODBOXMODULE_HPP

# include <stdint.h>
# include "ABaseModule.hpp"
# include "AWiFiCommunicator.hpp"

namespace woodBox {
    namespace module {
        template <typename T, size_t n>
        class AWoodBoxModule : public ABaseModule {
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
                typedef PROGMEM unsigned long   timestamp;
#else
                typedef char            moduleVendor[33];
                typedef char            moduleSerial[33];
                typedef unsigned long   timestamp;
#endif

                AWoodBoxModule(const AWoodBoxModule &) = delete;
                AWoodBoxModule &operator=(const AWoodBoxModule &) = delete;
                virtual ~AWoodBoxModule() {}

            protected:
                AWoodBoxModule():
                    ABaseModule(),
                    _type(UNKNOWN),
                    _nbMeasures(0) {
                    memset(_vendor, 0, sizeof(moduleVendor));
                    memset(_serial, 0, sizeof(moduleSerial));
                    memset(_measures, 0, sizeof(T) * n);
                    memset(_timestamps, 0, sizeof(T) * n);
                    _uploadDataTask.set(TASK_SECOND * 30, TASK_FOREVER, AWoodBoxModule::_uploadData);

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
                virtual void        uploadData() {}
                virtual void        onReset() {}
                virtual void        onStart() {}
                virtual void        onStop() {}
                virtual void        onPause() {}
                virtual void        onResume() {}
                virtual void        onBackupOnStorage() {}
                virtual void        onRestoreFromStorage() {}
                virtual void        onSampleSensor() {
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
                virtual void        onUpdateDisplay() = 0;
                virtual void        onCommunicate() {}
            private:
                static void         _uploadData() {
                    AWoodBoxModule* instance = (AWoodBoxModule*)ABaseModule::getABaseModuleInstance();
                    if (instance != nullptr) {
                        instance->uploadData();
                    }
                }
            private:
                moduleType          _type;
                size_t              _nbMeasures;
                moduleVendor        _vendor;
                moduleSerial        _serial;
                T                   _measures[n];
                timestamp           _timestamps[n];
                Task                _uploadDataTask;
        };
    }
}
#endif /* AWOODBOXMODULE_HPP */
