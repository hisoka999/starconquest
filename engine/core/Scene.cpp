/*
 * Scene.cpp
 *
 *  Created on: 18.04.2016
 *      Author: stefan
 */

#include "engine/core/Scene.h"
#include <boost/make_shared.hpp>
namespace core {

Scene::Scene(core::Renderer* pRenderer)
    : renderer(pRenderer)
{
    winMgr = std::make_shared<UI::WindowManager>();
    music = std::make_shared<core::Music>();
}

void Scene::unload()
{
    if (music->isMusicPlaying()) {
        music->stop();
    }
}

Scene::~Scene()
{
    // TODO Auto-generated destructor stub
}

} /* namespace character */