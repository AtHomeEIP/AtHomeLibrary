#ifndef IRGBLED_HPP
# define IRGBLED_HPP

# include <stdint.h>
# include "IDisplay.hpp"

namespace woodBox {
    namespace display {
        class IRGBLed : public IDisplay {
            public:
                struct Color {
                    uint8_t red;
                    uint8_t green;
                    uint8_t blue;
                };
                //virtual ~IRGBLed() = 0;
                virtual void clear() = 0;
                virtual void update() = 0;
                virtual const Color &getColor() const = 0;
                virtual void setColor(const Color &) = 0;
        };
    }
}

#endif /* IRGBLED_HPP */
