#ifndef TEST_IDISPLAY_HPP
#define TEST_IDISPLAY_HPP
#line 2 "TestARGBLed.hpp"
#include <Arduino.h>
#include <AtHome.h>
#include <ARGBLed.hpp>
#include <ArduinoUnit.h>
#include <string.h>

namespace {
    using athome::display::ARGBLed;
    using athome::display::IDisplay;
    using athome::sensor::ISensor;

    ARGBLed::Color *blue = new ARGBLed::Color{0, 0, 255};

    CommonCathodeRGBLed cathodeLed(3, 4, 5);
    CommonAnodeRGBLed anodeLed(3, 4, 5);
    GroveChainableLED groveLed(nullptr);
    NeoPixel *neopixelLed = nullptr;

    ARGBLed::Color totallyNewScale[] = {{0, 0, 0},
                      {255, 0, 155},
                      {230, 0, 180},
                      {205, 0, 205},
                      {180, 0, 230},
                      {155, 0, 255},
                      {0, 5, 0},
                      {0, 10, 0},
                      {0, 15, 0},
                      {0, 50, 0},
                      {0, 200, 0}};
}
void setTestNeoPixel(NeoPixel *led) {
    neopixelLed = led;
}
class TestIDisplay : public TestOnce {
    public:
        typedef int (*Comparator)(const void *a, const void *b);
        typedef bool (*IDisplayTest)(IDisplay *display,
                          void *given,
                          const void *expected,
                          Comparator cmp);
        TestIDisplay(IDisplay *display, IDisplayTest test,
                    const char *name = "",
                    void *given = nullptr,
                    const void *expected = nullptr,
                    Comparator cmp = nullptr):
            TestOnce(name),
            _display(display),
            _test(test),
            _given(given),
            _expected(expected),
            _cmp(cmp)
        {}
        virtual void once() {
            assertTrue(_test(_display, _given, _expected, _cmp));
        }
    public:
        static bool clearDisplay(IDisplay *display,
                                 void *given,
                                 const void *expected,
                                 Comparator cmp) {
            display->clear();
            return true;
        }
        static bool updateDisplay(IDisplay *display,
                                  void *given,
                                  const void *expected,
                                  Comparator cmp) {
            display->update();
            return true;
        }
        static bool setDisplayEstimate(IDisplay *display,
                                       void *given,
                                       const void *expected,
                                       Comparator cmp) {
            ISensor::ISensorScale *scale = reinterpret_cast<ISensor::ISensorScale *>(given);
            (void)expected;
            (void)cmp;
            display->setDisplayedEstimate(*scale);
            return true;
        }
    private:
        IDisplay *_display;
        IDisplayTest _test;
        void *_given;
        void *_expected;
        Comparator _cmp;
};
class TestARGBLed : public TestIDisplay {
    public:
        TestARGBLed(ARGBLed *led, IDisplayTest test,
                    const char *name = nullptr,
                    void *given = nullptr,
                    const void *expected = nullptr):
            TestIDisplay(led, test, name, given, expected, compareColor)
        {}
    public:
        static bool startAsBlack(IDisplay *led, void *color,
                                 const void *expected,
                                 Comparator cmp)
        {
            (void)color;
            (void)expected;
            (void)cmp;
            const ARGBLed &constLed = *(reinterpret_cast<ARGBLed *>(led));
            const ARGBLed::Color &receivedColor = constLed.getColor();
            return !receivedColor.red && !receivedColor.green && !receivedColor.blue;
        }
        static bool clearColor(IDisplay *display, void *given,
                               const void *expected, Comparator cmp) {
            ARGBLed *led = reinterpret_cast<ARGBLed *>(display);
            (void)given;
            (void)expected;
            (void)cmp;
            return clearDisplay(display, given, expected, cmp) &&
                   startAsBlack(display, given, expected, cmp);
        }
        static bool setColor(IDisplay *display, void *given,
                             const void *expected, Comparator cmp) {
            ARGBLed *led = reinterpret_cast<ARGBLed *>(display);
            ARGBLed::Color *input = reinterpret_cast<ARGBLed::Color *>(given);
            (void)cmp;
            led->setColor(*input);
            if (!updateDisplay(led, nullptr, nullptr, nullptr) ||
                cmp(&led->getColor(), expected)) {
                return false;
            }
            return true;
        }
        static bool setColorsScale(IDisplay *display, void *given,
                                   const void *expected, Comparator cmp) {
            ARGBLed *led = reinterpret_cast<ARGBLed *>(display);
            ARGBLed::Color *colors = reinterpret_cast<ARGBLed::Color *>(given);
            (void)expected;
            led->setColorsScale(colors);
            for (size_t i = ISensor::ONE; i <= ISensor::TEN; i++) {
                if (!setDisplayEstimate(led, &i, nullptr, nullptr) ||
                    !updateDisplay(led, nullptr, nullptr, nullptr) ||
                    cmp(&colors[i], &led->getColor())) {
                        return false;
                    }
            }
            return true;
        }
    private:
        static int compareColor(const void *a, const void *b) {
            return memcmp(a, b, sizeof(ARGBLed::Color));
        }
};

#define MSG_SHOULD_START_AS_BLACK(led) (\
    "[000] Given a new "\
    #led\
    " Then it should start with the black color"\
)
#define TEST_SHOULD_START_AS_BLACK(led, instance) \
TestARGBLed shouldStartAsBlack##led(\
    instance,\
    TestARGBLed::startAsBlack,\
    MSG_SHOULD_START_AS_BLACK(led)\
)

TEST_SHOULD_START_AS_BLACK(CommonCathodeRGBLed, &cathodeLed);
TEST_SHOULD_START_AS_BLACK(CommonAnodeRGBLed, &anodeLed);
TEST_SHOULD_START_AS_BLACK(GroveChainableLED, &groveLed);
//TEST_SHOULD_START_AS_BLACK(NeoPixel, neopixelLed);

#define MSG_SHOULD_SET_COLOR(led, color) (\
    "[900] Given a led "#led" and a color "#color\
    " When setting it and updating the display "\
    "Then it should be the new color"\
)
#define TEST_SHOULD_SET_COLOR(led, color, instance)\
TestARGBLed shouldSetColor##led(\
    instance,\
    TestARGBLed::setColor,\
    MSG_SHOULD_SET_COLOR(led, color),\
    color,\
    color\
)

TEST_SHOULD_SET_COLOR(CommonCathodeRGBLed, blue, &cathodeLed);
TEST_SHOULD_SET_COLOR(CommonAnodeRGBLed, blue, &anodeLed);
TEST_SHOULD_SET_COLOR(GroveChainableLED, blue, &groveLed);
//TEST_SHOULD_SET_COLOR(NeoPixel, blue, neopixelLed);

#define MSG_CLEARING_LED_SHOULD_SET_IT_BLACK(led) (\
    "[910] Given a led "#led" When clearing it Then it should set it black"\
)
#define TEST_CLEARING_LED_SHOULD_SET_IT_BLACK(led, instance)\
TestARGBLed clearShouldSetItBlack##led(\
    instance,\
    TestARGBLed::clearColor,\
    MSG_CLEARING_LED_SHOULD_SET_IT_BLACK(led)\
)

TEST_CLEARING_LED_SHOULD_SET_IT_BLACK(CommonCathodeRGBLed, &cathodeLed);
TEST_CLEARING_LED_SHOULD_SET_IT_BLACK(CommonAnodeRGBLed, &anodeLed);
TEST_CLEARING_LED_SHOULD_SET_IT_BLACK(GroveChainableLED, &groveLed);

#define MSG_SHOULD_SET_COLOR_SCALE(led, scale) (\
    "[500] Given a led "#led" and a scale of color "#scale\
    " When setting it and setting every possible estimate values"\
    " Then the colors should always match the scale"\
)
#define TEST_SHOULD_SET_COLOR_SCALE(led, scale, instance)\
TestARGBLed estimatesShouldMatchScale##led(\
    instance,\
    TestARGBLed::setColorsScale,\
    MSG_SHOULD_SET_COLOR_SCALE(led, scale),\
    scale\
)

TEST_SHOULD_SET_COLOR_SCALE(CommonCathodeRGBLed, totallyNewScale, &cathodeLed);
TEST_SHOULD_SET_COLOR_SCALE(CommonAnodeRGBLed, totallyNewScale, &anodeLed);
TEST_SHOULD_SET_COLOR_SCALE(GroveChainableLED, totallyNewScale, &groveLed);
#endif /* TEST_IDISPLAY_HPP */