#include "MModelManager.h"
#include "AssimpLoader.h"
#include <QFileInfo>

lcf::MModelManager *lcf::MModelManager::instance()
{
    static MModelManager s_instance;
    return &s_instance;
}

void lcf::MModelManager::initialize(QOpenGLContext *context)
{
    m_context = context;
    m_surface = new QOffscreenSurface(nullptr, this);
    m_surface->setFormat(context->format());
    m_surface->create();
}

lcf::Model::SharedPtr lcf::MModelManager::load(const QString &path)
{
    auto iter = m_models.find(path);
    if (iter != m_models.end()) {
        return this->clone(iter->second.get());
    }
    if (not QFileInfo(path).exists()) {
        qDebug() << "lcf::MModelManager::load() - File not found: " << path;
        return nullptr;
    }
    auto &model = m_models.emplace(std::make_pair(path, std::make_shared<Model>())).first->second;
    AssimpLoader *loader = new AssimpLoader(model.get(), path);
    connect(loader, &AssimpLoader::loaded, this, [this](Model *model) {
        this->m_context->makeCurrent(m_surface);
        model->create();
        this->m_context->doneCurrent();
        this->processToBeCloned(model);
    });
    connect(loader, &AssimpLoader::finished, loader, &QObject::deleteLater);
    loader->start();
    return model;
}

lcf::Model::UniquePtr lcf::MModelManager::clone(Model *model)
{
    Model::UniquePtr cloned = std::make_unique<Model>();
    if (model->isCreated()) {
        this->clone(model, cloned.get());
    } else {
        m_to_be_cloned[model].emplace_back(cloned.get());
    }
    return cloned;
}

void lcf::MModelManager::clone(Model *model, Model *cloned)
{
    cloned->m_created = true;
    for (auto &mesh : model->m_meshes) {
        if (not mesh) { continue; }
        auto cloned_mesh = std::make_unique<Mesh>(*mesh);
        cloned_mesh->setParent(cloned);
        cloned->addMesh(std::move(cloned_mesh));
    }
    cloned->m_root_bone = model->processSkeleton(cloned->m_bones, nullptr, model->m_root_bone);
    for (int i = 0; i < cloned->m_meshes.size(); ++i) {
        auto &mesh = model->m_meshes[i];
        if (not mesh->skeleton()) { continue; }
        auto &cloned_mesh = cloned->m_meshes[i];
        Skeleton::BonePtrs bones;
        for (auto &bone : mesh->skeleton()->bones()) {
            auto it = cloned->m_bones.find(bone->name());
            if (it == cloned->m_bones.end()) { continue; }
            bones.emplace_back(it->second);
        }
        cloned_mesh->setSkeleton(std::make_unique<Skeleton>(std::move(bones), mesh->skeleton()->offsetMatrices()));
    }
    for (auto &animation : model->m_animations) {
        if (not animation) { continue; }
        auto cloned_animation = std::make_unique<Animation>(*animation);
        cloned_animation->updateControlledBones(cloned->m_bones);
        cloned->addAnimation(std::move(cloned_animation));
    }
    cloned->playAnimation();
}

void lcf::MModelManager::processToBeCloned(Model *model)
{
    if (not m_to_be_cloned.contains(model)) { return; }
    for (auto cloned : m_to_be_cloned[model]) {
        this->clone(model, cloned);
    }
    m_to_be_cloned.remove(model);
}
