#ifndef STATICUTILS_HPP
# define STATICUTILS_HPP

# include <stdint.h>
# include <stddef.h>
# include <math.h>

# define LN2 0.693147180559945309417232121458

namespace athome {
    namespace utility {
        namespace math {
            template <typename T>
            constexpr T static_abs(T a) { return (a < 0) ? a * -1 : a; }

            template <typename T>
            constexpr T static_max(T a, T b) { return (a < b) ? b : a; }

            template <typename T>
            constexpr T static_min(T a, T b) { return (a < b) ? a : b; }

            template <typename T>
            constexpr T static_floor(T a) { return a - (a - static_cast<size_t>(a)); }

            template <typename T>
            constexpr T static_ceil(T a) { return (a - static_cast<size_t>(a)) ? (a + 1) - (a - static_cast<size_t>(a)) : a; }

            template <typename T>
            constexpr T static_rec_sqrt(T i, T n) {
                return (n <= 0) ? 0 : ((n == 1) ? 1 : (
                        (static_cast<size_t>(i * i) == static_cast<size_t>(n)) ? i : (
                                (i >= n) ? 0 : static_rec_sqrt(i + 1, n))));
            }

            template <typename T>
            constexpr T static_sqrt(T n) { return static_rec_sqrt(2, n); }

            template <typename T>
            constexpr T static_pow(T x, T y) { return (y == 1) ? x : x * static_pow(x, y - 1); }

            template <typename T>
            constexpr T static_m_precision(T x, T p, T m = 1) {
                return (x * static_pow(2, m) > static_pow(2, p / 2)) ? m : static_m_precision(x, p, m + 1);
            }

            /* template <typename T>
            constexpr T static_fk(T k) {
                return k;
            } */

            template <typename T>
            constexpr T static_rec_integrate(T (*f)(T x), T a, T b, int n, T step, T area, size_t i = 0) {
                return (i < n) ? static_rec_integrate(f, a, b, n, step, area + f(a + (i + 0.5) * step) * step, i + 1) : area;
            }

            template <typename T>
            constexpr T static_integrate(T (*f)(T x), T a, T b, int n = 100) {
                return static_rec_integrate<T>(f, a, b, n, (b - a) / n, 0);
            }

            /* template <typename T>
            constexpr T static_k(T k) { return static_integral(static_fk, 0, M_PI / 2); }

            template <typename T>
            constexpr T static_agm(T x, T y) { return (M_PI / 4) * ((x + y) / static_k((x - y) / (x + y))); }

            template <typename T>
            constexpr T static_ln(T x) {
                return M_PI / (2 * static_agm(1, static_pow(2, 2 - static_m_precision(x, 23)) / x)) - static_m_precision(x, 23) * LN2;
            } */
        }
    }
}

#endif /* STATICUTILS_HPP */
