#pragma once

#include <ulib/format.h>
#include <ulib/fmt/std/pair.h>
#include <ulib/fmt/detail/escape.h>

#include <map>

template <class K, class T>
struct fmt::formatter<std::map<K, T>>
{
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

    template <typename FormatContext>
    auto format(const std::map<K, T> &p, FormatContext &ctx)
    {
        ulib::u8string result;
        result += u8"{";
        if (p.empty())
        {
            result += u8"}";
        }
        else
        {

            for (auto it = p.begin(); it != p.end(); it++)
            {
                auto cop = it;
                cop++;

                const char *ending = ", ";
                if (cop == p.end())
                    ending = "}";

                if constexpr (ulib::is_string_v<K>)
                {
                    result += ulib::u8(fmt::format("\"{}\": ", ulib::fmt::detail::EscapeString(ulib::u8(it->first))));
                }
                else
                {
                    result += ulib::u8(fmt::format("{}: ", it->first));
                }

                if constexpr (ulib::is_string_v<T>)
                {
                    result += ulib::u8(
                        fmt::format("\"{}\"{}", ulib::fmt::detail::EscapeString(ulib::u8(it->second)), ending));
                }
                else
                {
                    result += ulib::u8(fmt::format("{}{}", it->second, ending));
                }
            }

            // for (auto it = p.begin(); it != p.end(); it++)
            // {
            //     auto cop = it;
            //     cop++;

            //     if (cop == p.end())
            //     {
            //         result += ulib::u8(fmt::format("{}]", *it));
            //     }
            //     else
            //     {
            //         result += ulib::u8(fmt::format("{}, ", *it));
            //     }
            // }
        }

        fmt::formatter<fmt::string_view> ft;
        fmt::string_view name((char *)result.raw_data(), result.size());
        return ft.format(name, ctx);
    }
};