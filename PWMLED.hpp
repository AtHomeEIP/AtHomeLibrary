#ifndef PWMLED_HPP
# define PWMLED_HPP

# include "IDisplay.hpp"

namespace athome {
    namespace display {
        class PWMLED : public IDisplay {
        public:
            PWMLED(int, bool = false);
            PWMLED(const PWMLED &) = delete;
            PWMLED &operator=(const PWMLED &) = delete;
            ~PWMLED();

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

#endif /* PWMLED_HPP */
