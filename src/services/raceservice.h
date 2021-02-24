#ifndef RACESERVICE_H
#define RACESERVICE_H

#include "../Race.h"
#include "jsonservice.h"
#include <engine/utils/json/parser.h>
#include <memory>
#include <mutex>
#include <vector>

class RaceService : public services::JSONService<Race>
{
public:
    static RaceService &Instance()
    {
        std::call_once(onceFlag, [] {
            initSingleton();
        });

        return *(instance);
    }

protected:
    virtual std::shared_ptr<Race> convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object);

private:
    RaceService();
    ~RaceService() = default;
    RaceService(const RaceService &) = delete;
    RaceService &operator=(const RaceService &) = delete;

    static RaceService *instance;
    static std::once_flag onceFlag;

    static void initSingleton()
    {
        instance = new RaceService();
    }
};

#endif // RACESERVICE_H