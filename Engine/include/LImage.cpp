#include "LImage.h"
#include "stb_image/stb_image.h"
#include <QFileInfo>


lcf::LImage::LImage(const char *filename, bool flip_y)
{
    QFileInfo file_info(filename);
    if (not file_info.exists()) {
        qDebug() << "lcf::LImage: file not found: " << filename;
    }
    stbi_set_flip_vertically_on_load(flip_y);
    if (file_info.suffix().toLower() == "hdr") {
        m_data = stbi_loadf(filename, &m_width, &m_height, &m_channels, 0);
        m_data_type = ImageDataType::FLOAT;
    } else {
        m_data = stbi_load(filename, &m_width, &m_height, &m_channels, 0);
        m_data_type = ImageDataType::UNSIGNED_BYTE;
    }
    stbi_set_flip_vertically_on_load(false);
}

lcf::LImage::LImage(const QString &filename, bool flip_y) :
    LImage(filename.toLocal8Bit().constData(), flip_y)
{
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

lcf::ImageDataType lcf::LImage::dataType() const
{
    return m_data_type;
}

lcf::ImageFormat lcf::LImage::format() const
{
    ImageFormat format;
    switch (m_channels) {
        case 3 : { format = ImageFormat::RGB; } break;
        case 4 : { format = ImageFormat::RGBA; } break;
    }
    return format;
}
