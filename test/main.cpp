#include <ulib/fmt/format.h>
#include <windows.h>

#include <ulib/encodings/utf16/string.h>
#include <ulib/encodings/utf32/string.h>

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

    ulib::u8string u8out = ulib::format(u8"u8:{} u16:{} u32:{} u8string_view:{}", u8str, u16str, u32str, test);
    auto u16out = ulib::Convert<ulib::Utf8, ulib::Utf16>(u8out);

    MessageBoxW(0, u16out.c_str(), L"ky", MB_OK);

    return 0;
}