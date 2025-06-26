/*
 * https://gist.github.com/Emill/d8e8df7269f75b9485a2
 */
#include "random.hpp"

// This is ASCII of "expand 16-byte k"
static const uint32_t chacha_constants[4] = {0x61707865, 0x3120646e, 0x79622d36, 0x6b206574};

static const uint8_t chacha_order[8][4] = {
    {0, 4, 8, 12},
    {1, 5, 9, 13},
    {2, 6, 10, 14},
    {3, 7, 11, 15},
    {0, 5, 10, 15},
    {1, 6, 11, 12},
    {2, 7, 8, 13},
    {3, 4, 9, 14}
};

static uint32_t chacha_key[4];
static uint64_t chacha_counter;
static uint32_t chacha_random_output[16];
static uint8_t chacha_random_output_left;

static void chacha_quarterround(const uint8_t indices[4]) {
    uint32_t* s = chacha_random_output;
    int a = indices[0];
    int b = indices[1];
    int c = indices[2];
    int d = indices[3];
    s[a] += s[b]; s[d] ^= s[a]; s[d] = (s[d] << 16) | (s[d] >> 16);
    s[c] += s[d]; s[b] ^= s[c]; s[b] = (s[b] << 12) | (s[b] >> 20);
    s[a] += s[b]; s[d] ^= s[a]; s[d] = (s[d] << 8) | (s[d] >> 24);
    s[c] += s[d]; s[b] ^= s[c]; s[b] = (s[b] << 7) | (s[b] >> 25);
}

static void chacha_run(void) {
    uint32_t state[16], i;
    for (i = 0; i < 4; ++i)
        state[i] = chacha_constants[i];
    for (i = 4; i < 8; ++i)
        state[i] = chacha_key[i-4];
    for (i = 8; i < 12; ++i)
        state[i] = chacha_key[i-8];
    for (i = 12; i < 14; ++i)
        state[i] = 0;
    ++chacha_counter;
    for (i = 14; i < 16; ++i)
        state[i] = chacha_counter;

    for (i = 0; i < 16; ++i)
        chacha_random_output[i] = state[i];
    
    for (i = 0; i < 10; ++i)
        for (int j = 0; j < 8; ++j)
            chacha_quarterround(chacha_order[j]);

    for (i = 0; i < 16; ++i)
        chacha_random_output[i] += state[i];
}

// Public functions

void csprng_seed(const uint8_t key[16]) {
    for (int i = 0; i < 4; ++i)
        chacha_key[i] = ((uint32_t*)key)[i];
    chacha_counter = 0;
    chacha_random_output_left = 0;
}

void cspring_seed() {
    std::random_device rnd;
    for (int i = 0; i < 4; ++i)
        chacha_key[i] = rnd();
    chacha_counter = 0;
    chacha_random_output_left = 0;
}

uint32_t csprng_get_next_uint32(void) {
    if (chacha_random_output_left == 0) {
        chacha_run();
        chacha_random_output_left = 16;
    }
    return chacha_random_output[--chacha_random_output_left];
}
