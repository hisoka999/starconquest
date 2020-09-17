/*
 * Tab.cpp
 *
 *  Created on: 03.06.2016
 *      Author: stefan
 */

#include "engine/ui/Tab.h"

namespace UI {

Tab::Tab(Object* parent, std::string title)
    : UI::Container()
    , UI::Object(parent)
    , title(title)
{
}
std::string Tab::getTitle()
{
    return title;
}
void Tab::setTitle(std::string title)
{
    this->title = title;
}
Tab::~Tab()
{
    // TODO Auto-generated destructor stub
}
void Tab::render(core::Renderer* pRender)
{
    UI::Container::render(pRender);
}
void Tab::handleEvents(core::Input* pInput)
{
    UI::Container::handleEvents(pInput);
}

} /* namespace UI */
