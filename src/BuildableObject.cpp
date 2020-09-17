/*
 * BuildableObject.cpp
 *
 *  Created on: 06.06.2016
 *      Author: stefan
 */

#include "BuildableObject.h"
#include <engine/graphics/TextureManager.h>

BuildableObject::BuildableObject(const std::string& name, int resources)
    : name(name)
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
