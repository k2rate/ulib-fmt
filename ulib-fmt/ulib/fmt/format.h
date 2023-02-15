#pragma once

#include <ulib/containers/encodedstring.h>
#include <ulib/containers/stringview.h>

#include <ulib/u8.h>
#include <ulib/string.h>

#include <ulib/allocators/standardallocator.h>
#include <ulib/allocators/mallocallocator.h>
#include <ulib/allocators/fastmemallocator.h>
#include <ulib/allocators/staticallocator.h>
#include <ulib/allocators/synchronizedallocator.h>

#include <fmt/format.h>

namespace ulib
{
    namespace detail
    {
        using FormatAllocatorT = ulib::StaticAllocator<ulib::SynchronizedAllocator<ulib::FastMemAllocator<ulib::MallocAllocator>>>;

        template <class EncodingT>
        constexpr bool IsRawEncoding = EncodingT::kType == EncodingType::Raw;

        template <class EncodingT, class CharT>
        constexpr bool IsEncodingSizeCompatible = sizeof(typename EncodingT::CharT) == sizeof(CharT);

        template <class EncodingT, class CharT>
        constexpr bool IsRawEncodingLike = IsRawEncoding<EncodingT> && IsEncodingSizeCompatible<EncodingT, CharT>;

        template <class EncodingT>
        constexpr bool IsEncodingFmtCompatible = std::is_same_v<EncodingT, Utf8> || detail::IsRawEncodingLike<EncodingT, char>;

        template <class T, class DefaultT>
        struct SelectType
        {
            using Type = T;
        };

        template <class DefaultT>
        struct SelectType<void, DefaultT>
        {
            using Type = DefaultT;
        };

        template <class T, class DefaultT>
        using SelectTypeT = typename SelectType<T, DefaultT>::Type;

        template <typename FormatContext, class EncodingT>
        auto FormatHandler(const ulib::EncodedStringView<EncodingT> p, FormatContext &ctx)
        {
            fmt::formatter<fmt::string_view> ft;

            if constexpr (IsEncodingFmtCompatible<EncodingT>)
            {
                fmt::string_view name((char *)p.Data(), p.Size());
                return ft.format(name, ctx);
            }
            else
            {
                auto u8str = Convert<Utf8, FormatAllocatorT>(p);

                fmt::string_view name((char *)u8str.Data(), u8str.Size());
                return ft.format(name, ctx);
            }
        }

        // template <class SourceEncodingT, class EncodingT>
        // inline constexpr bool IsParentCompatible = std::is_same_v<SourceEncodingT, typename EncodingT::ParentEncodingT> || std::is_same_v<typename SourceEncodingT::ParentEncodingT, EncodingT>;
    }
}

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
        return ulib::detail::FormatHandler<FormatContext, EncodingT>(p, ctx);
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
        return ulib::detail::FormatHandler(p, ctx);
    }
};

namespace ulib
{
    /*
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
    */

    /*
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
*/

    template <class EncodingT, class AllocatorT = DefaultAllocator>
    inline EncodedString<EncodingT, AllocatorT> vformat(fmt::basic_string_view<char> format_str,
                                                        fmt::basic_format_args<fmt::buffer_context<fmt::type_identity_t<char>>> args)
    {
        StandardAllocator<char, detail::FormatAllocatorT> al{};
        fmt::basic_memory_buffer<char, 0x1000, ulib::StandardAllocator<char, detail::FormatAllocatorT>> buffer(al);

        fmt::detail::vformat_to(buffer, format_str, args);

        if constexpr (detail::IsEncodingFmtCompatible<EncodingT>)
        {
            return ulib::EncodedString<EncodingT, AllocatorT>((typename EncodingT::CharT *)buffer.data(), buffer.size());
        }
        else
        {
            return ulib::Convert<EncodingT, AllocatorT>(EncodedStringView<Utf8>{(char8 *)buffer.data(), buffer.size()});
        }
    }

    template <class UOutputEncodingT = void, class UOutputAllocatorT = void, class EncodingT, class AllocatorT,
              class OutputEncodingT = detail::SelectTypeT<UOutputEncodingT, EncodingT>,
              class OutputAllocatorT = detail::SelectTypeT<UOutputAllocatorT, AllocatorT>, typename... T>
    inline ulib::EncodedString<OutputEncodingT, OutputAllocatorT> format(const ulib::EncodedString<EncodingT, AllocatorT> &fmt, T &&...args)
    {
        if constexpr (detail::IsEncodingFmtCompatible<EncodingT>)
        {
            fmt::basic_string_view<char> vv((char *)fmt.data(), fmt.size());
            return ulib::vformat<OutputEncodingT, OutputAllocatorT>(vv, fmt::make_format_args(args...));
        }
        else
        {
            auto u8fmt = Convert<Utf8, detail::FormatAllocatorT>(fmt);

            fmt::basic_string_view<char> vv((char *)u8fmt.data(), u8fmt.size());
            return ulib::vformat<OutputEncodingT, OutputAllocatorT>(vv, fmt::make_format_args(args...));
        }
    }

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class EncodingT,
              class OutputEncodingT = detail::SelectTypeT<UOutputEncodingT, EncodingT>, typename... T>
    inline ulib::EncodedString<OutputEncodingT, OutputAllocatorT> format(ulib::EncodedStringView<EncodingT> fmt, T &&...args)
    {
        if constexpr (detail::IsEncodingFmtCompatible<EncodingT>)
        {
            fmt::basic_string_view<char> vv((char *)fmt.data(), fmt.size());
            return ulib::vformat<OutputEncodingT, OutputAllocatorT>(vv, fmt::make_format_args(args...));
        }
        else
        {
            auto u8fmt = Convert<Utf8, detail::FormatAllocatorT>(fmt);

            fmt::basic_string_view<char> vv((char *)u8fmt.data(), u8fmt.size());
            return ulib::vformat<OutputEncodingT, OutputAllocatorT>(vv, fmt::make_format_args(args...));
        }
    }

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class CharT, class EncodingT = LiteralEncodingT<CharT>,
              class OutputEncodingT = detail::SelectTypeT<UOutputEncodingT, EncodingT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true, typename... T>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> format(const CharT *fmt, T &&...args)
    {
        if constexpr (detail::IsEncodingFmtCompatible<EncodingT>)
        {
            fmt::basic_string_view<char> vv((char *)fmt, CStringLengthHack(fmt));
            return ulib::vformat<OutputEncodingT, OutputAllocatorT>(vv, fmt::make_format_args(args...));
        }
        else
        {
            auto u8fmt = Convert<Utf8, detail::FormatAllocatorT>(fmt);

            fmt::basic_string_view<char> vv((char *)u8fmt.data(), u8fmt.size());
            return ulib::vformat<OutputEncodingT, OutputAllocatorT>(vv, fmt::make_format_args(args...));
        }
    }
}
