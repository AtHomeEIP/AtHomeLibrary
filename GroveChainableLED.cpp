#ifdef GROVE_RGB_CHAINABLE_LED_ENABLED

# include "GroveChainableLED.hpp"

namespace woodBox {
	namespace display {
		GroveChainableLED::GroveChainableLED(const Pins *pins):
			_color({0, 0, 0}),
			_led(nullptr)
		{
			setup(pins);
		}

		GroveChainableLED::GroveChainableLED(const IRGBLed::Color &color, const Pins *pins):
			_color(color),
			_led(nullptr)
		{
			setup(pins);
		}

		GroveChainableLED::GroveChainableLED(const IRGBLed &other, const Pins *pins):
			_color(other.getColor()),
			_led(nullptr)
		{
			setup(pins);
		}

		GroveChainableLED &GroveChainableLED::operator=(const IRGBLed::Color &color) {
			_color = color;
			return *this;
		}

		GroveChainableLED &GroveChainableLED::operator=(const IRGBLed &other) {
			_color = other.getColor();
			return *this;
		}

		GroveChainableLED::~GroveChainableLED() {
			if (_led != nullptr)
				delete _led;
		}

		void GroveChainableLED::clear() {
			_color = {0, 0, 0};
		}

		void GroveChainableLED::update() {
			_led->setColorRGB(0, _color.red, _color.green, _color.blue);
		}

		const IRGBLed::Color &GroveChainableLED::getColor() const {
			return _color;
		}

		void GroveChainableLED::setColor(const IRGBLed::Color &color) {
			_color = color;
		}

		void GroveChainableLED::setup(const Pins *pins) {
			if (_led != nullptr)
				delete _led;
			_led = (pins != nullptr) ? new ChainableLED(pins->clock, pins->data, 1) : new ChainableLED(7, 8, 1);
			_led->init();
			update();
		}
	}
}

#endif /* GROVE_RGB_CHAINABLE_LED_ENABLED */
