#include "object.h"
namespace utils {
namespace JSON {

    Object::Object()
    {
    }

    std::string Object::getStringValue(const std::string& attr) const
    {
        return std::get<std::string>(attributes.at(attr));
    }
    int Object::getIntValue(const std::string& attr) const
    {
        return std::get<int>(attributes.at(attr));
    }
    std::shared_ptr<Object> Object::getObjectValue(const std::string& attr) const
    {
        return std::get<std::shared_ptr<Object>>(attributes.at(attr));
    }

}
}
