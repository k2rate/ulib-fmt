#pragma once

#include <ulib/format.h>
#include <array>
#include <ulib/fmt/detail/escape.h>

template <class T, size_t Size>
struct fmt::formatter<std::array<T, Size>>
{
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

    template <typename FormatContext>
    auto format(const std::array<T, Size> &p, FormatContext &ctx)
    {
        ulib::u8string result;
        result += u8"[";
        if (p.empty())
        {
            result += u8"]";
        }
        else
        {
            for (auto it = p.begin(); it != p.end(); it++)
            {
                const char *ending = ", ";
                if (it + 1 == p.end())
                    ending = "]";

                if constexpr (ulib::is_string_v<T>)
                {
                    result += ulib::u8(fmt::format("\"{}\"{}", ulib::fmt::detail::EscapeString(ulib::u8(*it)), ending));
                }
                else
                {
                    result += ulib::u8(fmt::format("{}{}", *it, ending));
                }
            }
        }

        fmt::formatter<fmt::string_view> ft;
        // ft.format(const basic_string_view<char> &val, FormatContext &ctx)
        fmt::string_view name((char *)result.raw_data(), result.size());
        return ft.format(name, ctx);
    }
};