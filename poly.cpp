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

void Poly::mul(const Poly& a, const Poly& b) {
    reserve(N);
    int acc[N] = {0};

    int i, j, k, t;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            k = i - j;
            if (k >= 0) {
                t = a.get(k) * b.get(j);
                acc[i] += t;
            }
            else {
                t = a.get(N + k) * b.get(j);
                acc[i] -= t;
            }
        }
    }

    for (int i = 0; i < N; ++i)
        set(i, utils::mod(acc[i], Q));
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
