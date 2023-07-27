#include <gtest/gtest.h>

#include <ulib/format.h>
#include <ulib/fmt/std/vector.h>
#include <ulib/fmt/std/map.h>
#include <ulib/fmt/std/list.h>
#include <ulib/fmt/std/array.h>
#include <ulib/fmt/std/pair.h>

#include <ulib/u16.h>
#include <ulib/u32.h>
#include <ulib/wchar.h>
#include <ulib/string.h>

TEST(Format, StdArray)
{
    std::vector<ulib::u32string> array = {U"hello", U"bye", U"fullplak"};
    auto result = ulib::format(u8"{}", array);

    ASSERT_EQ(result, u8R"(["hello", "bye", "fullplak"])");
}

TEST(Format, StdPair)
{
    std::pair<std::string, std::string> pair = {"ky", "plak"};
    auto result = ulib::format(u8"{}", pair);

    ASSERT_EQ(result, u8R"(["ky", "plak"])");
}

TEST(Format, StdVector)
{
    std::vector<ulib::u32string> vec = {U"hello", U"bye", U"fullplak"};
    auto result = ulib::format(u8"{}", vec);

    // fmt::print("\n\n\n\n");
    ASSERT_EQ(result, u8R"(["hello", "bye", "fullplak"])");
}

TEST(Format, StdList)
{
    std::list<ulib::u16string> list = {u"hello", u"bye", u"fullplak"};
    auto result = ulib::format(u8"{}", list);

    // fmt::print("\n\n\n\n");
    ASSERT_EQ(result, u8R"(["hello", "bye", "fullplak"])");
}

TEST(Format, StdMap)
{
    std::map<ulib::string, uint> map;
    map["port"] = 25565;
    map["password"] = 1332;
    map["count"] = 8;

    auto result = ulib::format(u8"{}", map);
    // ASSERT_EQ(result, u8"[[count, 8], [password, 1332], [port, 25565]]");
}
