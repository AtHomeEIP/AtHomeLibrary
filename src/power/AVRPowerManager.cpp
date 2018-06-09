#include "AtHomeConfig.h"
#if !defined(DISABLE_POWER_MANAGEMENT) && !defined(DISABLE_AVR_POWER_MANAGEMENT) && defined(__AVR__)
# include <avr/sleep.h>
# include <avr/interrupt.h>
# include <avr/power.h>
# include "AVRPowerManager.hpp"

namespace athome {
    namespace power {
        static volatile bool        _avr_sleeping = false;
        static volatile uint16_t    _sleep_duration = 0;

        ISR(WDT_vect) {
            if (_avr_sleeping && _sleep_duration) {
                _sleep_duration--;
            }
            else {
                _avr_sleeping = false;
                _sleep_duration = 0;
            }
        }

        AVRPowerManagement::AVRPowerManagement() {}

        AVRPowerManagement::AVRPowerManagement(const AVRPowerManagement &other) {}

        AVRPowerManagement& AVRPowerManagement::operator=(const AVRPowerManagement &other) { return *this; }

        AVRPowerManagement::~AVRPowerManagement() {}

        const IPower::PowerInfo* AVRPowerManagement::getPowerInfo() { return nullptr; }

        void AVRPowerManagement::sleep(IPower::SLEEP_MODE mode, uint32_t duration) {
            if (!duration || _avr_sleeping) {
                return;
            }
            _sleep_duration = (duration / 1000) + 1;
            _avr_sleeping = true;
            _setupWatchdog();
            while (_avr_sleeping) {
                switch (mode) {
                    case SLEEP_MODE::LIGHT_SLEEP:
                        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
                        break;
                    case SLEEP_MODE::SLEEP:
                        set_sleep_mode(SLEEP_MODE_STANDBY);
                        break;
                    case SLEEP_MODE::DEEP_SLEEP:
                        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
                        break;
                    default:
                        break;
                }
                sleep_enable();
                sleep_mode();
                sleep_disable();
                power_all_enable();
            }
        }

        void AVRPowerManagement::_setupWatchdog() {
            MCUSR &= ~(1<<WDRF);
            WDTCSR |= (1<<WDCE) | (1<<WDE);
# if    F_CPU == 1000000UL
            WDTCSR |= (0<<WDP3) | (0<<WDP2) | (1<<WDP1) | (0<<WDP0);
# elif  F_CPU == 2000000UL
            WDTCSR |= (0<<WDP3) | (0<<WDP2) | (1<<WDP1) | (1<<WDP0);
# elif  F_CPU == 4000000UL
            WDTCSR |= (0<<WDP3) | (1<<WDP2) | (0<<WDP1) | (0<<WDP0);
# elif  F_CPU == 8000000UL
            WDTCSR |= (0<<WDP3) | (1<<WDP2) | (0<<WDP1) | (0<<WDP0);
# elif  F_CPU == 16000000UL
            WDTCSR |= (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (0<<WDP0);
# else
#  error CPU frequency not supported
# endif /* F_CPU == 16000000UL */
            WDTCSR |= _BV(WDIE);
        }
    }
}

#endif /* !defined(DISABLE_POWER_MANAGEMENT) && !defined(DISABLE_AVR_POWER_MANAGEMENT) && defined(__AVR__) */