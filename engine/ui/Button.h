#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <engine/graphics/rect.h>
#include <engine/graphics/text.h>
#include <engine/graphics/texture.h>
#include <engine/ui/ButtonListener.h>
#include <engine/ui/Object.h>
#include <iostream>
#include <string>

namespace UI {

class Button : public UI::Object {
public:
    /** Default constructor */
    Button(Object* parent = nullptr);
    /** Default destructor */
    virtual ~Button();

    void setLabel(const std::string& label);
    void setFont(const std::string& fontname, unsigned int font_size);
    void setStaticWidth(const int pWidth);
    void setColor(SDL_Color color);
    void enable();
    void disable();
    bool isEnabled();
    virtual void render(core::Renderer* pRender);
    virtual void handleEvents(core::Input* pInput);
    virtual graphics::Rect displayRect();

    static std::string buttonClickCallback()
    {
        return "buttonClick";
    }

    bool getBorderless() const;
    void setBorderless(bool value);

protected:
private:
    std::string label;
    SDL_Color color;
    int width;
    int static_width;
    bool hover;
    bool enabled;
    bool borderless;
    std::shared_ptr<graphics::Texture> texture;
};

} // namespace UI

#endif // UI_BUTTON_H
