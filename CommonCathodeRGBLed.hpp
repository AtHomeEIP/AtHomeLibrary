#ifndef COMMONCATHODERGBLED_HPP
# define COMMONCATHODERGBLED_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_COMMON_CATHODE_RGB_LED) && !defined(DISABLE_DISPLAY)
#  include <stdint.h>
#  include "ARGBLed.hpp"

namespace athome {
    namespace display {
        class CommonCathodeRGBLed : public ARGBLed {
        public:
            CommonCathodeRGBLed(uint8_t, uint8_t, uint8_t);
            CommonCathodeRGBLed(const CommonCathodeRGBLed &) = delete;
            CommonCathodeRGBLed &operator=(const CommonCathodeRGBLed &);
            ~CommonCathodeRGBLed();
            void update();

        private:
            uint8_t _pin_red;
            uint8_t _pin_green;
            uint8_t _pin_blue;
        };
    }
}

#endif /* !defined(DISABLE_COMMON_CATHODE_RGB_LED) && !defined(DISABLE_DISPLAY) */
#endif /* COMMONCATHODERGBLED_HPP */
