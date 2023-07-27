#pragma once

#include <ulib/format.h>
#include <utility>
#include <ulib/fmt/detail/escape.h>

template <class K, class T>
struct fmt::formatter<std::pair<K, T>>
{
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

    template <typename FormatContext>
    auto format(const std::pair<K, T> &p, FormatContext &ctx)
    {
        ulib::u8string result;
        if constexpr (ulib::is_string_v<K>)
        {
            result += ulib::u8(fmt::format("[\"{}\", ", ulib::fmt::detail::EscapeString(ulib::u8(p.first))));
        }
        else
        {
            result += ulib::u8(fmt::format("[{}, ", p.first));
        }

        if constexpr (ulib::is_string_v<T>)
        {
            result += ulib::u8(fmt::format("\"{}\"]", ulib::fmt::detail::EscapeString(ulib::u8(p.second))));
        }
        else
        {
            result += ulib::u8(fmt::format("{}]", p.second));
        }


        fmt::formatter<fmt::string_view> ft;
        fmt::string_view name((char *)result.raw_data(), result.size());
        return ft.format(name, ctx);
    }
};