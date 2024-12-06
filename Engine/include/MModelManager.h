#pragma once

#include <QObject>
#include <memory>
#include <unordered_map>
#include "Model.h"
#include <QOpenGLContext>
#include <QOffscreenSurface>


namespace lcf {
    class MModelManager : public QObject
    {
        Q_OBJECT
    public:
        static MModelManager *instance();
        void initialize(QOpenGLContext *context);
        Model::SharedPtr load(const QString &path);
        Model::UniquePtr clone(Model *model);
    private:
        MModelManager() = default;
        void clone(Model *model, Model *cloned);
        void processToBeCloned(Model *model);
    private:
        QOpenGLContext *m_context = nullptr;
        QOffscreenSurface *m_surface = nullptr;
        std::unordered_map<QString, Model::SharedPtr> m_models;
        QHash<Model *, QList<Model *>> m_to_be_cloned;
    };
}