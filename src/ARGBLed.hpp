#ifndef ARGBLED_HPP
#define ARGBLED_HPP

#include "AtHomeConfig.h"
#if !defined(DISABLE_DISPLAY)

#include <stdint.h>
#include <string.h>
#include "IDisplay.hpp"

namespace athome {
namespace display {
/**
 * Abstract class used to control RGB LEDs (Common cathode/anode, Grove
 * Chainable LEDs, NeoPixels, ...etc)
 *
 * Example of use:
 *
 * \code{.cpp}
 * void my_function_changing_led_color(ARGBLed &led) {
 *   Color red = {255, 0, 0}; // Create a Color structure with red fully on,
 * green off and blue off led.setColor(red); // Set the new color by calling
 * setColor method led.update(); // Display the new color on the LED
 * }
 * \endcode
 */
class ARGBLed : public IDisplay {
 public:
  /**
   * Structure used to represent the color displayed on a LED, composed of 3
   * fields (red, green and blue) accepting a value between 0 (off) and 255
   * (fully on) for each field.
   *
   * Examples of colors:
   *
   * \code{.cpp}
   * Color red = {255, 0, 0};
   * Color green = {0, 255, 0};
   * Color blue = {0, 0, 255};
   * Color white = {255, 255, 255};
   * Color off = {0, 0, 0}; // LED will be off, no light will be emit. If LED is
   * behind a screen, it will appear black Color faint_white = {10, 10, 10};
   * Color yellow = {255, 255, 0};
   * Color cyan = {0, 255, 255};
   * Color magenta = {255, 0, 255};
   * Color orange = {255, 128, 0};
   * \endcode
   */
  struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
  };
  ~ARGBLed() {}
  ARGBLed()
      : _color({0, 0, 0}),
        _colorsScale({{0, 0, 0},
                      {255, 0, 0},
                      {230, 0, 0},
                      {205, 0, 0},
                      {180, 0, 0},
                      {155, 0, 0},
                      {0, 5, 0},
                      {0, 5, 0},
                      {0, 5, 0},
                      {0, 5, 0},
                      {0, 5, 0}}) {}
  ARGBLed(ARGBLed const &) = delete;
  ARGBLed &operator=(ARGBLed const &) = delete;
  void clear() {
    ARGBLed::Color black = {0, 0, 0};
    setColor(black);
    update();
  }
  virtual void update() = 0;
  /**
   * Return a reference on the Color structure representing the current color
   * displayed (or next color, if LED color hasn't been updated since last
   * change of color made by setColor)
   *
   * Example:
   *
   * \code{.cpp}
   * void my_function_printing_led_color(const ARGBLed &led) {
   *   const Color &color = led.getColor();
   * #ifdef ARDUINO
   *   Serial.print("Red: ");
   *   Serial.print(led.red);
   *   Serial.print(", Green: ");
   *   Serial.print(led.green);
   *   Serial.print(", Blue: ");
   *   Serial.println(led.blue);
   * #else
   *   printf("Red: %u, Green: %u, Blue: %u\n", led.red, led.green, led.blue);
   * #endif
   * }
   * \endcode
   */
  const Color &getColor() const { return _color; }
  /**
   * Set the new color to display on the LED.
   *
   * Note: You need to call the "update" method after this one, as it doesn't
   * update automatically the new color on the LED.
   *
   * Example:
   *
   * \code{.cpp}
   * const Color red = {255, 0, 0};
   *
   * void my_function_changing_a_led_color_to_red(ARGBLed &led) {
   *   led.setColor(red); // Set the new color in the LED object
   *   led.update(); // You need to call this method if you want the new color
   * to be displayed on the LED
   * }
   * \endcode
   */
  void setColor(const Color &color) { _color = color; }
  virtual void setDisplayedEstimate(sensor::ISensor::ISensorScale value) {
    _color = _colorsScale[value];
  }
  /**
   * Change the scale of colors associated to each values of ISensorScale by
   * passing an array containing all colors as a parameter
   */
  void setColorsScale(const Color scale[]) {
    memcpy(_colorsScale, scale, sizeof(Color) * 11);
  }
  /**
   * Change the color associated to a certain value of the scale ISensorScale
   */
  void setIndividualColorScale(const Color &color,
                               sensor::ISensor::ISensorScale value) {
    _colorsScale[value] = color;
  }

 private:
  Color _color;
  Color _colorsScale[11];
};
}  // namespace display
}  // namespace athome

#endif /* !defined(DISABLE_DISPLAY) */
#endif /* ARGBLED_HPP */
