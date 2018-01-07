#ifndef CHAINABLELED_HPP
# define CHAINABLELED_HPP

# ifdef GROVE_RGB_CHAINABLE_LED_ENABLED

#  include <ChainableLED.h>
#  include "IRGBLed.hpp"

namespace woodBox {
	namespace display {
		class GroveChainableLED : public IRGBLed {
			public:
				struct Pins {
					uint8_t clock;
					uint8_t data;
				};
				GroveChainableLED(const Pins * = nullptr);
				GroveChainableLED(const IRGBLed::Color &, const Pins * = nullptr);
				GroveChainableLED(const IRGBLed &, const Pins * = nullptr);
				GroveChainableLED &operator=(const IRGBLed &);
				GroveChainableLED &operator=(const IRGBLed::Color &);
				virtual ~GroveChainableLED();
				virtual void clear();
				virtual void update();
				virtual const IRGBLed::Color &getColor() const;
				virtual void setColor(const IRGBLed::Color &);
			private:
				void setup(const Pins *);
			protected:
				IRGBLed::Color _color;
				ChainableLED *_led;
		};
	}
}

# endif /* GROVE_RGB_CHAINABLE_LED_ENABLED */

#endif /* CHAINABLELED_HPP */
