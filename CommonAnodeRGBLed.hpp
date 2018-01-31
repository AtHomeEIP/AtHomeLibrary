#ifndef COMMONANODERGBLED_HPP
# define COMMONANODERGBLED_HPP

# include <stdint.h>
# include "ARGBLed.hpp"

namespace athome {
    namespace display {
        class CommonAnodeRGBLed : public ARGBLed {
        public:
            CommonAnodeRGBLed(uint8_t, uint8_t, uint8_t);
            CommonAnodeRGBLed(const CommonAnodeRGBLed &) = delete;
            CommonAnodeRGBLed &operator=(const CommonAnodeRGBLed &);
            ~CommonAnodeRGBLed();
            void update();

        private:
            uint8_t _pin_red;
            uint8_t _pin_green;
            uint8_t _pin_blue;
        };
    }
}

#endif /* COMMONANODERGBLED_HPP */
