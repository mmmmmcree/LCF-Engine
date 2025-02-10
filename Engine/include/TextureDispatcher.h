#pragma once

#include <unordered_map>
#include "TextureWrapper.h"

namespace lcf {
    class TextureDispatcher
    {
    public:
        using TextureMap = std::unordered_map<std::string, TextureWrapper>;
        static TextureDispatcher *instance();
        void setTextureByName(const std::string &name, const TextureWrapper &wrapper);
        TextureWrapper getTextureByName(const std::string &name) const;
    private:
        TextureDispatcher() = default;
    private:
        TextureMap m_texture_map;
    };
}