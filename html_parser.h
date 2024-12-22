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
        }
        void setClosed(bool closed){
            this->closed = closed;
        }

    private:
        bool closed = false;
    };

    using DomTree = std::vector<HtmlElement>;

    template <typename T>
    using ParseResult = std::variant<std::string, T>;

    ParseResult<DomTree> parse(std::string raw);

    std::string to_string(DomTree dt);

    std::vector<HtmlElement> notClosedElements(DomTree dt);
}