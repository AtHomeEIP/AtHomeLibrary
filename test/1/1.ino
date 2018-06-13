#include <SoftwareSerial.h>
#include <string.h>
#include <AtHome.h>

#ifdef __AVR__

/**
 * Compute the gap between the top of the heap and the current bottom of the stack to have the current free RAM
 */
int freeRam () {
  extern int __heap_start, *__brkval;
  int v = (uint8_t *)(SP);
  return (int) &v - ((__brkval == 0) ? (int) &__heap_start : (int) __brkval);
}

#endif

AtHomeWiFiModule<uint16_t, 15> *me = AtHomeModule<uint16_t, 15>::getInstance<AtHomeWiFiModule<uint16_t, 15> >();
ESP8266WiFiCommunicator wifi_com(2, 3);
MonochromaticLED led(13);
TMP36GZTemperatureSensor5V sensor(A0);
SoftwareSerial softSerial(10, 11);
Stream *streams[] = {&Serial, &softSerial, nullptr};
ArduinoEEPROM storage;
FakeRTC rtc;

const PROGMEM char ssid[] = "ESP01Test";
const PROGMEM char password[] = "HelloWorld";

void setup() {
    // put your setup code here, to run once:
    delay(10000);
    Serial.begin(9600);
    softSerial.begin(9600);
    delay(1);
    Serial.println(F("Setup..."));
    Serial.flush();
    wifi_com.setStreamToChipset(&Serial);
    wifi_com.setWiFiMode(athome::communication::wifi::wifi_mode::STATION);
    athome::communication::wifi::WiFi_ap ap;
    strcpy_P(ap.ssid, ssid);
    strcpy_P(ap.password, password);
    ap.norm = athome::communication::wifi::wifi_norm::WIFI_N;
    ap.channel = 5;
    ap.band = athome::communication::wifi::wifi_frequency_band::TWO_POINT_FOUR_GHZ;
    wifi_com.setAccessPoint(ap);
    wifi_com.open();
    wifi_com.connect();
    me->setSensor(&sensor);
    me->setDisplay(&led);
    me->setStreams(streams);
    me->setStorage(&storage);
    me->setClock(&rtc);
    me->setWiFiCommunicator(wifi_com);
    me->setSerial(0);
    me->setup();
#ifdef __AVR__
    Serial.println(freeRam());
    Serial.flush();
#endif
}

void loop() {
    // put your main code here, to run repeatedly:
    //exit(0);
#ifdef __AVR__
    Serial.println(freeRam());
    Serial.flush();
#endif
    me->run();
    //delay(1000);
}