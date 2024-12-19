#pragma once

#include "TextureWrapper.h"
#include "GLShaderProgram.h"
#include "Image.h"
#include "Uniform.h"
#include <vector>
#include <map>

namespace lcf {
    class AssimpLoader;
    class Material
    {
        friend class AssimpLoader;
    public:
        enum Type
        {
            Phong,
        };
        using TextureInfoMap = std::map<int, TextureWrapper>;
        using TextureDataInfo = std::pair<int, Image>;
        using TextureDataInfoList = std::vector<TextureDataInfo>;
        using SharedPtr = std::shared_ptr<Material>;
        static SharedPtr newShared();
        Material();
        Material(const Material&) = default;
        ~Material() = default;
        void setTexture(int texture_type, TextureWrapper texture);
        const TextureInfoMap &textureInfoMap() const;
        void setTextures(const TextureInfoMap& texture_info_map);
        void create();
        void bind(); //- 纹理绑定的纹理单元顺序为TextureType枚举值的递增顺序
        void release();
        const UniformList &asUniformList() const;
    private:
        void setImageData(int texture_type, unsigned char* data, int width, int height);
        void setImageData(int texture_type, const Image& image);
        void setImageData(int texture_type, Image&& image);
    private:
        Type m_type = Type::Phong;
        TextureInfoMap m_textures;
        TextureDataInfoList m_image_data;
        float m_shininess = 32.0f;
        UniformList m_phong_uniforms;
    };
}

/*
struct PhongMaterial
{
    TextureList textures[3];
    float shininess;
    int diffuse_map = 0;
    int specular_map = 1;
    int normal_map = 2;
};
*/
/*
todo 目前的材质类不是很好，以下是一些可能的改进方向：
Material类中存取所有的纹理信息，根据这些信息得到可能的材质类型
比如含有diffuse_map, specular_map, normal_map的组合，可以提供PhongUniforms的类型选择
具体来说，如果type == Phong，需要将diffuse_map绑定到纹理单元0, specular_map绑定到纹理单元1, normal_map绑定到纹理单元2，
再将shininess传递给shader
如果缺少normal_map?
*/

/*
修改思路：MaterialBinder类中存取所有的纹理信息；在MaterialBinder类中有一个Material指针，为具体的材质类型，比如PhongMaterial，
MaterialBinder类似一个状态机，根据设定的材质类型生成不同的具体材质类型，实际绑定时调用指针的绑定方法即可
*/