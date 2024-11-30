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
        explicit ImageLoader(const QString &file_path, QObject *parent = nullptr);
    signals:
        void imageLoaded(const QImage &image);
    protected:
        void run() override;
    private:
        QString m_path;
    };
}