#pragma once

#include <ulib/string.h>
#include <ulib/typetraits/allocator.h>
#include <ulib/typetraits/typeordefault.h>
#include <ulib/typetraits/string.h>

#include <ulib/allocators/tempallocator.h>
#include <ulib/allocators/standardallocator.h>
#include <ulib/allocators/staticallocator.h>

#include <fmt/format.h>

namespace ulib
{
    using FormatAllocatorT = TempAllocatorT;

    namespace detail
    {
        template <typename FormatContext, class EncodingT>
        auto FormatHandler(const EncodedStringView<EncodingT> p, FormatContext &ctx)
        {
            fmt::formatter<fmt::string_view> ft;

            if constexpr (is_encodings_raw_movable_v<EncodingT, Utf8>)
            {
                fmt::string_view name((char *)p.Data(), p.Size());
                return ft.format(name, ctx);
            }
            else
            {
                auto u8str = Convert<Utf8, TempAllocatorT>(p);

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

template <class EncodingT, class CharT>
struct fmt::formatter<ulib::EncodedStringSpan<EncodingT, CharT>>
{
    using StringViewT = ulib::EncodedStringSpan<EncodingT, CharT>;
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
        fmt::basic_memory_buffer<char, 0x1000, StandardAllocator<char, FormatAllocatorT>> buffer(al);
        fmt::detail::vformat_to(buffer, format_str, args);

        if constexpr (is_encodings_raw_movable_v<EncodingT, Utf8>)
        {
            return ulib::EncodedString<EncodingT, AllocatorT>((typename EncodingT::CharT *)buffer.data(),
                                                              buffer.size());
        }
        else
        {
            return ulib::Convert<EncodingT, AllocatorT>(EncodedStringView<Utf8>{(char8 *)buffer.data(), buffer.size()});
        }
    }

    template <class UOutputEncodingT = missing_type, class UOutputAllocatorT = missing_type, class StringT,
              class EncodingT = argument_encoding_or_die_t<StringT>,
              class AllocatorT = constainer_choose_ulib_allocator_or_die_t<StringT>,
              class OutputEncodingT = type_or_default_t<UOutputEncodingT, EncodingT>,
              class OutputAllocatorT = type_or_default_t<UOutputAllocatorT, AllocatorT>, typename... T>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> format(const StringT &fmt, T &&...args)
    {
        EncodedStringView<EncodingT> view = fmt;
        
        if constexpr (is_encodings_raw_movable_v<EncodingT, Utf8>)
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
