#pragma once

#include "../gamestate.h"
#include "Action.h"
#include <vector>

#include <thread>
#include <atomic>

namespace AI
{

    class ExecutionThread
    {
    public:
        ExecutionThread(const std::shared_ptr<GameState> &gameState);
        ~ExecutionThread();
        void stop();
        void start();
        void pause();

    private:
        void update();
        bool paused;

        std::thread thread;
        std::atomic_bool running;
        std::vector<std::shared_ptr<Action>> actions;
        std::shared_ptr<GameState> gameState;
    };
} // namespace AI
