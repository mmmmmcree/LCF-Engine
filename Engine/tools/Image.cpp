#include "Image.h"


lcf::ImageLoader::ImageLoader(const QString &file_path, bool mirroed, Image::Format format, QObject *parent) :
    QThread(parent), m_path(file_path), m_mirrored(mirroed), m_format(format)
{
}

void lcf::ImageLoader::run()
{
    Image img(m_path);
    if (not img.isNull()) {
        emit imageLoaded(m_mirrored ? img.mirrored().convertToFormat(m_format) : img.convertToFormat(m_format));
    }
}

lcf::Image lcf::dataToImage(unsigned char *data, int width, int height)
{
    Image image;
    if (height == 0) {
        QByteArray texture_data(reinterpret_cast<const char*>(data), width);
        image.loadFromData(texture_data);
        image = image.convertToFormat(Image::Format_RGBA8888);
    } else {
        image = Image(data, width, height, Image::Format_RGBA8888);
    }   
    return image;
}
