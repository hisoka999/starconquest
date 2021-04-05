/*
 * Building.h
 *
 *  Created on: 06.06.2016
 *      Author: stefan
 */

#ifndef BUILDING_H_
#define BUILDING_H_

#include "BuildableObject.h"
#include <map>

enum class ModifierType
{
    Food,
    Money,
    Ressource,
    Research,
};

class Building : public BuildableObject
{
public:
    Building(const std::string &name, const std::string &localisedName, int resources, unsigned limit);
    virtual ~Building();

    virtual bool canBuildObject(int planetResources);

    void addModifier(ModifierType type, int value);

    int getModifier(ModifierType type);

    unsigned getLimit() const;

private:
    std::map<ModifierType, int> modifiers;

    unsigned limit;
};

#endif /* BUILDING_H_ */
