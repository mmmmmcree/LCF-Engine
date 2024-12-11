#pragma once

#include "ShaderToy.h"
#include <unordered_map>
#include <QObject>

namespace lcf {
    class ShaderToyManager : public QObject
    {
        Q_OBJECT
    public:
        using ShaderToyMap = std::unordered_map<std::string, std::unique_ptr<ShaderToy>>;
        using ActivatedShaderToyMap = std::unordered_map<std::string, ShaderToy *>;
        static ShaderToyManager *instance();
        ShaderToy *load(const std::string &name, int width, int height, const QStringList &frag_paths);
        void updateActivated();
        void activate(const std::string &name);
        void deactivate(const std::string &name);
        ShaderToy *get(const std::string &name);
    private:
        ShaderToyManager() = default;
        ShaderToyMap s_shader_toys;
        ActivatedShaderToyMap s_activated_shader_toys;
    };
}