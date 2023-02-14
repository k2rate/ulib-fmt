#pragma once

#include <fmt/format.h>
#include <filesystem>

template<>
struct fmt::formatter<std::filesystem::path>
{
    // Parses format specifications of the form ['f' | 'e'].
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.end();
    }

    // Formats the point p using the parsed format specification (presentation)
    // stored in this formatter.
    template <typename FormatContext>
    auto format(const std::filesystem::path &p, FormatContext &ctx)
    {
        auto u8str = p.u8string();
        fmt::string_view name((char*)u8str.data(), u8str.size());

        fmt::formatter<fmt::string_view> ft;
        return ft.format(name, ctx);
    }
};