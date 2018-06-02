#ifndef ITIME_HPP
# define ITIME_HPP
# include "AtHomeConfig.h"
# ifndef DISABLE_TIME
#  include <stdint.h>
#  include <Printable.h>

namespace athome {
    namespace time {
        extern uint16_t absolute_year;

        struct ITime {
            class DateTime {
            public:
                uint32_t second : 6;
                uint32_t minute : 6;
                uint32_t hour : 5;
                uint32_t day : 5;
                uint32_t month : 4;
                uint32_t year : 6;

                DateTime &operator=(const DateTime &date) {
                    second = date.second;
                    minute = date.minute;
                    hour = date.hour;
                    day = date.day;
                    month = date.month;
                    year = date.year;
                    return *this;
                }
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

                JSONDateTime &operator=(const DateTime &date) {
                    second = date.second;
                    minute = date.minute;
                    hour = date.hour;
                    day = date.day;
                    month = date.month;
                    year = date.year;
                    return *this;
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

                ISO8601DateTime &operator=(const DateTime &date) {
                    second = date.second;
                    minute = date.minute;
                    hour = date.hour;
                    day = date.day;
                    month = date.month;
                    year = date.year;
                    return *this;
                }
            };

            virtual const ISO8601DateTime &getDateTime();
            virtual void setCurrentDateTime (const DateTime &);
        };
    }
}

# endif /* DISABLE_TIME */
#endif /* ITIME_HPP */