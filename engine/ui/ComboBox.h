#ifndef UI_COMBOBOX_H
#define UI_COMBOBOX_H

#include <engine/graphics/text.h>
#include <engine/graphics/texture.h>
#include <engine/ui/Object.h>
#include <vector>
#include <type_traits>

namespace UI
{

    template <typename T>
    class ComboBox : public UI::Object
    {
    public:
        /** Default constructor */
        ComboBox(Object *parent = nullptr) : UI::Object(parent), selection(0)
        {
            width = 100;
            //texture = new CTexture("ArkanaLook.png");
            //border = texture->subSurface(7,217,6,6);
            //background = texture->subSurface(250,80,50,50);
            //border->zoomImage(width/6.0,28.0/6.0);
            mouseDown = false;
            renderOrder = 99;

            elementFunction = nullptr;
        }
        /** Default destructor */
        virtual ~ComboBox()
        {
        }

        void render(core::Renderer *pRender)
        {

            int tx = getX();

            int ty = getY();

            if (this->getParent())
            {
                graphics::Rect dsp = getParent()->displayRect();
                tx += dsp.x;
                ty += dsp.y;
            }

            graphics::Rect rect;
            rect.x = tx;
            rect.y = ty;
            rect.width = getWidth();
            rect.height = 28;

            if (mouseDown && elements.size() > 0)
            {
                rect.height *= elements.size();
            }
            SDL_Color textColor = {255, 255, 255, 255};
            SDL_Color selectionColor = {93, 103, 108, 255};
            pRender->setDrawColor(12, 21, 24, 255);

            pRender->fillRect(rect);
            pRender->setDrawColor(93, 103, 108, 255);
            pRender->drawRect(rect);

            if (mouseDown)
            {
                for (size_t i = 0; i < elements.size(); ++i)
                {
                    if (selection == i)
                    {
                        getFont()->render(pRender, elementFunction(elements[i]), selectionColor, rect.x + 5, rect.y + (i * 28));
                    }
                    else
                    {
                        getFont()->render(pRender, elementFunction(elements[i]), textColor, rect.x + 5, rect.y + (i * 28));
                    }
                }
            }
            else
            {
                getFont()->render(pRender, elementFunction(elements[selection]), textColor, rect.x + 5, rect.y);
            }
        }
        void setWidth(int width) { this->width = width; }
        int getWidth() { return width; }
        void addElement(T elem) { elements.push_back(elem); }
        void setElementFunction(std::function<std::string(T)> func)
        {
            elementFunction = func;
        }
        void clearElements()
        {
            elements.clear();
            selection = 0;
        }
        virtual void handleEvents(core::Input *pInput)
        {
            SDL_Event e = pInput->getEvent();
            int tx = getX();

            int ty = getY();

            if (this->getParent())
            {
                graphics::Rect dsp = getParent()->displayRect();
                tx += dsp.x;
                ty += dsp.y;
            }
            int height = 28;
            if (mouseDown && elements.size() > 0)
            {
                height *= elements.size();
            }
            if (e.button.x >= tx && e.button.x <= tx + width + 28 && e.button.y >= ty && e.button.y <= ty + height)
            {
                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                {

                    mouseDown = true;
                }
                else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
                {
                    mouseDown = false;
                    setSelection((e.button.y - ty) / 28);
                }
            }
            else
            {
                mouseDown = false;
            }
        }
        int getSelection() { return selection; }
        void setSelection(unsigned int selection)
        {
            this->selection = selection;
            this->fireFuncionCall("selectionChanged", selection);
        }

        void setSelectionByText(T text)
        {
            // Get index of element from iterator
            int index = -1;
            auto it = std::find(elements.begin(), elements.end(), text);
            if (it != elements.end())
            {
                index = std::distance(elements.begin(), it);
                setSelection(index);
            }
        }

    protected:
    private:
        int width;
        std::vector<T> elements;
        bool mouseDown;
        unsigned int selection;

        std::function<std::string(T)> elementFunction;
    };

} // namespace UI

#endif // UI_COMBOBOX_H
