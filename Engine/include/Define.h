#pragma once
#include "GLTexture.h"

namespace lcf {
    enum GLDataType
    {
        BYTE = 0x1400,
        UNSIGNEDBYTE = 0x1401,
        SHORT = 0x1402,
        USHORT = 0x1403,
        INT = 0x1404,
        UINT = 0x1405,
        FLOAT = 0x1406,
        TWOBYTES = 0x1407,
        THREEBYTES = 0x1408,
        FOURBYTES = 0x1409,
        DOUBLE = 0x140A,
    };

    template <typename T>
    struct GLTypeMapper;

    template <>
    struct GLTypeMapper<float>
    {
        static constexpr int type = FLOAT;
    };

    template <>
    struct GLTypeMapper<int>
    {
        static constexpr int type = INT;
    };

    template <>
    struct GLTypeMapper<unsigned int>
    {
        static constexpr int type = UINT;
    };

    template <>
    struct GLTypeMapper<double>
    {
        static constexpr int type = DOUBLE;
    };

    enum GLBeginMode
    {
        POINTS = 0x0000,   
        LINES = 0x0001,    
        LINE_LOOP = 0x0002, 
        LINE_STRIP = 0x0003,
        TRIANGLES = 0x0004,
        TRIANGLE_STRIP = 0x0005,
        TRIANGLE_FAN = 0x0006,
        QUADS = 0x0007,
        QUAD_STRIP = 0x0008,
        POLYGON = 0x0009,   
    };


    using PixelFormat = GLTexture::PixelFormat;
    using PixelType = GLTexture::PixelType;

    using GLTextureTarget = GLTexture::Target;

    using GLTextureFormat = GLTexture::TextureFormat;

    enum TextureType
    {
        None = 0,
        Diffuse = 1,
        Specular = 2,
        Ambient = 3,
        Emissive = 4,
        Height = 5,
        Normal = 6,
        Shininess = 7,
        Opacity = 8,
        Displacement = 9,
        Lightmap = 10,
        Reflection = 11,
        BaseColor = 12,
        NormalCamera = 13,
        EmissionColor = 14,
        Metalness = 15,
        Roughness = 16,
        AmbientOcclusion = 17,
        Unknown = 18, 
        Sheen = 19,
        ClearCoat = 20,
        Transmission = 21,
        MayaBaseColor = 22,
        MayaSpecular = 23,
        MayaSpecularColor = 24,
        MayaSpecularRoughness = 25,
        Anisotropy = 26,
        GltfMetallicRoughness = 27,
        IBLIrradiance,
        IBLPrefilter,
        IBLBRDF,
        UserCustom0, 
        UserCustom1,
        UserCustom2,
        UserCustom3,
        UserCustom4,
        UserCustom5,
        UserCustom6,
        UserCustom7,
        UserCustom8,
        UserCustom9,
        UserCustom10,
        UserCustom11,
        UserCustom12,
        UserCustom13,
        UserCustom14,
        UserCustom15,
        SIZE
    };

    using GLTextureFilter = GLTexture::Filter;

    using GLTextureWrapMode = GLTexture::WrapMode;

    enum class MaterialType
    {
        None,
        UserCustom,
        Phong,
        PBR, 
        IBL,
        SIZE
    };

    enum LightType
    {
        Directional,
        Point,
        Spot
    };

    enum class Object3DType
    {
        Group,
        Mesh,
        Model, 
        Light, 
        Scene
    };
}