#ifndef ATEMPERATURESENSOR_HPP
# define ATEMPERATURESENSOR_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_SENSOR) && !defined(DISABLE_TEMPERATURE_SENSOR)
#  include "ISensor.hpp"

namespace athome {
    namespace sensor {
        class ATemperatureSensor : public ISensor {
        public:
            ATemperatureSensor();
            ATemperatureSensor(const ATemperatureSensor &) = delete;
            ATemperatureSensor &operator=(const ATemperatureSensor &) = delete;
            ~ATemperatureSensor();
            /**
             * Returns a pointer on the sample of the sensor. This pointer needs to point on a float variable.
             */
            virtual const ISensorValue &getSample();
            /**
             * Returns the last value sampled from the sensor (do not actually resample it).
             */
            virtual int32_t getSensorSample() = 0;
            virtual void setThresholds(const ISensorThresholds &);

        private:
            ISensorValue    _value;
            int32_t         _temp;
            int32_t         _min;
            int32_t         _max;
        };
    }
}

# endif /* if !defined(DISABLE_SENSOR) && !defined(DISABLE_TEMPERATURE_SENSOR) */
#endif /* ATEMPERATURESENSOR_HPP */
