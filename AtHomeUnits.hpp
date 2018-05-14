#ifndef ATHOMEUNITS_HPP
# define ATHOMEUNITS_HPP

namespace athome {
    namespace utility {
        namespace units {
            namespace si {
                enum UNIT {
                    // 0 is an invalid value
                    UNKNOWN = 0,
                    // Base units
                    METRE,
                    KILOGRAM,
                    SECOND,
                    AMPERE,
                    KELVIN,
                    MOLE,
                    CANDELA,
                    // Derived units
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
                    KATAL
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
            }
        }
    }
}

#endif /* ATHOMEUNITS_HPP */