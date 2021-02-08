#include "SettingsWindow.h"
#include "../translate.h"
#include <engine/graphics/TextureManager.h>
#include <functional>
SettingsWindow::SettingsWindow(core::GameWindow *window)
    : UI::Window(50, 50, 490, 300), window(window)
{
    uiText = graphics::TextureManager::Instance().loadFont("fonts/Audiowide-Regular.ttf", 12);
    uiIconText = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 20);

    setFont(uiText.get());
    this->fullscreen = std::make_shared<UI::Checkbox>(this);
    this->cancelButton = std::make_shared<UI::Button>(this);
    this->saveButton = std::make_shared<UI::Button>(this);
    addObject(fullscreen);
    addObject(saveButton);
    addObject(cancelButton);
    saveButton->setLabel(_("Save"));
    saveButton->setPos(30, 200);
    saveButton->connect(UI::Button::buttonClickCallback(), [=]() {
        auto settings = window->getSettings();
        settings->setAttrB("Base", "Fullscreen", fullscreen->isChecked());
        int i = resolutions->getSelection();
        SDL_DisplayMode mode;
        if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0)
        {
            SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
            return;
        }
        settings->setAttrI("Base", "Height", mode.h);
        settings->setAttrI("Base", "Width", mode.w);

        settings->write();
        closeWindow();
    });

    cancelButton->setLabel(_("Cancel"));
    cancelButton->setPos(200, 200);
    fullscreen->setPos(30, 50);
    fullscreen->setText(_("Fullscreen"));
    setTitle(_("Settings"));
    resolutions = std::make_shared<UI::ComboBox<std::string>>(this);

    resolutions->setElementFunction([](std::string val) {
        return val;
    });
    addObject(resolutions);
    resolutions->setPos(30, 70);
    auto settings = window->getSettings();
    fullscreen->setChecked(settings->getValueB("Base", "Fullscreen"));
    cancelButton->connect("buttonClick", [&]() { closeWindow(); });
    int screenWidth = settings->getValueI("Base", "Width");
    int screenHeight = settings->getValueI("Base", "Height");

    /* Get available fullscreen/hardware modes */

    int display_mode_count = SDL_GetNumDisplayModes(display_in_use);
    if (display_mode_count < 1)
    {
        throw SDLException("SDL_GetNumDisplayModes");
    }
    SDL_DisplayMode mode;
    for (int i = 0; i < display_mode_count; ++i)
    {
        if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0)
        {
            SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
            return;
        }
        resolutions->addElement(std::to_string(mode.w) + " x " + std::to_string(mode.h));
        if (mode.w == screenWidth && mode.h == screenHeight)
            resolutions->setSelection(i);
    }
}

SettingsWindow::~SettingsWindow()
{
    //dtor
}
void SettingsWindow::closeWindow()
{
    this->setVisible(false);
}
