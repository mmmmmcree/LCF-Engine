#include "Scene.h"
#include <QOpenGLFunctions>
#include "ShaderManager.h"
#include "TextureManager.h"

lcf::Scene::Scene() : Object3D()
{
    Mesh::MaterialPtr material = std::make_shared<Material>();
    material->create();
    m_skybox = std::make_unique<Mesh>(Mesh::GeometryPtr(Geometry::cube()), material);
    m_skybox->setShader(ShaderManager::instance()->get("skybox"));
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

void lcf::Scene::addSharedChild(const Object3D::SharedPtr &child)
{
    if (not child) { return; }
    child->setParent(this);
    m_shared_children.emplace_back(child);
}

void lcf::Scene::draw()
{
    Object3D::draw();
    if (m_skybox) {
        auto gl = QOpenGLContext::currentContext()->functions();
        gl->glDepthFunc(GL_LEQUAL); 
        m_skybox->draw();
        gl->glDepthFunc(GL_LESS);
    }
}

void lcf::Scene::setSkyboxTexture(TextureWrapper texture)
{
    m_skybox->material()->setTexture(TextureType::Diffuse, texture);
}

QTimer *lcf::Scene::timer()
{
    return &m_timer;
}
