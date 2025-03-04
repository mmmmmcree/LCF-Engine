#include "Scene.h"
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include "ShaderManager.h"
#include "TextureManager.h"
#include "GlobalCamera.h"
#include "DirectionalLight.h"
#include "Constants.h"

lcf::Scene::Scene() : Object3D()
{
    m_timer.setInterval(1000 / 60);
}

lcf::Object3DType lcf::Scene::type() const
{
    return Object3DType::Scene;
}

lcf::LightArray &lcf::Scene::lights()
{
    return m_lights;
}

void lcf::Scene::addLight(const Light::SharedPtr &light)
{
    switch (light->lightType()) {
        case LightType::Directional : {
            m_lights.addDirectionalLight(std::static_pointer_cast<DirectionalLight>(light));
        } break;
        case LightType::Point : {
            m_lights.addPointLight(std::static_pointer_cast<PointLight>(light));
        } break;
        case LightType::Spot : {
            m_lights.addSpotLight(std::static_pointer_cast<SpotLight>(light));
        } break;
    }
}

void lcf::Scene::addModel(const Model::SharedPtr &model)
{
    for (const auto &existing_model : m_models) {
        if (model == existing_model) { return; }
    }
    m_models.emplace_back(model);
    if (m_signal_sender) {
        m_signal_sender->sendModelsUpdatedSignal(model.get());
    }
}

void lcf::Scene::addMesh(const Mesh::SharedPtr &mesh)
{
    for (const auto &existing_mesh : m_meshes) {
        if (mesh == existing_mesh) { return; }
    }
    m_meshes.emplace_back(mesh);
}

void lcf::Scene::addGroup(const Object3D::SharedPtr &group)
{
    for (const auto &existing_group : m_groups) {
        if (group == existing_group) { return; }
    }
    m_groups.emplace_back(group);
}

void lcf::Scene::addObject3D(const Object3D::SharedPtr &object3d)
{
    if (not object3d) { return; }
    if (not object3d->parent()) {
        object3d->setParent(this);
    }
    switch (object3d->type()) {
        case Object3DType::Group : {
            this->addGroup(std::static_pointer_cast<Object3D>(object3d));
        } break;
        case Object3DType::Model : {
            this->addModel(std::static_pointer_cast<Model>(object3d));
        } break;
        case Object3DType::Mesh : {
            this->addMesh(std::static_pointer_cast<Mesh>(object3d));
        } break;
        case Object3DType::Light : {
            this->addLight(std::static_pointer_cast<Light>(object3d));
        } break;
        default : { return; }
    }
}

void lcf::Scene::draw()
{
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    gl->glEnable(GL_DEPTH_TEST);
    this->shadowPass();
    m_environment.bind();
    Object3D::draw();
    m_environment.release();
    gl->glDepthFunc(GL_LEQUAL); 
    m_environment.drawSkybox();
    gl->glDepthFunc(GL_LESS);
    gl->glDisable(GL_DEPTH_TEST);
}

lcf::Environment *lcf::Scene::environment()
{
    return &m_environment;
}

QTimer *lcf::Scene::timer()
{
    return &m_timer;
}

lcf::Model *lcf::Scene::takeModel(int index)
{
    if (index < 0 or index >= m_models.size()) { return nullptr; }
    return m_models[index].get();
}

const lcf::Scene::ModelList &lcf::Scene::models() const
{
    return m_models;
}

void lcf::Scene::shadowPass()
{
    for (auto &light : m_lights) {
        if (not light->castShadow()) { continue; }
        light->bind();
        Object3D::drawShadow(light->lightType());
        light->release();
    }
}
