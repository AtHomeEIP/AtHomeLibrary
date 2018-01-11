#include <string.h>
#include <woodBox.h>

class MyModule : public woodBox::module::AWoodBoxModule {
  public:
    MyModule():AWoodBoxModule() {}
    virtual ~MyModule() {}
  protected:
                virtual void onReset() {}
                virtual void onStart() {}
                virtual void onStop() {}
                virtual void onPause() {}
                virtual void onResume() {}
                virtual void onBackupOnStorage() {}
                virtual void onRestoreFromStorage() {}
                virtual void onSampleSensor() {}
                virtual void onUpdateDisplay() {}
                virtual void onCommunicate() {}
};

woodBox::module::ABaseModule* me = MyModule::getInstance<MyModule>();
woodBox::communication::wifi::ESP8266WiFiCommunicator wifi_com(2, 3);
woodBox::communication::commands::CommandSetWiFi *setWiFi = new woodBox::communication::commands::CommandSetWiFi(wifi_com);

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    wifi_com.setStreamToChipset(&Serial);
    wifi_com.setWiFiMode(wifi_mode::STATION);
    WiFi_ap ap;
    strcpy(ap.ssid, "ESP01Test");
    strcpy(ap.password, "HelloWorld");
    ap.norm = wifi_norm::WIFI_N;
    ap.channel = 5;
    ap.band = wifi_frequency_band::TWO_POINT_FOUR_GHZ;
    wifi_com.setAccessPoint(ap);
    delay(5000);
    Serial.println(F("Ready? Emulate esp!"));
    delay(1000);
    wifi_com.open();
    wifi_com.connect();
}

void loop() {
  // put your main code here, to run repeatedly:
    delay(1000);
}
