#ifndef AWOODBOXMODULE_HPP
# define AWOODBOXMODULE_HPP

# include <stdint.h>
# include "ABaseModule.hpp"
# include "AWiFiCommunicator.hpp"

namespace woodBox {
    namespace module {
        class AWoodBoxModule : public ABaseModule {
            public:
                enum moduleType {
                    AIR_QUALITY, // Dust, smoke, ...etc
                    TEMPERATURE,
                    HUMIDITY,
                    LUMINOSITY,
                    NOISE,
                    CARBON, // Monoxyde, Dioxyde, Methane, Butane, Propane, ...etc
                    CARBON_MONOXYDE,
                    ELECTROMAGNETIC,
                    RADIOACTIVITY // Geiger counter
                };

                typename char       moduleVendor[33];
                typename char       moduleSerial[33];
                typename uint16_t   measure;
                typename uint32_t   timestamp;

                AWoodBoxModule(const AWoodBoxModule &) = delete;
                AWoodBoxModule &operator=(const AWoodBoxModule &) = delete;
                virtual ~AWoodBoxModule();
            protected:
                AWoodBoxModule();
            private:
                static moduleType   _type;
                static moduleVendor _vendor;
                static moduleSerial _serial;
                size_t              _nbMeasures;
                measure             *_measures;
                timestamp           *_timestamps;
                size_t              _measureId;
        };
    }
}
#endif /* AWOODBOXMODULE_HPP */
