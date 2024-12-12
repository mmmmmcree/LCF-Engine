#include "SceneManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "Constants.h"
#include "utils.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "ShaderToyManager.h"

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
    static QString scene_name = "grass_land";
    if (m_scenes.find(scene_name) != m_scenes.end()) { return m_scenes[scene_name].get(); }

    auto &scene = m_scenes.emplace(std::make_pair(scene_name, new Scene)).first->second;
    SharedGLTexturePtr texture = TextureManager::instance()->load(path::res_prefix + "bk.jpg", true);
    texture->setMinMagFilters(GLTexture::Nearest, GLTexture::Nearest);
    scene->setSkyboxTexture(texture);

    DirectionalLight::SharedPtr directional_light = std::make_shared<DirectionalLight>();
    directional_light->setName("directional_light[0]");
    directional_light->setAmbientIntensity(0.4f);
    directional_light->setDiffuseIntensity(1.5f);
    scene->addSharedChild(directional_light);
    const auto &directional_light_as_uniform_list = directional_light->asUniformList();

    Model::SharedPtr dinasour = ModelManager::instance()->load(path::source_dir + "models/dinosaur/source/Rampaging T-Rex.glb");
    scene->addSharedChild(dinasour);
    dinasour->scale(0.3f);
    dinasour->translate(3.0f, 0.0f, 0.0f);
    SharedGLShaderProgramPtr shader = ShaderManager::instance()->load({
        {QOpenGLShader::Vertex, path::shaders_prefix + "animated_illumination.vert"},
        {QOpenGLShader::Fragment, path::shaders_prefix + "phong.frag"},
    });
    GLHelper::setShaderUniforms(shader.get(), {
        {"diffuse_channel", 0}, {"specular_channel", 1},
        {"directional_light_num", 1}, {"point_light_num", 0}, {"spot_light_num", 0}
    });
    auto su_binder = std::make_shared<ShaderUniformBinder>(shader);
    su_binder->setUniforms(directional_light_as_uniform_list);
    dinasour->setShaderUniformBinder(su_binder);
    dinasour->playAnimation(0, 1.0f);
    Model::SharedPtr dinasour2 = ModelManager::instance()->clone(dinasour.get());
    scene->addSharedChild(dinasour2);
    dinasour2->scale(0.3f);
    dinasour2->playAnimation(1, 2.0f);

    Model::SharedPtr grass = ModelManager::instance()->load(path::source_dir + "models/grassNew.obj");
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
    auto material = Material::newShared();
    SharedGLTexturePtr grass_color = TextureManager::instance()->load(path::source_dir + "res/GRASS.png");
    material->setTexture(TextureType::Diffuse, grass_color);
    SharedGLTexturePtr grass_mask = TextureManager::instance()->load(path::source_dir + "res/grassMask.png");
    grass_mask->setMinMagFilters(GLTexture::Nearest, GLTexture::Nearest);
    material->setTexture(TextureType::Opacity, grass_mask);
    SharedGLTexturePtr cloud = TextureManager::instance()->load(path::source_dir + "res/CLOUD.png");
    cloud->setMinMagFilters(GLTexture::LinearMipMapLinear, GLTexture::LinearMipMapLinear);
    material->setTexture(TextureType::UserCustom0, cloud);
    grass->setMaterial(material); 
    shader = ShaderManager::instance()->load({
        {QOpenGLShader::Vertex, path::shaders_prefix + "grass.vert"}, 
        {QOpenGLShader::Fragment, path::shaders_prefix + "grass.frag"}, 
    });
    GLHelper::setShaderUniforms(shader.get(), {
        {"channel0", 0}, {"channel1", 1}, {"channel2", 2}
    });
    su_binder = std::make_shared<ShaderUniformBinder>(shader);
    su_binder->setSingleUniform({"time", utils::elapsed_time_s});
    grass->setShaderUniformBinder(su_binder);

    Model::SharedPtr tree1 = ModelManager::instance()->load(path::source_dir + "models/stylized_hand_painted_tree.glb");
    scene->addSharedChild(tree1);
    for (int i = 0; i < 16; ++i) {
        Matrix4x4 tree_mat;
        tree_mat.translate(utils::random_bounded(-32.0f, 32.0f), utils::random_bounded(-32.0f, 32.0f), 0.0f);
        tree_mat.rotate(utils::random_bounded(90.0f), {0.0f, 0.0f, 1.0f});
        tree_mat.scale(utils::random_bounded(0.4f) + 0.8f);
        tree1->instanceHelper()->addInstanceOffset(tree_mat);
    }
    shader = ShaderManager::instance()->load({
        {QOpenGLShader::Vertex, path::shaders_prefix + "instance3D.vert"},
        {QOpenGLShader::Fragment, path::shaders_prefix + "sampler2D_debug.frag"}
    });
    shader->setUniformValue("channel0", 0);
    tree1->setShader(shader);

    Model::SharedPtr ground = ModelManager::instance()->load(path::source_dir + "models/grassland.glb");
    scene->addSharedChild(ground);
    material = Material::newShared();
    material->setTexture(TextureType::Diffuse, grass_color);
    ground->setMaterial(material);
    ground->scale(0.7f);
    ground->translateY(-0.12f);
    shader = ShaderManager::instance()->load( {
        {QOpenGLShader::Vertex, path::shaders_prefix + "grass_land.vert"}, 
        {QOpenGLShader::Fragment, path::shaders_prefix + "grass_land.frag"}, 
    });
    ground->setShader(shader);

    Model::SharedPtr house1 = ModelManager::instance()->load(path::source_dir + "models/house.fbx");
    scene->addSharedChild(house1);
    house1->translate(4.0f, 0.9f, 0.0f);
    shader = ShaderManager::instance()->load({
        {QOpenGLShader::Vertex, path::shaders_prefix + "illumination.vert"},
        {QOpenGLShader::Fragment, path::shaders_prefix + "phong.frag"},
    });
    GLHelper::setShaderUniforms(shader.get(), {
        {"diffuse_channel", 0}, {"specular_channel", 1},
        {"directional_light_num", 1}, {"point_light_num", 0}, {"spot_light_num", 0}
    });
    su_binder = std::make_shared<ShaderUniformBinder>(shader);
    su_binder->setUniforms(directional_light_as_uniform_list);
    house1->setShaderUniformBinder(su_binder);

    connect(scene->timer(), &QTimer::timeout, this, [=] {
        dinasour->translate(0.0f, 0.0f, 0.15f);
        dinasour->rotateY(1.0f);
    });
    scene->timer()->start(1000 / 60);
    return scene.get();
}

lcf::Scene * lcf::SceneManager::testScene()
{
    static QString scene_name = "test";
    if (m_scenes.find(scene_name) != m_scenes.end()) { return m_scenes[scene_name].get(); }
    auto &scene = m_scenes.emplace(std::make_pair(scene_name, new Scene)).first->second;
    UniqueGLTexturePtr texture = TextureManager::instance()->load(path::res_prefix + "black.png");
    texture->setMinMagFilters(GLTexture::Nearest, GLTexture::Nearest);
    scene->setSkyboxTexture(std::move(texture));

    Light::SharedPtr directional_light = std::make_shared<DirectionalLight>();
    directional_light->setName("directional_light[0]");
    scene->addSharedChild(directional_light);
    const auto &light_as_uniform_list = directional_light->asUniformList();

    Model::SharedPtr dinasour = ModelManager::instance()->load(path::source_dir + "models/dinosaur/source/Rampaging T-Rex.glb");
    scene->addSharedChild(dinasour);
    dinasour->scale(0.3f);
    SharedGLShaderProgramPtr shader = ShaderManager::instance()->load({
        {GLShader::Vertex, path::shaders_prefix + "animated_illumination.vert"},
        {GLShader::Fragment, path::shaders_prefix + "phong.frag"},
    });
    GLHelper::setShaderUniforms(shader.get(), {
        {"diffuse_channel", 0}, {"specular_channel", 1},
        {"directional_light_num", 1}, {"point_light_num", 0}, {"spot_light_num", 0}
    });
    auto su_binder = std::make_shared<ShaderUniformBinder>(shader);
    su_binder->setUniforms(light_as_uniform_list);
    dinasour->setShaderUniformBinder(su_binder);
    dinasour->playAnimation(1, 1.0f);


    Model::SharedPtr house1 = ModelManager::instance()->load(path::source_dir + "models/house.fbx");
    scene->addSharedChild(house1);
    house1->translate(4.0f, 0.9f, 0.0f);
    shader = ShaderManager::instance()->load({
        {GLShader::Vertex, path::shaders_prefix + "illumination.vert"},
        {GLShader::Fragment, path::shaders_prefix + "phong.frag"},
    });
    GLHelper::setShaderUniforms(shader.get(), {
        {"diffuse_channel", 0}, {"specular_channel", 1},
        {"directional_light_num", 1}, {"point_light_num", 0}, {"spot_light_num", 0}
    });
    su_binder = std::make_shared<ShaderUniformBinder>(shader);
    su_binder->setUniforms(light_as_uniform_list);
    house1->setShaderUniformBinder(su_binder);
    return scene.get();
}

lcf::Scene * lcf::SceneManager::testShaderToy()
{
    static QString scene_name = "shader_toy";
    if (m_scenes.find(scene_name) != m_scenes.end()) { return m_scenes[scene_name].get(); }
    auto &scene = m_scenes.emplace(std::make_pair(scene_name, new Scene)).first->second;
    auto shader_toy = ShaderToyManager::instance()->load("train", 1024, 768, {
        path::shaders_prefix + "train_buffer.frag",
        path::shaders_prefix + "train.frag",
    });
    ShaderToyManager::instance()->activate("train");
    SharedGLTexturePtr texture = TextureManager::instance()->load(path::res_prefix + "train_noise.png");
    texture->setMinMagFilters(GLTexture::Linear, GLTexture::Linear);
    texture->setWrapMode(GLTexture::Repeat);
    shader_toy->setBuffer(0, {texture, 0});
    shader_toy->setBuffer(1, {0});
    Material::SharedPtr material = std::make_shared<Material>();
    material->setTexture(TextureType::UserCustom0, shader_toy);
    Mesh::SharedPtr mesh = std::make_shared<Mesh>(Geometry::quad(), material);
    mesh->setShader(ShaderManager::instance()->get(ShaderManager::Simple2D));
    mesh->scale(1.5f, 1.0f, 1.0f);
    scene->addSharedChild(mesh);
    return scene.get();
}
