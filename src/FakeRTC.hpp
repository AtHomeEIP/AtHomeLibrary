#ifndef FAKERTC_HPP
#define FAKERTC_HPP
#include "AtHomeConfig.h"
#if !defined(DISABLE_TIME) && !defined(DISABLE_FAKE_RTC)
#include <Arduino.h>
#include <stdint.h>
#include "ITime.hpp"
#define SECONDS_PER_MINUTE 60
#define SECONDS_PER_HOUR 3600
#define SECONDS_PER_DAY 86400

namespace athome {
namespace time {
class FakeRTC : public ITime {
 public:
  FakeRTC();
  FakeRTC(const FakeRTC &) = delete;
  FakeRTC &operator=(const FakeRTC &) = delete;
  virtual ~FakeRTC();

  virtual const DateTime &getDateTime();
  virtual void setCurrentDateTime(const DateTime &);

 private:
  void addSeconds(uint8_t);
  void addMinutes(uint8_t);
  void addHours(uint8_t);
  void addDays(uint8_t);

 private:
  uint32_t _lastTime;
  DateTime _date;
};
}  // namespace time
}  // namespace athome

#endif /* !defined(DISABLE_TIME) && !defined(DISABLE_FAKE_RTC) */
#endif /* FAKERTC_HPP */
