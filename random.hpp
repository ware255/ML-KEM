#ifndef _RANDOM_HPP_
#define _RANDOM_HPP_

#include <random>
#include <cstdint>

void csprng_seed(const uint8_t key[16]);
void cspring_seed();
uint32_t csprng_get_next_uint32(void);

#endif // _RANDOM_HPP_
