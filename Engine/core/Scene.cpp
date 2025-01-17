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
    m_skybox = std::make_unique<Mesh>(Mesh::GeometryPtr(Geometry::cube()));
    m_skybox->setShader(ShaderManager::instance()->get(ShaderManager::Skybox));
    m_skybox->materialController()->setMaterialType(MaterialType::UserCustom);
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
            m_lights.addConfiguredDirectionalLight(std::static_pointer_cast<DirectionalLight>(light));
        } break;
        case LightType::Point : {
            m_lights.addConfiguredPointLight(std::static_pointer_cast<PointLight>(light));
        } break;
        case LightType::Spot : {
            m_lights.addConfiguredSpotLight(std::static_pointer_cast<SpotLight>(light));
        } break;
    }
    this->addSharedChild(light);
    m_lights.allocateShadowMapUnits();
}

void lcf::Scene::addModel(const Model::SharedPtr &model)
{
    if (not model or model->parent() == this) { return; }
    model->setParent(this);
    m_models.emplace_back(model);
    if (m_signal_sender) {
        m_signal_sender->sendModelsUpdatedSignal(model.get());
    }
}

void lcf::Scene::addMesh(const Mesh::SharedPtr &mesh)
{
    if (not mesh or mesh->parent() == this) { return; }
    mesh->setParent(this);
    m_meshes.emplace_back(mesh);
}

void lcf::Scene::addSharedChild(const Object3D::SharedPtr &child)
{
    if (not child or child->parent() == this) { return; }
    child->setParent(this);
    m_shared_children.emplace_back(child);
}

void lcf::Scene::draw()
{
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    gl->glEnable(GL_DEPTH_TEST);
    this->shadowPass();
    Object3D::draw();
    gl->glDepthFunc(GL_LEQUAL); 
    m_skybox->draw();
    gl->glDepthFunc(GL_LESS);
    gl->glDisable(GL_DEPTH_TEST);
    /*
    todo: 模板测试可以在深度测试关闭后进行；
    todo: 将所有需要绘制边界的物体存储在一个list中，在这里进行绘制； 
    todo: 给Object3D添加一个virtual drawBorder(),在原draw()中修改shader和传入shader的model_matrix即可
    todo: 也可以在这里统一给一个shader，在drawBorder中修改model_matrix即可
    */
}

void lcf::Scene::setSkyboxTexture(TextureWrapper texture)
{
    m_skybox->materialController()->setTexture(TextureType::UserCustom0, texture);
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
