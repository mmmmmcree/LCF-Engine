#include "ModelManager.h"
#include <QFileInfo>
#include "TextureManager.h"
#include <QThread>
#include "Scene.h"
#include <QTimer>
#include <QRandomGenerator>
#include <QApplication>


lcf::ModelManager::ModelManager() : QObject()
{
    QThread *thread = new QThread(nullptr);
    this->moveToThread(thread);
    connect(this, &ModelManager::startInitialLoading, this, &ModelManager::initialLoad);
    connect(this, &ModelManager::startLoading, this, &ModelManager::load);
    connect(this, &ModelManager::destroyed, this, [thread] {
        thread->quit();
        thread->wait();
        thread->deleteLater();
    });
    thread->start();
    emit startInitialLoading(QString(SOURCE_DIR) + "/models/Vampire/vampire.dae");
    emit startInitialLoading(QString(SOURCE_DIR) + "/models/dinosaur/source/Rampaging T-Rex.glb");
    emit startInitialLoading(QString(SOURCE_DIR) + "/models/Deadpool 1/Source/deadpool.fbx");
}

void lcf::ModelManager::initialLoad(const QString &model_path)
{
    Model *model = this->loadSingleThread(model_path);
    if (model) {
        emit initialModelLoaded(model);
    }
}

void lcf::ModelManager::load(const QString &model_path)
{
    Model *model = this->loadSingleThread(model_path);
    if (model) {
        emit modelLoaded(model);
    }
}

lcf::ModelManager *lcf::ModelManager::instance()
{
    static ModelManager s_instance;
    return &s_instance;
}

void lcf::ModelManager::initialize()
{
    instance();
}

lcf::Model *lcf::ModelManager::loadSingleThread(const QString &model_path)
{
    QFileInfo file_info(model_path);
    if (not file_info.exists()) {
        qDebug() << "Model file does not exist:" << model_path;
        return nullptr;
    }
    const QString &path = file_info.path() + '/';
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(model_path.toStdString(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if (not scene or scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE or not scene->mRootNode) {
        qDebug() << "Failed to load model:" << model_path;
        return nullptr;
    }
    Materials materials;
    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        materials.emplace_back(processMaterial(scene->mMaterials[i], scene, path));
    }
    BoneMap bone_map;
    Bone *root_bone = this->processSkeleton(scene->mRootNode, scene, nullptr, bone_map);
    Model *model = new Model;
    this->processNode(model, scene->mRootNode, scene, materials, bone_map);
    this->processAnimations(model, scene, bone_map);
    model->setBones(root_bone, std::move(bone_map));
    return model;
}


lcf::Material *lcf::ModelManager::processMaterial(aiMaterial *ai_material, const aiScene *scene, const QString &path)
{
    Material *material = new Material;
    std::unordered_map<std::string, Image> image_map;
    for (int type = aiTextureType_NONE; type <= aiTextureType_TRANSMISSION; ++type) {
        int count = ai_material->GetTextureCount(static_cast<aiTextureType>(type));
        for (int i = 0; i < count; ++i) {
            aiString texture_path;
            ai_material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture_path);
            auto iter = image_map.find(texture_path.C_Str());
            if (iter != image_map.end()) { continue; }
            const aiTexture *ai_texture = scene->GetEmbeddedTexture(texture_path.C_Str());
            Image image;
            if (ai_texture) {
                unsigned char *data = reinterpret_cast<unsigned char *>(ai_texture->pcData);
                image = dataToImage(data, ai_texture->mWidth, ai_texture->mHeight);
            } else if (texture_path.length > 0) {
                image = Image(path + texture_path.C_Str());
            }
            image_map.insert(std::make_pair(texture_path.C_Str(), image));
            material->addImageData(image);
        }
    }
    return material;
}

void lcf::ModelManager::processAnimations(Model *model, const aiScene *scene, const BoneMap &bone_map)
{
    for (unsigned int i = 0; i < scene->mNumAnimations; ++i) {
        auto ai_animation = scene->mAnimations[i];
        std::string name = ai_animation->mName.C_Str();
        float duration = ai_animation->mDuration;
        float ticks_per_second = ai_animation->mTicksPerSecond;
        Animation::KeyFrameTracks tracks;
        for (unsigned int j = 0; j < ai_animation->mNumChannels; ++j) {
            auto ai_channel = ai_animation->mChannels[j];
            std::string bone_name = ai_channel->mNodeName.C_Str();
            KeyFrameTrack::Positions positions;
            for (unsigned int k = 0; k < ai_channel->mNumPositionKeys; ++k) {
                Vector3D position = toVector3D(ai_channel->mPositionKeys[k].mValue);
                float time = ai_channel->mPositionKeys[k].mTime;
                positions.emplace_back(time, position);
            }
            KeyFrameTrack::Rotations rotations;
            for (unsigned int k = 0; k < ai_channel->mNumRotationKeys; ++k) {
                Quaternion rotation = toQuaternion(ai_channel->mRotationKeys[k].mValue);
                float time = ai_channel->mRotationKeys[k].mTime;
                rotations.emplace_back(time, rotation);
            }
            KeyFrameTrack::Scales scales;
            for (unsigned int k = 0; k < ai_channel->mNumScalingKeys; ++k) {
                Vector3D scale = toVector3D(ai_channel->mScalingKeys[k].mValue);
                float time = ai_channel->mScalingKeys[k].mTime;
                scales.emplace_back(time, scale);
            }
            KeyFrameTrack track(bone_name, std::move(positions), std::move(rotations), std::move(scales));
            tracks.emplace_back(std::move(track));
        }
        Animation::KeyFrameTracksPtr tracks_ptr = std::make_shared<Animation::KeyFrameTracks>(std::move(tracks));
        Animation *animation = new Animation(name, duration, ticks_per_second, tracks_ptr);
        animation->updateControlledBones(bone_map);
        model->addAnimation(Model::AnimationPtr(animation));
    }
}

void lcf::ModelManager::processNode(Model *model, aiNode *node, const aiScene *scene, const Materials &materials, const BoneMap &bone_map)
{
    Bone *bone = bone_map.find(node->mName.C_Str())->second;
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        unsigned int mesh_id = node->mMeshes[i];
        aiMesh *ai_mesh = scene->mMeshes[mesh_id];
        Mesh *mesh = this->processMesh(ai_mesh, scene, materials, bone_map);
        mesh->setParent(model);
        model->addMesh(Model::MeshPtr(mesh));
        if (scene->mNumAnimations > 0) { continue; }
        mesh->setLocalMatrix(bone->worldMatrix());
    }
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        this->processNode(model, node->mChildren[i], scene, materials, bone_map);
    }
}

lcf::Mesh *lcf::ModelManager::processMesh(aiMesh *ai_mesh, const aiScene *scene, const Materials &materials, const BoneMap &bone_map)
{
    int num_vertices = ai_mesh->mNumVertices;
    std::vector<float> positions(num_vertices * 3);
    std::vector<float> normals(num_vertices * 3);
    std::vector<float> uvs(num_vertices * 2);
    std::vector<unsigned int> indices;
    for (int i = 0; i < num_vertices; ++i) {
        memcpy(positions.data() + i * 3, ai_mesh->mVertices + i, sizeof(float) * 3);
        memcpy(normals.data() + i * 3, ai_mesh->mNormals + i, sizeof(float) * 3); 
        float uv[2] = {0.0f, 0.0f};
        if (ai_mesh->mTextureCoords[0]) {
            memcpy(uv, ai_mesh->mTextureCoords[0] + i, sizeof(float) * 2);
        }
        memcpy(uvs.data() + i * 2, uv, sizeof(float) * 2);
    }
    for (unsigned int i = 0; i < ai_mesh->mNumFaces; ++i) {
        aiFace face = ai_mesh->mFaces[i];
        indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
    }
    Skeleton::BonePtrs bones;
    Skeleton::Matrices offset_matrices;
    constexpr int max_bones_per_vertex = 4;
    std::vector<float> bone_ids(num_vertices * max_bones_per_vertex, -1.0f); // 如果是int传到gpu有bug，暂时用float
    std::vector<float> bone_weights(num_vertices * max_bones_per_vertex, 0.0f);
    for (const auto &[name, _] : bone_map) {
    }
    for (unsigned int i = 0; i < ai_mesh->mNumBones; ++i) {
        auto ai_bone = ai_mesh->mBones[i];
        auto iter = bone_map.find(ai_bone->mName.C_Str());
        if (iter == bone_map.end()) { continue; }
        bones.emplace_back(iter->second);
        offset_matrices.emplace_back(toMatrix4x4(ai_bone->mOffsetMatrix));
        auto weights = ai_bone->mWeights;
        for (unsigned int j = 0; j < ai_bone->mNumWeights; ++j) {
            int id = weights[j].mVertexId;
            float weight = weights[j].mWeight;
            for (int k = 0; k < max_bones_per_vertex; ++k) {
                auto &bone_id = bone_ids[id * max_bones_per_vertex + k];
                if (bone_id != -1.0f) { continue; }
                bone_id = i;
                bone_weights[id * max_bones_per_vertex + k] = weight;
                break;
            }
        }
    }
    Material *material = materials[ai_mesh->mMaterialIndex];
    Geometry *geometry = new Geometry;
    geometry->addAttribute(positions.data(), positions.size(), 3);
    geometry->addAttribute(normals.data(), normals.size(), 3);
    geometry->addAttribute(uvs.data(), uvs.size(), 2);
    geometry->setIndices(indices.data(), indices.size());
    Mesh *mesh = new Mesh(Mesh::GeometryPtr(geometry), Mesh::MaterialPtr(material));
    if (bones.empty()) { return mesh ; }
    geometry->addAttribute(bone_ids.data(), bone_ids.size(), 4);
    geometry->addAttribute(bone_weights.data(), bone_weights.size(), 4);
    Skeleton::MatricesPtr matrices_ptr = std::make_shared<Skeleton::Matrices>(std::move(offset_matrices));
    mesh->setSkeleton(std::make_unique<Skeleton>(std::move(bones), matrices_ptr));
    return mesh;
}

lcf::Bone *lcf::ModelManager::processSkeleton(aiNode *node, const aiScene *scene, Bone *parent, BoneMap &bone_map)
{
    Bone *bone = new Bone;
    bone->setParent(parent);
    bone->setName(node->mName.C_Str());
    bone->setLocalMatrix(toMatrix4x4(node->mTransformation));
    bone_map.insert(std::make_pair(bone->name(), bone));
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        this->processSkeleton(node->mChildren[i], scene, bone, bone_map);
    }
    return bone;
}
