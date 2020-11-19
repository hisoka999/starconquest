/*
 * BuildableObject.cpp
 *
 *  Created on: 06.06.2016
 *      Author: stefan
 */

#include "BuildableObject.h"
#include <engine/graphics/TextureManager.h>

BuildableObject::BuildableObject(const std::string& name, const std::string& localisedName, int resources)
    : name(name)
    , localisedName(localisedName)
    , resources(resources)
{
    // TODO Auto-generated constructor stub
}

BuildableObject::~BuildableObject()
{
    // TODO Auto-generated destructor stub
}

std::string BuildableObject::getName()
{
    return name;
}

int BuildableObject::getRessources()
{
    return resources;
}

std::shared_ptr<graphics::Texture> BuildableObject::getTexture()
{
    return texture;
}
void BuildableObject::loadTexture(std::string path)
{
    texture = graphics::TextureManager::Instance().loadTexture(path);
}

std::string BuildableObject::getLocalisedName() const
{
    return localisedName;
}
