#include "ExecutionThread.h"
#include "ResearchAction.h"
#include <engine/utils/os.h>
#include <chrono>
#include <iostream>
#include <iomanip>

namespace AI
{
    ExecutionThread::ExecutionThread(const std::shared_ptr<GameState> &gameState)
        : gameState(gameState)
    {
        running = true;
        paused = false;
        thread = std::thread(&ExecutionThread::update, this);
        utils::os::SetThreadName(&thread, "AIExecutionThread");

        actions.push_back(std::make_shared<ResearchAction>());
    }

    ExecutionThread::~ExecutionThread()
    {
        stop();
        thread.join();
    }

    void ExecutionThread::stop()
    {

        running = false;
    }

    void ExecutionThread::start()
    {
        running = true;
        paused = false;
    }

    void ExecutionThread::pause()
    {
        paused = true;
    }

    void ExecutionThread::update()
    {
        while (this->running)
        {

            while (!paused && running)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::clock_t c_start = std::clock();
                auto t_start = std::chrono::high_resolution_clock::now();

                for (const auto &player : gameState->getPlayers())
                {
                    if (player != gameState->getHumanPlayer())
                    {
                        for (auto action : actions)
                        {
                            action->execute(gameState, player);
                        }
                    }
                }
                std::clock_t c_end = std::clock();
                auto t_end = std::chrono::high_resolution_clock::now();

                std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
                          << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n"
                          << "Wall clock time passed: "
                          << std::chrono::duration<double, std::milli>(t_end - t_start).count()
                          << " ms\n";
            }
        }
    }

} // namespace AI
