#include <iostream>
#include "html_parser.h"
#include "html_renderer.h"
#include <iostream>

int main()
{
    std::string raw = R"(<html>   <head>    </head>    <body>    <div>    <p>   test    </p> <div>bla </div>  </div>   </body>   </html>)";
    std::string raw1 = R"(<html><div>    <p>   test    </p> </div><div>bla </div>  <div>third row </div> </html>)";
    std::string raw2 = R"(<p>bla</p><p>test</p>)";
    HtmlParser::ParseResult<HtmlParser::HtmlTree> dt = HtmlParser::parse(raw1);

    if (std::holds_alternative<std::string>(dt))
    {
        std::cout << std::get<std::string>(dt);
        return 0;
    }
    HtmlParser::HtmlTree htmlTree = std::get<HtmlParser::HtmlTree>(dt);
    std::string str = HtmlParser::to_string(htmlTree);

    std::cout << str;
    std::vector<HtmlParser::HtmlElement> unclosedElements = HtmlParser::notClosedElements(htmlTree);
    if (unclosedElements.size() > 0)
    {
        for (const auto &elem : unclosedElements)
        {
            std::cout << "Element not closed: " << elem.tagName << "\n";
        }
        return 0;
    }

    HtmlRenderer::render(htmlTree);

    return 0;
}