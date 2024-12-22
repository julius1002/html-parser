#include <iostream>
#include "html_parser.h"

int main()
{
    std::string raw = R"(  <html>   <head>    </head>    <body>    <div>    <p>   test    </p>   </div>   </body>   </html>)";
    HtmlParser::ParseResult<HtmlParser::HtmlTree> dt = HtmlParser::parse(raw);

    if (std::holds_alternative<std::string>(dt))
    {
        std::cout << std::get<std::string>(dt);
    }
    else
    {
        HtmlParser::HtmlTree d = std::get<HtmlParser::HtmlTree>(dt);
        std::string str = HtmlParser::to_string(d);

        std::cout << str;
        std::vector<HtmlParser::HtmlElement> unclosedElements = HtmlParser::notClosedElements(d);
        for (const auto &elem : unclosedElements)
        {
            std::cout << "Element not closed: " << elem.tagName << "\n";
        }
    }
    return 0;
}