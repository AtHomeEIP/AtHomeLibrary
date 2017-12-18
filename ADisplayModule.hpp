#ifndef ADISPLAYMODULE_HPP
# define ADISPLAYMODULE_HPP

# include "IModule.hpp"
# include "IDisplay.hpp"

namespace woodBox {
    namespace module {
        class ADisplayModule : public IModule {
            public:
                ADisplayModule(display::IDisplay *);
                ADisplayModule(ADisplayModule &);
                virtual ADisplayModule &operator=(ADisplayModule &);
                virtual ~ADisplayModule();
                virtual void run() = 0;
                virtual void stop() = 0;
                virtual const display::IDisplay *getDisplay();
                virtual void setDisplay(display::IDisplay *);
			protected:
				virtual void onUpdateDisplay() = 0;
            protected:
                display::IDisplay   *_display;
        };
    }
}

#endif /* ADISPLAYMODULE_HPP */
