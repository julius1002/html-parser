#include <iostream>
#include "html_parser.h"
#include "html_renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string read_file(std::string location)
{
    std::ifstream file(location);

    // Check if the file is open
    if (!file.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        return ""; // Exit with an error code
    }

    std::string line;

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string fileContents = buffer.str();

    file.close();
    return fileContents;
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        std::cerr << "Please provide a valid path to a html file as parameter.\n";
        exit(1);
    }

    std::string content = read_file(argv[1]);
    HtmlParser::ParseResult<HtmlParser::HtmlTree> dt = HtmlParser::parse(content);

    if (std::holds_alternative<std::string>(dt))
    {
        std::cout << std::get<std::string>(dt);
        return 0;
    }
    HtmlParser::HtmlTree htmlTree = std::get<HtmlParser::HtmlTree>(dt);
    std::string str = HtmlParser::to_string(htmlTree);

    std::cout << str;
    std::vector<HtmlParser::HtmlElement*> unclosedElements = HtmlParser::notClosedElements(htmlTree);
    if (unclosedElements.size() > 0)
    {
        for (const auto &elem : unclosedElements)
        {
            std::cout << "Element not closed: " << HtmlParser::serialize_html_tag(elem->tagName) << "\n";
        }
        return 0;
    }

    HtmlRenderer::render(htmlTree);
    HtmlParser::clean_up(htmlTree);

    return 0;
}