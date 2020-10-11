#ifndef LOCALISATION_H
#define LOCALISATION_H

#include <map>
#include <memory>
#include <mutex>

class Localisation {
public:
    static Localisation& Instance()
    {
        std::call_once(onceFlag, [] {
            initSingleton();
        });

        return *(instance);
    }
    void loadLocalisation(std::string filename);

    std::string translate(const std::string& msgid) const;

private:
    Localisation();
    ~Localisation() = default;
    Localisation(const Localisation&) = delete;
    Localisation& operator=(const Localisation&) = delete;

    static Localisation* instance;
    static std::once_flag onceFlag;
    std::map<std::string, std::string> translations;

    static void initSingleton()
    {
        instance = new Localisation();
    }
};

#endif // LOCALISATION_H
