#include <string>

namespace ParsingUtils
{
    void skip_spaces(std::string raw, int &index);

    std::string parse_string(std::string raw, int &index);
}