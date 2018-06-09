#ifndef ATHOMEUNITS_HPP
# define ATHOMEUNITS_HPP

# include <stdint.h>

namespace athome {
    namespace utility {
        namespace units {
            enum UNIT {
                // 0 is an invalid value
                UNKNOWN = 0,
                // Base SI units
                METRE,
                KILOGRAM,
                SECOND,
                AMPERE,
                KELVIN,
                MOLE,
                CANDELA,
                // Derived SI units
                RADIAN,
                STERADIAN,
                HERTZ,
                NEWTON,
                PASCAL,
                JOULE,
                WATT,
                COULOMB,
                VOLT,
                FARAD,
                OHM,
                SIEMENS,
                WEBER,
                TESLA,
                HENRY,
                DEGREE_CELSIUS,
                LUMEN,
                LUX,
                BECQUEREL,
                GRAY,
                SIEVERT,
                KATAL,
                // Non-SI units
                PART_PER_MILLION,
                RELATIVE_HUMIDITY,
                // Struct of values with different units
                AGGREGATE
            };

            enum PREFIX {
                // 0 is no prefix
                BASE_UNIT = 0,
                // Multiples
                DECA,
                HECTO,
                KILO,
                MEGA,
                GIGA,
                TERA,
                PETA,
                EXA,
                ZETTA,
                YOTA,
                // Submultiples
                DECI,
                CENTI,
                MILLI,
                MICRO,
                NANO,
                PICO,
                FEMTO,
                ATTO,
                ZEPTO,
                YOCTO
            };

            struct Unit {
                uint16_t unit : 8;
                uint16_t prefix : 8;
            };
        }
    }
}

#endif /* ATHOMEUNITS_HPP */