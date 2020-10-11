#include "localisation.h"
#include <engine/utils/exceptions.h>
#include <engine/utils/string.h>
#include <fstream>
#include <iostream>
#include <sstream>

Localisation* Localisation::instance = nullptr;
std::once_flag Localisation::onceFlag;

Localisation::Localisation()
{
}

std::string Localisation::translate(const std::string& msgid) const
{
    if (translations.count(msgid) > 0) {
        return translations.at(msgid);
    }
    return msgid;
}

void Localisation::loadLocalisation(std::string filename)
{
    std::fstream file;
    std::istringstream is;
    std::string s;
    std::string group;
    std::cout << filename << std::endl;

    file.open(filename.c_str(), std::ios::in);
    if (!file.is_open()) {
        throw IOException(filename, "file does not exists");
    }
    std::string msgid = "";
    std::string msgstr = "";
    while (getline(file, s)) {

        std::size_t pos = 0;
        //std::cout << "line = " << s << std::endl;
        pos = s.find("#");
        if (pos < s.length() - 1) {

        } else {
            pos = s.find("msgid");
            if (pos < s.length() - 1) {
                std::string part = s.substr(pos + 5);
                msgid = utils::trim(utils::trim(part), "\"");
            } else {
                pos = s.find("msgstr");
                if (pos < s.length() - 1) {
                    std::string part = s.substr(pos + 6);
                    msgstr = utils::trim(utils::trim(part), "\"");
                    translations[msgid] = msgstr;
                }
            }
        }
    }
    //while(!s.empty());
    file.close();
}
