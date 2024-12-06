#include "SceneManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "Constants.h"
#include "utils.h"
#include "MModelManager.h"

lcf::SceneManager *lcf::SceneManager::instance()
{
    static SceneManager s_instance;
    return &s_instance;
}

void lcf::SceneManager::initialize(QOpenGLContext *context)
{
    m_context = context;
    m_surface = new QOffscreenSurface(nullptr, this);
    m_surface->setFormat(context->format());
    m_surface->create();
}

lcf::Scene *lcf::SceneManager::get(const QString &name)
{
    auto iter = m_scenes.find(name);
    if (iter == m_scenes.end()) {
        qDebug() << "Scene not found: " << name;
        return nullptr;
    }
    return iter->second.get();
}

lcf::Scene *lcf::SceneManager::makeGrassLand()
{
    static bool initialized = false;
    QString scene_name = "grass_land";
    if (initialized) { return m_scenes[scene_name].get(); }
    initialized = true;

    auto &scene = m_scenes.emplace(std::make_pair(scene_name, new Scene)).first->second;
    
    UniqueGLTexturePtr texture = TextureManager::instance()->load(path::res_prefix + "bk.jpg", true);
    texture->setMinMagFilters(GLTexture::Nearest, GLTexture::Nearest);
    scene->setSkyboxTexture(std::move(texture));

    Model::SharedPtr dinasour = MModelManager::instance()->load(path::source_dir + "models/dinosaur/source/Rampaging T-Rex.glb");
    scene->addSharedChild(dinasour);
    dinasour->scale(0.3f);
    dinasour->translate(3.0f, 0.0f, 0.0f);
    dinasour->setShader(ShaderManager::instance()->get("animation_debug_3D"));
    dinasour->playAnimation(0, 1.0f);
    Model::SharedPtr dinasour2 = MModelManager::instance()->clone(dinasour.get());
    scene->addSharedChild(dinasour2);
    dinasour2->scale(0.3f);
    dinasour2->setShader(ShaderManager::instance()->get("animation_debug_3D"));
    dinasour2->playAnimation(1, 2.0f);


    Model::SharedPtr grass = MModelManager::instance()->load(path::source_dir + "models/grassNew.obj");
    scene->addSharedChild(grass);
    Matrix4x4 grass_mat;
    grass_mat.translate(-16.0f, 0.0f, 16.0f);
    for (int i = 0; i < 160; ++i) {
        grass_mat.translate(0.2f, 0.0f, -32.0f);
        for (int j = 0; j < 160; ++j) {
            grass_mat.translate(0.0f, 0.0f, 0.2f);
            Matrix4x4 m = grass_mat;
            m.rotate(utils::random_bounded(90.0f), {0.0f, 1.0f, 0.0f});
            m.translate(0.0f, utils::random_bounded(-0.05f, 0.05f), 0.0f);
            m.scale(utils::random_bounded(0.2f) + 0.8f);
            grass->instanceHelper()->addInstanceOffset(m);
        }
    }
    auto material = Material::newCreatedShared();
    SharedGLTexturePtr grass_color = TextureManager::instance()->load(path::source_dir + "res/GRASS.png");
    material->addTexture(grass_color);
    SharedGLTexturePtr grass_mask = TextureManager::instance()->load(path::source_dir + "res/grassMask.png");
    grass_mask->setMinMagFilters(GLTexture::Nearest, GLTexture::Nearest);
    material->addTexture(grass_mask);
    SharedGLTexturePtr cloud = TextureManager::instance()->load(path::source_dir + "res/CLOUD.png");
    material->addTexture(cloud);
    grass->setMaterial(material); 

    UniqueGLShaderProgramPtr shader = nullptr;
    shader = ShaderManager::instance()->load({
        {QOpenGLShader::Vertex, path::shaders_prefix + "grass.vert"}, 
        {QOpenGLShader::Fragment, path::shaders_prefix + "grass.frag"}, 
    });
    GLHelper::setShaderUniforms(shader.get(), {
        {"channel0", 0}, {"channel1", 1}, {"channel2", 2}
    });
    grass->setShader(std::move(shader));

    Model::SharedPtr ground = MModelManager::instance()->load(path::source_dir + "models/grassland.glb");
    scene->addSharedChild(ground);
    material = Material::newCreatedShared();
    material->addTexture(grass_color);
    ground->setMaterial(material);
    ground->scale(0.7f);
    ground->translateY(-0.12f);
    shader = ShaderManager::instance()->load( {
        {QOpenGLShader::Vertex, path::shaders_prefix + "grass_land.vert"}, 
        {QOpenGLShader::Fragment, path::shaders_prefix + "grass_land.frag"}, 
    });
    ground->setShader(std::move(shader));

    Model::SharedPtr house1 = MModelManager::instance()->load(path::source_dir + "models/house.fbx");
    scene->addSharedChild(house1);
    house1->translate(4.0f, 0.9f, 0.0f);
    house1->setShader(ShaderManager::instance()->get("sampler2D_debug_3D"));

    connect(scene->timer(), &QTimer::timeout, this, [=] {
        dinasour->translate(0.0f, 0.0f, 0.15f);
        dinasour->rotateY(1.0f);
    });
    scene->timer()->start(1000 / 60);
    return scene.get();
}
