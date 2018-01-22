#ifndef ARGBLED_HPP
# define ARGBLED_HPP

# include <stdint.h>
# include "IDisplay.hpp"

namespace woodBox {
    namespace display {
        class ARGBLed : public IDisplay {
            public:
                struct Color {
                    uint8_t red;
                    uint8_t green;
                    uint8_t blue;
                };
                ~ARGBLed() {}
                ARGBLed():_color({0, 0, 0}) {}
                ARGBLed(ARGBLed const &) = delete;
                ARGBLed &operator=(ARGBLed const &) = delete;
                virtual void clear() = 0;
                virtual void update() = 0;
                const Color &getColor() const { return _color; }
                void setColor(const Color &color) { _color = color; }
            private:
                Color   _color;
        };
    }
}

#endif /* ARGBLED_HPP */
