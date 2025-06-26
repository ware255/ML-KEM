#ifndef _UTILS_HPP_
#define _UTILS_HPP_

struct Barrett {
    int MOD;
    unsigned long long m;

    Barrett(int MOD_) : MOD(MOD_) {
        m = (1ULL << 32) / MOD;
    }

    int reduce(int x) const {
        unsigned long long q = (x * m) >> 32;
        x -= q * MOD;
        return x < MOD ? x : x - MOD;
    }
};

namespace utils {
    template<typename T>
    T max(T a, T b) { return a >= b ? a : b; }

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
