#include <ulib/string.h>
#include <ulib/wchar.h>
#include <ulib/u8.h>
#include <ulib/u16.h>
#include <ulib/u32.h>

#include <ulib/format.h>

#include <windows.h>

int main()
{
    bool bchar = false;
#ifdef __cpp_char8_t
    bchar = true;
#endif

    ulib::string_view ky;
    ulib::u8string_view re;

    ulib::u8string u8str(u8"pizdec");
    ulib::u16string u16str(u"ахуеть");
    ulib::u32string u32str(U"ЕБАТЬ");
    ulib::u8string_view test = u8str;

    ulib::u8string u8out = ulib::format(u8"bchar: {} u8:{} u16:{} u32:{} u8string_view:{}\ncurrent_path: {}",
                                        bchar, u8str, u16str, u32str, test,
                                        std::filesystem::current_path());

    ulib::u16string u16out = ulib::format(u"bchar: {} u8:{} u16:{} u32:{} u8string_view:{}\ncurrent_path: {}",
                                          bchar, u8str, u16str, u32str, test,
                                          std::filesystem::current_path());

    ulib::u32string u32out = ulib::format(U"bchar: {} u8:{} u16:{} u32:{} u8string_view:{}\ncurrent_path: {}",
                                          bchar, u8str, u16str, u32str, test,
                                          std::filesystem::current_path());

    ulib::string out = ulib::format("bchar: {} u8:{} u16:{} u32:{} u8string_view:{}\ncurrent_path: {}",
                                    bchar, u8str, u16str, u32str, test,
                                    std::filesystem::current_path());

    ulib::wstring wout = ulib::format(L"bchar: {} u8:{} u16:{} u32:{} u8string_view:{}\ncurrent_path: {}",
                                      bchar, u8str, u16str, u32str, test,
                                      std::filesystem::current_path());

    u32out = ulib::format<ulib::Utf32>(u8"bchar: {} u8:{} u16:{} u32:{} u8string_view:{}\ncurrent_path: {}",
                                       bchar, u8str, u16str, u32str, test,
                                       std::filesystem::current_path());

    wout = ulib::format<ulib::WideEncoding>(u8"bchar: {} u8:{} u16:{} u32:{} u8string_view:{}\ncurrent_path: {}",
                                            bchar, u8str, u16str, u32str, test,
                                            std::filesystem::current_path());

    MessageBoxW(0, (wchar_t *)u16out.c_str(), L"ky", MB_OK);

    return 0;
}