/*
 * BuildableObjectView.h
 *
 *  Created on: 13.03.2018
 *      Author: stefan
 */

#ifndef UI_BUILDABLEOBJECTVIEW_H_
#define UI_BUILDABLEOBJECTVIEW_H_

#include "../BuildableObject.h"
#include <engine/ui/Container.h>
#include <engine/ui/ImageButton.h>
#include <engine/ui/Label.h>
#include <engine/ui/Object.h>

namespace UI {

class BuildableObjectView : public Object, private Container {
public:
    BuildableObjectView(std::shared_ptr<BuildableObject> object, Object* parent);
    virtual void render(core::Renderer* pRender);
    virtual void handleEvents(core::Input* pInput);
    virtual ~BuildableObjectView();

private:
    void buildUI();
    std::shared_ptr<BuildableObject> object;
    std::shared_ptr<UI::Label> titleLabel;
    std::shared_ptr<UI::Label> priceLabel;
    std::shared_ptr<UI::ImageButton> buildingImage;
};

} /* namespace UI */

#endif /* UI_BUILDABLEOBJECTVIEW_H_ */
