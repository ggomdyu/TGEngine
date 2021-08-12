#pragma once

#include <mutex>

namespace tg
{

class Debug
{
/**@section Constructor */
public:
    Debug() = delete;

/**@section Method */
public:
    static void Assert(bool condition);
    static void Assert(bool condition, const std::u8string_view& message);
    static void Assert(bool condition, const std::u8string_view& message, const std::u8string_view& detailMessage);
    [[noreturn]] static void Fail(const std::u8string_view& message);
    [[noreturn]] static void Fail(const std::u8string_view& message, const std::u8string_view& detailMessage);
    static void Write(const std::u8string_view& message);
    static void WriteLine(const std::u8string_view& message);
    static void SetIndentLevel(int32_t indentLevel) noexcept;
    [[nodiscard]] static int32_t GetIndentLevel() noexcept;
    static void Indent();
    static void Unindent();

/**@section Variable */
private:
    inline static int32_t m_indentLevel = 0;
    inline static std::recursive_mutex m_mutex;
};

}
