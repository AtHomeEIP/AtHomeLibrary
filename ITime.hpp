#ifndef ITIME_HPP
# define ITIME_HPP
# ifndef DISABLE_TIME
#  include <stdint.h>
#  include <Printable.h>

namespace athome {
    namespace time {
        extern uint16_t absolute_year;

        class ITime {
        public:
            struct DateTime {
                uint32_t second : 6;
                uint32_t minute : 6;
                uint32_t hour : 5;
                uint32_t day : 5;
                uint32_t month : 4;
                uint32_t year : 6;
            };

            struct JSONDateTime : public DateTime, public Printable {
                virtual size_t printTo(Print &p) const {
                    size_t len = p.print(F("{\"Day\":"));
                    len += p.print(day);
                    len += p.print(F(",\"Month\":"));
                    len += p.print(month);
                    len += p.print(F(",\"Year\":"));
                    len += p.print(absolute_year + year);
                    len += p.print(F(",\"Hour\":"));
                    len += p.print(hour);
                    len += p.print(F(",\"Minute\":"));
                    len += p.print(minute);
                    len += p.print(F(",\"Second\":"));
                    len += p.print(second);
                    len += p.print(F("}"));
                    return len;
                }
            };

            struct ISO8601DateTime : public DateTime, public Printable {
                virtual size_t printTo(Print &p) const {
                    size_t len = p.print(absolute_year + year);
                    len += p.print(F("-"));
                    if (month < 10) {
                        len += p.print(F("0"));
                    }
                    len += p.print(month);
                    len += p.print(F("-"));
                    if (day < 10) {
                        len += p.print(F("0"));
                    }
                    len += p.print(day);
                    len += p.print(F("T"));
                    if (!hour) {
                        len += p.print(F("0"));
                    }
                    if (hour < 10) {
                        len += p.print(F("0"));
                    }
                    if (hour) {
                        len += p.print(hour);
                    }
                    len += p.print(F(":"));
                    if (!minute) {
                        len += p.print(F("0"));
                    }
                    if (minute < 10) {
                        len += p.print(F("0"));
                    }
                    if (len) {
                        len += p.print(minute);
                    }
                    len += p.print(F(":"));
                    if (!second) {
                        len += p.print(F("0"));
                    }
                    if (second < 10) {
                        len += p.print(F("0"));
                    }
                    if (second) {
                        len += p.print(second);
                    }
                    return len;
                }
            };

            virtual const ISO8601DateTime &getDateTime() const;
            virtual void setCurrentDateTime (const DateTime &);
        };
    }
}

# endif /* DISABLE_TIME */
#endif /* ITIME_HPP */