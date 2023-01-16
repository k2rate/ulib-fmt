
/*
template <class EncodingT, class AllocatorT>
struct fmt::formatter<ulib::EncodedString<EncodingT, AllocatorT>> : formatter<double>
{
    ulib::EncodedString<EncodingT, AllocatorT> format(const ulib::EncodedString<EncodingT, AllocatorT> &c, format_context &ctx)
    {
        return formatter<double>::format(ulib::Stri, ctx);
    }
};

*/

/*
namespace ulib
{
    template <typename... Args>
    using u32format_string = basic_format_string<char32_t, fmt::type_identity_t<Args>...>;

    template <typename... Args>
    constexpr format_arg_store<fmt::buffer_context<char32_t>, Args...> make_u32format_args(
        const Args &...args)
    {
        return {args...};
    }

    template <typename... T>
    auto format(u32format_string<T...> fmt, T &&...args) -> std::u32string
    {
        return vformat(fmt::basic_string_view<char32_t>(fmt), make_u32format_args(args...));
    }
}
*/


#include <ulib/fmt/format.h>
#include <windows.h>

int main()
{
    ulib::string_view ky;
    ulib::u8string_view re;

    ulib::u8string u8str(u8"pizdec");
    ulib::u16string u16str(u"ахуеть");
    ulib::u32string u32str(U"ЕБАТЬ");

    ulib::u8string_view test = u8str;

    ulib::format<ulib::Utf8>(u8"{}", u8str);
    ulib::format(u8"hi {}", u16str);

    ulib::u8string u8out = ulib::format(u8"u8:{} u16:{} u32:{}", u8str, u16str, u32str);
    auto u16out = ulib::Convert<ulib::Utf8, ulib::Utf16>(u8out);

    MessageBoxW(0, u16out.c_str(), L"ky", MB_OK);

    return 0;
}