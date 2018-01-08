#ifndef ABASEMODULE_HPP
# define ABASEMODULE_HPP

# include <stdint.h>

# ifdef ARDUINO
#  include <Arduino.h>
# else
inline void delay(uint32_t);
# endif /* ARDUINO */

# include "ADisplayModule.hpp"
# include "ACommunicativeModule.hpp"
# include "APoweredModule.hpp"
# include "ASensorModule.hpp"
# include "AStorageModule.hpp"

namespace woodBox {
    namespace module {
        class ABaseModule : public ADisplayModule, public ACommunicativeModule, public APoweredModule, public ASensorModule, public AStorageModule {
            public:
                virtual ABaseModule &operator=(ABaseModule &);
                virtual ~ABaseModule();
                virtual void run();
                virtual void stop();
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
                ABaseModule(display::IDisplay * = nullptr, communication::ICommunicator ** = nullptr,
                            power::IPower * = nullptr, sensor::ISensor * = nullptr, storage::IStorage * = nullptr);
                ABaseModule(ABaseModule &);
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
                virtual void sleep();
                virtual void wakeUp();
                virtual void setup();
                virtual void loop();
            private:
                static ABaseModule *_instance;
        };
    }
}

#endif /* ABASEMODULE_HPP */
