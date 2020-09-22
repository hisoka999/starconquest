#include "parser.h"
#include <algorithm>
#include <iostream>

namespace utils {
namespace JSON {

    Parser::Parser()
    {
    }

    void Parser::parseString(const std::string& jsonData)
    {

        for (size_t i = 0; i < jsonData.length(); ++i) {
            char data = jsonData[i];
            switch (data) {
            case '[': //array begin
                //parse sub string
                parseArray(jsonData.substr(i + 1));
                break;
            case ']': //array end
                break;
            case '{': //object begin
                parseObject(jsonData.substr(i + 1));
                break;
            case '}': //object end
                break;
            case ':': //object end
                break;
            }
        }
    }

    std::vector<std::variant<int, float, std::string, std::shared_ptr<Object>>> Parser::parseArray(const std::string& jsonData)
    {
        std::vector<std::variant<int, float, std::string, std::shared_ptr<Object>>> vector;

        size_t startPos = jsonData.find_first_of("[");
        size_t endPos = jsonData.find_last_of("]");
        if (startPos > jsonData.size())
            return vector;

        std::string data = jsonData.substr(startPos + 1, endPos - startPos - 1);

        size_t splitPos = data.find_first_of(",");
        size_t lastSplitPos = 0;
        while (lastSplitPos < data.size()) {

            std::string attrValue = data.substr(lastSplitPos, splitPos - lastSplitPos);

            if (attrValue.find_first_of("{") < attrValue.size()) {
                splitPos = data.find_first_of("}", lastSplitPos) + 1;
                attrValue = data.substr(lastSplitPos, splitPos - lastSplitPos);
                attrValue = rtrim(ltrim(attrValue, "{"), "}");
                vector.push_back(parseObject(attrValue));
                splitPos = data.find_first_of(",", splitPos + 1);
            } else if (attrValue.find_first_of("\"") < attrValue.size()) {
                //found string
                vector.push_back(trim(attrValue, "\""));
            } else if (attrValue == "true") {
                vector.push_back(1);
            } else if (attrValue == "false") {
                vector.push_back(0);
            } else {
                vector.push_back(std::atoi(attrValue.c_str()));
            }
            //std::cout << "array value: " << attrValue << std::endl;

            if (splitPos > data.size())
                break;
            lastSplitPos = splitPos + 1;
            splitPos = data.find_first_of(",", splitPos + 1);
            //std::cout << "split pos: " << splitPos << std::endl;
            //std::cout << "last split pos: " << lastSplitPos << std::endl;
        };
        return vector;
    }

    std::shared_ptr<Object> Parser::parseObject(const std::string& jsonData)
    {
        std::shared_ptr<Object> object = std::make_shared<Object>();
        size_t lastAttr = 0;

        bool hasAttr = true;

        while (hasAttr) {
            size_t splitPos = jsonData.find_first_of(":", lastAttr);
            size_t endPos = jsonData.find_first_of(",", lastAttr);
            if (splitPos > jsonData.size())
                break;
            //std::cout << splitPos << ":" << endPos << std::endl;

            std::string attrName = jsonData.substr(lastAttr, splitPos - lastAttr);

            attrName.erase(std::find_if(attrName.rbegin(), attrName.rend(), [](unsigned char ch) {
                return ch != '\"' && !std::isspace(ch);
            }).base(),
                attrName.end());
            attrName.erase(attrName.begin(), std::find_if(attrName.begin(), attrName.end(), [](unsigned char ch) {
                return ch != '\"' && !std::isspace(ch);
            }));
            hasAttr = !attrName.empty();
            if (hasAttr) {
                if (endPos > jsonData.size())
                    endPos = jsonData.size();
                std::string attrValue = jsonData.substr(splitPos + 1, endPos - splitPos - 1);
                attrValue = trim(attrValue);
                //std::cout << attrName << ":" << attrValue << std::endl;
                //test if the value is a string
                if (attrValue.find_first_of("{") < attrValue.size()) {
                    lastAttr = attrValue.find_first_of("}");
                    attrValue = rtrim(ltrim(attrValue, "{"), "}");
                    object->setAttribute(attrName, parseObject(attrValue));
                } else if (attrValue.find_first_of("[") < attrValue.size()) {
                    lastAttr = attrValue.find_first_of("]");
                    attrValue = jsonData.substr(splitPos + 1, lastAttr - splitPos - 1);
                    object->setArrayAttribute(attrName, parseArray(attrValue));

                } else if (attrValue.find_first_of("\"") < attrValue.size()) {
                    //found string
                    object->setAttribute(attrName, trim(attrValue, "\""));
                }

                else if (attrValue == "true") {
                    object->setAttribute(attrName, 1);
                } else if (attrValue == "false") {
                    object->setAttribute(attrName, 0);
                } else {
                    object->setAttribute(attrName, std::atoi(attrValue.c_str()));
                }

                lastAttr = endPos + 1;
            }
        }

        return object;
    }
}
}
