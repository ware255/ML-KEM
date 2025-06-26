#ifndef _UTILS_HPP_
#define _UTILS_HPP_

namespace utils {
    template<typename T>
    T max(T a, T b) { return a >= b ? a : b; }

    template<typename T>
    T mod(T n, T m) {
        T rem = n % m;
        if (rem < 0) return rem + m;
        return rem;
    }

    template<typename T>
    T abs(T n) {
        T mask = n >> (sizeof(T)*8 - 1);
        return (n + mask) ^ mask;
    }

    template<typename T>
    T ceil(T x) {
        return x > static_cast<long>(x) ? static_cast<long>(x + 1.0) : static_cast<long>(x);
    }
}

#endif // _UTILS_HPP_
