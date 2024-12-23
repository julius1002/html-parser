#include <string>

namespace ParsingUtils
{
    void skip_spaces_newlines(std::string raw, int &index)
    {
        while (raw.at(index) == ' ' || raw.at(index) == '\n')
        {
            index++;
        }
    }

    std::string parse_string(std::string raw, int &index)
    {
        std::string output;

        while (isalnum(raw.at(index)) || isspace(raw.at(index)))
        {
            output += raw.at(index++);
        }

        return output;
    };
}
