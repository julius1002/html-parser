#pragma once

#include <string>
namespace HtmlParser
{
    enum class HtmlTag
    {
        HTML,
        HEAD,
        BODY,
        TITLE,
        META,
        LINK,
        SCRIPT,
        STYLE,
        DIV,
        SPAN,
        P,
        A,
        IMG,
        UL,
        OL,
        LI,
        TABLE,
        TR,
        TH,
        TD,
        FORM,
        INPUT,
        BUTTON,
        H1,
        H2,
        H3,
        H4,
        H5,
        H6,
        HR,
        BR,
        PRE,
        CODE,
        STRONG,
        I,
        B,
        U,
        S,
        Q,
        SMALL,
        BIG,
        FIGURE,
        ADDRESS,
        HEADER,
        FOOTER,
        ARTICLE,
        SECTION,
        NAV,
        SVG,
        AUDIO,
        VIDEO,
        AREA,
    };

    class HtmlElement
    {
    public:
        HtmlTag tagName;
        std::string content;
        std::vector<HtmlElement*> children;
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

    using HtmlTree = std::vector<HtmlElement*>;

    template <typename T>
    using ParseResult = std::variant<std::string, T>;

    ParseResult<HtmlTree> parse(std::string raw);

    std::string to_string(HtmlTree dt);

    std::vector<HtmlElement*> notClosedElements(HtmlTree htmlTree);

    const char *serialize_html_tag(HtmlTag tag);

    void clean_up(HtmlTree dt);
}