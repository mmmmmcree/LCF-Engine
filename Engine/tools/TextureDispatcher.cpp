#include "TextureDispatcher.h"

lcf::TextureDispatcher *lcf::TextureDispatcher::instance()
{
    static TextureDispatcher s_instance;
    return &s_instance;
}

void lcf::TextureDispatcher::setTextureByName(const std::string &name, const TextureWrapper &wrapper)
{
    m_texture_map[name] = wrapper;
}

lcf::TextureWrapper lcf::TextureDispatcher::getTextureByName(const std::string &name) const
{
    auto iter = m_texture_map.find(name);
    if (iter == m_texture_map.end()) {
        return TextureWrapper();
    }
    return iter->second;
}
