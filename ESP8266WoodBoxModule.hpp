#ifndef ESP8266WOODBOXMODULE_HPP
# define ESP8266WOODBOXMODULE_HPP

# include "WoodBoxModule.hpp"

namespace woodBox {
    namespace module {
        template <typename T, size_t n>
        class ESP8266WoodBoxModule : public WoodBoxModule<T, n> {
        friend class WoodBoxModule<T, n>;
        public:
            ESP8266WoodBoxModule(const ESP8266WoodBoxModule &) = delete;
            ESP8266WoodBoxModule &operator=(const ESP8266WoodBoxModule &) = delete;
            ~ESP8266WoodBoxModule() {}
        protected:
            ESP8266WoodBoxModule():
                WoodBoxModule<T, n>() {}
        };
    }
}

#endif /* ESP8266WOODBOXMODULE_HPP */
