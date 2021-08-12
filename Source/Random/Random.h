#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace tg
{

class Random
{
/**@section Constructor */
public:
    Random();
    Random(const Random& rhs) = delete;
    Random(Random&& rhs) = default;

/**@section Destructor */
public:
    virtual ~Random() = default;

/**@section Operator */
public:
    Random& operator=(const Random& rhs) = delete;
    Random& operator=(Random&& rhs) = default;

/**@section Method */
public:
    [[nodiscard]] int32_t Next() noexcept;
    [[nodiscard]] int32_t Next(int32_t maxValue) noexcept;
    [[nodiscard]] int32_t Next(int32_t minValue, int32_t maxValue) noexcept;
    void NextBytes(std::byte* bytes, int32_t count) noexcept;
    void NextBytes(const std::span<std::byte>& bytes) noexcept;
    [[nodiscard]] double NextDouble() noexcept;
    [[nodiscard]] double NextDouble(double minValue, double maxValue) noexcept;

protected:
    [[nodiscard]] virtual double Sample() noexcept;
};

}
