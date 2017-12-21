#ifndef AWIFICOMMUNICATOR_HPP
# define AWIFICOMMUNICATOR_HPP

# include "ICommunicator.hpp"
# include "AWiFiCommunicator_types.hpp"

namespace woodBox {
	namespace communication {
		class AWiFiCommunicator : public ICommunicator {
			public:
				virtual ~AWiFiCommunicator() = 0;
				virtual uint8_t &read(uint32_t) = 0; // Read n bytes
				virtual void write(const uint8_t &, uint32_t) = 0; // Write n bytes
			protected:
				
		};
	}
}

#endif /* AWIFICOMMUNICATOR_HPP */