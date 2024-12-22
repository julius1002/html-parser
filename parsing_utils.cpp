#include <string>

void skip_spaces(std::string raw, int &index)
{
    while (raw.at(index) == ' ')
    {
        index++;
    }
}