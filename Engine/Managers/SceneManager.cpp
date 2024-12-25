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
    SharedGLTexturePtr texture = TextureManager::instance()->load(path::res_prefix + "bk.jpg");
    texture->setMinMagFilters(GLTexture::Nearest, GLTexture::Nearest);
    scene->setSkyboxTexture(texture);

    DirectionalLight::SharedPtr directional_light = DirectionalLight::createShared();
    directional_light->rotateX(-90.0f);
    directional_light->setName("directional_light[0]");
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
        {"directional_light_num", 1}, {"point_light_num", 0}, {"spot_light_num", 0}
    });
    auto su_binder = ShaderUniformBinder::createShared(shader);
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
    SharedGLTexturePtr grass_color = TextureManager::instance()->load(path::source_dir + "res/GRASS.png");
    grass->materialController()->setType(MaterialType::UserCustom);
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
    ground->materialController()->setType(MaterialType::UserCustom);
    ground->materialController()->setTexture(TextureType::Diffuse, grass_color);
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
    return scene.get();
}

lcf::Scene *lcf::SceneManager::testScene()
{
    static QString scene_name = "test";
    if (m_scenes.find(scene_name) != m_scenes.end()) { return m_scenes[scene_name].get(); }
    auto &scene = m_scenes.emplace(std::make_pair(scene_name, new Scene)).first->second;
    UniqueGLTexturePtr texture = TextureManager::instance()->load(path::res_prefix + "bk.jpg");
    texture->setMinMagFilters(GLTexture::Nearest, GLTexture::Nearest);
    scene->setSkyboxTexture(std::move(texture));

    auto directional_light = DirectionalLight::createShared();
    directional_light->rotateX(-90.0f);
    directional_light->setName("directional_light[0]");
    directional_light->setColor({100.0f, 100.0f, 100.0f});
    scene->addLight(directional_light);
    const auto &light_as_uniform_list = directional_light->asUniformList();

    Model::SharedPtr room = ModelManager::instance()->load(path::source_dir + "models/abandoned_warehouse_-_interior_scene.glb");
    scene->addSharedChild(room);
    room->translateZ(40.0f);
    room->scale(8.0f);
    SharedGLShaderProgramPtr shader = ShaderManager::instance()->load({
        {GLShader::Vertex, path::shaders_prefix + "illumination.vert"},
        {GLShader::Fragment, path::shaders_prefix + "phong.frag"},
    });
    GLHelper::setShaderUniforms(shader.get(), {
        {"directional_light_num", 1}, {"point_light_num", 0}, {"spot_light_num", 0}
    });
    auto su_binder = ShaderUniformBinder::createShared(shader);
    su_binder->setUniforms(light_as_uniform_list);
    room->setShaderUniformBinder(su_binder);

    
    return scene.get();
}

lcf::Scene * lcf::SceneManager::testPointLightShadow()
{
    static QString scene_name = "test_point_light_shadow";
    if (m_scenes.find(scene_name) != m_scenes.end()) { return m_scenes[scene_name].get(); }
    auto &scene = m_scenes.emplace(std::make_pair(scene_name, new Scene)).first->second;
    UniqueGLTexturePtr texture = TextureManager::instance()->load(path::res_prefix + "bk.jpg");
    texture->setMinMagFilters(GLTexture::Nearest, GLTexture::Nearest);
    scene->setSkyboxTexture(std::move(texture));

    auto point_light = PointLight::createShared();
    point_light->setCastShadow(true);
    point_light->setName("point_light[0]");
    point_light->setPosition({1.0f, -2.0f, 1.0f});
    point_light->scale(0.3f);
    scene->addLight(point_light);
    const auto &light_as_uniform_list = point_light->asUniformList();

    Model::SharedPtr dinasour = ModelManager::instance()->load(path::source_dir + "models/dinosaur/source/Rampaging T-Rex.glb");
    scene->addSharedChild(dinasour);
    dinasour->translateX(-1.0f);
    dinasour->setCastShadow(true);
    dinasour->scale(0.3f);
    SharedGLShaderProgramPtr shader = ShaderManager::instance()->load({
        {GLShader::Vertex, path::shaders_prefix + "animated_illumination.vert"},
        {GLShader::Fragment, path::shaders_prefix + "phong.frag"},
    });
    GLHelper::setShaderUniforms(shader.get(), {
        {"directional_light_num", 0}, {"point_light_num", 1}, {"spot_light_num", 0}
    });
    auto su_binder = ShaderUniformBinder::createShared(shader);
    su_binder->setUniforms(light_as_uniform_list);
    dinasour->setShaderUniformBinder(su_binder);
    dinasour->playAnimation(1, 1.0f);


    Model::SharedPtr house1 = ModelManager::instance()->load(path::source_dir + "models/house.fbx");
    scene->addSharedChild(house1);
    house1->setCastShadow(true);
    house1->translate(5.0f, 0.9f, 0.0f);
    shader = ShaderManager::instance()->load({
        {GLShader::Vertex, path::shaders_prefix + "illumination.vert"},
        {GLShader::Fragment, path::shaders_prefix + "phong.frag"},
    });
    GLHelper::setShaderUniforms(shader.get(), {
        {"directional_light_num", 0}, {"point_light_num", 1}, {"spot_light_num", 0}
    });
    su_binder = ShaderUniformBinder::createShared(shader);
    su_binder->setUniforms(light_as_uniform_list);
    house1->setShaderUniformBinder(su_binder);

    Mesh::SharedPtr cube = Mesh::createShared(Geometry::cube());
    cube->scale(16.0f);
    shader = ShaderManager::instance()->load({
        {GLShader::Vertex, path::shaders_prefix + "point_shadow_map_debug.vert"},
        {GLShader::Fragment, path::shaders_prefix + "point_shadow_map_debug.frag"}
    });

    cube->materialController()->setType(MaterialType::UserCustom);
    cube->materialController()->setTexture(TextureType::UserCustom0, point_light->shadowMapTexture());
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
    su_binder = ShaderUniformBinder::createShared(shader);
    su_binder->setSingleUniform({"light_index", point_light->index()});
    cube->setShader(shader);
    scene->addSharedChild(cube);
    connect(scene->timer(), &QTimer::timeout, this, [=] {
        // static float d = 0;
        // point_light->translateY(qSin(d) * 0.2f);
        // d += 0.02f;
    });
    scene->timer()->start(1000 / 60);
    return scene.get();
}

lcf::Scene *lcf::SceneManager::testDirectionalLightShadow()
{
    static QString scene_name = "test_directional_light_shadow";
    if (m_scenes.find(scene_name) != m_scenes.end()) { return m_scenes[scene_name].get(); }
    auto &scene = m_scenes.emplace(std::make_pair(scene_name, new Scene)).first->second;
    UniqueGLTexturePtr texture = TextureManager::instance()->load(path::res_prefix + "bk.jpg");
    texture->setMinMagFilters(GLTexture::Nearest, GLTexture::Nearest);
    scene->setSkyboxTexture(std::move(texture));

    auto directional_light = DirectionalLight::createShared();
    directional_light->setCastShadow(true);
    directional_light->setName("directional_light[0]");
    directional_light->setPosition({1.0f, 7.0f, 3.0f});
    directional_light->rotateX(-90.0f);
    directional_light->setSpecularIntensity(10.0f);
    scene->addLight(directional_light);
    const auto &light_as_uniform_list = directional_light->asUniformList();

    Model::SharedPtr dinasour = ModelManager::instance()->load(path::source_dir + "models/dinosaur/source/Rampaging T-Rex.glb");
    scene->addSharedChild(dinasour);
    dinasour->setCastShadow(true);
    dinasour->scale(0.3f);
    SharedGLShaderProgramPtr shader = ShaderManager::instance()->load({
        {GLShader::Vertex, path::shaders_prefix + "animated_illumination.vert"},
        {GLShader::Fragment, path::shaders_prefix + "phong.frag"},
    });
    GLHelper::setShaderUniforms(shader.get(), {
        {"directional_light_num", 1}, {"point_light_num", 0}, {"spot_light_num", 0}
    });
    auto su_binder = ShaderUniformBinder::createShared(shader);
    su_binder->setUniforms(light_as_uniform_list);
    dinasour->setShaderUniformBinder(su_binder);
    dinasour->playAnimation(1, 1.0f);


    Model::SharedPtr house1 = ModelManager::instance()->load(path::source_dir + "models/house.fbx");
    scene->addSharedChild(house1);
    house1->setCastShadow(true);
    house1->translate(4.0f, 0.9f, 0.0f);
    shader = ShaderManager::instance()->load({
        {GLShader::Vertex, path::shaders_prefix + "illumination.vert"},
        {GLShader::Fragment, path::shaders_prefix + "phong.frag"},
    });
    GLHelper::setShaderUniforms(shader.get(), {
        {"directional_light_num", 1}, {"point_light_num", 0}, {"spot_light_num", 0}
    });
    su_binder = ShaderUniformBinder::createShared(shader);
    su_binder->setUniforms(light_as_uniform_list);
    house1->setShaderUniformBinder(su_binder);

    Mesh::SharedPtr plane = Mesh::createShared(Geometry::quad());
    plane->translateY(-3.0f);
    plane->rotateX(-90.0f);
    plane->scale(8.0f);
    shader = ShaderManager::instance()->load({
        {GLShader::Vertex, path::shaders_prefix + "directional_shadow_map_debug.vert"},
        {GLShader::Fragment, path::shaders_prefix + "directional_shadow_map_debug.frag"}
    });

    plane->materialController()->setType(MaterialType::UserCustom);
    plane->materialController()->setTexture(TextureType::UserCustom0, directional_light->shadowMapTexture());
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
    su_binder = ShaderUniformBinder::createShared(shader);
    su_binder->setSingleUniform({"light_index", directional_light->index()});
    plane->setShader(shader);
    scene->addSharedChild(plane);
    connect(scene->timer(), &QTimer::timeout, this, [=] {
        static float d = 0;
        if (d < 45) {
            directional_light->rotateX(0.1f);
        } else {
            directional_light->rotateX(-0.1f);
        }
        d = fmod(d + 0.1f, 90.0f);
    });
    scene->timer()->start(1000 / 60);
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
    Mesh::SharedPtr mesh = Mesh::createShared(Geometry::quad());
    mesh->materialController()->setType(MaterialType::UserCustom);
    mesh->materialController()->setTexture(TextureType::UserCustom0, shader_toy);
    mesh->setShader(ShaderManager::instance()->get(ShaderManager::Simple2D));
    scene->addSharedChild(mesh);
    return scene.get();
}
