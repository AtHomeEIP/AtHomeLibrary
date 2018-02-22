#ifndef ATHOME_ANOISESENSOR_HPP
# define ATHOME_ANOISESENSOR_HPP

# include "ISensor.hpp"

namespace athome {
    namespace sensor {
        class ANoiseSensor : public ISensor {
        public:
            ANoiseSensor();
            ANoiseSensor(const ANoiseSensor &) = delete;
            ANoiseSensor &operator=(const ANoiseSensor &) = delete;
            ~ANoiseSensor();

            virtual uint8_t *getSample() = 0;
            virtual float   getLastSample() = 0;
            ISensorScale    getEstimate();
        };
    }
}

#endif //ATHOME_ANOISESENSOR_HPP
