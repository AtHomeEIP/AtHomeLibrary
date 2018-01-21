#ifndef ABASEMODULE_HPP
# define ABASEMODULE_HPP

# define _TASK_INLINE // Macro used by TaskScheduler library

# include <stdint.h>
# include <Arduino.h>
# include <TaskScheduler.h>
# include "IDisplay.hpp"
# include "IPower.hpp"
# include "ISensor.hpp"
# include "IStorage.hpp"

namespace woodBox {
    namespace module {
        class ABaseModule {
            public:
                ABaseModule &operator=(ABaseModule &) = delete; // Singleton class
                ~ABaseModule();
                virtual void run() = 0;
            protected:
                ABaseModule(display::IDisplay * = nullptr, Stream ** = nullptr,
                            power::IPower * = nullptr, sensor::ISensor * = nullptr, storage::IStorage * = nullptr);
                ABaseModule(ABaseModule &) = delete; // Singleton class
                Stream **getStreams();
                void setStreams(Stream **);
                display::IDisplay *getDisplay();
                void setDisplay(display::IDisplay *);
                const power::IPower *getPowerSource() const;
                void setPowerSource(power::IPower *);
                const sensor::ISensor *getSensor() const;
                void setSensor(sensor::ISensor *);
                const storage::IStorage *getStorage() const;
                void setStorage(storage::IStorage *);
            protected:
                display::IDisplay   *_display;
                Stream              **_streams;
                power::IPower       *_power;
                sensor::ISensor     *_sensor;
                storage::IStorage   *_storage;
        };
    }
}

#endif /* ABASEMODULE_HPP */
