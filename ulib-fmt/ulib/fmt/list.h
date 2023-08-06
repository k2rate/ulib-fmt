#pragma once

#include <ulib/format.h>
#include <ulib/list.h>
#include <ulib/fmt/detail/escape.h>

namespace ulib
{
    namespace detail
    {
        template <class T>
        ulib::u8string FormatSpan(SpanView<T> view)
        {
            ulib::u8string result;
            result += u8"[";
            if (view.empty())
            {
                result += u8"]";
            }
            else
            {
                for (auto it = view.begin(); it != view.end(); it++)
                {
                    const char *ending = ", ";
                    if (it + 1 == view.end())
                        ending = "]";

                    if constexpr (ulib::is_string_v<T>)
                    {
                        result +=
                            ulib::u8(::fmt::format("\"{}\"{}", ulib::fmt::detail::EscapeString(ulib::u8(*it)), ending));
                    }
                    else
                    {
                        result += ulib::u8(::fmt::format("{}{}", *it, ending));
                    }
                }
            }

            return result;
        }
    } // namespace detail
} // namespace ulib

template <class T>
struct fmt::formatter<ulib::Span<T>>
{
    using SpanT = ulib::Span<T>;
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

    template <typename FormatContext>
    auto format(SpanT p, FormatContext &ctx)
    {
        ulib::u8string result = ulib::detail::FormatSpan(p);
        fmt::formatter<fmt::string_view> ft;
        fmt::string_view name((char *)result.raw_data(), result.size());
        return ft.format(name, ctx);
    }
};

template <class T, class AllocatorT>
struct fmt::formatter<ulib::List<T, AllocatorT>>
{
    using ListT = ulib::List<T, AllocatorT>;
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

    template <typename FormatContext>
    auto format(const ListT &p, FormatContext &ctx)
    {
        ulib::u8string result = ulib::detail::FormatSpan<typename ListT::value_type>(p);
        
        fmt::formatter<fmt::string_view> ft;
        fmt::string_view name((char *)result.raw_data(), result.size());
        return ft.format(name, ctx);
    }
};