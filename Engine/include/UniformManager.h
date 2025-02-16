#pragma once

#include <QObject>
#include "Uniform.h"
#include <QSet>
#include <QList>

namespace lcf {
    class UniformManager : public QObject
    {
        Q_OBJECT
        using Uniform = Uniform;
        using UniformPtrSet = QSet<const Uniform *>;
        using UniformPtrList = QList<const Uniform *>;
    public:
        static UniformManager *instance();
        static bool isInstanceExists();
        ~UniformManager();
        void emitUniformUpdatedSignal(const Uniform *uniform);
        void addUniform(const Uniform *uniform);
        void removeUniform(const Uniform *uniform);
        bool existUniform(const Uniform *uniform) const;
        UniformPtrList existedUniforms() const;
    signals:
        void uniformUpdated(const Uniform *uniform);
    private:
        UniformManager() = default;
    private:
        inline static bool s_instance_exists = false;
    private:
        UniformPtrSet m_uniforms;
    };
}
