#include <AtHome.h>

using SimplestModule = AtHomeModule<uint16_t, 15>;

Stream *streams[] = {&Serial, nullptr};
DummySensor<ALuminositySensor, uint16_t, 50000> dummy;
SimplestModule *module = SimplestModule::getInstance();

void setup() {
    Serial.begin(115200);
    module->setStreams(streams);
    module->setSensor(&dummy);
    module->setup();
}

void loop() {
    module->run();
}