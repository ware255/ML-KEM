#ifndef _ML_KEM_HPP_
#define _ML_KEM_HPP_

#include "poly.hpp"
#include "polymat.hpp"

class ML_KEM {
    PolyMat *ek_pke_t;
    PolyMat *ek_pke_a;
    PolyMat *dk_pke_s;
    PolyMat *c_u;
    Poly *c_v;
    Poly *k;
public:
    ML_KEM();
    ~ML_KEM();

    void keygen();
    void encapsulate();
    void decapsulate();

    PolyMat  get_t() { return *ek_pke_t; }
    PolyMat  get_a() { return *ek_pke_a; }
    PolyMat  get_s() { return *dk_pke_s; }
    PolyMat  get_u() { return *c_u; }
    Poly     get_v() { return *c_v; }
    Poly&    get_k() { return *k;   }
};

#endif // _ML_KEM_HPP_
