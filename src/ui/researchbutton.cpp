#include "researchbutton.h"
#include "../Player.h"
#include "engine/graphics/TextureManager.h"
#include <engine/utils/os.h>
#include <engine/utils/string.h>
#include "../translate.h"
namespace UI
{
    ResearchButton::ResearchButton(const std::shared_ptr<Research> &research, std::shared_ptr<Player> &player, UI::Object *parent)
        : UI::Object(parent), research(research), player(player)
    {
        iconFont = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 20);
        textFont = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts", "Audiowide-Regular.ttf"), 12);

        smallIconFont = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 8);
        smallTextFont = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts", "Audiowide-Regular.ttf"), 8);

        setWidth(250);
        setHeight(40);
    }

    ResearchButton::~ResearchButton()
    {
        iconFont = nullptr;
        textFont = nullptr;
        smallIconFont = nullptr;
        smallTextFont = nullptr;
    }

    void ResearchButton::render(core::Renderer *pRender)
    {
        auto rect = displayRect();
        pRender->setDrawColor(12, 21, 24, 255);

        pRender->fillRect(rect);
        if (research->canResearch())
            pRender->setDrawColor(0, 200, 200, 255);
        else
        {
            pRender->setDrawColor(93, 103, 108, 255);
        }
        pRender->drawRect(rect);

        auto iconRect = rect;

        iconRect.width = rect.height;
        pRender->fillRect(iconRect);
        SDL_Color textColor = {255, 255, 255, 0};
        if (hovered)
        {
            textColor = {93, 103, 108, 255};
        }
        iconFont->render(pRender, research->getIcon(), textColor, rect.x + 10, rect.y + 10);

        textFont->render(pRender, research->getLocalisedName(), textColor, rect.x + rect.height + 10, rect.y + 5);

        //draw research time
        if (research->getResearched())
        {
            smallTextFont->render(pRender, _("researched"), textColor, rect.x + rect.width - 70, rect.y + rect.height - 10);
        }
        else
        {
            smallIconFont->render(pRender, "\uf254", textColor, rect.x + rect.width - 70, rect.y + rect.height - 10);
            int resPerMonth = (player->getResearchPerMonth() == 0) ? 1 : player->getResearchPerMonth();
            auto researchTime = utils::string_format("%i months", research->getCurrentCosts() / resPerMonth);
            smallTextFont->render(pRender, researchTime, textColor, rect.x + rect.width - 60, rect.y + rect.height - 10);
        }
    }

    void ResearchButton::handleEvents(core::Input *pInput)
    {
        auto rect = eventRect();
        if (rect.intersects(pInput->getMousePostion()) && research->canResearch() && !research->getResearched())
        {
            hovered = true;
            if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
            {
                this->fireFuncionCall("buttonClicked");
            }
        }
        else
        {
            hovered = false;
        }
    }

}
