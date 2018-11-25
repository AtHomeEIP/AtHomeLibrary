#ifndef ESP8266WIFICOMMUNICATOR_HPP
#define ESP8266WIFICOMMUNICATOR_HPP

#include "AtHomeConfig.h"
#if !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) && \
    !defined(DISABLE_WIFI) && !defined(DISABLE_ESP_AT_MODULE)
#include <stdint.h>
#include "AWiFiCommunicator.hpp"

#ifndef ESP8266_COMMAND_BUFFER_SIZE
#define ESP8266_COMMAND_BUFFER_SIZE 4
#endif  // ESP8266_COMMAND_BUFFER_SIZE

#ifndef DEFAULT_WIFI_MAX_RETRY_OPERATION
#define DEFAULT_WIFI_MAX_RETRY_OPERATION 10
#endif  // WIFI_MAX_RETRY_OPERATION

#define TRY_COMMAND(cmd)                                       \
  {                                                            \
    uint8_t retry = 0;                                         \
    do                                                         \
      cmd while (_esp_answer_check() && retry++ < _max_retry); \
    if (retry >= _max_retry) {                                 \
      return -8;                                               \
    }                                                          \
  }

namespace athome {
namespace communication {
namespace wifi {
class ESP8266WiFiCommunicator : public AWiFiCommunicator {
 public:
  ESP8266WiFiCommunicator(int, int);
  ESP8266WiFiCommunicator(const ESP8266WiFiCommunicator &) = delete;
  ESP8266WiFiCommunicator &operator=(const ESP8266WiFiCommunicator &) = delete;
  virtual int available();
  virtual int read();
  virtual int peek();
  virtual size_t write(uint8_t);
  virtual void flush();
  void enable();
  void disable();
  virtual int connect();
  virtual int disconnect();
  virtual int connectToHost();
  virtual int disconnectFromHost();
  virtual bool isConnected() const;

 private:
  int _read();
  void _write();
  void _flush_input();
  void _flush_output();
  void _switch_on_esp();
  void _switch_off_esp();

 private:
  int _command_check();
  int _esp_answer_check();
  int _test_esp();
  int _set_esp_station_mode();
  int _set_esp_ap_mode();
  int _connect_ap();
  int _create_ap();
  int _reset_esp();
  int _wait_for_esp_ready();
  int _connect_to_tcp_socket();
  int _enable_transparent_mode();
  int _go_to_send_mode();
  int _command_check_peer();
  int _set_echo(bool);

 private:
  int _enable_pin;
  int _reset_pin;
  bool _enabled;
  bool _connected;
  bool _connected_to_host;
  bool _receiving_data;
  size_t _receiving_len;
  uint8_t _max_retry;
};
}  // namespace wifi
}  // namespace communication
}  // namespace athome

#endif /* !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) && \
   !defined(DISABLE_WIFI) && !defined(DISABLE_ESP_AT_MODULE) */
#endif /* ESP8266WIFICOMMUNICATOR_HPP */
