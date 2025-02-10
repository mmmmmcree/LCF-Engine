#include "LImage.h"
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"
#include <QFileInfo>


lcf::LImage::LImage(const char *filename, bool flip_y)
{
    QFileInfo file_info(filename);
    if (not file_info.exists()) {
        qDebug() << "lcf::LImage: file not found: " << filename;
        return;
    }
    stbi_set_flip_vertically_on_load(flip_y);
    if (file_info.suffix().toLower() == "hdr") {
        m_data = stbi_loadf(filename, &m_width, &m_height, &m_channels, 0);
        m_data_type = PixelType::Float32;
    } else {
        m_data = stbi_load(filename, &m_width, &m_height, &m_channels, 0);
        m_data_type = PixelType::UInt8;
    }
    stbi_set_flip_vertically_on_load(false);
}

lcf::LImage::LImage(const QString &filename, bool flip_y) :
    LImage(filename.toLocal8Bit().constData(), flip_y)
{
}

lcf::LImage::LImage(int width, int height, int channels, PixelType data_type) :
    m_width(width),
    m_height(height),
    m_channels(channels),
    m_data_type(data_type)
{
    switch (m_data_type) {
        case PixelType::UInt8 : { m_data = new unsigned char[m_width * m_height * m_channels]; } break;
        case PixelType::Float32 : { m_data = new float[m_width * m_height * m_channels]; } break;
        default: { m_data = nullptr; } break;
    }
}

lcf::LImage::~LImage()
{
    delete m_data;
    m_data = nullptr;
}

lcf::LImage::operator bool() const
{
    return m_data;
}

void *lcf::LImage::data()
{
    return m_data;
}

const void *lcf::LImage::data() const
{
    return m_data;
}

int lcf::LImage::width() const
{
    return m_width;
}

int lcf::LImage::height() const
{
    return m_height;
}

int lcf::LImage::channels() const
{
    return m_channels;
}

lcf::PixelType lcf::LImage::dataType() const
{
    return m_data_type;
}

lcf::PixelFormat lcf::LImage::format() const
{
    PixelFormat format;
    switch (m_channels) {
        case 1 : { format = PixelFormat::Red; } break;
        case 2 : { format = PixelFormat::RG; } break;
        case 3 : { format = PixelFormat::RGB; } break;
        case 4 : { format = PixelFormat::RGBA; } break;
    }
    return format;
}

void lcf::LImage::saveAsHDR(const char *filename) const
{
    if (not m_data or m_data_type != PixelType::Float32) { return; }
    stbi_write_hdr(filename, m_width, m_height, m_channels, static_cast<const float *>(m_data));
}
