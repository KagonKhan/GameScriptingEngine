#pragma once
#include <cstdint>
#include <random>


class Random {
public:
    static void     Init();
    static uint32_t UInt() { return s_Distribution(s_RandomEngine); }
    static uint32_t UInt(const uint32_t min, const uint32_t max);

    static float UnitFloat();
    static double Double();



    static uint32_t PCG_Hash(const uint32_t input);
    static float    RandomFloatPCG(uint32_t& seed);

private:
    inline static thread_local std::mt19937 s_RandomEngine{std::random_device()()};
    inline static thread_local std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};
