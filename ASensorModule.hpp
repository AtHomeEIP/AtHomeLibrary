#ifndef ASENSORMODULE_HPP
# define ASENSORMODULE_HPP

# include "IModule.hpp"
# include "ISensor.hpp"

namespace woodBox {
	namespace module {
		class ASensorModule : public IModule {
			public:
				ASensorModule(sensor::ISensor *sensor);
				ASensorModule(ASensorModule &);
				ASensorModule &operator=(ASensorModule &);
				virtual ~ASensorModule();
				virtual void run() = 0;
				virtual void stop() = 0;
				virtual const sensor::ISensor *getSensor();
				virtual void setSensor(sensor::ISensor *);
			protected:
				virtual void onSampleSensor() = 0;
			protected:
				sensor::ISensor *_sensor;
		};
	}
}

#endif /* ASENSORMODULE_HPP */