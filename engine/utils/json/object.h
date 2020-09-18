#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <memory>
#include <string>
#include <variant>

namespace utils {
namespace JSON {

    class Object {
    public:
        Object();

        template <typename T>
        void setAttribute(const std::string& attr, T value)
        {
            attributes[attr] = value;
        }

        std::string getStringValue(const std::string& attr) const;
        int getIntValue(const std::string& attr) const;
        std::shared_ptr<Object> getObjectValue(const std::string& attr) const;

    private:
        std::map<std::string, std::variant<int, float, std::string, std::shared_ptr<Object>>> attributes;
    };
}
}

#endif // OBJECT_H
