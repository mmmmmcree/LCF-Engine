#pragma once

#include <QObject>
#include <memory>
#include <unordered_map>
#include "Model.h"
#include <QOpenGLContext>
#include <QSurface>


namespace lcf {
    class ModelManager : public QObject
    {
        Q_OBJECT
    public:
        static ModelManager *instance();
        void initialize(QOpenGLContext *context);
        Model::SharedPtr load(const QString &path, std::string name = "");
        Model::UniquePtr clone(Model *model, std::string name = "");
    private:
        ModelManager() = default;
        void clone(Model *model, Model *cloned);
        void processToBeCloned(Model *model);
    private:
        QOpenGLContext *m_context = nullptr;
        QSurface *m_surface = nullptr;
        std::unordered_map<QString, Model::SharedPtr> m_models;
        QHash<Model *, QList<Model *>> m_to_be_cloned;
    };
}