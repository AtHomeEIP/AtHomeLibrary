#ifndef IPOWER_HPP
# define IPOWER_HPP

# include <stdint.h>

namespace woodBox {
	namespace power {
		class IPower {
			public:
		        //virtual ~IPower() = 0;
				virtual uint32_t getVoltage() = 0; //! Hard-coded voltage in mV or calculated depending of the type of power supply
				virtual uint32_t getCurrent() = 0; //! Hard-coded current in uA or able to source depending of the type of power supply
		};
	}
}

#endif /* IPOWER_HPP */
