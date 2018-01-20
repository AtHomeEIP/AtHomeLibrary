#ifndef ABASEMODULE_HPP
# define ABASEMODULE_HPP

# define _TASK_INLINE // Macro used by TaskScheduler library

# include <stdint.h>
# include <Arduino.h>
# include <TaskScheduler.h>
# include "ADisplayModule.hpp"
# include "ACommunicativeModule.hpp"
# include "APoweredModule.hpp"
# include "ASensorModule.hpp"
# include "AStorageModule.hpp"

namespace woodBox {
    namespace module {
        class ABaseModule : public ADisplayModule, public ACommunicativeModule, public APoweredModule, public ASensorModule, public AStorageModule {
            public:
                virtual ABaseModule &operator=(ABaseModule &); // Singleton class
                virtual ~ABaseModule();
                virtual void run();
                virtual void stop();
                void setScheduler(Scheduler &);
                Scheduler *getScheduler();
                template <typename T>
                static ABaseModule *getInstance() {
                    if (_instance == nullptr) {
                        _instance = new T();
                        return _instance;
                    }
                    return _instance;
                    // Dynamic cast doesn't works on embedded target. Not troublesome here, but will be later in code
                    //return dynamic_cast<T*>(_instance);
                }
            protected:
                ABaseModule(display::IDisplay * = nullptr, Stream ** = nullptr,
                            power::IPower * = nullptr, sensor::ISensor * = nullptr, storage::IStorage * = nullptr);
                ABaseModule(ABaseModule &); // Singleton class
                static ABaseModule *getABaseModuleInstance() { return _instance; }
                /* Callbacks */
            protected:
                virtual void onReset() = 0;
                virtual void onStart() = 0;
                virtual void onStop() = 0;
                virtual void onPause() = 0;
                virtual void onResume() = 0;
                virtual void onBackupOnStorage() = 0;
                virtual void onRestoreFromStorage() = 0;
                virtual void onSampleSensor() = 0;
                virtual void onUpdateDisplay() = 0;
                virtual void onCommunicate() = 0;
                /* Default execution methods */
            protected:
                typedef void (*customCallback)();
                //virtual void sleep();
                //virtual void wakeUp();
                virtual void setup();
                virtual void loop();
                void setSensorExecutionInterval(unsigned long);
                void setDisplayExecutionInterval(unsigned long);
                void setCommunicationExecutionInterval(unsigned long);
                unsigned long getSensorExecutionInterval() const;
                unsigned long getDisplayExecutionInterval() const;
                unsigned long getCommunicationExecutionInterval() const;
                void setSensorExecutionCallback(customCallback);
                void setDisplayExecutionCallback(customCallback);
                void setCommunicationExecutionCallback(customCallback);
            private:
                static void _onSampleSensor() { _instance->onSampleSensor(); }
                static void _onUpdateDisplay() { _instance->onUpdateDisplay(); }
                static void _onCommunicate() { _instance->onCommunicate(); }
            private:
                Scheduler           *_scheduler;
                unsigned long       _sensorInterval;
                unsigned long       _displayInterval;
                unsigned long       _communicationInterval;
                Task                _sensorTask;
                Task                _displayTask;
                Task                _communicationTask;
                static ABaseModule  *_instance;
        };
    }
}

#endif /* ABASEMODULE_HPP */
