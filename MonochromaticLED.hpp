#ifndef MONOCHROMATICLED_HPP
# define MONOCHROMATICLED_HPP

# include "IDisplay.hpp"

namespace athome {
    namespace display {
        class MonochromaticLED : public IDisplay {
        public:
            MonochromaticLED(int, bool = false);
            MonochromaticLED(const MonochromaticLED &) = delete;
            MonochromaticLED &operator=(const MonochromaticLED &) = delete;
            ~MonochromaticLED();

            virtual void clear();
            virtual void update();
            virtual void setDisplayedEstimate(sensor::ISensor::ISensorScale);

        private:
            int     _pin;
            bool    _reversed;
            uint8_t _value;
        };
    }
}

#endif /* MONOCHROMATICLED_HPP */
