#include <iostream>
#include <chrono>
#include <random>

#include "k_pke.hpp"
#include "poly.hpp"
#include "params.hpp"

void gen_random_message(Poly& msg, int n) {
    msg.reserve(n);
    std::random_device rnd;
    for (int i = 0; i < n - 1; ++i) {
        msg.set(i, rnd() % 2); // 0 or 1
    }
    msg.set(n - 1, 1);
}

int main() {
    K_PKE k_pke;
    Poly msg, msg_decrypted;

    gen_random_message(msg, N);

    const int REPEAT = 1000;

    // --- 鍵生成のベンチマーク ---
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < REPEAT; ++i) {
        k_pke.keygen();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "keygen():     " << elapsed.count() << " seconds for " << REPEAT << " runs\n";

    // --- 暗号化のベンチマーク ---
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < REPEAT; ++i) {
        gen_random_message(msg, N);  // 毎回違うメッセージを暗号化
        k_pke.encrypt(&msg);
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "encrypt():    " << elapsed.count() << " seconds for " << REPEAT << " runs\n";

    // --- 復号のベンチマーク ---
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < REPEAT; ++i) {
        k_pke.decrypt(&msg_decrypted);
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "decrypt():    " << elapsed.count() << " seconds for " << REPEAT << " runs\n";

    return 0;
}
