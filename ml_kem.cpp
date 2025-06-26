#include "params.hpp"
#include "k_pke.hpp"
#include "ml_kem.hpp"
#include "random.hpp"

ML_KEM::ML_KEM() {
    ek_pke_t = new PolyMat(K, 1);
    ek_pke_a = new PolyMat(K, K);
    dk_pke_s = new PolyMat(K, 1);
    c_u      = new PolyMat(K, 1);

    c_v = new Poly();
    k   = new Poly();

    for (int i = 0; i < K; i++)
        for (int j = 0; j < K; j++)
            ek_pke_a->set(i, j, Poly());

    for (int i = 0; i < K; i++) {
        dk_pke_s->set(i, 0, Poly());
        ek_pke_t->set(i, 0, Poly());
        c_u->set(i, 0, Poly());
    }
}

ML_KEM::~ML_KEM() {
    delete ek_pke_t;
    delete ek_pke_a;
    delete dk_pke_s;
    delete c_u;
    delete c_v;
    delete k;
}

void ML_KEM::keygen() {
    K_PKE *k_pke = new K_PKE();
    k_pke->keygen();

    *ek_pke_a = k_pke->get_a();
    *ek_pke_t = k_pke->get_t();
    *dk_pke_s = k_pke->get_s();

    delete k_pke;
}

void ML_KEM::encapsulate() {
    K_PKE *k_pke = new K_PKE();

    k->reserve(N);

    cspring_seed();
    for (int i = 0; i < k->size(); i++)
        k->set(i, csprng_get_next_uint32() % 2);

    k_pke->set_a(*ek_pke_a);
    k_pke->set_t(*ek_pke_t);

    k_pke->encrypt(k);
    *c_u = k_pke->get_u();
    *c_v = k_pke->get_v();

    delete k_pke;
}

void ML_KEM::decapsulate() {
    K_PKE *k_pke = new K_PKE();

    k_pke->set_s(*dk_pke_s);
    k_pke->set_u(*c_u);
    k_pke->set_v(*c_v);

    Poly *k_temp = new Poly();
    k_pke->decrypt(k_temp);

    *k = *k_temp;

    delete k_temp;
    delete k_pke;
}
