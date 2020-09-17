/*
 * BuildableObject.h
 *
 *  Created on: 06.06.2016
 *      Author: stefan
 */

#ifndef BUILDABLEOBJECT_H_
#define BUILDABLEOBJECT_H_

#include <engine/graphics/texture.h>
#include <memory>
#include <string>

class BuildableObject {

public:
    BuildableObject(const std::string& name, int resources);
    virtual ~BuildableObject();
    virtual bool canBuildObject(int planetResources) = 0;
    std::string getName();
    int getRessources();
    std::shared_ptr<graphics::Texture> getTexture();
    void loadTexture(std::string path);

private:
    std::string name;
    int resources;
    std::shared_ptr<graphics::Texture> texture;
};

#endif /* BUILDABLEOBJECT_H_ */
