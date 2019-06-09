#ifdef __MSP430__
#include <SPI.h>
#include <Enrf24.h>
#include <nRF24L01.h>
#include <AtHome.h>

using TemperatureModule = AtHomeModule<int32_t, 60>;

class StreamRadio : public virtual Stream, public virtual Enrf24 {
  public:
    StreamRadio(int ce, int csn, int irq):rxbuf{0},idx(0),Enrf24(ce, csn, irq) {}
    int available() {
      pullRxBuffer();
      return rxlength - idx;
    }
    int peek() { return (idx < rxlength) ? rxbuf[idx] : 0; }
    int read() {
      int ret = 0;
      if (idx < rxlength) {
        ret = rxbuf[idx++];
      }
      return ret;
    }
    void flush() { Enrf24::flush(); }
    size_t write(uint8_t b) { return Enrf24::write(b); }

  private:
    void pullRxBuffer() {
      if (idx >= rxlength && Enrf24::available(true)) {
        rxlength = Enrf24::read(rxbuf);
        idx = 0;
      }
    }

    uint8_t rxbuf[33];
    size_t idx;
    size_t rxlength;
};

static StreamRadio radio(P2_0, P2_1, P2_2);  // P2.0=CE, P2.1=CSN, P2.2=IRQ
static const uint8_t txaddr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01 };
static Stream *streams[] = {&radio, nullptr};
static TMP36GZTemperatureSensor3V3 tempSensor(A6);
static TemperatureModule *module = TemperatureModule::getInstance();
static MonochromaticLED led(RED_LED, true);
static FakeRTC rtc;

void initRadio() {
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  radio.begin(250000);
  radio.setTXaddress(txaddr);
  radio.setTXpower(-18);
}

void setup() {
  // put your setup code here, to run once:
  initRadio();
  module->setStreams(streams);
  module->setSensor(&tempSensor);
  module->setDisplay(&led);
  module->setClock(&rtc);
  module->setSerial(1); // Gives the module the first id
  module->setUploadDataExecutionInterval(60000); // Sends sensor readings every minutes (1 reading each second)
  module->setCommunicationExecutionInterval(65535); // Never try to read incoming command as RX has not been enabled
  module->setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  module->run();
}
#endif  // __MSP430__
