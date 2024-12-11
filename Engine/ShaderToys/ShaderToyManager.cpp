#include "ShaderToyManager.h"

lcf::ShaderToyManager *lcf::ShaderToyManager::instance()
{
    static ShaderToyManager s_instance;
    return &s_instance; 
}

lcf::ShaderToy *lcf::ShaderToyManager::load(const std::string &name, int width, int height, const QStringList &frag_paths)
{
    if (s_shader_toys.find(name) != s_shader_toys.end()) { return nullptr; }
    s_shader_toys.emplace(name, std::make_unique<ShaderToy>(width, height, frag_paths));
    return s_shader_toys[name].get();
}

void lcf::ShaderToyManager::updateActivated()
{
    for (auto &[name, shader_toy] : s_activated_shader_toys) {
        shader_toy->update();
    }
}

void lcf::ShaderToyManager::activate(const std::string &name)
{
    auto iter = s_shader_toys.find(name);
    if (iter == s_shader_toys.end()) { return; }
    auto activated_iter = s_activated_shader_toys.find(name);
    if (activated_iter != s_activated_shader_toys.end()) { return; }
    s_activated_shader_toys.emplace(name, iter->second.get());
}

void lcf::ShaderToyManager::deactivate(const std::string &name)
{
    auto activated_iter = s_activated_shader_toys.find(name);
    if (activated_iter == s_activated_shader_toys.end()) { return; }
    s_activated_shader_toys.erase(activated_iter);
}

lcf::ShaderToy *lcf::ShaderToyManager::get(const std::string &name)
{
    return s_shader_toys[name].get();
}
