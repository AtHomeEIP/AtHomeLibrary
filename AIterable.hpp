#ifndef AITERABLE_HPP
# define AITERABLE_HPP

namespace woodBox {
    namespace utility {
        template <typename T>
        class AIterable {
        public:
            AIterable(const AIterable &other) = delete;
            AIterable &operator=(const AIterable &other) = delete;
            ~AIterable() {
                if (_data != nullptr)
                    delete _data;
            }
            T *get() { return _data; }
            const T *get() const { return _data; }
            void set(T *data) { _data = data; }
            virtual AIterable *next() = 0;
            virtual const AIterable *next() const = 0;
        protected:
            AIterable(T *data = nullptr):_data(data) {}
        private:
            T   *_data;
        };
    }
}

#endif /* AITERABLE_HPP */
