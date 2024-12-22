#include <string>

namespace ParsingUtils
{

    void skip_spaces(std::string raw, int &index)
    {
        while (raw.at(index) == ' ')
        {
            index++;
        }
    }

    std::string parse_string(std::string raw, int &index)
    {
        std::string output;

        while (isalpha(raw.at(index)))
        {
            output += raw.at(index++);
        }

        return output;
    };
}