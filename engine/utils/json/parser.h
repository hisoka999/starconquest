#ifndef PARSER_H
#define PARSER_H

#include "object.h"
#include <memory>
#include <string>
#include <vector>

namespace utils {

const static char* ws = " \t\n\r\f\v";

// trim from end of string (right)
inline std::string& rtrim(std::string& s, const char* t = ws)
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from beginning of string (left)
inline std::string& ltrim(std::string& s, const char* t = ws)
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from both ends of string (right then left)
inline std::string& trim(std::string& s, const char* t = ws)
{
    return ltrim(rtrim(s, t), t);
}

namespace JSON {

    class Parser {
    public:
        Parser();

        void parseString(const std::string& jsonData);
        std::vector<std::variant<int, float, std::string, std::shared_ptr<Object>>> parseArray(const std::string& jsonData);
        std::shared_ptr<Object> parseObject(const std::string& jsonData);

    private:
        void parseAttribute(const std::string& jsonData);
    };
}
}

#endif // PARSER_H
