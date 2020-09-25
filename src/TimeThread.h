/*
 * TimeThread.h
 *
 *  Created on: 12.03.2018
 *      Author: stefan
 */

#ifndef TIMETHREAD_H_
#define TIMETHREAD_H_

#include "gamestate.h"
#include <chrono>
#include <ctime>
#include <thread>

class TimeThread {
public:
    TimeThread(const std::shared_ptr<GameState>& gameState);
    virtual ~TimeThread();

    void stop();
    void start();
    void pause();

    std::chrono::system_clock::time_point getTime();

    void setSpeed(int value);

    int getSpeed() const;

    bool getPaused() const;

private:
    void update();
    std::thread thread;
    std::chrono::system_clock::time_point startTime;
    bool running;
    bool paused;
    int speed;
    std::shared_ptr<GameState> gameState;
};

#endif /* TIMETHREAD_H_ */
