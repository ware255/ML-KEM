#ifndef _K_PKE_HPP_
#define _K_PKE_HPP_

#include "poly.hpp"
#include "polymat.hpp"

class K_PKE {
    PolyMat *a;
    PolyMat *s;
    PolyMat *e;
    PolyMat *t;
    PolyMat *r;
    PolyMat *e1;
    PolyMat *u;
    Poly *e2;
    Poly *v;
public:
    K_PKE();
    ~K_PKE();

    void keygen();
    void encrypt(Poly *msg);
    void decrypt(Poly *msg);

    PolyMat& get_a() { return *a; }
    PolyMat& get_t() { return *t; }
    PolyMat& get_s() { return *s; }
    PolyMat  get_u() { return *u; }
    Poly     get_v() { return *v; }

    void set_a(PolyMat& x) { *a = x; }
    void set_t(PolyMat& x) { *t = x; }
    void set_s(PolyMat& x) { *s = x; }
    void set_u(PolyMat& x) { *u = x; }
    void set_v(Poly& x)    { *v = x; }
};

#endif // _K_PKE_HPP_
