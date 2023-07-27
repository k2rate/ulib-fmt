#include <gtest/gtest.h>

#include <ulib/format.h>
#include <ulib/fmt/list.h>

#include <ulib/u16.h>
#include <ulib/u32.h>
#include <ulib/wchar.h>
#include <ulib/string.h>

TEST(Format, List)
{
    ulib::list<ulib::u32string> list = {U"hello", U"bye", U"fullplak"};
    auto result = ulib::format("{}", list);
 
    ASSERT_EQ(result, R"(["hello", "bye", "fullplak"])");
}
