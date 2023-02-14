#pragma once

#include <ulib/containers/encodedstring.h>
#include <ulib/containers/stringview.h>

#include <ulib/u8.h>

#include <ulib/allocators/standardallocator.h>

#include <fmt/format.h>

template <class EncodingT, class AllocatorT>
struct fmt::formatter<ulib::EncodedString<EncodingT, AllocatorT>>
{
    using CharT = typename EncodingT::CharT;
    using StringT = ulib::EncodedString<EncodingT, AllocatorT>;

    // Parses format specifications of the form ['f' | 'e'].
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.end();
    }

    // Formats the point p using the parsed format specification (presentation)
    // stored in this formatter.
    template <typename FormatContext>
    auto format(const StringT &p, FormatContext &ctx)
    {
        if constexpr (std::is_same_v<EncodingT, ulib::Utf8>)
        {
            fmt::string_view name((char *)p.Data(), p.Size());

            fmt::formatter<fmt::string_view> ft;
            return ft.format(name, ctx);
        }
        else
        {
            auto u8str = ulib::Converter<AllocatorT>::Convert<EncodingT, ulib::Utf8>(p);
            fmt::string_view name((char *)u8str.Data(), u8str.Size());

            fmt::formatter<fmt::string_view> ft;
            return ft.format(name, ctx);
        }
    }
};

template <class EncodingT>
struct fmt::formatter<ulib::EncodedStringView<EncodingT>>
{
    using CharT = typename EncodingT::CharT;
    using StringViewT = ulib::EncodedStringView<EncodingT>;

    // Parses format specifications of the form ['f' | 'e'].
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.end();
    }

    // Formats the point p using the parsed format specification (presentation)
    // stored in this formatter.
    template <typename FormatContext>
    auto format(const StringViewT &p, FormatContext &ctx)
    {
        if constexpr (std::is_same_v<EncodingT, ulib::Utf8>)
        {
            fmt::string_view name((char *)p.Data(), p.Size());

            fmt::formatter<fmt::string_view> ft;
            return ft.format(name, ctx);
        }
        else
        {
            auto u8str = ulib::u8(p);
            fmt::string_view name((char *)u8str.Data(), u8str.Size());

            fmt::formatter<fmt::string_view> ft;
            return ft.format(name, ctx);
        }
    }
};

namespace ulib
{
    inline ulib::u8string vformat(fmt::basic_string_view<char> format_str,
                           fmt::basic_format_args<fmt::buffer_context<fmt::type_identity_t<char>>> args)
    {
        fmt::basic_memory_buffer<char> buffer;
        fmt::detail::vformat_to(buffer, format_str, args);
        return ulib::u8string((char8 *)buffer.data(), buffer.size());
    }

#ifdef __cpp_char8_t
    template <typename... T>
    inline ulib::string format(ulib::string_view fmt, T &&...args)
    {
        fmt::basic_string_view<char> vv((char *)fmt.data(), fmt.size());
        return ulib::vformat(vv, fmt::make_format_args(args...));
    }
#endif

    template <typename... T>
    inline ulib::u8string format(ulib::u8string_view fmt, T &&...args)
    {
        fmt::basic_string_view<char> vv((char *)fmt.data(), fmt.size());
        return ulib::vformat(vv, fmt::make_format_args(args...));
    }

    template <class EncodingT, class AllocatorT = ulib::DefaultAllocator>
    inline ulib::EncodedString<EncodingT, AllocatorT> vformat(fmt::basic_string_view<char> format_str,
                                                       fmt::basic_format_args<fmt::buffer_context<fmt::type_identity_t<char>>> args)
    {
        ulib::StandardAllocator<char, AllocatorT> al{};
        fmt::basic_memory_buffer<char, 0x1000, ulib::StandardAllocator<char, AllocatorT>> buffer(al);

        fmt::detail::vformat_to(buffer, format_str, args);
        ulib::EncodedString<ulib::Utf8, AllocatorT> temp((char8 *)buffer.data(), buffer.size());

        if constexpr (std::is_same_v<EncodingT, ulib::Utf8>)
        {
            return temp;
        }
        else
        {
            return ulib::Converter<AllocatorT>::Convert<ulib::Utf8, EncodingT>(temp);
        }
    }

#ifdef __cpp_char8_t
    template <class EncodingT, class AllocatorT = ulib::DefaultAllocator, typename... T>
    inline ulib::EncodedString<EncodingT, AllocatorT> format(ulib::string_view fmt, T &&...args)
    {
        fmt::basic_string_view<char> vv((char *)fmt.data(), fmt.size());
        return ulib::vformat<EncodingT, AllocatorT>(vv, fmt::make_format_args<fmt::buffer_context<char>>(args...));
    }
#endif

    template <class EncodingT, class AllocatorT = ulib::DefaultAllocator, typename... T>
    inline ulib::EncodedString<EncodingT, AllocatorT> format(ulib::u8string_view fmt, T &&...args)
    {
        fmt::basic_string_view<char> vv((char *)fmt.data(), fmt.size());
        return ulib::vformat<EncodingT, AllocatorT>(vv, fmt::make_format_args<fmt::buffer_context<char>>(args...));
    }
}
