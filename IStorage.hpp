#ifndef ISTORAGE_HPP
# define ISTORAGE_HPP

# include <stddef.h>

namespace woodBox {
    namespace storage {
        class IStorage {
            public:
                //virtual ~IStorage() = 0;
                // Copy n bytes (3rd parameter) in dest (2nd parameter) from offset x (1st parameter)
                virtual void read(size_t, void *, size_t) = 0;
                // Write n bytes (3rd parameter) in offset x (1st parameter) from src (2nd parameter)
                virtual void write(size_t, const void *, size_t) = 0;
        };
    }
}

#endif /* ISTORAGE_HPP */
