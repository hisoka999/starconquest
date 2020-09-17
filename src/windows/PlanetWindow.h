/*
 * PlanetWindow.h
 *
 *  Created on: 29.05.2016
 *      Author: stefan
 */

#ifndef WINDOWS_PLANETWINDOW_H_
#define WINDOWS_PLANETWINDOW_H_

#include "../Planet.h"
#include "BuildWindow.h"
#include <engine/graphics/text.h>
#include <engine/ui/Button.h>
#include <engine/ui/ImageButton.h>
#include <engine/ui/Label.h>
#include <engine/ui/Tab.h>
#include <engine/ui/Window.h>
#include <engine/ui/scrollarea.h>
#include <memory>

namespace windows {

class PlanetWindow : public UI::Window {
public:
    PlanetWindow();
    virtual ~PlanetWindow();
    void setPlanet(const std::shared_ptr<Planet>& planet);
    void setPlayer(const std::shared_ptr<Player>& player);
    virtual void render(core::Renderer* pRender, std::shared_ptr<graphics::Texture> pTexture);
    virtual void handleEvents(core::Input* pInput);

private:
    void onSurfaceClick(const utils::Vector2& pos);
    virtual void onOpen();
    void updateData();

    std::shared_ptr<graphics::Text> uiText;
    std::shared_ptr<graphics::Text> uiIconText;
    std::shared_ptr<UI::Button> colonizeButton;
    std::shared_ptr<UI::Label> nameLabel;
    std::shared_ptr<UI::Label> typeLabel;
    std::shared_ptr<UI::Label> planetOwnerLabel;
    std::shared_ptr<UI::Tab> mainTab;
    std::shared_ptr<UI::Tab> buildTab;
    std::shared_ptr<UI::Button> buildButton;
    std::shared_ptr<UI::ImageButton> planetSurface;
    std::shared_ptr<UI::ScrollArea> buildQueueArea;
    std::shared_ptr<graphics::Texture> surfaceTexture;
    std::shared_ptr<graphics::Texture> surfaceTiles;

    std::shared_ptr<UI::Label> foodIconLabel;
    std::shared_ptr<UI::Label> foodLabel;
    std::shared_ptr<UI::Label> populationLabel;
    std::shared_ptr<UI::Label> populationIconLabel;
    std::shared_ptr<UI::Label> ressourceLabel;
    std::shared_ptr<UI::Label> ressourceIconLabel;
    std::shared_ptr<UI::ImageButton> planetIcon;
    std::map<PlanetType, std::shared_ptr<graphics::Texture>> planetTextures;

    bool updatePlanet;
    std::shared_ptr<Planet> planet;
    BuildWindow* buildWindow;
    std::shared_ptr<Player> player;
};

} /* namespace windows */

#endif /* WINDOWS_PLANETWINDOW_H_ */
