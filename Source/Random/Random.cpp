#include "PrecompiledHeader.h"

#include "Random.h"

#define W 32
#define R 32
#define M1 3
#define M2 24
#define M3 10

#define MAT0POS(t, v) (v ^ (v >> t))
#define MAT0NEG(t, v) (v ^ (v << (-(t))))
#define Identity(v) (v)

#define V0 state[state_i]
#define VM1 state[(state_i + M1) & 0x0000001fU]
#define VM2 state[(state_i + M2) & 0x0000001fU]
#define VM3 state[(state_i + M3) & 0x0000001fU]
#define VRm1 state[(state_i + 31) & 0x0000001fU]
#define newV0 state[(state_i + 31) & 0x0000001fU]
#define newV1 state[state_i]

#define FACT 2.32830643653869628906e-10

namespace tg
{
namespace
{

thread_local std::unique_ptr<unsigned int[]> state;

}

void SrandWELL1024a()
{
    if (state != nullptr)
    {
        return;
    }
    
    state = std::make_unique<unsigned int[]>(R);

    srand(static_cast<unsigned>(std::time(nullptr)));
    for (int i = 0; i < R; ++i)
    {
        state[i] = std::rand();
    }
}

double WELL1024a() noexcept
{
    thread_local unsigned int state_i;
    thread_local unsigned int z0, z1, z2;

    z0 = VRm1;
    z1 = Identity(V0) ^ MAT0POS(8, VM1);
    z2 = MAT0NEG(-19, VM2) ^ MAT0NEG(-14, VM3);
    newV1 = z1 ^ z2;
    newV0 = MAT0NEG(-11, z0) ^ MAT0NEG(-7, z1) ^ MAT0NEG(-13, z2);
    state_i = (state_i + 31) & 0x0000001fU;

    return static_cast<double>(state[state_i]) * FACT;
}

Random::Random()
{
    SrandWELL1024a();
}

int32_t Random::Next() noexcept
{
    return Next(0, INT32_MAX);
}

int32_t Random::Next(int32_t maxValue) noexcept
{
    return Next(0, maxValue);
}

int32_t Random::Next(int32_t minValue, int32_t maxValue) noexcept
{
    return minValue + int32_t(Sample() * (static_cast<double>(maxValue) - minValue));
}

void Random::NextBytes(std::byte* bytes, int32_t count) noexcept
{
    for (auto i = 0; i < count; ++i)
    {
        bytes[i] = std::byte(Next(0, 255));
    }
}

void Random::NextBytes(const std::span<std::byte>& bytes) noexcept
{
    return NextBytes(bytes.data(), bytes.size());
}

double Random::NextDouble() noexcept
{
    return Sample();
}

double Random::NextDouble(double minValue, double maxValue) noexcept
{
    return minValue + (this->Sample() * (maxValue - minValue));
}

double Random::Sample() noexcept
{
    return WELL1024a();
}

}
