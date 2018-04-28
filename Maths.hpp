#ifndef STATICUTILS_HPP
# define STATICUTILS_HPP

# include <stdint.h>
# include <stddef.h>
# include <math.h>

# define LN2 0.693147180559945309417232121458

namespace athome {
    namespace utility {
        namespace math {
            /**
             * General example:
             *
             * \code{.cpp}
             * template <typename T>
             * constexpr T square(T x) {
             *   return x * x;
             * }
             *
             * int main(void) {
             *   constexpr float a = athome::utility::math::static_integrate<float>(square<float>, 0, 5);
             *   constexpr float b = athome::utility::math::static_floor<float>(42.3);
             *   constexpr float c = athome::utility::math::static_ceil<float>(3.14);
             *   constexpr float d = athome::utility::math::static_sqrt<float>(4.0);
             *   constexpr float e = athome::utility::math::static_pow<float>(2, 8);
             *   std::cout << "integrate: " << a << std::endl;
             *   std::cout << "floor: " << b << std::endl;
             *   std::cout << "ceil: " << c << std::endl;
             *   std::cout << "sqrt: " << d << std::endl;
             *   std::cout << "pow: " << e << std::endl;
             *   return 0;
             * }
             */
            /**
             * Return the absolute value of the parameter
             */
            template <typename T>
            constexpr T static_abs(T a) { return (a < 0) ? a * -1 : a; }

            /**
             * Return the biggest value between the two parameters
             */
            template <typename T>
            constexpr T static_max(T a, T b) { return (a < b) ? b : a; }

            /**
             * Return the smallest value between the two parameters
             */
            template <typename T>
            constexpr T static_min(T a, T b) { return (a < b) ? a : b; }

            /**
             * Return the biggest integer equal or below to the passed value
             */
            template <typename T>
            constexpr T static_floor(T a) { return a - (a - static_cast<int32_t>(a)); }

            /**
             * Return the smallest integer equal or above to the passed value
             */
            template <typename T>
            constexpr T static_ceil(T a) { return (a - static_cast<int32_t>(a)) ? (a + 1) - (a - static_cast<int32_t>(a)) : a; }

            template <typename T>
            static constexpr T static_rec_sqrt(T ix, double x0) {
                return (static_abs<T>(2 * x0) < M_E) ? (x0 * x0) - ix : (
                        (static_abs<T>((x0 - ((x0 * x0 - ix) / (2 * x0))) - x0) <=
                                (0.00000001 * static_abs<T>((x0 - ((x0 * x0 - ix) / (2 * x0)))))) ?
                                        (x0 - ((x0 * x0 - ix) / (2 * x0))) :
                                        static_rec_sqrt(ix, (x0 - ((x0 * x0 - ix) / (2 * x0)))));
            }

            /**
             * Return the square root of the passed value as parameter
             */
            template <typename T>
            constexpr T static_sqrt(T x) {
                return static_rec_sqrt<T>(x, 10);
            }

            /**
             * Return the power of the x value (1st parameter) by the y value (2nd parameter)
             */
            template <typename T>
            constexpr T static_pow(T x, T y) { return (y < 1) ? NAN : ((static_cast<int32_t>(y) == 1) ? x : x * static_pow<T>(x, y - 1)); }

            template <typename T>
            T pow(T x, T y) {
                if (y == 0) {
                    return 1;
                }
                T res = x;
                for (size_t i = 1; i < y; i++) {
                    res *= x;
                }
                return res;
            }

            /**
             * Return the value of e^x
             */
            template <typename T>
            constexpr T static_exp(T x) { return static_pow<T>(M_E, x); }

            /**
             * Return the value of 2^x
             */
            template <typename T>
            constexpr T static_exp2(T x) { return static_pow<T>(2, x); }

            template <typename T>
            static constexpr T static_rec_integrate(T (*f)(T x), T a, T b, int n, T step, T area, size_t i = 0) {
                return (i < n) ? static_rec_integrate(f, a, b, n, step, area + f(a + (i + 0.5) * step) * step, i + 1) : area;
            }

            /**
             * Integrate the function passed as first parameter from the range a to b.
             *
             * The optional 4th parameter allows to set the number of subdivisions used to integrate the function,
             * thus directly affects the precision of the result
             */
            template <typename T>
            constexpr T static_integrate(T (*f)(T x), T a, T b, int n = 100) {
                return static_rec_integrate<T>(f, a, b, n, (b - a) / n, 0);
            }

            template <typename T>
            constexpr T static_log(T z, size_t iterations = 150, T n = 0) {
                return (z > 0 && n < iterations) ? (
                        (n == 0) ? 2 * (((1 / (2 * n + 1)) * static_pow<T>((z - 1) / (z + 1), 2 * n + 1) + static_log(z, iterations, n + 1))) :
                                ((1 / (2 * n + 1)) * static_pow<T>((z - 1) / (z + 1), 2 * n + 1) + static_log(z, iterations, n + 1))) : 0;
            }

            template <typename T>
            T log(T z) {
                T res = 0;
                for (T n = 0; n < 150; n++) {
                    res += (1 / (2 * n + 1)) * athome::utility::math::pow<T>((z - 1) / (z + 1), 2 * n + 1);
                }
                return res * 2;
            }

            /* template <typename T>
            static constexpr T static_m_precision(T x, T p, T m = 1) {
                return (x * static_pow(2, m) > static_pow<T>(2, p / 2)) ? m : static_m_precision<T>(x, p, m + 1);
            }

            template <typename T>
            constexpr T static_fk(T k) {
                return k;
            }

            template <typename T>
            constexpr T static_k(T k) { return static_integral(static_fk, 0, M_PI / 2); }

            template <typename T>
            constexpr T static_agm(T x, T y) { return (M_PI / 4) * ((x + y) / static_k((x - y) / (x + y))); }

            // Was supposed to use arithmetic geometric approximation
            template <typename T>
            constexpr T static_ln(T x) {
                return M_PI / (2 * static_agm(1, static_pow(2, 2 - static_m_precision(x, 23)) / x)) - static_m_precision(x, 23) * LN2;
            } */
        }
    }
}

#endif /* STATICUTILS_HPP */
