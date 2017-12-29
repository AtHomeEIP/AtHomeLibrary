#ifndef APOWEREDMODULE_HPP
# define APOWEREDMODULE_HPP

# include "IModule.hpp"
# include "IPower.hpp"

namespace woodBox {
	namespace module {
		class APoweredModule : public IModule {
			public:
				APoweredModule(power::IPower *);
				APoweredModule(APoweredModule &);
				APoweredModule &operator=(APoweredModule &);
				virtual ~APoweredModule();
				virtual void run() = 0;
				virtual void stop() = 0;
				virtual const power::IPower *getPowerSource();
				virtual void setPowerSource(power::IPower *);
			protected:
				power::IPower *_power;
		};
	}
}

#endif /* APOWEREDMODULE_HPP */
