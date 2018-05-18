#ifndef IPOWER_HPP
# define IPOWER_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_POWER_MANAGEMENT)
#  include <stdint.h>

namespace athome {
	namespace power {
		class IPower {
			public:
		        enum SLEEP_MODE {
		            ACTIVE = 0,
                    LIGHT_SLEEP,
                    SLEEP,
                    DEEP_SLEEP
		        };

		        struct PowerInfo {
		            uint32_t    voltage;
		            uint32_t    current;
		            uint8_t     remainingCapacity;
		        };
		        //virtual ~IPower() = 0;
				virtual const PowerInfo *getPowerInfo() = 0;
				virtual void            sleep(SLEEP_MODE) = 0;
		};
	}
}

# endif /* DISABLE_POWER_MANAGEMENT */
#endif /* IPOWER_HPP */
