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
        static RaceService instance;
        return instance;
    }

protected:
    virtual std::shared_ptr<Race> convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object);

private:
    RaceService();
    ~RaceService() = default;
    RaceService(const RaceService &) = delete;
    RaceService &operator=(const RaceService &) = delete;
};

#endif // RACESERVICE_H
