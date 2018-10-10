#ifndef ITIME_HPP
#define ITIME_HPP
#include "AtHomeConfig.h"
#ifndef DISABLE_TIME
#include <Printable.h>
#include <stdint.h>
#include "AtHomeCommunicationProtocol.hpp"

namespace athome {
namespace time {
extern uint16_t absolute_year;

struct ITime {
  struct DateTime {
    uint32_t second : 6;
    uint32_t minute : 6;
    uint32_t hour : 5;
    uint32_t day : 5;
    uint32_t month : 4;
    uint32_t year : 6;

    DateTime() : second(0), minute(0), hour(0), day(1), month(1), year(0) {}

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

  virtual const DateTime &getDateTime() = 0;
  virtual void setCurrentDateTime(const DateTime &) = 0;
};
}  // namespace time
}  // namespace athome

#endif /* DISABLE_TIME */
#endif /* ITIME_HPP */