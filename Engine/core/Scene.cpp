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
    Mesh::MaterialPtr material = std::make_shared<Material>();
    m_skybox = std::make_unique<Mesh>(Mesh::GeometryPtr(Geometry::cube()), material);
    m_skybox->setShader(ShaderManager::instance()->get(ShaderManager::Skybox));

    m_fbo = std::make_unique<GLFrameBufferObject>(1036, 674, GLFrameBufferObject::Depth);
}

lcf::Scene *lcf::Scene::global()
{
    static Scene s_instance;
    return &s_instance;
}

lcf::Scene *lcf::Scene::current()
{
    if (not s_current) { return global(); }
    return s_current;
}

void lcf::Scene::setCurrent(Scene *scene)
{
    if (not scene) { return; }
    current()->timer()->stop();
    s_current = scene;
    s_current->timer()->start();
}

void lcf::Scene::addLight(const Light::SharedPtr &light)
{
    if (not light or light->parent() == this) { return; }
    light->setParent(this);
    m_lights.emplace_back(light);
}

void lcf::Scene::addSharedChild(const Object3D::SharedPtr &child)
{
    if (not child) { return; }
    child->setParent(this);
    m_shared_children.emplace_back(child);
}

void lcf::Scene::draw()
{
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    gl->glEnable(GL_DEPTH_TEST);
    this->shadowPass();
    GlobalCamera::instance()->bind();
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
    m_skybox->material()->setTexture(TextureType::UserCustom0, texture);
}

QTimer *lcf::Scene::timer()
{
    return &m_timer;
}

void lcf::Scene::shadowPass()
{
    static SharedGLShaderProgramPtr shadow_shader = ShaderManager::instance()->get(ShaderManager::ShadowMap);
    static SharedGLShaderProgramPtr animated_shadow_shader = ShaderManager::instance()->get(ShaderManager::AnimatedShadowMap);
    for (int i = 0; i < m_lights.size(); ++i) {
        GLHelper::setShaderUniform(shadow_shader.get(), {"light_index", i});
        GLHelper::setShaderUniform(animated_shadow_shader.get(), {"light_index", i});
        if (not m_lights[i]->castShadow()) { continue; }
        auto &light = m_lights[i];
        light->bind(i);
        Object3D::drawShadow();
        light->release();
    }
}
