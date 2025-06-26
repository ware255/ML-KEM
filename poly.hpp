#ifndef _POLY_HPP_
#define _POLY_HPP_

class Poly {
    int *coeff;
    int _size;
private:
    void add(const Poly& a);
    void sub(const Poly& a);
    void mul(const Poly& a, const Poly& b);
public:
    Poly();
    Poly(const Poly& x);
    ~Poly();
    Poly& operator=(const Poly& x);
    void reserve(int n);
    int get(int i) const;
    void set(int i, int x);
    int size();
    void gen(int n);
    void cbd(int eta);
    void compress(Poly& x);
    void decompress(Poly& x);
    Poly& operator +=(const Poly& x);
    Poly& operator -=(const Poly& x);
    friend Poly operator +(const Poly& x, const Poly& y);
    friend Poly operator -(const Poly& x, const Poly& y);
    friend Poly operator *(const Poly& x, const Poly& y);
    void print();
    void println();
};

#endif // _POLY_HPP_
