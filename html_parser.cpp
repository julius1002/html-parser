#include <string>
#include "html_parser.h"
#include <ostream>
#include <iostream>
#include <sstream>
#include <ranges>
#include <algorithm>
#include <numeric>
#include "parsing_utils.h"

namespace HtmlParser
{
    std::string to_string(const HtmlElement &htmlElement)
    {
        std::stringstream ss;
        std::vector<std::string> cs;
        std::ranges::transform(htmlElement.children,
                               std::back_inserter(cs),
                               [](HtmlElement he)
                               { return to_string(he); });

        std::string result = std::accumulate(cs.begin(), cs.end(), std::string());

        ss << "<" << htmlElement.tagName << ">" << htmlElement.content << result << "</" << htmlElement.tagName << ">";
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

    ParseResult<std::string> parse_tag(std::string raw, int &index)
    {
        std::string output;
        ParseResult<std::string> pr;
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
        pr.emplace<1>(output);
        return pr;
    };

    ParseResult<HtmlElement> parse_html_element(std::string raw, int &index)
    {
        HtmlElement he;
        ParseResult<char> isChar = parse_character('<', raw, index);
        if (std::holds_alternative<std::string>(isChar))
        {
            return std::get<std::string>(isChar);
        }
        ParseResult<std::string> tagName = parse_tag(raw, index);
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
            if (isalnum(raw.at(index)))
            {
                std::string content = ParsingUtils::parse_string(raw, index);
                he.content = content;
            }
            if (raw.at(index) == '<')
            {
                if (raw.at(index + 1) == '/')
                {
                    index += 2;
                    ParseResult<std::string> closingTag = parse_tag(raw, index);
                    if (closingTag.index() == 0)
                    {
                        // return error message
                        return std::get<0>(closingTag);
                    }
                    else if (std::get<1>(closingTag) != he.tagName)
                    {
                        std::stringstream ss;
                        ss << "Expected closing tag '" << std::get<1>(tagName) << "' but found: " << std::get<1>(closingTag) << "\n";
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
                            ParseResult<std::string> closingTag = parse_tag(raw, index);
                            if (closingTag.index() == 0)
                            {
                                return std::get<0>(closingTag);
                            }
                            else if (std::get<1>(closingTag) != he.tagName)
                            {
                                std::stringstream ss;
                                ss << "Expected closing tag '" << std::get<1>(tagName) << "' but found: " << std::get<1>(closingTag) << "\n";
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

        ParsingUtils::skip_spaces(raw, index);
        std::vector<HtmlElement> elements{};
        while (true)
        {
            if (static_cast<size_t>(index) == raw.length())
            {
                break;
            }
            ParseResult<HtmlElement> he = parse_html_element(raw, index);

            if (std::holds_alternative<std::string>(he))
            {
                return std::get<std::string>(he);
            }

            elements.push_back(std::get<HtmlElement>(he));
        }
        return elements;
    }
}