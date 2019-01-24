#include "AtHomeConfig.h"
#if !defined(DISABLE_TIME) && !defined(DISABLE_FAKE_RTC)
#include "FakeRTC.hpp"

namespace athome {
namespace time {
FakeRTC::FakeRTC() : _lastTime(millis()) {}

//FakeRTC::~FakeRTC() {}

void FakeRTC::setCurrentDateTime(const ITime::DateTime &date) { _date = date; }

const ITime::DateTime &FakeRTC::getDateTime() {
  uint32_t now = millis();
  uint32_t elapsed;
  if (now > _lastTime) {
    elapsed = now - _lastTime;
  } else {
    elapsed = (4294967295 - _lastTime) + now;  // Handle overflow of millis
  }
  _lastTime = now;
  elapsed /= 1000;  // Convert in seconds
  if (!elapsed) {
    return _date;  // No seconds elapsed, only milliseconds so return current
                   // date
  }
  uint8_t tmp = elapsed % 60;  // Get the number of seconds to add
  if (tmp) {
    addSeconds(
        tmp);  // If we have a non-null number of seconds in the current minute
  }
  elapsed /= 60;  // Convert in minutes
  if (!elapsed) {
    return _date;  // There was only seconds added
  }
  tmp = elapsed % 60;  // Get the number of minutes to add
  if (tmp) {
    addMinutes(
        tmp);  // If we have a non-null number of minutes in the current hour
  }
  elapsed /= 60;  // Convert in hours
  if (!elapsed) {
    return _date;  // We had only minutes elapsed
  }
  tmp = elapsed % 24;  // Get the number of hours of the day
  if (tmp) {
    addHours(tmp);  // If we have a non-null number
  }
  elapsed /= 24;  // Convert in days
  if (!elapsed) {
    return _date;
  }
  addDays(elapsed);  // Add remaining days (can't be years in theory, as
                     // milliseconds are coded on 32-bit and will overflow after
                     // about 50 days
  return _date;
}

void FakeRTC::addSeconds(uint8_t seconds) {
  uint8_t new_seconds = _date.second + seconds;
  if (new_seconds < 60) {
    _date.second = new_seconds;
  } else {
    addMinutes(1);
    _date.second = new_seconds - 60;
  }
}

void FakeRTC::addMinutes(uint8_t minutes) {
  uint8_t new_minutes = _date.minute + minutes;
  if (new_minutes < 60) {
    _date.minute = new_minutes;
  } else {
    addHours(1);
    _date.minute = new_minutes - 60;
  }
}

void FakeRTC::addHours(uint8_t hours) {
  uint8_t new_hours = _date.hour + hours;
  if (new_hours < 24) {
    _date.hour = new_hours;
  } else {
    addDays(1);
    _date.hour = new_hours - 24;
  }
}

void FakeRTC::addDays(uint8_t days) {
  uint8_t new_days = _date.day + days;
  uint16_t year = absolute_year + _date.year;
  if (_date.month == 2) {
    // If it's not a leap year, there's 28 days in the year
    if ((!(year % 100) && (year % 400)) || ((year % 100) && (year % 4))) {
      if (new_days > 28) {
        _date.month = 3;
        _date.day = new_days - 28;
      } else {
        _date.day = new_days;
      }
    } else {
      if (new_days > 29) {
        _date.month = 3;
        _date.day = new_days - 29;
      } else {
        _date.day = new_days;
      }
    }
  } else if (_date.month == 12) {
    if (new_days > 31) {
      if (_date.year == 63) {
        absolute_year += 64;
        _date.year = 0;
      } else {
        _date.year++;
      }
      _date.day = new_days - 31;
      _date.month = 1;
    } else {
      _date.day = new_days;
    }
  } else if (_date.month == 1 || _date.month == 3 || _date.month == 5 ||
             _date.month == 7 || _date.month == 8 || _date.month == 10) {
    if (new_days > 31) {
      _date.day = new_days - 31;
      _date.month++;
    } else {
      _date.day = new_days;
    }
  } else {
    if (new_days > 30) {
      _date.day = new_days - 30;
      _date.month++;
    } else {
      _date.day = new_days;
    }
  }
}
}  // namespace time
}  // namespace athome
#endif /* !defined(DISABLE_TIME) && !defined(DISABLE_FAKE_RTC) */
