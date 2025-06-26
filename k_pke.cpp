#include "params.hpp"
#include "k_pke.hpp"

K_PKE::K_PKE() {
    a  = new PolyMat(K, K);
    s  = new PolyMat(K, 1);
    e  = new PolyMat(K, 1);
    t  = new PolyMat(K, 1);
    r  = new PolyMat(K, 1);
    e1 = new PolyMat(K, 1);
    u  = new PolyMat(K, 1);
    e2 = new Poly();
    v  = new Poly();
}

K_PKE::~K_PKE() {
    delete a;
    delete s;
    delete e;
    delete t;
    delete r;
    delete e1;
    delete u;
    delete e2;
    delete v;
}

void K_PKE::keygen() {
    for (int i = 0; i < a->row(); i++)
        for (int j = 0; j < a->col(); j++)
            a->get(i, j).gen(N);

    for (int i = 0; i < s->row(); i++)
        for (int j = 0; j < s->col(); j++)
            s->get(i, j).cbd(N1);

    for (int i = 0; i < e->row(); i++)
        for (int j = 0; j < e->col(); j++)
            e->get(i, j).cbd(N1);

    *t = *a * *s;
    *t += *e;
}

void K_PKE::encrypt(Poly *msg) {
    PolyMat *At  = new PolyMat(K, K);
    PolyMat *tt  = new PolyMat(1, K);
    PolyMat *mul = new PolyMat(1, 1);

    At->transpose(*a);
    tt->transpose(*t);

    for (int i = 0; i < r->row(); ++i)
        for (int j = 0; j < r->col(); ++j)
            r->get(i, j).cbd(N1);

    for (int i = 0; i < e1->row(); ++i)
        for (int j = 0; j < e1->col(); ++j)
            e1->get(i, j).cbd(N2);

    e2->cbd(N2);

    *u = *At * *r;
    *u += *e1;

    *mul = *tt * *r;
    *v = mul->get(0, 0) + *e2;
    msg->decompress(*msg);

    *v += *msg;

    delete At;
    delete tt;
    delete mul;
}

void K_PKE::decrypt(Poly *msg) {
    PolyMat *st  = new PolyMat(1, K);
    PolyMat *mul = new PolyMat(1, 1);

    st->transpose(*s);
    *mul = *st * *u;
    *msg = mul->get(0, 0);

    *msg = *v - *msg;
    msg->compress(*msg);

    delete st;
    delete mul;
}
