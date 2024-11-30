#include "Scene.h"

lcf::Scene *lcf::Scene::global()
{
    static Scene s_instance;
    return &s_instance;
}

lcf::Scene *lcf::Scene::current()
{
    if (not s_current) { return global(); }
    return s_current;
}

void lcf::Scene::setCurrent(Scene *scene)
{
    s_current = scene;
}
