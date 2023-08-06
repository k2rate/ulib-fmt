// #include <gtest/gtest.h>
// #include <ulib/format.h>

// #include <ulib/allocators/staticallocator.h>
// #include <ulib/allocators/fastmemallocator.h>
// #include <ulib/allocators/defaultallocator.h>

// #include <ulib/string.h>

// TEST(Format, StdCompatibility)
// {
//     std::string raw_str = "hello";
//     std::u8string u8_str = u8"hello";
//     std::u16string u16_str = u"hello";
//     std::u32string u32_str = U"hello";
//     std::wstring w_str = L"hello";

//     std::string raw_result = ulib::format("{}", raw_str);
//     std::u8string u8_result = ulib::format(u8"{}", raw_str);  // u8_str fmt error
//     std::u16string u16_result = ulib::format(u"{}", raw_str); // u16_str fmt error
//     std::u32string u32_result = ulib::format(U"{}", raw_str); // u32_str fmt error
//     std::wstring w_result = ulib::format(L"{}", raw_str);     // w_str fmt error

//     ASSERT_TRUE(raw_str == raw_result);
//     ASSERT_TRUE(u8_str == u8_result);
//     ASSERT_TRUE(u16_str == u16_result);
//     ASSERT_TRUE(u32_str == u32_result);
//     ASSERT_TRUE(w_str == w_result);

// }

// TEST(Format, ReturnTypeEquals)
// {
//     {
//         using AllocatorT = ulib::StaticAllocator<ulib::FastMemAllocator<ulib::DefaultAllocator>>;
//         using test_string = ulib::EncodedString<ulib::Utf8, AllocatorT>;

//         test_string test_str = u8"hello";
//         ulib::string raw_str = "hello";
//         ulib::u8string u8_str = u8"hello";
//         ulib::u16string u16_str = u"hello";
//         ulib::u32string u32_str = U"hello";
//         ulib::wstring w_str = L"hello";

//         auto test_result = ulib::format(test_str);
//         auto raw_result = ulib::format(raw_str);
//         auto u8_result = ulib::format(u8_str);   // u8_str fmt error
//         auto u16_result = ulib::format(u16_str); // u16_str fmt error
//         auto u32_result = ulib::format(u32_str); // u32_str fmt error
//         auto w_result = ulib::format(w_str);     // w_str fmt error

//         ASSERT_EQ(test_result, test_str);
//         ASSERT_EQ(raw_result, raw_str);
//         ASSERT_EQ(u8_result, u8_str);
//         ASSERT_EQ(u16_result, u16_str);
//         ASSERT_EQ(u32_result, u32_str);
//         ASSERT_EQ(w_result, w_str);

//         ASSERT_TRUE((std::is_same_v<decltype(test_str), decltype(test_result)>));
//         ASSERT_TRUE((std::is_same_v<decltype(raw_str), decltype(raw_result)>));
//         ASSERT_TRUE((std::is_same_v<decltype(u8_str), decltype(u8_result)>));
//         ASSERT_TRUE((std::is_same_v<decltype(u16_str), decltype(u16_result)>));
//         ASSERT_TRUE((std::is_same_v<decltype(u32_str), decltype(u32_result)>));
//         ASSERT_TRUE((std::is_same_v<decltype(w_str), decltype(w_result)>));
//     }

//     {
//         ulib::string raw_str = "hello";
//         ulib::u8string u8_str = u8"hello";
//         ulib::u16string u16_str = u"hello";
//         ulib::u32string u32_str = U"hello";
//         ulib::wstring w_str = L"hello";

//         std::string raw_result = ulib::format(raw_str);
//         std::u8string u8_result = ulib::format(u8_str);    // u8_str fmt error
//         std::u16string u16_result = ulib::format(u16_str); // u16_str fmt error
//         std::u32string u32_result = ulib::format(u32_str); // u32_str fmt error
//         std::wstring w_result = ulib::format(w_str);       // w_str fmt error

//         ASSERT_TRUE(raw_result == raw_str);
//         ASSERT_TRUE(u8_result == u8_str);
//         ASSERT_TRUE(u16_str == u16_result);
//         ASSERT_TRUE(u32_str == u32_result);
//         ASSERT_TRUE(w_str == w_result);
//     }
// }



// TEST(Format, MoreEncodings)
// {
//     ulib::string raw_str = "hello";
//     ulib::u8string u8_str = u8"ку брат";
//     ulib::u16string u16_str = u"🚀🚀🚀";
//     ulib::u32string u32_str = U"🚣🚣🚣";
//     ulib::wstring w_str = L"それを停止する";

//     std::wstring w_result = ulib::format(L"{} {} {} {} {}", raw_str, u8_str, u16_str, u32_str, w_str);
//     ulib::u8string u8_result = ulib::format(u8"{}", ulib::wstring_view{w_result});

//     ASSERT_EQ(u8_result, u8"hello ку брат 🚀🚀🚀 🚣🚣🚣 それを停止する");
// }

// TEST(Format, StringView)
// {
//     ulib::string_view raw_str = "hello";
//     ulib::u8string_view u8_str = u8"ку брат";
//     ulib::u16string_view u16_str = u"🚀🚀🚀";
//     ulib::u32string_view u32_str = U"🚣🚣🚣";
//     ulib::wstring_view w_str = L"それを停止する";

//     std::wstring_view w_result = ulib::format(L"{} {} {} {} {}", raw_str, u8_str, u16_str, u32_str, w_str);
//     ulib::u8string u8_result = ulib::format(u8"{}", ulib::wstring_view{w_result});

//     ASSERT_EQ(u8_result, u8"hello ку брат 🚀🚀🚀 🚣🚣🚣 それを停止する");
// }

// TEST(Format, Integers)
// {
//     ulib::u16string u16_result = ulib::format(u"{} {} {} {} {}", 10, 20, -30, -40, 50);
//     ASSERT_EQ(u16_result, u"10 20 -30 -40 50");

//     ulib::u32string u32_result = ulib::format(U"{0:X} {0:x}", 0xABCD);
//     ASSERT_EQ(u32_result, U"ABCD abcd");

//     ulib::u8string u8_result = ulib::format(u8"{:.2f}", 1.23f);
//     ASSERT_TRUE(u8_result.length() == 4);
// }

// TEST(Format, FloatPrecision)
// {
//     ulib::u16string u16_result = ulib::format(u"{:.2f}", 1.23f);
//     ASSERT_TRUE(u16_result.length() == 4);
// }

// TEST(Format, ExplicitEncoding)
// {
//     ulib::u16string u16_result = ulib::format<ulib::Utf16>("{:.2f}", 1.23f);
//     ASSERT_TRUE(u16_result.length() == 4);

//     ulib::u8string u8_result = ulib::format<ulib::Utf8>("{:.2f}", 1.23f);
//     ASSERT_TRUE(u8_result.length() == 4);

//     ulib::u32string u32_result = ulib::format<ulib::Utf32>("{:.2f}", 1.23f);
//     ASSERT_TRUE(u32_result.length() == 4);

//     ulib::wstring w_result = ulib::format<ulib::WideEncoding>("{:.2f}", 1.23f);
//     ASSERT_TRUE(w_result.length() == 4);

//     ulib::string result = ulib::format<ulib::MultibyteEncoding>(L"{:.2f}", 1.23f);
//     ASSERT_TRUE(result.length() == 4);
// }