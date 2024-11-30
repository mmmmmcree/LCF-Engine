#pragma once

#include "ShaderToy.h"
#include <unordered_map>

struct ShaderToyManager
{
    static ShaderToy *add(const std::string &name, int width, int height, const QStringList &frag_paths);
    static void updateActivated();
    static void activate(const std::string &name);
    static void deactivate(const std::string &name);
    static ShaderToy *get(const std::string &name);
private:
    inline static std::unordered_map<std::string, std::unique_ptr<ShaderToy>> s_shader_toys;
    inline static std::unordered_map<std::string, ShaderToy *> s_activated_shader_toys;
};