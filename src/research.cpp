#include "research.h"

Research::Research(const std::string name, const std::string localizedName, const std::string description, const std::string icon, int costs)
    : name(name)
    , localisedName(localizedName)
    , description(description)
    , icon(icon)
    , costs(costs)
{
}

std::string Research::getName() const
{
    return name;
}

std::string Research::getLocalisedName() const
{
    return localisedName;
}

std::string Research::getDescription() const
{
    return description;
}

std::string Research::getIcon() const
{
    return icon;
}

int Research::getCosts() const
{
    return costs;
}

std::vector<std::shared_ptr<Research>> Research::getRequirements() const
{
    return requirements;
}

bool Research::getResearched() const
{
    return researched;
}

std::vector<std::string> Research::getRequirementNames() const
{
    return requirementNames;
}

void Research::setRequirementNames(const std::vector<std::string>& value)
{
    requirementNames = value;
}

void Research::setEnabledObjects(const std::vector<std::string>& value)
{
    enabledObjects = value;
}
void Research::addRequirement(const std::shared_ptr<Research>& requirement)
{
    requirements.push_back(requirement);
}
