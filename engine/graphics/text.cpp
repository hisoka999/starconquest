#include "engine/graphics/text.h"
#include "engine/utils/exceptions.h"
#include "engine/utils/logger.h"

namespace graphics
{

    Text::Text()
    {
        //ctor
        font = nullptr;
        //logger = new utils::Logger();
        texture = nullptr;
    }

    Text::~Text()
    {
        for (auto surf : textCache)
        {
            SDL_DestroyTexture(surf.second);
        }
        textCache.clear();

        if (texture != nullptr)
        {
            SDL_DestroyTexture(texture);
        }

        if (font != nullptr)
            TTF_CloseFont(font);
    }

    Text::Text(const Text &org)
    {
        font = nullptr;
        texture = nullptr;
        this->fontFile = "";
    }

    void Text::openFont(const std::string fontFile, int fontSize)
    {
        this->fontFile = fontFile + std::string(":" + std::to_string(fontSize));
        //Open the font
        font = TTF_OpenFont(fontFile.c_str(), fontSize);
        if (font == nullptr)
        {
            logger.logSDLError("TTF_OpenFont");
            throw new SDLException("TTF_OpenFont");
        }
    }

    int Text::size(const std::string &message, int *w, int *h)
    {
        if (font != nullptr)
            return TTF_SizeUTF8(font, message.c_str(), w, h);
        else
            return -1;
    }

    const std::string Text::genTextHash(const std::string &message, SDL_Color color)
    {
        std::string hash;
        hash.reserve(6 + message.length());
        hash.append(std::to_string(color.r));
        hash.append(",");
        hash.append(std::to_string(color.g));
        hash.append(",");
        hash.append(std::to_string(color.b));
        hash.append("#");
        hash.append(message);

        return hash;
    }

    void Text::render(core::Renderer *ren, const std::string &message,
                      SDL_Color color, int x, int y)
    {
        if (message.size() == 0)
            return;
        //We need to first render to a surface as that's what TTF_RenderText
        //returns, then load that surface into a texture
        int w, h;
        if (size(message, &w, &h) == -1)
        {
            throw new SDLException("TTF_SizeText");
        }
        if (w == 0)
        {
            return;
        }
        auto hash = genTextHash(message, color);
        SDL_Surface *surf = nullptr;
        if (textCache.count(hash) > 0)
        {
            texture = textCache[hash];
        }
        else
        {
            surf = TTF_RenderUTF8_Blended(font, message.c_str(),
                                          color);

            if (surf == nullptr)
            {
                TTF_GetError();
                logger.logSDLError("TTF_RenderText_Blended");
                TTF_CloseFont(font);
                font = nullptr;
                throw new SDLException("TTF_RenderText_Blended");
            }
            texture = SDL_CreateTextureFromSurface(ren->getRenderer(), surf);
            if (texture == nullptr)
            {
                logger.logSDLError("SDL_CreateTextureFromSurface");
                throw new SDLException("SDL_CreateTextureFromSurface");
            }
            textCache[hash] = texture;
        }

        int iW, iH;
        SDL_QueryTexture(texture, nullptr, nullptr, &iW, &iH);

        SDL_Rect dst;
        dst.x = x;
        dst.y = y;
        dst.w = iW;
        dst.h = iH;

        SDL_Rect src;
        src.x = 0;
        src.y = 0;
        src.w = iW;
        src.h = iH;

        SDL_RenderCopy(ren->getRenderer(), texture, &src, &dst);
        if (surf != nullptr)
        {
            SDL_FreeSurface(surf);
        }
        texture = nullptr;
    }

} // namespace graphics
