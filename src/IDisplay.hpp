#ifndef IDISPLAY_HPP
# define IDISPLAY_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_DISPLAY)
#  include "ISensor.hpp"

namespace athome {
    namespace display {
        /**
         * Interface used to control any kind of display.
         *
         * As the content displayed can change from on display to another (text, color, pixels, ...etc),
         *
         * it has a very few common methods.
         *
         * Unfortunately, embedded platforms tends to not have runtime (no `dynamic_cast` or `typeof`), so user will have to manage the use of derived interfaces / classes by himself
         */
        class IDisplay {
            public:
                //virtual ~IDisplay() = 0;
                /**
                 * Remove display content
                 */
                virtual void clear() = 0;
                /**
                 * Update display content
                 */
                virtual void update() = 0;
                /**
                 * Set the value displayed on the screen, whatever the type of display is.
                 *
                 * ISensorScale is an enumeration representing the correctness on a scale from 1 to 10 (1 is worst, 10 is best).
                 *
                 * This scale can be represented by various ways for many different displays. For example:
                 *
                 * - An LED could stay off from 6 and on below 6
                 * - A dimmable LED could set it's percentage of brightness corresponding to the value (for example 1 = 100%, 5 = 50%, 10 = 0%)
                 * - A 7 segments display could display the digit itself and stay off at 10
                 * - An LCD screen could write a sentence, the digit, draw a gauge, ...etc
                 * - A speaker or buzzer could do noise of different intensities below 6
                 * - ...etc
                 */
                virtual void setDisplayedEstimate(sensor::ISensor::ISensorScale) = 0;
        };
    }
}

# endif /* DISABLE_DISPLAY */
#endif /* IDISPLAY_HPP */
