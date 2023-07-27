#pragma once

#include <ulib/format.h>
#include <list>
#include <ulib/fmt/detail/escape.h>

template <class T>
struct fmt::formatter<std::list<T>>
{
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

    template <typename FormatContext>
    auto format(const std::list<T> &p, FormatContext &ctx)
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
                auto cop = it;
                cop++;

                const char *ending = ", ";
                if (cop == p.end())
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
        fmt::string_view name((char *)result.raw_data(), result.size());
        return ft.format(name, ctx);
    }
};