#ifndef IDISPLAY_HPP
# define IDISPLAY_HPP

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
        };
    }
}

#endif /* IDISPLAY_HPP */
