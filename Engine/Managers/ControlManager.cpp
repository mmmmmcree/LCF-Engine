#include "ControlManager.h"

lcf::ControlManager *lcf::ControlManager::instance()
{
    static ControlManager s_instance;
    return &s_instance;
}

void lcf::ControlManager::setCurrentScene(Scene *scene)
{
    if (m_current_scene == scene) { return; }
    if (m_current_scene) {
        m_current_scene->timer()->stop();
        m_current_scene->setSignalSender(nullptr);
    }
    m_current_scene = scene;
    if (not m_current_scene) { return; }
    m_current_scene->setSignalSender(&m_scene_signal_sender);
    m_current_scene->timer()->start();
    emit this->currentSceneChanged(m_current_scene);
}

lcf::Scene *lcf::ControlManager::currentScene() const
{
    static Scene s_default_scene;
    if (not m_current_scene) {
        qDebug() << "lcf::ControlManager::currentScene() : no current scene set, using default scene";
        return &s_default_scene;
    }
    return m_current_scene;
}

const lcf::SignalSender *lcf::ControlManager::sceneSignalSender() const
{
    return &m_scene_signal_sender;
}

void lcf::ControlManager::setCurrentModel(Model *model)
{
    if (m_current_model) {
        m_current_model->setSignalSender(nullptr);
    }
    m_current_model = model;
    if (not m_current_model) { return; }
    m_current_model->setSignalSender(&m_model_signal_sender);
    emit this->currentModelChanged(m_current_model);
}

lcf::Model *lcf::ControlManager::currentModel() const
{
    static Model s_default_model;
    if (not m_current_model) {
        qDebug() << "lcf::ControlManager::currentModel() : no current model set, using default model";
        return &s_default_model;
    }
    return m_current_model;
}

void lcf::ControlManager::setCurrentMesh(Mesh *mesh)
{
    m_current_mesh = mesh;
    if (m_current_mesh) {
        emit this->currentMeshChanged(m_current_mesh);
    }
}

lcf::Mesh *lcf::ControlManager::currentMesh() const
{
    static Mesh s_default_mesh(Geometry::cube());
    if (not m_current_mesh) {
        qDebug() << "lcf::ControlManager::currentMesh() : no current mesh set, using default mesh";
        return &s_default_mesh;
    }
    return m_current_mesh;
}

void lcf::ControlManager::setCurrentObject(Object3D * object)
{
    m_current_object = object;
}

lcf::Object3D *lcf::ControlManager::currentObject() const
{
    if (not m_current_object) {
        qDebug() << "lcf::ControlManager::currentObject() : no current object set, using current mesh";
        return this->currentMesh();
    }
    return m_current_object;
}
