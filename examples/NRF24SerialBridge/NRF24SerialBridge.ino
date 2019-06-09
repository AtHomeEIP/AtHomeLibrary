#ifdef __MSP430__
#include <SPI.h>
#include <Enrf24.h>
#include <nRF24L01.h>
#include <string.h>

Enrf24 radio(P2_0, P2_1, P2_2);
const uint8_t rxaddr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01 };

void setup() {
  Serial.begin(9600);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  radio.begin(250000);  // 250kbps, channel 0, max TX power
  radio.setRXaddress((void*)rxaddr);
  radio.enableRX();
}

void loop() {
  char inbuf[33];
  while (!radio.available(true));
  ssize_t len = radio.read(inbuf, 32);
  if (len) {
    for (int i = 0; i < len; i++) {
      Serial.write(inbuf[i]);
    }
  }
  if (Serial.available()) {
    radio.disableRX();
    while (Serial.available()) {
      radio.write(Serial.read());
    }
    radio.flush();
    radio.enableRX();
  }
}
#endif  // __MSP430__
