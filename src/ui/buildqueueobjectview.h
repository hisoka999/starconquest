#ifndef BUILDQUEUEOBJECTVIEW_H
#define BUILDQUEUEOBJECTVIEW_H

#include "../Planet.h"
#include <engine/ui/Container.h>
#include <engine/ui/ImageButton.h>
#include <engine/ui/Label.h>
#include <engine/ui/Object.h>
#include <engine/ui/Button.h>

namespace UI{

class BuildQueueObjectView : public Object, private Container {

public:
    BuildQueueObjectView(BuildQueueElement* object, Object* parent);
    virtual void render(core::Renderer* pRender);
    virtual void handleEvents(core::Input* pInput);
    virtual ~BuildQueueObjectView();
    void update(const int planetRessources);

private:
    void buildUI();
    BuildQueueElement* object;
    std::shared_ptr<UI::Label> titleLabel;
    std::shared_ptr<UI::Label> priceLabel;
    std::shared_ptr<UI::Label> timeLabel;
    std::shared_ptr<UI::ImageButton> buildingImage;
    std::shared_ptr<UI::Button> cancelButton;
};

} /* namespace UI */

#endif // BUILDQUEUEOBJECTVIEW_H
