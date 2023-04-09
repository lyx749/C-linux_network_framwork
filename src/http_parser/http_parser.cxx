#include "http_parser.h"
#include <iostream>
httpParser::httpParser(char *msg)
{
    std::string buf(msg);
    std::istringstream bufStream(buf);
    enum parts
    {
        startLine,
        headers,
        body
    };

    parts part = startLine;
    std::string line;
    std::string bodyString;
    while (std::getline(bufStream, line))
    {
        switch (part)
        {
        case startLine:
        {
            std::istringstream line_stream(line);
            std::string tmp;
            line_stream >> tmp;
            if (tmp.find("HTTP") == std::string::npos)
            {
                http.push_back(std::make_pair("method", tmp));
                line_stream >> tmp;
                http.push_back(std::make_pair("path", tmp));
                line_stream >> tmp;
                http.push_back(std::make_pair("version", tmp));
            }
            else
            {
                http.push_back(std::make_pair("version", tmp));
                line_stream >> tmp;
                http.push_back(std::make_pair("status", tmp));
                line_stream >> tmp;
                http.push_back(std::make_pair("status_text", tmp));
            }
            part = headers;
            break;
        }
        case headers:
        {
            if (line.size() == 1)
            {
                part = body;
                break;
            }
            auto pos = line.find(":");
            if (pos == std::string::npos)
                continue;
            std::string tmp1(line, 0, pos);
            std::string tmp2(line, pos + 2);
            http.push_back(std::make_pair(format_key(tmp1), tmp2));
            break;
        }
        case body:
        {
            bodyString.append(line);
            bodyString.push_back('\n');
            break;
        }
        default:
            break;
        }
    }
    http.push_back(std::make_pair("body", bodyString));
}

httpParser::~httpParser()
{
    http.clear();
}

std::string httpParser::format_key(std::string &str)
{
    if (str[0] >= 'a' && str[0] <= 'z')
    {
        str[0] = str[0] + 'A' - 'a';
    }
    int position = 0;
    while ((position = str.find("-", position)) != std::string::npos)
    {
        if (str[position + 1] >= 'a' && str[position + 1] <= 'z')
        {
            str[position + 1] = str[position + 1] + 'A' - 'a';
        }
        position++;
    }
    return str;
}
