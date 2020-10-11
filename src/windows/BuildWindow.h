/*
 * BuildWindow.h
 *
 *  Created on: 12.03.2018
 *      Author: stefan
 */

#ifndef WINDOWS_BUILDWINDOW_H_
#define WINDOWS_BUILDWINDOW_H_

#include "../BuildableObject.h"
#include "../Player.h"
#include <engine/ui/Window.h>
#include <engine/ui/scrollarea.h>
namespace windows {

class BuildWindow : public UI::Window {
public:
    BuildWindow(std::shared_ptr<Player> p);
    virtual ~BuildWindow();

    void addBuilding(std::shared_ptr<BuildableObject> object, const int planetRessources);
    void setPlayer(std::shared_ptr<Player> p);

    std::shared_ptr<BuildableObject> getSelectedBuilding();

    void clearBuildings();

private:
    std::shared_ptr<Player> player;
    int selection;
    std::shared_ptr<UI::ScrollArea> scrollArea;
    std::vector<std::shared_ptr<BuildableObject>> buildings;
};

} /* namespace windows */

#endif /* WINDOWS_BUILDWINDOW_H_ */
