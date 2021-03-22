/*
 * TimeThread.cpp
 *
 *  Created on: 12.03.2018
 *      Author: stefan
 */

#include "TimeThread.h"

#include "messagetypes.h"
#include <engine/core/MessageSystem.h>
#include <engine/utils/os.h>

TimeThread::TimeThread(const std::shared_ptr<GameState> &gameState)
    : running(false), gameState(gameState), paused(false), speed(400)
{

    std::tm ttm = std::tm();
    ttm.tm_year = 200;
    ttm.tm_mon = 0;
    ttm.tm_mday = 1;

    std::time_t ttime = std::mktime(&ttm);
    startTime = std::chrono::system_clock::from_time_t(ttime);
    running = true;
    thread = std::thread(&TimeThread::update, this);
    utils::os::SetThreadName(&thread, "TimerThread");
    //thread.join();
}

TimeThread::~TimeThread()
{
    stop();
    thread.join();
}

void TimeThread::update()
{
    while (this->running)
    {

        while (!paused && running)
        {
            startTime += std::chrono::hours(24);
            std::this_thread::sleep_for(std::chrono::milliseconds(speed));

            std::time_t tmpTime = std::chrono::system_clock::to_time_t(startTime);

            std::tm *tm = std::localtime(&tmpTime);
            //gameState->updateFleetPosition(0);

            if (tm->tm_mday == 1)
            {
                for (const auto &player : gameState->getPlayers())
                {
                    gameState->updatePlayerState(player);
                }
                auto &msgSystem = core::MessageSystem<MessageTypes>::get();
                std::shared_ptr<core::Message<MessageTypes, int>> msg = std::make_shared<core::Message<MessageTypes, int>>(MessageTypes::NewMonth, 0);
                msgSystem.sendMessage(msg);
            }
        }
    }
}

bool TimeThread::getPaused() const
{
    return paused;
}

int TimeThread::getSpeed() const
{
    return speed;
}

void TimeThread::setSpeed(int value)
{
    speed = value;
}

void TimeThread::stop()
{
    running = false;
}

void TimeThread::start()
{
    running = true;
    paused = false;
}
void TimeThread::pause()
{
    paused = true;
}

std::chrono::system_clock::time_point TimeThread::getTime()
{
    return startTime;
}
