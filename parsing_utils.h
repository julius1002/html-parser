#pragma once

#include <string>

namespace ParsingUtils
{
    void skip_spaces_newlines(std::string raw, int &index);

    std::string parse_string(std::string raw, int &index);
}