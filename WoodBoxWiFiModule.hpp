#ifndef WOODBOXWIFIMODULE_HPP
# define WOODBOXWIFIMODULE_HPP

# include "WoodBoxModule.hpp"

namespace woodBox {
    namespace module {
        template <typename T, size_t n>
        class WoodBoxWiFiModule : public WoodBoxModule<T, n> {
        friend class WoodBoxModule<T, n>;
        public:
            WoodBoxWiFiModule(const WoodBoxWiFiModule &) = delete;
            WoodBoxWiFiModule &operator=(const WoodBoxWiFiModule &) = delete;
            ~WoodBoxWiFiModule() {}
        protected:
            WoodBoxWiFiModule():
                WoodBoxModule<T, n>() {}
        };
    }
}

#endif /* WOODBOXWIFIMODULE_HPP */
