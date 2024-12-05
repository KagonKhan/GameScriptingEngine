#include "Random.hpp"

#include <cstdint>
#include <limits>
#include <random>


uint32_t Random::UInt(const uint32_t min, const uint32_t max) {
    return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
}

float Random::UnitFloat() {
    return static_cast<float>(s_Distribution(s_RandomEngine)) /
           static_cast<float>(std::numeric_limits<uint32_t>::max());
}

double Random::Double() { return s_Distribution(s_RandomEngine) / std::numeric_limits<uint32_t>::max(); }

uint32_t Random::PCG_Hash(const uint32_t input) {
    uint32_t state = input * 747796405u + 2891336453u;
    uint32_t word  = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

float Random::RandomFloatPCG(uint32_t& seed) {
    seed = PCG_Hash(seed);
    return static_cast<float>(seed) / static_cast<float>(std::numeric_limits<uint32_t>::max());
}

// static glm::vec3 Vec3() { return glm::vec3(Float(), Float(), Float()); }
// static glm::vec3 Vec3(float min, float max) {
//     return glm::vec3(Float() * (max - min) + min, Float() * (max - min) + min, Float() * (max - min) + min);
// }
