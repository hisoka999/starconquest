#ifndef LOCALISATION_H
#define LOCALISATION_H

#include <map>
#include <memory>
#include <mutex>

enum class Language : long {
    de,
    en
};

class Localisation {
public:
    static Localisation& Instance()
    {
        std::call_once(onceFlag, [] {
            initSingleton();
        });

        return *(instance);
    }

    void loadLanguage(const Language lang);
    void detectLanguage();

    std::string translate(const std::string& msgid) const;

private:
    void loadLocalisation(std::string filename);
    Localisation();
    ~Localisation() = default;
    Localisation(const Localisation&) = delete;
    Localisation& operator=(const Localisation&) = delete;
    static void initSingleton()
    {
        instance = new Localisation();
    }
    static Localisation* instance;
    static std::once_flag onceFlag;

    std::map<std::string, std::string> translations;
    std::string language;
    Language lang;
};

#endif // LOCALISATION_H
