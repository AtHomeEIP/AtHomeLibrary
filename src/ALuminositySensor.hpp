#ifndef ALUMINOSITYSENSOR_HPP
# define ALUMINOSITYSENSOR_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_SENSOR) && !defined(DISABLE_LUMINOSITY_SENSOR)
#  include "ISensor.hpp"

namespace athome {
    namespace sensor {
        class ALuminositySensor : public ISensor {
        public:
            enum Room_Type {
                NO,
                CORRIDORS,
                STAIRS,
                WAREHOUSES,
                SHOPS,
                SHOPS_CHECKOUTS,
                PUBLIC_AREAS_AND_ENTRANCES,
                COUNTERS,
                RESTAURANTS_HOSTELS_RECEPTIONS,
                KITCHENS,
                CLASSROOMS,
                CONFERENCE_ROOMS,
                INDUSTRIAL_DRAWING_ROOMS,
                OFFICE,
                ARCHIVES
            };

            ALuminositySensor();
            ALuminositySensor(const ALuminositySensor &) = delete;
            ALuminositySensor &operator=(const ALuminositySensor &) = delete;
            ~ALuminositySensor();

            /**
             * Return a reference on ISensorValue storing the number of lux as an integer.
             */
            const ISensorValue  &getSample();
            /**
             * Return the number of lux as an integer.
             */
            virtual uint16_t    getSensorSample() = 0;
            void                setThresholds(const ISensorThresholds &);
            /**
             * Set thresholds based on the room type
             */
            void                setRoom(Room_Type);

        private:
            ISensorValue    _value;
            uint16_t        _lux;
            uint16_t        _min;
            uint16_t        _max;
        };
    }
}

# endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_LUMINOSITY_SENSOR) */
#endif /* ALUMINOSITYSENSOR_HPP */
