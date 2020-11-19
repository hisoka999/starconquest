#include "localisation.h"
#include <cstdlib>
#include <engine/utils/exceptions.h>
#include <engine/utils/string.h>
#include <fstream>
#include <iostream>
#include <magic_enum.hpp>
#include <sstream>
#ifdef __WIN32
#include <windows.h>
#endif

Localisation* Localisation::instance = nullptr;
std::once_flag Localisation::onceFlag;

Localisation::Localisation()
{
}

Language Localisation::getLang() const
{
    return lang;
}

std::string Localisation::getLanguage() const
{
    return language;
}

void Localisation::loadLanguage(const Language lang)
{
    std::string langName = std::string(magic_enum::enum_name(lang));
    std::string fileName = "locale/" + langName + "/LC_MESSAGES/starconquest.po";
    loadLocalisation(fileName);
}
void Localisation::detectLanguage()
{
    std::string lang = "en";
#ifdef __linux
    const char* envLang = std::getenv("LANG");
    language = std::string(envLang);
#elif __WIN32
    WORD Lang1 = GetLangFromLocale(GetThreadLocale());

    WORD Lang2 = GetLangFromLocale(LOCALE_USER_DEFAULT);
    WORD Lang3 = GetLangFromLocale(LOCALE_SYSTEM_DEFAULT);

    std::string s;
    if (int cch = GetLocaleInfo(Lang1, LOCALE_SLANGUAGE, 0, 0)) {
        s.resize(cch - 1);
        GetLocaleInfo(id, type, &*s.begin(), cch);
    }
    language = s;
#endif

    if (language.empty()) {
        language = lang;
    }

    language = language.substr(0, 2);
    this->lang = magic_enum::enum_cast<Language>(language).value();

    loadLanguage(this->lang);
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
