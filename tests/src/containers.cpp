#include <gtest/gtest.h>

// #include <ulib/format.h>
#include <ulib/fmt/list.h>
#include <ulib/string.h>
#include <ulib/format.h>

// TEST(Format, List)
// {
//     ulib::list<ulib::u32string> list = {U"hello", U"bye", U"fullplak"};
//     auto result = ulib::format("{}", list);

//     ASSERT_EQ(result, R"(["hello", "bye", "fullplak"])");
// }

TEST(GithubCICheck, Check0) { ASSERT_EQ(1, 1); }

TEST(GithubCICheck, Check1)
{
    std::string str;
    ASSERT_EQ(2, 2);
}

TEST(GithubCICheck, Check2)
{
    ulib::string str;
    ASSERT_EQ(3, 3);
}

TEST(GithubCICheck, Check3)
{
    ulib::list<ulib::u32string> list = {U"hello", U"bye", U"fullplak"};
    auto result = ulib::format("{}", list);

    ASSERT_EQ(result, R"(["hello", "bye", "fullplak"])");
}
