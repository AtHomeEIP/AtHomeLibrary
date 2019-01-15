#ifndef DUMMYSENSOR_HPP
# define DUMMYSENSOR_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_SENSOR) && !defined(DISABLE_DUMMY_SENSOR)
#  include <stdlib.h>
#  include <string.h>
#  include "ISensor.hpp"

namespace athome {
    namespace sensor {
        template <class T, typename U, int limit>
        class DummySensor : public T {
        public:
            DummySensor() {}
            DummySensor(const DummySensor &) = delete;
            DummySensor &operator=(const DummySensor &) = delete;
            ~DummySensor() {}

            virtual U getSensorSample() {
                memset(&_data, rand() % limit, sizeof(U));
                return _data;
            }

            virtual U getSensorSample() const {
                U data;
                memset(&data, rand() % limit, sizeof(U));
                return data;
            }

        private:
            U   _data;
        };
    }
}

# endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_DUMMY_SENSOR) */
#endif /* DUMMYSENSOR_HPP */