#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H
#include <string>
#include <vector>
#include <map>
#include <sstream>


class httpParser
{
private:
    std::vector<std::pair<std::string, std::string>> http;
    std::string format_key(std::string &str);

public:
    httpParser(char *msg);
    ~httpParser();
    auto getHttp() { return this->http; };
};

#endif