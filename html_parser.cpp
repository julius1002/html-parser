#include <string>
#include "html_parser.h"
#include <ostream>
#include <iostream>
#include <sstream>
#include <ranges>
#include <algorithm>
#include <numeric>
#include "parsing_utils.h"
#include <unordered_map>
#include <stdexcept>

namespace HtmlParser
{
    const char *serialize_html_tag(HtmlTag tag)
    {
        switch (tag)
        {
        case HtmlTag::HTML:
            return "html";
        case HtmlTag::HEAD:
            return "head";
        case HtmlTag::BODY:
            return "body";
        case HtmlTag::TITLE:
            return "title";
        case HtmlTag::SCRIPT:
            return "script";
        case HtmlTag::STYLE:
            return "style";
        case HtmlTag::DIV:
            return "div";
        case HtmlTag::P:
            return "p";
        case HtmlTag::A:
            return "a";
        case HtmlTag::IMG:
            return "img";
        case HtmlTag::UL:
            return "ul";
        case HtmlTag::OL:
            return "ol";
        case HtmlTag::LI:
            return "li";
        case HtmlTag::TABLE:
            return "table";
        case HtmlTag::TR:
            return "tr";
        case HtmlTag::TH:
            return "th";
        case HtmlTag::TD:
            return "td";
        case HtmlTag::FORM:
            return "form";
        case HtmlTag::INPUT:
            return "input";
        case HtmlTag::BUTTON:
            return "button";
        case HtmlTag::H1:
            return "h1";
        case HtmlTag::H2:
            return "h2";
        case HtmlTag::H3:
            return "h3";
        case HtmlTag::H4:
            return "h4";
        case HtmlTag::H5:
            return "h5";
        case HtmlTag::H6:
            return "h6";
        case HtmlTag::NAV:
            return "nav";
        default:
            return "unknown";
        }
    }

    std::optional<HtmlTag> parse_string_to_tag(const std::string &tagName)
    {
        static const std::unordered_map<std::string, HtmlTag> tagMap = {
            {"html", HtmlTag::HTML},
            {"head", HtmlTag::HEAD},
            {"body", HtmlTag::BODY},
            {"script", HtmlTag::SCRIPT},
            {"style", HtmlTag::STYLE},
            {"div", HtmlTag::DIV},
            {"span", HtmlTag::SPAN},
            {"p", HtmlTag::P},
            {"a", HtmlTag::A},
            {"img", HtmlTag::IMG},
            {"ul", HtmlTag::UL},
            {"ol", HtmlTag::OL},
            {"li", HtmlTag::LI},
            {"table", HtmlTag::TABLE},
            {"tr", HtmlTag::TR},
            {"th", HtmlTag::TH},
            {"td", HtmlTag::TD},
            {"form", HtmlTag::FORM},
            {"input", HtmlTag::INPUT},
            {"button", HtmlTag::BUTTON},
            {"h1", HtmlTag::H1},
            {"h2", HtmlTag::H2},
            {"h3", HtmlTag::H3},
            {"h4", HtmlTag::H4},
            {"h5", HtmlTag::H5},
            {"h6", HtmlTag::H6},
            {"title", HtmlTag::TITLE}};

        auto it = tagMap.find(tagName);
        if (it != tagMap.end())
        {
            return {it->second};
        }
        else
        {
            return {};
        }
    }

    std::string to_string(const HtmlElement &htmlElement)
    {
        std::stringstream ss;
        std::vector<std::string> cs;
        std::ranges::transform(htmlElement.children,
                               std::back_inserter(cs),
                               [](HtmlElement he)
                               { return to_string(he); });

        std::string result = std::accumulate(cs.begin(), cs.end(), std::string());
        std::string tagString = serialize_html_tag(htmlElement.tagName);
        ss << "<" << tagString << ">" << htmlElement.content << result << "</" << tagString << ">";
        return ss.str();
    }

    std::string to_string(HtmlTree dt)
    {
        std::stringstream ss;
        for (auto const &e : dt)
        {
            ss << to_string(e);
        }
        return ss.str();
    }

    ParseResult<char> parse_character(char ch, std::string raw, int &index)
    {
        if (raw.at(index) == ch)
        {
            index++;
            return ch;
        }
        else
        {
            std::stringstream ss;
            ss << "Expected element '" << ch << "' but found '" << raw.at(index) << "' at index: " << index << "\n";
            return ss.str();
        }
    }

    ParseResult<HtmlTag> parse_tag(std::string raw, int &index)
    {
        ParseResult<HtmlTag> pr;

        std::string output;
        if (!isalpha(raw.at(index)))
        {
            std::stringstream ss;
            ss << "Expected alphanumeric element but found '" << raw.at(index) << "' at index: " << index << "\n";
            pr.emplace<0>(ss.str());
            return pr;
        }
        while (isalpha(raw.at(index)))
        {
            output += raw.at(index++);
        }
        if (raw.at(index) != '>')
        {
            std::stringstream ss;
            ss << "Expected '>' but found '" << raw.at(index) << "' at index: " << index << "\n";
            pr.emplace<0>(ss.str());
            return pr;
        }
        index++;
        std::optional<HtmlTag> optionalTag = parse_string_to_tag(output);
        if (optionalTag.has_value())
        {
            pr.emplace<1>(optionalTag.value());
        }
        else
        {
            pr.emplace<0>("Unknown HTML tag\n");
        }

        return pr;
    }

    ParseResult<HtmlElement> parse_html_element(std::string raw, int &index)
    {
        HtmlElement he;
        ParseResult<char> isChar = parse_character('<', raw, index);

        if (std::holds_alternative<std::string>(isChar))
        {
            return std::get<std::string>(isChar);
        }

        ParseResult<HtmlTag> tagName = parse_tag(raw, index);

        if (tagName.index() == 0)
        {
            // return error message
            return std::get<0>(tagName);
        }
        else
        {
            he.tagName = std::get<1>(tagName);
        }
        try
        {
            ParsingUtils::skip_spaces_newlines(raw, index);
            if (isalnum(raw.at(index)))
            {
                std::string content = ParsingUtils::parse_string(raw, index);
                he.content = content;
            }
            ParsingUtils::skip_spaces_newlines(raw, index);
            if (raw.at(index) == '<')
            {
                if (raw.at(index + 1) == '/')
                {
                    index += 2;
                    ParseResult<HtmlTag> closingTag = parse_tag(raw, index);

                    if (closingTag.index() == 0)
                    {
                        // return error message
                        return std::get<0>(closingTag);
                    }
                    else if (std::get<1>(closingTag) != he.tagName)
                    {
                        std::stringstream ss;
                        ss << "Expected closing tag '" << serialize_html_tag(std::get<1>(tagName)) << "' but found: " << serialize_html_tag(std::get<1>(closingTag)) << "\n";
                        return ss.str();
                    }
                    else
                    {
                        he.setClosed(true);
                    }
                }
                else
                {
                    while (true)
                    {
                        ParsingUtils::skip_spaces_newlines(raw, index);
                        if (raw.at(index) == '<' && raw.at(index + 1) == '/')
                        {
                            break;
                        }
                        ParseResult<HtmlElement> child = parse_html_element(raw, index);

                        if (std::holds_alternative<std::string>(child))
                        {
                            return std::get<std::string>(child);
                        }
                        else
                        {
                            he.children.push_back(std::get<HtmlElement>(child));
                        }
                    }

                    if (raw.at(index) == '<')
                    {
                        if (raw.at(index + 1) == '/')
                        {
                            index += 2;
                            ParseResult<HtmlTag> closingTag = parse_tag(raw, index);

                            if (closingTag.index() == 0)
                            {
                                return std::get<0>(closingTag);
                            }
                            else if (std::get<1>(closingTag) != he.tagName)
                            {
                                std::stringstream ss;
                                ss << "Expected closing tag '" << serialize_html_tag(std::get<1>(tagName)) << "' but found: " << serialize_html_tag(std::get<1>(closingTag)) << "\n";
                                return ss.str();
                            }
                            else
                            {
                                he.setClosed(true);
                            }
                        }
                        else
                        {
                            std::stringstream ss;
                            ss << "Expected closing tag '/' but found: " << raw.at(index + 1) << "\n";
                            return ss.str();
                        }
                    }
                    else
                    {
                        std::stringstream ss;
                        ss << "Expected closing tag '<' but found: '" << raw.at(index) << "'\n";
                        return ss.str();
                    }
                }
            }
        }
        catch (std::out_of_range exception)
        {
            return "Error, string out of range";
        }
        return he;
    }

    std::vector<HtmlElement> notClosedElements(HtmlTree dt)
    {
        std::vector<HtmlElement> result;
        std::ranges::copy(
            dt | std::views::filter(
                     [](const HtmlElement &e)
                     { return !e.isClosed(); }),
            std::back_inserter(result));
        return result;
    }

    ParseResult<HtmlTree> parse(std::string raw)
    {
        int index = 0;

        std::vector<HtmlElement> elements{};
        while (true)
        {
            ParsingUtils::skip_spaces_newlines(raw, index);
            ParseResult<HtmlElement> he = parse_html_element(raw, index);
            if (std::holds_alternative<std::string>(he))
            {
                return std::get<std::string>(he);
            }
            elements.push_back(std::get<HtmlElement>(he));
            if (static_cast<size_t>(index) >= raw.length())
            {
                break;
            }
        }
        return elements;
    }
}