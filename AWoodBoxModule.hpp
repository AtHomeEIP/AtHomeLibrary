#ifndef AWOODBOXMODULE_HPP
# define AWOODBOXMODULE_HPP

# include <stdint.h>
# include "ABaseModule.hpp"
# include "AWiFiCommunicator.hpp"

# define END_OF_LINE            "\r\n"
# define SEPARATOR              "================================================================================"
# define HEADER_SPACER          "\t"
# define END_OF_COMMAND         "\X03"
# define END_OF_COMMUNICATION   "\X04"

# define ENUMERATE              "Enumerate"
# define UPLOAD_DATA            "UploadData"
# define SYNC_TIME              "SyncTime"
# define SET_WIFI               "SetWiFi"
# define SET_ENDPOINT           "SetEndpoint"
# define SET_PROFILE            "SetProfile"

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
                typename void       (AWoodBoxModule::*communicationCommand)(communication::ICommunicator*);
                typename Tuple<char[], communicationCommand> communicationTuple;

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
            private:
                template <typename T, typename U>
                struct Tuple {
                    T a;
                    U b;
                };
        };
    }
}
#endif /* AWOODBOXMODULE_HPP */
