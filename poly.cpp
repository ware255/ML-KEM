#include <cstdio>

#include "params.hpp"
#include "poly.hpp"
#include "utils.hpp"
#include "random.hpp"

Poly::Poly() {
    coeff = nullptr;
    _size = 0;
}

Poly::Poly(const Poly& x) {
    _size = x._size;
    coeff = new int[_size];
    for (int i = 0; i < _size; ++i)
        coeff[i] = x.coeff[i];
}

Poly::~Poly() {
    while (_size) { _size -= 1; coeff[_size] = 0; }
    delete[] coeff;
    _size = 0;
}

Poly& Poly::operator=(const Poly& x) {
    if (this == &x) return *this;

    delete[] coeff;

    _size = x._size;
    coeff = new int[_size];
    for (int i = 0; i < _size; ++i)
        coeff[i] = x.coeff[i];

    return *this;
}

void Poly::reserve(int n) {
    if (n > _size) {
        int *a = new int[n];
        for (int i = 0; i < _size; ++i) a[i] = coeff[i];
        for (int i = _size; i < n; ++i) a[i] = 0;
        delete [] coeff;
        coeff = a;
        _size = n;
    }
}

int Poly::get(int i) const {
    if (i >= _size) return 0;
    return coeff[i];
}

int* Poly::get_coeff() const {
    return coeff;
}

void Poly::set(int i, int x) {
    if (i < _size) {
        coeff[i] = x;
    }
    else if (x) {
        reserve(i+1);
        for (int j = _size; j < i; ++j) coeff[j] = 0;
        coeff[i] = x;
        _size = i+1;
    }
}

int Poly::size() {
    return _size;
}

void Poly::gen(int n) {
    reserve(n);
    cspring_seed();
    for (int i = 0; i < n-1; i++)
        set(i, csprng_get_next_uint32() % Q);
    set(n-1, (csprng_get_next_uint32() % (Q-1)) + 1);
}

void Poly::cbd(int eta) {
    reserve(N);
    cspring_seed();
    for (int i = 0; i < N; i++) {
        int a = csprng_get_next_uint32() % (eta + 1);
        int b = csprng_get_next_uint32() % (eta + 1);
        set(i, a - b);
    }
}

void Poly::compress(Poly& x) {
    int half_q = (Q / 2.0) + 0.5, u;
    for (int i = 0; i < x._size; i++) {
        int dist_center = utils::abs(half_q - x.get(i));
        int dist_bound = x.get(i) < Q - x.get(i) ? x.get(i) : Q - x.get(i);
        int mask = -(dist_center < dist_bound);
        u = mask & 1;
        set(i, u);
    }
}

void Poly::decompress(Poly& x) {
    int u;
    for (int i = 0; i < x._size; i++) {
        u = (int)utils::ceil((Q / 2.0) * x.get(i));
        set(i, u);
    }
}

void Poly::add(const Poly& a) {
    reserve(N);
    int u;
    for (int i = 0; i < N; ++i) {
        u = utils::mod(get(i) + a.get(i), Q);
        set(i, u);
    }
}

void Poly::sub(const Poly& a) {
    reserve(N);
    int u;
    for (int i = 0; i < N; ++i) {
        u = utils::mod(get(i) - a.get(i), Q);
        set(i, u);
    }
}

void Poly::karatsuba_mul(const int* a, const int* b, int n, int* out) {
    if (n <= 64) {
        for (int i = 0; i < n << 1; ++i) out[i] = 0;
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j) {
                out[i + j] += a[i] * b[j];
            }
        for (int i = 0; i < n << 1; ++i)
            out[i] = utils::mod(out[i], Q);
        return;
    }

    int m = n >> 1;
    int *a_low = new int[m](), *a_high = new int[m]();
    int *b_low = new int[m](), *b_high = new int[m]();

    for (int i = 0; i < m; ++i) {
        a_low[i] = a[i];
        a_high[i] = a[i + m];
        b_low[i] = b[i];
        b_high[i] = b[i + m];
    }

    // z0 = a_low * b_low
    int* z0 = new int[m << 1]();
    karatsuba_mul(a_low, b_low, m, z0);

    // z2 = a_high * b_high
    int* z2 = new int[m << 1]();
    karatsuba_mul(a_high, b_high, m, z2);

    // a_sum = a_low + a_high, b_sum = b_low + b_high
    int *a_sum = new int[m](), *b_sum = new int[m]();
    for (int i = 0; i < m; ++i) {
        a_sum[i] = a_low[i] + a_high[i];
        b_sum[i] = b_low[i] + b_high[i];
    }

    // z1 = (a_low + a_high) * (b_low + b_high)
    int* z1 = new int[m << 1]();
    karatsuba_mul(a_sum, b_sum, m, z1);

    // z1 = z1 - z0 - z2
    for (int i = 0; i < m << 1; ++i)
        z1[i] = z1[i] - z0[i] - z2[i];
    for (int i = 0; i < m << 1; ++i)
        z1[i] = utils::mod(z1[i], Q);

    // out = z0 + z1 * x^m + z2 * x^{2m}
    for (int i = 0; i < m << 1; ++i) {
        out[i]         += z0[i];
        out[i + m]     += z1[i];
        out[i + 2 * m] += z2[i];
    }
    for (int i = 0; i < m * 3; ++i)
        out[i] = utils::mod(out[i], Q);

    delete[] a_low;
    delete[] a_high;
    delete[] b_low;
    delete[] b_high;
    delete[] z0;
    delete[] z1;
    delete[] z2;
}

void Poly::mul(const Poly& a, const Poly& b) {
    reserve(N);
    int* a_coeff = a.get_coeff();
    int* b_coeff = b.get_coeff();
    int acc[N << 1] = {0};

    karatsuba_mul(a_coeff, b_coeff, N, acc);

    // mod X^N + 1
    for (int i = N; i < N << 1; ++i)
        acc[i - N] -= acc[i];
    for (int i = 0; i < N; ++i)
        acc[i] = utils::mod(acc[i], Q);

    for (int i = 0; i < N; ++i)
        set(i, acc[i]);
}

Poly& Poly::operator +=(const Poly& x) {
    add(x);
    return *this;
}

Poly& Poly::operator -=(const Poly& x) {
    sub(x);
    return *this;
}

Poly operator +(const Poly& x, const Poly& y) {
    Poly result(x);
    result += y;
    return result;
}

Poly operator -(const Poly& x, const Poly& y) {
    Poly result(x);
    result -= y;
    return result;
}

Poly operator *(const Poly& x, const Poly& y) {
    Poly result;
    result.mul(x, y);
    return result;
}

void Poly::print() {
    printf("[");
    for (int i = 0; i < _size; ++i) {
        if (i == _size - 1)
            printf("%i", coeff[i]);
        else
            printf("%i ", coeff[i]);
    }
    printf("]");
}

void Poly::println() {
    printf("[");
    for (int i = 0; i < _size; ++i){
        if (i == _size - 1)
            printf("%i", coeff[i]);
        else
            printf("%i,", coeff[i]);
    }
    printf("]\n\n");
}
