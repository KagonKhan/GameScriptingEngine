#pragma once
#include <algorithm>
#include <chrono>
#include <random>

static void clamped(int& value, const int low, const int high) { value = std::clamp(value, low, high); }

// TODO: assert alignment works
struct Interval {
    int minutes = {0};
    int seconds = {0};
    int milli   = {0};
    int micro   = {0};


    [[nodiscard]] std::chrono::microseconds asMicroseconds() const {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::minutes{minutes}) +
               std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::seconds{seconds}) +
               std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::milliseconds{milli}) +
               std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::microseconds{micro});
    }
};


class Random {
public:
    static void     Init() { s_RandomEngine.seed(std::random_device()()); }
    static uint32_t UInt() { return s_Distribution(s_RandomEngine); }
    static uint32_t UInt(const uint32_t min, const uint32_t max) {
        return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
    }

    static float UnitFloat() {
        return static_cast<float>(s_Distribution(s_RandomEngine)) /
               static_cast<float>(std::numeric_limits<uint32_t>::max());
    }

    static double Double() { return s_Distribution(s_RandomEngine) / std::numeric_limits<uint32_t>::max(); }


    // static glm::vec3 Vec3() { return glm::vec3(Float(), Float(), Float()); }
    // static glm::vec3 Vec3(float min, float max) {
    //     return glm::vec3(Float() * (max - min) + min, Float() * (max - min) + min, Float() * (max - min) + min);
    // }


    static uint32_t PCG_Hash(const uint32_t input) {
        uint32_t state = input * 747796405u + 2891336453u;
        uint32_t word  = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
        return (word >> 22u) ^ word;
    }

    static float RandomFloatPCG(uint32_t& seed) {
        seed = PCG_Hash(seed);
        return static_cast<float>(seed) / static_cast<float>(std::numeric_limits<uint32_t>::max());
    }

private:
    inline static thread_local std::mt19937                                             s_RandomEngine;
    inline static thread_local std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

namespace Utils {
using timepoint = std::chrono::steady_clock::time_point;

inline timepoint now() { return std::chrono::steady_clock::now(); }

inline long long duration_ms(timepoint const& t1, timepoint const& t2) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
}
inline long long duration_us(timepoint const& t1, timepoint const& t2) {
    return std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
}
} // namespace Utils