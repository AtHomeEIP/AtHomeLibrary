#include <string.h>
#include <woodBox.h>

class MyModule : public woodBox::module::AWoodBoxModule<uint16_t, 15> {
  public:
    MyModule():AWoodBoxModule() {}
    virtual ~MyModule() {}
  protected:
                virtual void onSampleSensor() {}
                virtual void onUpdateDisplay() {}
};

woodBox::module::ABaseModule* me = MyModule::getInstance<MyModule>();
woodBox::communication::wifi::ESP8266WiFiCommunicator wifi_com(2, 3);
woodBox::communication::commands::CommandSetWiFi *setWiFi = new woodBox::communication::commands::CommandSetWiFi(wifi_com);

#ifdef __AVR__

/* AVRGCC Stack monitoring */
// From: http://www.avrfreaks.net/forum/soft-c-avrgcc-monitoring-stack-usage?name=PNphpBB2&file=viewtopic&t=52249
extern uint8_t _end;
extern uint8_t __stack;

void StackPaint(void) __attribute__((naked)) __attribute__((section(".init1")));

void StackPaint(void) {
  uint8_t *p = &_end;
  while (p <= &__stack) {
    *p = 0x2a;
    p++;
  }
}

size_t StackCount() {
  const uint8_t *p = &_end;
  size_t c = 0;

  while (*p == 0x2a && p <= &__stack) {
    p++;
    c++;
  }

  return c;
}

#endif

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    wifi_com.setStreamToChipset(&Serial);
    wifi_com.setWiFiMode(wifi_mode::ACCESS_POINT);
    WiFi_ap ap;
    strcpy(ap.ssid, "ESP01Test");
    strcpy(ap.password, "HelloWorld");
    ap.norm = wifi_norm::WIFI_N;
    ap.channel = 5;
    ap.band = wifi_frequency_band::TWO_POINT_FOUR_GHZ;
    wifi_com.setAccessPoint(ap);
#ifdef __AVR__
    Serial.println(StackCount(), DEC);
#endif
    //wifi_com.open();
    //wifi_com.connect();
}

void loop() {
  // put your main code here, to run repeatedly:
  exit(0);
    //delay(1000);
}
