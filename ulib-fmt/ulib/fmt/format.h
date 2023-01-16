#pragma once

#include <fmt/format.h>
#include <ulib/containers/encodedstring.h>
#include <ulib/containers/stringview.h>
#include <ulib/encodings/utf8/string.h>
#include <ulib/encodings/utf8/stringview.h>
#include <ulib/encodings/converter.h>
#include <ulib/encodings/utf16/string.h>
#include <ulib/encodings/utf32/string.h>
#include <ulib/allocators/standardallocator.h>

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
            fmt::string_view name((CharT *)p.Data(), p.Size());

            fmt::formatter<fmt::string_view> ft;
            return ft.format(name, ctx);
        }
        else
        {
            auto u8str = ulib::Converter<AllocatorT>::Convert<EncodingT, ulib::Utf8>(p);
            fmt::string_view name((typename ulib::Utf8::CharT *)u8str.Data(), u8str.Size());

            fmt::formatter<fmt::string_view> ft;
            return ft.format(name, ctx);
        }
    }
};

namespace ulib
{
    ulib::u8string vformat(fmt::basic_string_view<char> format_str,
                           fmt::basic_format_args<fmt::buffer_context<fmt::type_identity_t<char>>> args)
    {
        fmt::basic_memory_buffer<char> buffer;
        fmt::detail::vformat_to(buffer, format_str, args);
        return ulib::u8string((char8 *)buffer.data(), buffer.size());
    }

#ifdef __cpp_char8_t
    template <typename... T>
    ulib::string format(ulib::string_view fmt, T &&...args)
    {
        fmt::basic_string_view<char> vv((char *)fmt.data(), fmt.size());
        return ulib::vformat(vv, fmt::make_format_args(args...));
    }
#endif

    template <typename... T>
    ulib::string format(ulib::u8string_view fmt, T &&...args)
    {
        fmt::basic_string_view<char> vv((char *)fmt.data(), fmt.size());
        return ulib::vformat(vv, fmt::make_format_args(args...));
    }

    template <class EncodingT, class AllocatorT = ulib::DefaultAllocator>
    ulib::EncodedString<EncodingT, AllocatorT> vformat(fmt::basic_string_view<char> format_str,
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
    ulib::EncodedString<EncodingT, AllocatorT> format(ulib::string_view fmt, T &&...args)
    {
        fmt::basic_string_view<char> vv((char *)fmt.data(), fmt.size());
        return ulib::vformat<EncodingT, AllocatorT>(vv, fmt::make_format_args<fmt::buffer_context<char>>(args...));
    }
#endif

    template <class EncodingT, class AllocatorT = ulib::DefaultAllocator, typename... T>
    ulib::EncodedString<EncodingT, AllocatorT> format(ulib::u8string_view fmt, T &&...args)
    {
        fmt::basic_string_view<char> vv((char *)fmt.data(), fmt.size());
        return ulib::vformat<EncodingT, AllocatorT>(vv, fmt::make_format_args<fmt::buffer_context<char>>(args...));
    }
}
