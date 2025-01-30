#pragma once
#include <QString>
#include "Define.h"

namespace lcf {
    class LImage
    {
    public:
        LImage(const char *filename, bool flip_y = false);
        LImage(const QString &filename, bool flip_y = false);
        ~LImage();
        operator bool() const;
        const void *data() const;
        template <typename T>
        const T *data() const { return static_cast<const T *>(m_data); }
        int width() const;
        int height() const;
        int channels() const;
        ImageDataType dataType() const;
        ImageFormat format() const;
    private:
        ImageDataType m_data_type;
        int m_width = 0;
        int m_height = 0;
        int m_channels = 0;
        void *m_data = nullptr;
    };
}