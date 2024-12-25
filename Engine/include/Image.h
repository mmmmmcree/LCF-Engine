#pragma once

#include <QImage>
#include <QThread>

namespace lcf {
    using Image = QImage;

    Image dataToImage(unsigned char *data, int width, int height);

    class ImageLoader : public QThread
    {
        Q_OBJECT
    public:
        explicit ImageLoader(const QString &file_path, bool mirroed = true, Image::Format format = Image::Format_RGBA8888, QObject *parent = nullptr);
    signals:
        void imageLoaded(const QImage &image);
    protected:
        void run() override;
    private:
        QString m_path;
        bool m_mirrored;
        Image::Format m_format;
    };
}