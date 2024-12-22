#pragma once

#include <string>
namespace HtmlParser
{
    class HtmlElement
    {
    public:
        std::string tagName;
        std::string content;
        std::vector<HtmlElement> children;
        bool isClosed() const
        {
            return closed;
        };
        void setClosed(bool closed)
        {
            this->closed = closed;
        };

    private:
        bool closed = false;
    };

    using HtmlTree = std::vector<HtmlElement>;

    template <typename T>
    using ParseResult = std::variant<std::string, T>;

    ParseResult<HtmlTree> parse(std::string raw);

    std::string to_string(HtmlTree dt);

    std::vector<HtmlElement> notClosedElements(HtmlTree dt);
}