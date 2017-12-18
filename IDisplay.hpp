#ifndef IDISPLAY_HPP
# define IDISPLAY_HPP

namespace woodBox {
    namespace display {
        class IDisplay {
            public:
                virtual ~IDisplay() = 0;
                virtual void clear() = 0; // Remove display content
                virtual void update() = 0; // Update display content
        };
    }
}

#endif /* IDISPLAY_HPP */
