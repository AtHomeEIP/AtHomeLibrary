#include <string.h>
#include <woodBox.h>

#ifdef __AVR__

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

#endif

WoodBoxWiFiModule<uint16_t, 15> *me = WoodBoxModule<uint16_t, 15>::getInstance<WoodBoxWiFiModule<uint16_t, 15> >();
ESP8266WiFiCommunicator wifi_com(2, 3);

const PROGMEM char ssid[] = "ESP01Test";
const PROGMEM char password[] = "HelloWorld";

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    delay(1);
    Serial.println(F("Setup..."));
    Serial.flush();
    wifi_com.setStreamToChipset(&Serial);
    wifi_com.setWiFiMode(woodBox::communication::wifi::wifi_mode::ACCESS_POINT);
    woodBox::communication::wifi::WiFi_ap ap;
    strcpy_P(ap.ssid, ssid);
    strcpy_P(ap.password, password);
    ap.norm = woodBox::communication::wifi::wifi_norm::WIFI_N;
    ap.channel = 5;
    ap.band = woodBox::communication::wifi::wifi_frequency_band::TWO_POINT_FOUR_GHZ;
#ifdef __AVR__
    Serial.println(freeRam());
    Serial.flush();
#endif
    wifi_com.setAccessPoint(ap);
    wifi_com.connect();
}

void loop() {
  // put your main code here, to run repeatedly:
  exit(0);
    //delay(1000);
}
