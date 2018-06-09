#ifndef COMMONANODERGBLED_HPP
# define COMMONANODERGBLED_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_DISPLAY) && !defined(DISABLE_COMMON_ANODE_RGB_LED)
#  include <stdint.h>
#  include "ARGBLed.hpp"

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

# endif /* !defined(DISABLE_DISPLAY) && !defined(DISABLE_COMMON_ANODE_RGB_LED) */
#endif /* COMMONANODERGBLED_HPP */
