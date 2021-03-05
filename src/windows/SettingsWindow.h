#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <engine/ui/Window.h>
#include <engine/ui/Object.h>
#include <engine/ui/Checkbox.h>
#include <engine/ui/Button.h>
#include <engine/ui/ComboBox.h>
#include <engine/core/gamewindow.h>

class SettingsWindow : public UI::Window
{
public:
    /** Default constructor */
    SettingsWindow(const core::GameWindow *window);

    /** Default destructor */
    virtual ~SettingsWindow();

protected:
    void closeWindow();

private:
    std::shared_ptr<UI::Checkbox> fullscreen;
    std::shared_ptr<UI::Button> saveButton;
    std::shared_ptr<UI::Button> cancelButton;
    std::shared_ptr<UI::ComboBox<std::string>> resolutions;
    const core::GameWindow *window;
    int display_in_use = 0;
    std::shared_ptr<graphics::Text> uiText;
    std::shared_ptr<graphics::Text> uiIconText;
};

#endif // SETTINGSWINDOW_H
