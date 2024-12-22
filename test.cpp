#include <iostream>
#include <assert.h>
#include "html_parser.h"

void tc001()
{
    std::string raw = R"(<html>)";
    HtmlParser::ParseResult<HtmlParser::HtmlTree> dt = HtmlParser::parse(raw);
    assert(std::holds_alternative<std::string>(dt));
    assert(std::get<std::string>(dt) == "Error, string out of range");
}

void tc002()
{
    std::string raw = R"(<html></html>)";
    HtmlParser::ParseResult<HtmlParser::HtmlTree> dt = HtmlParser::parse(raw);

    assert(std::holds_alternative<HtmlParser::HtmlTree>(dt));
    HtmlParser::HtmlTree d = std::get<HtmlParser::HtmlTree>(dt);
    std::string str = HtmlParser::to_string(d);
    assert(str == "<html></html>");
}

void tc003()
{
    std::string raw = R"(<html><head></head><body></body></html>)";
    HtmlParser::ParseResult<HtmlParser::HtmlTree> dt = HtmlParser::parse(raw);

    assert(std::holds_alternative<HtmlParser::HtmlTree>(dt));
    HtmlParser::HtmlTree d = std::get<HtmlParser::HtmlTree>(dt);
    std::string str = HtmlParser::to_string(d);
    assert(str == "<html><head></head><body></body></html>");
}

void tc004()
{
    std::string raw = R"(<html><head></head><body><div><p></p></div></body></html>)";
    HtmlParser::ParseResult<HtmlParser::HtmlTree> dt = HtmlParser::parse(raw);

    assert(std::holds_alternative<HtmlParser::HtmlTree>(dt));
    HtmlParser::HtmlTree d = std::get<HtmlParser::HtmlTree>(dt);
    std::string str = HtmlParser::to_string(d);
    assert(str == "<html><head></head><body><div><p></p></div></body></html>");
}

void tc005()
{
    std::string raw = R"(<html><head></head><body><div><p>test</p></div></body></html>)";
    HtmlParser::ParseResult<HtmlParser::HtmlTree> dt = HtmlParser::parse(raw);

    assert(std::holds_alternative<HtmlParser::HtmlTree>(dt));
    HtmlParser::HtmlTree d = std::get<HtmlParser::HtmlTree>(dt);
    std::string str = HtmlParser::to_string(d);
    assert(str == "<html><head></head><body><div><p>test</p></div></body></html>");
}

int main()
{
    tc001();
    tc002();
    tc003();
    tc004();
    tc005();
    std::cout << "All tests worked!" << std::endl;
    return 0;
}