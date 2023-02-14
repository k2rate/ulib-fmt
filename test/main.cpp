#include <ulib/encodings/utf16/string.h>
#include <ulib/encodings/utf32/string.h>

#include <ulib/string.h>
#include <ulib/wchar.h>
#include <ulib/u8.h>
#include <ulib/u16.h>
#include <ulib/u32.h>

#include <ulib/fmt/format.h>
#include <ulib/fmt/path.h>

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

    bool bchar = false;
#ifdef __cpp_char8_t
    bchar = true;
#endif

    ulib::u8string u8out = ulib::format(u8"bchar: {} u8:{} u16:{} u32:{} u8string_view:{}\ncurrent_path: {}",
                                        bchar, u8str, u16str, u32str, test, std::filesystem::current_path());

    auto u16out = ulib::Convert<ulib::Utf8, ulib::Utf16>(u8out);
    MessageBoxW(0, (wchar_t *)u16out.c_str(), L"ky", MB_OK);

    return 0;
}