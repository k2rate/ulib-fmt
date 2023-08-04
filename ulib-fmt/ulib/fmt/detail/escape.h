#pragma once

#include <ulib/string.h>
#include <ulib/typetraits/string.h>

namespace ulib::fmt::detail
{
    inline ulib::u8string EscapeString(ulib::u8string_view view)
    {
        ulib::u8string result(args::Capacity{view.size() * 2});
        for (auto ch : view)
        {
            if (ch == '\n')
                result.append(u8"\\n");
            else if (ch == '\t')
                result.append(u8"\\t");
            else if (ch == '\r')
                result.append(u8"\\r");
            else if (ch == '"')
                result.append(u8"\"");
            else
                result.push_back(ch);
        }

        return result;
    }
} // namespace ulib::fmt::detail