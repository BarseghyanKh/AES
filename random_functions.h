#ifndef RANDOM_FUNCTIONS_H
#define RANDOM_FUNCTIONS_H

#include "headers.h"
#include "byte.h"

struct generate_random_byte {

	my_cryptography_lib::byte operator()() {
        static std::mt19937 randomEngine(std::random_device{}());
        static std::uniform_int_distribution<std::uint16_t> distribution(0, 255);
        return static_cast<std::uint8_t>(distribution(randomEngine));
    }
};



#endif // RANDOM_FUNCTIONS_H
