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

#ifdef __AVR__

/* AVRGCC Stack monitoring */
// From: http://www.avrfreaks.net/forum/soft-c-avrgcc-monitoring-stack-usage?name=PNphpBB2&file=viewtopic&t=52249
/*extern "C" {
extern uint8_t _end;
extern uint8_t __stack;
//void StackPaint(void) __attribute__ ((naked)) __attribute__ ((section (".init1")));

void StackPaint(void)
{
#if 1
    uint8_t *p = &_end;

    while(p <= &__stack)
    {
        *p = 0x4e;
        p++;
    }
#else
    __asm volatile ("    ldi r30,lo8(_end)\n"
                    "    ldi r31,hi8(_end)\n"
                    "    ldi r24,lo8(0x4e)\n" // STACK_CANARY = 0xc5
                    "    ldi r25,hi8(__stack)\n"
                    "    rjmp .cmp\n"
                    ".loop:\n"
                    "    st Z+,r24\n"
                    ".cmp:\n"
                    "    cpi r30,lo8(__stack)\n"
                    "    cpc r31,r25\n"
                    "    brlo .loop\n"
                    "    breq .loop"::);
#endif
}

uint16_t StackCount(void)
{
    const uint8_t *p = &_end;
    uint16_t       c = 0;

    while(p <= &__stack)
    {
        if (*p == 0x4e)
            c++;
        p++;
    }

    return c;
}

}*/

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

#endif

MyModule *me = MyModule::getInstance<MyModule>();
woodBox::communication::wifi::ESP8266WiFiCommunicator wifi_com(2, 3);
woodBox::communication::commands::CommandSetWiFi set_wifi(wifi_com);

const PROGMEM char ssid[] = "ESP01Test";
const PROGMEM char password[] = "HelloWorld";

void setup() {
  // put your setup code here, to run once:
    //StackPaint();
    Serial.begin(9600);
    delay(1);
    Serial.println(F("Setup..."));
    Serial.flush();
    wifi_com.setStreamToChipset(&Serial);
    wifi_com.setWiFiMode(wifi_mode::ACCESS_POINT);
    WiFi_ap ap;
    strcpy_P(ap.ssid, ssid);
    strcpy_P(ap.password, password);
    ap.norm = wifi_norm::WIFI_N;
    ap.channel = 5;
    ap.band = wifi_frequency_band::TWO_POINT_FOUR_GHZ;
#ifdef __AVR__
    /*if (&_end >= &__stack) {
      Serial.print(F("Stack overflow: "));
      Serial.println((size_t)(&_end - &__stack));
    }
    else
        Serial.println(StackCount(), DEC);*/
    Serial.println(freeRam());
    Serial.flush();
#endif
    //wifi_com.setAccessPoint(ap);
    //wifi_com.open();
    //wifi_com.connect();
}

void loop() {
  // put your main code here, to run repeatedly:
  exit(0);
    //delay(1000);
}
