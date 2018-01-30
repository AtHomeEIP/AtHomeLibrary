#ifndef NEOPIXEL_HPP
# define NEOPIXEL_HPP

# if !defined(__MSP430__) && !defined(TARGET_IS_MSP432P4XX) && !defined(__PIC32MX__)
#  include <Adafruit_NeoPixel.h>
#  include "ARGBLed.hpp"

namespace athome {
    namespace display {
        class NeoPixel : public ARGBLed {
        public:
            NeoPixel(uint8_t, neoPixelType = NEO_GRB + NEO_KHZ800);
            NeoPixel(const NeoPixel &) = delete;
            NeoPixel(uint8_t, const NeoPixel &);
            NeoPixel &operator=(const NeoPixel &);
            ~NeoPixel();
            void update();

        private:
            neoPixelType        _conf;
            Adafruit_NeoPixel   _pixel;
        };
    }
}
# endif /* List of incompatible architectures */

#endif /* NEOPIXEL_HPP */
