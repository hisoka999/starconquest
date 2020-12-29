#include "researchwindow.h"

ResearchWindow::ResearchWindow(const std::shared_ptr<Player>& player)
    : UI::Window(0, 0, 800, 400)
    , player(player)
{
    setTitle("Research");

    int x = 0;
    int y = 0;
    scrollArea = std::make_shared<UI::ScrollArea>(800 - 50, 400 - 50, this);
    scrollArea->setPos(5, 5);
    addObject(scrollArea);

    auto researchList = player->getRace().getAvailableResearch();

    buildResearchTree(x + 10, y + 10, nullptr, researchList);
    scrollArea->reset();
}

void ResearchWindow::buildResearchTree(int x, int y, const std::shared_ptr<Research>& baseResearch, const std::vector<std::shared_ptr<Research>>& researchList)
{
    //filter List
    std::vector<std::shared_ptr<Research>> filteredList;
    for (auto res : researchList) {
        if (baseResearch == nullptr && res->getRequirements().empty()) {
            filteredList.push_back(res);
        } else {

            for (auto req : res->getRequirements()) {
                if (req == baseResearch) {
                    filteredList.push_back(res);
                }
            }
        }
    }
    for (auto& res : filteredList) {
        auto researchButton = std::make_shared<UI::ResearchButton>(res, player, scrollArea.get());
        researchButton->setPos(x, y);
        researchButton->connect("buttonClicked", [=]() {
            player->addResearchToQueue(res);
        });
        scrollArea->addObject(researchButton);
        buildResearchTree(x + 300, y, res, researchList);
        y += 70;
    }
}
