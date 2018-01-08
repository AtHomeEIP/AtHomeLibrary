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
woodBox::communication::wifi::ESP8266WiFiCommunicator wifi_com;
woodBox::communication::commands::CommandSetWiFi *setWiFi = new woodBox::communication::commands::CommandSetWiFi(wifi_com);

void setup() {
  // put your setup code here, to run once:
    me->run();
}

void loop() {
  // put your main code here, to run repeatedly: 
  
}
