#include "ADisplayModule.hpp"

namespace woodBox {
    namespace module {
        ADisplayModule::ADisplayModule(display::IDisplay *display):
            _display(display)
        {}

        ADisplayModule::ADisplayModule(ADisplayModule &other):
            _display(other._display)
        {
            other._display = nullptr;
        }

        ADisplayModule &ADisplayModule::operator=(ADisplayModule &other) {
			if (_display != nullptr)
				delete _display;
            _display = other._display;
            other._display = nullptr;
            return *this;
        }

        ADisplayModule::~ADisplayModule() {
			if (_display != nullptr)
				delete _display;
        }

        const display::IDisplay *ADisplayModule::getDisplay() {
            return _display;
        }

        void ADisplayModule::setDisplay(display::IDisplay *display) {
			if (_display != nullptr)
				delete _display;
            _display = display;
        }
    }
}

