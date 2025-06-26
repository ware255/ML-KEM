#ifndef _POLYMAT_HPP_
#define _POLYMAT_HPP_

#include "poly.hpp"

class PolyMat {
    Poly *poly;
    int rows;
    int cols;
private:
    void add(const PolyMat& x);
    void mul(const PolyMat& x, const PolyMat& y);
public:
    PolyMat();
    PolyMat(int r, int c);
    PolyMat(const PolyMat& x);
    PolyMat& operator=(const PolyMat& x);
    ~PolyMat();
    int row() { return rows; }
    int col() { return cols; }
    Poly& get(int r, int c);
    const Poly& get(int r, int c) const;
    void reserve(int r, int c);
    void set(int r, int c, const Poly& x);
    void transpose(PolyMat& x);
    PolyMat& operator +=(const PolyMat& x);
    friend PolyMat operator +(const PolyMat& x, const PolyMat& y);
    friend PolyMat operator *(const PolyMat& x, const PolyMat& y);
    void println();
};

#endif // _POLYMAT_HPP_
