#include <gtest/gtest.h>

#include <ulib/format.h>
#include <ulib/fmt/path.h>

#include <ulib/u16.h>
#include <ulib/u32.h>
#include <ulib/wchar.h>
#include <ulib/string.h>

TEST(Format, Path)
{
    auto path = std::filesystem::current_path();
    auto result = ulib::format(u8"{}", path);

    ASSERT_EQ(ulib::u8(path.u8string()), result);
}
