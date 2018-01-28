#if !defined(__MSP430__) && !defined(TARGET_IS_MSP432P4XX) && !defined(__PIC32MX__)
# include "NeoPixel.hpp"

namespace woodBox {
    namespace display {
        NeoPixel::NeoPixel(uint8_t pin, neoPixelType conf):
            ARGBLed(),
            _conf(conf),
            _pixel{1, pin, conf} {
                _pixel.begin();
                _pixel.show();
        }

        NeoPixel::NeoPixel(uint8_t pin, const NeoPixel &other):
            ARGBLed(),
            _conf(other._conf),
            _pixel{1, pin, other._conf} {
                _pixel.begin();
                setColor(other.getColor());
                update();
        }

        NeoPixel &NeoPixel::operator=(const NeoPixel &other) {
            setColor(other.getColor());
            update();
        }

        NeoPixel::~NeoPixel() {
            clear();
        }

        void NeoPixel::update() {
            const Color &color = getColor();
            _pixel.setPixelColor(0, color.red, color.green, color.blue);
            _pixel.show();
        }
    }
}
#endif /* List of incompatible architectures */
