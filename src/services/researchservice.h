#ifndef RESEARCHSERVICE_H
#define RESEARCHSERVICE_H

#include "../research.h"
#include "jsonservice.h"
#include <engine/utils/json/parser.h>
#include <memory>
#include <mutex>
#include <vector>

std::string utf8chr(int cp);

class ResearchService : public services::JSONService<Research> {
public:
    static ResearchService& Instance()
    {
        std::call_once(onceFlag, [] {
            initSingleton();
        });

        return *(instance);
    }

    void loadResearch(std::string fileName);

protected:
    std::shared_ptr<Research> convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object>& object);

private:
    ResearchService();
    ~ResearchService() = default;
    ResearchService(const ResearchService&) = delete;
    ResearchService& operator=(const ResearchService&) = delete;

    static ResearchService* instance;
    static std::once_flag onceFlag;

    static void initSingleton()
    {
        instance = new ResearchService();
    }
};
#endif // RESEARCHSERVICE_H
