#include <cstdio>

#include "ml_kem.hpp"

int main() {
    ML_KEM *ml_kem = new ML_KEM();

    // Bob generates public/private keys
    ml_kem->keygen();

    printf("EK_PKE_T (Public key part): ");
    ml_kem->get_t().println();

    printf("EK_PKE_A (Public key part): ");
    ml_kem->get_a().println();

    printf("DK_PKE_S (Secret key, kept by Bob): ");
    ml_kem->get_s().println();

    // Alice uses public key to generate (K, C_u, C_v)
    ml_kem->encapsulate();

    printf("K (Alice's shared key): ");
    ml_kem->get_k().compress(ml_kem->get_k());
    ml_kem->get_k().println();

    printf("C_U (Ciphertext part): ");
    ml_kem->get_u().println();

    printf("C_V (Ciphertext part): ");
    ml_kem->get_v().println();

    // Bob decrypts C_u, C_v to obtain K
    ml_kem->decapsulate();

    printf("K (Bob's recovered key): ");
    ml_kem->get_k().println();

    delete ml_kem;

    return 0;
}
