#ifndef RESEARCHBUTTON_H
#define RESEARCHBUTTON_H

#include "../research.h"
#include <engine/ui/Object.h>
class Player;

namespace UI {

class ResearchButton : public UI::Object {
public:
    ResearchButton(const std::shared_ptr<Research>& research, std::shared_ptr<Player>& player, UI::Object* parent = nullptr);

    void render(core::Renderer* pRender);

    void handleEvents(core::Input* pInput);

private:
    std::shared_ptr<Research> research;
    std::shared_ptr<Player>& player;

    std::shared_ptr<graphics::Text> iconFont;
    std::shared_ptr<graphics::Text> textFont;

    std::shared_ptr<graphics::Text> smallIconFont;
    std::shared_ptr<graphics::Text> smallTextFont;

    bool hovered = false;
    bool clicked = false;
};

}

#endif // RESEARCHBUTTON_H
