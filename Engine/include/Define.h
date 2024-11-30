#pragma once

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
/*
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
#define GL_QUADS                          0x0007
#define GL_QUAD_STRIP                     0x0008
#define GL_POLYGON                        0x0009*/
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
}