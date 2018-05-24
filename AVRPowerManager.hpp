#ifndef AVRPOWERMANAGER_HPP
# define AVRPOWERMANAGER_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_POWER_MANAGEMENT) && !defined(DISABLE_AVR_POWER_MANAGEMENT) && defined(__AVR__)
#  include "IPower.hpp"

namespace athome {
    namespace power {
        class AVRPowerManagement : public IPower {
        public:
            AVRPowerManagement();
            AVRPowerManagement(const AVRPowerManagement &);
            AVRPowerManagement &operator=(const AVRPowerManagement &);
            ~AVRPowerManagement();

            const   PowerInfo *getPowerInfo();

            void    sleep(SLEEP_MODE, uint32_t);

        protected:
            void    _setupWatchdog();
        };
    }
}

# endif /* !defined(DISABLE_POWER_MANAGEMENT) && !defined(DISABLE_AVR_POWER_MANAGEMENT) */
#endif /* AVRPOWERMANAGER_HPP */