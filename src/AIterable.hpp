#ifndef AITERABLE_HPP
# define AITERABLE_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_ITERABLE)

namespace athome {
    namespace utility {
        /**
         * Class created for internal use of this library, but not used right now. Could be removed if considered useless.
         */
        template <typename T>
        class AIterable {
        public:
            AIterable(const AIterable &other) = delete;
            AIterable &operator=(const AIterable &other) = delete;
            ~AIterable() {}
            T &get() { return _data; }
            const T &get() const { return _data; }
            void set(T &data) { _data = data; }
            virtual AIterable *next() = 0;
            virtual const AIterable *next() const = 0;
        protected:
            AIterable(T &data):_data(data) {}
        private:
            T   &_data;
        };
    }
}

# endif /* DISABLE_ITERABLE */
#endif /* AITERABLE_HPP */
