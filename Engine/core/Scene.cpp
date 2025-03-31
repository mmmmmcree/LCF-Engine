#include "Scene.h"
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include "ShaderManager.h"
#include "TextureManager.h"
#include "GlobalCamera.h"
#include "DirectionalLight.h"
#include "Constants.h"
#include "SignalSender.h"
#include "bullet/btBulletDynamicsCommon.h"

lcf::Scene::Scene()
{
    m_timer.setInterval(1000 / 60);
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
    SignalSender::instance()->sendModelAddedSignal(model.get());
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

void lcf::Scene::render()
{
    this->update();

    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    gl->glEnable(GL_DEPTH_TEST);
    this->shadowPass();
    m_environment.bind();
    for (auto mesh : m_renderable_list) {
        mesh->draw();
    }
    for (auto &light : m_lights) {
        light->draw();
    }
    m_environment.release();
    gl->glDepthFunc(GL_LEQUAL); 
    m_environment.drawSkybox();
    gl->glDepthFunc(GL_LESS);
    gl->glDisable(GL_DEPTH_TEST);
}

void lcf::Scene::activatePhysicalWorld(bool active)
{
    if (not active and not m_physical_world) { return; }
    if (not m_physical_world) { m_physical_world = PhysicalWorld::createUnique(); }
    m_physical_world->activate(active);
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

void lcf::Scene::update()
{
    if (m_physical_world) { m_physical_world->stepSimulation(1.0f / 60.0f, 10); }
    m_culler.setFrustumPlanes(GlobalCamera::instance()->getProjectionViewMatrix());
    m_renderable_list.clear();
    for (auto &light : m_lights) {
        light->update();
    }
    for (auto &model : m_models) {
        model->update(1.0f / 60.0f);
        for (auto &mesh : model->meshes()) {
            if (m_culler.isCulled(mesh->getBoundingShape<Sphere>())) { continue; }
            m_renderable_list.emplace_back(mesh.get());
        }
    }
    for (auto &mesh : m_meshes) {
        if (m_culler.isCulled(mesh->getBoundingShape<Sphere>())) { continue; }
        m_renderable_list.emplace_back(mesh.get());
    }
}

void lcf::Scene::shadowPass()
{
    for (auto &light : m_lights) {
        if (not light->isCastShadow()) { continue; }
        light->bind();
        for (auto mesh : m_renderable_list) {
            mesh->drawShadow(light->lightType());
        }
        light->release();
    }
}
