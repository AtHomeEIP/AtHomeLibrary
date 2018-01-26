#ifdef ARDUINO
# include <Arduino.h>
#else
extern void analogWrite(uint8_t, int);
#endif

#include "CommonCathodeRGBLed.hpp"

namespace woodBox {
    namespace display {
        CommonCathodeRGBLed::CommonCathodeRGBLed(uint8_t pin_red, uint8_t pin_green, uint8_t pin_blue):
                ARGBLed(),
                _pin_red(pin_red),
                _pin_green(pin_green),
                _pin_blue(pin_blue) {
        }

        CommonCathodeRGBLed &CommonCathodeRGBLed::operator=(const CommonCathodeRGBLed &other) {
            setColor(other.getColor());
        }

        CommonCathodeRGBLed::~CommonCathodeRGBLed() {}

        void CommonCathodeRGBLed::update() {
            const Color &color = getColor();
            analogWrite(_pin_red, color.red);
            analogWrite(_pin_green, color.green);
            analogWrite(_pin_blue, color.blue);
        }

        void CommonCathodeRGBLed::clear() {
            analogWrite(_pin_red, 0);
            analogWrite(_pin_green, 0);
            analogWrite(_pin_blue, 0);
        }
    }
}
