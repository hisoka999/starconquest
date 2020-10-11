#ifndef SHIPYARDTAB_H
#define SHIPYARDTAB_H

#include <engine/ui/Tab.h>
#include <engine/ui/scrollarea.h>
#include "BuildableObjectView.h"
#include <engine/ui/Button.h>
namespace UI {

class ShipyardTab : public UI::Tab
{
public:
    ShipyardTab(Object* parent, std::string title);


private:
    std::shared_ptr<UI::ScrollArea> buildArea;
    std::shared_ptr<UI::Button> buildButton;

    //TODO ship details

};
}


#endif // SHIPYARDTAB_H
