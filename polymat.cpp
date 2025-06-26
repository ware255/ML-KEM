#include <cstdio>

#include "polymat.hpp"

PolyMat::PolyMat() {
    poly = nullptr;
    rows = 0;
    cols = 0;
}

PolyMat::PolyMat(int r, int c) {
    rows = r;
    cols = c;
    poly = new Poly[r * c];
}

PolyMat::PolyMat(const PolyMat& x) {
    rows = x.rows;
    cols = x.cols;

    poly = new Poly[rows * cols];
    for (int i = 0; i < rows * cols; ++i)
        poly[i] = x.poly[i];
}

PolyMat& PolyMat::operator=(const PolyMat& x) {
    if (this == &x) return *this;

    delete[] poly;

    rows = x.rows;
    cols = x.cols;
    poly = new Poly[rows * cols];
    for (int i = 0; i < rows * cols; ++i)
        poly[i] = x.poly[i];

    return *this;
}

PolyMat::~PolyMat() {
    delete[] poly;
}

Poly& PolyMat::get(int r, int c) {
    return poly[r * cols + c];
}

const Poly& PolyMat::get(int r, int c) const {
    return poly[r * cols + c];
}

void PolyMat::reserve(int r, int c) {
    if (r < rows && c < cols) return;
    Poly* a = new Poly[r*c];
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            if (i < rows && j < cols)
                a[i * c + j] = poly[i * cols + j];
            else
                a[i * c + j] = Poly();
        }
    }

    delete[] poly;

    poly = a;
    rows = r;
    cols = c;
}

void PolyMat::set(int r, int c, const Poly& x) {
    if (r < rows && c < cols) {
        poly[r * cols + c] = x;
    }
    else {
        reserve(r+1, c+1);
        poly[r * cols + c] = x;
        rows = r+1;
        cols = c+1;
    }
}

void PolyMat::transpose(PolyMat& x) {
    if (this == &x) {
        PolyMat tmp = x;
        transpose(tmp);
        return;
    }
    reserve(x.cols, x.rows);
    for (int i = 0; i < x.rows; i++)
        for (int j = 0; j < x.cols; j++)
            set(j, i, x.get(i, j));
}

void PolyMat::add(const PolyMat& x) {
    if (x.rows > rows || x.cols > cols)
        reserve(x.rows, x.cols);

    for (int i = 0; i < x.rows; ++i)
        for (int j = 0; j < x.cols; ++j)
            poly[i * cols + j] += x.get(i,j);
}

void PolyMat::mul(const PolyMat& x, const PolyMat& y) {
    if (x.cols != y.rows) return;

    reserve(x.rows, y.cols);

    for (int i = 0; i < x.rows; ++i) {
        for (int j = 0; j < y.cols; ++j) {
            Poly sum, temp;

            for (int k = 0; k < x.cols; ++k) {
                temp = x.get(i,k) * y.get(k,j);
                sum += temp;
            }

            poly[i * cols + j] = sum;
        }
    }
}

PolyMat& PolyMat::operator +=(const PolyMat& x) {
    add(x);
    return *this;
}

PolyMat operator +(const PolyMat& x, const PolyMat& y) {
    PolyMat result(x);
    result += y;
    return result;
}

PolyMat operator *(const PolyMat& x, const PolyMat& y) {
    PolyMat result;
    result.mul(x, y);
    return result;
}

void PolyMat::println() {
    printf("[");
    for (int i = 0; i < rows; ++i) {
        printf("[");
        for (int j = 0; j < cols; ++j) {
            poly[i * cols + j].print();
            if (j < cols - 1) {
                printf(",");
            }
        }
        printf("]");
        if (i < rows - 1) {
            printf(",");
        }
    }
    printf("]\n\n");
}
