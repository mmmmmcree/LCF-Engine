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
#include "UserCustomMaterial.h"
#include "LightArray.h"
#include "ControlManager.h"

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

void lcf::SceneManager::makeGrassLand()
{
    static QString scene_name = "grass_land";
    if (m_scenes.find(scene_name) != m_scenes.end()) {
        ControlManager::instance()->setCurrentScene(m_scenes[scene_name].get());
        return;
    }
    auto &scene = m_scenes.emplace(std::make_pair(scene_name, new Scene)).first->second;
    ControlManager::instance()->setCurrentScene(scene.get());

    SharedGLTexturePtr texture = TextureManager::instance()->load(path::res_prefix + "bk.jpg");
    texture->setMinMagFilters(GLTexture::Nearest, GLTexture::Nearest);
    scene->setSkyboxTexture(texture);

    DirectionalLight::SharedPtr directional_light = DirectionalLight::createShared();
    directional_light->rotateX(-90.0f);
    directional_light->setName("directional_light[0]");
    scene->addObject3D(directional_light);
    const auto &directional_light_as_uniform_list = directional_light->asUniformList();

    Model::SharedPtr dinasour = ModelManager::instance()->load(path::source_dir + "models/dinosaur/source/Rampaging T-Rex.glb");
    scene->addObject3D(dinasour);
    dinasour->scale(0.3f);
    dinasour->translate(3.0f, 0.0f, 0.0f);
    SharedGLShaderProgramPtr shader = ShaderManager::instance()->load({
        {QOpenGLShader::Vertex, path::shaders_prefix + "animated_illumination.vert"},
        {QOpenGLShader::Fragment, path::shaders_prefix + "phong.frag"},
    });
    GLHelper::setShaderUniforms(shader.get(), {
        {"directional_light_num", 1}, {"point_light_num", 0}, {"spot_light_num", 0}
    });
    auto su_binder = ShaderUniformBinder::createShared(shader);
    su_binder->setUniforms(directional_light_as_uniform_list);
    dinasour->setShaderUniformBinder(su_binder);
    dinasour->playAnimation(0, 1.0f);
    Model::SharedPtr dinasour2 = ModelManager::instance()->clone(dinasour.get());
    scene->addObject3D(dinasour2);
    dinasour2->scale(0.3f);
    dinasour2->playAnimation(1, 2.0f);

    Model::SharedPtr grass = ModelManager::instance()->load(path::source_dir + "models/grassNew.obj");
    scene->addObject3D(grass);
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
    SharedGLTexturePtr grass_color = TextureManager::instance()->load(path::source_dir + "res/GRASS.png");
    grass->materialController()->setMaterialType(MaterialType::UserCustom);
    grass->materialController()->setTexture(TextureType::Diffuse, grass_color);
    SharedGLTexturePtr grass_mask = TextureManager::instance()->load(path::source_dir + "res/grassMask.png");
    grass_mask->setMinMagFilters(GLTexture::Nearest, GLTexture::Nearest);
    grass->materialController()->setTexture(TextureType::Opacity, grass_mask);
    SharedGLTexturePtr cloud = TextureManager::instance()->load(path::source_dir + "res/CLOUD.png");
    cloud->setMinMagFilters(GLTexture::LinearMipMapLinear, GLTexture::LinearMipMapLinear);
    grass->materialController()->setTexture(TextureType::UserCustom0, cloud);
    shader = ShaderManager::instance()->load({
        {QOpenGLShader::Vertex, path::shaders_prefix + "grass.vert"}, 
        {QOpenGLShader::Fragment, path::shaders_prefix + "grass.frag"}, 
    });
    GLHelper::setShaderUniforms(shader.get(), {
        {"channel0", 0}, {"channel1", 1}, {"channel2", 2}
    });
    su_binder = ShaderUniformBinder::createShared(shader);
    su_binder->setSingleUniform({"time", utils::elapsed_time_s});
    grass->setShaderUniformBinder(su_binder);

    Model::SharedPtr tree1 = ModelManager::instance()->load(path::source_dir + "models/stylized_hand_painted_tree.glb");
    scene->addObject3D(tree1);
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
    scene->addObject3D(ground);
    ground->materialController()->setMaterialType(MaterialType::UserCustom);
    ground->materialController()->setTexture(TextureType::Diffuse, grass_color);
    ground->scale(0.7f);
    ground->translateY(-0.12f);
    shader = ShaderManager::instance()->load( {
        {QOpenGLShader::Vertex, path::shaders_prefix + "grass_land.vert"}, 
        {QOpenGLShader::Fragment, path::shaders_prefix + "grass_land.frag"}, 
    });
    ground->setShader(shader);

    Model::SharedPtr house1 = ModelManager::instance()->load(path::source_dir + "models/house.fbx");
    scene->addObject3D(house1);
    house1->translate(4.0f, 0.9f, 0.0f);
    shader = ShaderManager::instance()->load({
        {QOpenGLShader::Vertex, path::shaders_prefix + "illumination.vert"},
        {QOpenGLShader::Fragment, path::shaders_prefix + "phong.frag"},
    });
    GLHelper::setShaderUniforms(shader.get(), {
        {"material.diffuse_map", 0}, {"material.specular_map", 1}, {"material.normal_map", 2}, 
        {"directional_light_num", 1}, {"point_light_num", 0}, {"spot_light_num", 0}
    });
    su_binder = ShaderUniformBinder::createShared(shader);
    su_binder->setUniforms(directional_light_as_uniform_list);
    house1->setShaderUniformBinder(su_binder);

    connect(scene->timer(), &QTimer::timeout, this, [=] {
        dinasour->translate(0.0f, 0.0f, 0.15f);
        dinasour->rotateY(1.0f);
    });
}

void lcf::SceneManager::makeTestScene()
{
    static QString scene_name = "test";
    if (m_scenes.find(scene_name) != m_scenes.end()) { return; }
    auto &scene = m_scenes.emplace(std::make_pair(scene_name, new Scene)).first->second;
    ControlManager::instance()->setCurrentScene(scene.get());

    SharedGLTexturePtr texture = TextureManager::instance()->load(path::res_prefix + "bk.jpg");
    texture->setMinMagFilters(GLTexture::Nearest, GLTexture::Nearest);
    scene->setSkyboxTexture(texture);

    PointLight::SharedPtr point_light0 = PointLight::createShared();
    point_light0->setColor({100.0f, 100.0f, 100.0f});
    point_light0->setTranslation({1.8f, 0.8f, 0.0f});
    point_light0->scale(0.3f);
    point_light0->setCastShadow(true);
    scene->addObject3D(point_light0);

    DirectionalLight::SharedPtr directional_light = DirectionalLight::createShared();
    directional_light->setTranslation({0.0f, 3.0f, 0.0f});
    directional_light->rotateX(-90.0f);
    directional_light->setColor({10.0f, 10.0f, 10.0f});
    directional_light->setCastShadow(true);
    scene->addObject3D(directional_light);

    Model::SharedPtr room = ModelManager::instance()->load(path::source_dir + "models/original_backrooms.glb");
    room->scale(2.0f);
    room->translateY(-0.12f);
    SharedGLShaderProgramPtr shader = ShaderManager::instance()->get(ShaderManager::ShadowedPhong);
    room->setShader(shader);
    scene->addObject3D(room);

    Model::SharedPtr robot = ModelManager::instance()->load(path::source_dir + "models/nuirter_real-time.glb");
    robot->materialController()->setMaterialType(MaterialType::PBR);
    robot->setCastShadow(true);
    shader = ShaderManager::instance()->get(ShaderManager::ShadowedPBR);
    robot->setShader(shader);
    scene->addObject3D(robot);

    Model::SharedPtr helmet = ModelManager::instance()->load(path::source_dir + "models/DamagedHelmet/glTF-Binary/DamagedHelmet.glb");
    helmet->scale(0.3f);
    helmet->translateY(5.0f);
    helmet->materialController()->setMaterialType(MaterialType::PBR);
    helmet->setCastShadow(true);
    scene->addObject3D(helmet);

    Model::SharedPtr dinosaur = ModelManager::instance()->load(path::source_dir + "models/dinosaur/source/Rampaging T-Rex.glb");
    dinosaur->scale(0.3f);
    dinosaur->translate(-3.0f, 0.0f, 0.0f);
    dinosaur->setCastShadow(true);
    shader = ShaderManager::instance()->get(ShaderManager::AnimatedPhong);
    dinosaur->setShader(shader);
    scene->addObject3D(dinosaur);
    dinosaur->playAnimation(1, 1.0f);

    Model::SharedPtr dinosaur2 = ModelManager::instance()->clone(dinosaur.get());
    dinosaur2->translate(3.0f, 0.0f, 0.0f);
    dinosaur2->setCastShadow(true);
    scene->addObject3D(dinosaur2);
    dinosaur2->playAnimation(0, 1.0f);

    connect(scene->timer(), &QTimer::timeout, this, [=] {
        static float d = 0;
        helmet->translateX(qSin(d) * 0.1f);
        point_light0->translateY(qSin(d) * 0.1f);
        d += 0.02f;
    });
}

void lcf::SceneManager::testShaderToy()
{
    static QString scene_name = "shader_toy";
    if (m_scenes.find(scene_name) != m_scenes.end()) { return; }
    auto &scene = m_scenes.emplace(std::make_pair(scene_name, new Scene)).first->second;
    ControlManager::instance()->setCurrentScene(scene.get());
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
    Mesh::SharedPtr mesh = Mesh::createShared(Geometry::quad());
    mesh->materialController()->setMaterialType(MaterialType::UserCustom);
    mesh->materialController()->setTexture(TextureType::UserCustom0, shader_toy);
    mesh->setShader(ShaderManager::instance()->get(ShaderManager::Simple2D));
    scene->addObject3D(mesh);
}
