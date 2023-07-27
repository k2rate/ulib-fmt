#pragma once

#include <ulib/u8.h>

#include <ulib/allocators/standardallocator.h>
#include <ulib/allocators/staticallocator.h>

#include <ulib/typetraits/encoding.h>
#include <ulib/typetraits/allocator.h>
#include <ulib/typetraits/selecttype.h>
#include <ulib/typetraits/string.h>

#include <fmt/format.h>

namespace ulib
{
    using FormatAllocatorT = ulib::StaticAllocator<ulib::DefaultAllocator>;

    namespace detail
    {
        template <class S, class NewEncoding, class NewAlloc>
        struct rebind_string
        {
            using Type = ulib::EncodedString<NewEncoding, NewAlloc>;
        };

        template <class CharT, class Traits, class A, class NewEncoding, class NewAlloc>
        struct rebind_string<std::basic_string<CharT, Traits, A>, NewEncoding, NewAlloc>
        {
            using Type = std::basic_string<typename NewEncoding::CharT, std::char_traits<typename NewEncoding::CharT>,
                                           ulib::StandardAllocator<CharT, NewAlloc>>;
        };

        template <class CharT, class Traits, class NewEncoding, class NewAlloc>
        struct rebind_string<std::basic_string_view<CharT, Traits>, NewEncoding, NewAlloc>
        {
            using Type = std::basic_string<typename NewEncoding::CharT, std::char_traits<typename NewEncoding::CharT>,
                                           ulib::StandardAllocator<CharT, NewAlloc>>;
        };

        template <class S, class NewEncoding, class NewAlloc>
        using rebind_string_t = typename rebind_string<S, NewEncoding, NewAlloc>::Type;

        // template <class EncodingT, class AllocatorT, class NewEncoding, class NewAlloc>
        // struct rebind_string<ulib::EncodedString<EncodingT, AllocatorT>, NewEncoding, NewAlloc>
        // {
        //     using Type = ulib::EncodedString<NewEncoding, NewAlloc>;
        // };

        // template <class EncodingT, class NewEncoding, class NewAlloc>
        // struct rebind_string<ulib::EncodedStringView<EncodingT>, NewEncoding, NewAlloc>
        // {
        //     using Type = ulib::EncodedString<NewEncoding, NewAlloc>;
        // };

        template <class EncodingT>
        constexpr bool IsRawEncoding = EncodingT::kType == EncodingType::Raw;

        template <class EncodingT, class CharT>
        constexpr bool IsEncodingSizeCompatible = sizeof(typename EncodingT::CharT) == sizeof(CharT);

        template <class EncodingT, class CharT>
        constexpr bool IsRawEncodingLike = IsRawEncoding<EncodingT> && IsEncodingSizeCompatible<EncodingT, CharT>;

        template <class EncodingT>
        constexpr bool IsEncodingFmtCompatible =
            std::is_same_v<EncodingT, Utf8> || detail::IsRawEncodingLike<EncodingT, char>;

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
        // inline constexpr bool IsParentCompatible = std::is_same_v<SourceEncodingT, typename
        // EncodingT::ParentEncodingT> || std::is_same_v<typename SourceEncodingT::ParentEncodingT, EncodingT>;
    } // namespace detail
} // namespace ulib

template <class EncodingT, class AllocatorT>
struct fmt::formatter<ulib::EncodedString<EncodingT, AllocatorT>>
{
    using StringT = ulib::EncodedString<EncodingT, AllocatorT>;
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

    template <typename FormatContext>
    auto format(const StringT &p, FormatContext &ctx)
    {
        return ulib::detail::FormatHandler<FormatContext, EncodingT>(p, ctx);
    }
};

template <class EncodingT>
struct fmt::formatter<ulib::EncodedStringView<EncodingT>>
{
    using StringViewT = ulib::EncodedStringView<EncodingT>;
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

    template <typename FormatContext>
    auto format(const StringViewT &p, FormatContext &ctx)
    {
        return ulib::detail::FormatHandler(p, ctx);
    }
};

namespace ulib
{
    template <class EncodingT, class AllocatorT = DefaultAllocator>
    inline EncodedString<EncodingT, AllocatorT> vformat(
        fmt::basic_string_view<char> format_str,
        fmt::basic_format_args<fmt::buffer_context<fmt::type_identity_t<char>>> args)
    {
        StandardAllocator<char, FormatAllocatorT> al{};
        fmt::basic_memory_buffer<char, 0x1000, ulib::StandardAllocator<char, FormatAllocatorT>> buffer(al);
        fmt::detail::vformat_to(buffer, format_str, args);

        if constexpr (detail::IsEncodingFmtCompatible<EncodingT>)
        {
            return ulib::EncodedString<EncodingT, AllocatorT>((typename EncodingT::CharT *)buffer.data(),
                                                              buffer.size());
        }
        else
        {
            return ulib::Convert<EncodingT, AllocatorT>(EncodedStringView<Utf8>{(char8 *)buffer.data(), buffer.size()});
        }
    }

    template <class UOutputEncodingT = void, class UOutputAllocatorT = void, class StringT,
              class EncodingT = argument_encoding_t<StringT>,
              class AllocatorT = SelectTypeT<ulib_container_allocator_t<StringT>, DefaultAllocator>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              class OutputAllocatorT = SelectTypeT<UOutputAllocatorT, AllocatorT>,
              //   class OutputStringT = detail::rebind_string_t<StringT, OutputEncodingT, OutputAllocatorT>,
              typename... T>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> format(const StringT &fmt, T &&...args)
    {
        EncodedStringView<EncodingT> view = fmt;

        if constexpr (detail::IsEncodingFmtCompatible<EncodingT>)
        {
            fmt::basic_string_view<char> vv((char *)view.data(), view.size());
            return ulib::vformat<OutputEncodingT, OutputAllocatorT>(vv, fmt::make_format_args(args...));
        }
        else
        {
            auto u8fmt = Convert<Utf8, FormatAllocatorT>(view);

            fmt::basic_string_view<char> vv((char *)u8fmt.data(), u8fmt.size());
            return ulib::vformat<OutputEncodingT, OutputAllocatorT>(vv, fmt::make_format_args(args...));
        }
    }
} // namespace ulib
