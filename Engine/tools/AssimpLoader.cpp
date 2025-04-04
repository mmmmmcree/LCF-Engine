#include "AssimpLoader.h"
#include <QFileInfo>


lcf::AssimpLoader::AssimpLoader(Model *model, const QString &model_path, QObject *parent) :
    m_model(model),
    m_path(model_path),
    QThread(parent)
{
}

void lcf::AssimpLoader::run()
{
    QFileInfo file_info(m_path);
    if (not file_info.exists()) {
        qDebug() << "Model file does not exist:" << m_path;
        return;
    }
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(m_path.toStdString(),
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace
    );
    if (not scene or scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE or not scene->mRootNode) {
        qDebug() << "Failed to load model:" << m_path;
        return;
    }
    m_path = file_info.path() + '/'; // 修改m_path为路径前缀用于加载贴图
    if (not m_model) { m_model = new Model; }
    MaterialControllerList materials;
    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        materials.emplace_back(processMaterial(scene->mMaterials[i], scene));
    }
    BoneMap bone_map;
    Bone *root_bone = this->processSkeleton(scene->mRootNode, scene, nullptr, bone_map);
    m_model->scale(root_bone->getScale());
    root_bone->setScale(1.0f); //- 把根骨骼的缩放转移到模型上，方便后续处理
    this->processNode(scene->mRootNode, scene, materials, bone_map);
    this->processAnimations(scene, bone_map);
    m_model->setBones(std::move(bone_map));
    emit loaded(m_model);
}

lcf::MaterialController::SharedPtr lcf::AssimpLoader::processMaterial(aiMaterial *ai_material, const aiScene *scene)
{
    MaterialController::SharedPtr mat_controller = MaterialController::createShared();
    std::unordered_map<std::string, SharedImagePtr> image_map;
    for (int type = aiTextureType_DIFFUSE; type <= aiTextureType_GLTF_METALLIC_ROUGHNESS; ++type) {
        int count = ai_material->GetTextureCount(static_cast<aiTextureType>(type));
        // if (count) { qDebug() << "Texture name:" << ai_material->GetName().C_Str() << " type:" << type << " count:" << count; }
        for (int i = 0; i < count; ++i) {
            aiString texture_path;
            ai_material->Get(AI_MATKEY_TEXTURE(static_cast<aiTextureType>(type), i), texture_path);
            if (texture_path.length == 0) { continue; }
            auto iter = image_map.find(texture_path.C_Str());
            if (iter != image_map.end()) { 
                mat_controller->setImageData(type, iter->second);
                continue;
            }
            const aiTexture *ai_texture = scene->GetEmbeddedTexture(texture_path.C_Str());
            Image image;
            if (ai_texture) {
                unsigned char *data = reinterpret_cast<unsigned char *>(ai_texture->pcData);
                image = dataToImage(data, ai_texture->mWidth, ai_texture->mHeight);
            } else {
                image = Image(m_path + texture_path.C_Str()).mirrored().convertToFormat(Image::Format_RGBA8888);
            }
            SharedImagePtr img_ptr = std::make_shared<Image>(image);
            image_map.insert(std::make_pair(texture_path.C_Str(), img_ptr));
            mat_controller->setImageData(type, img_ptr);
        }
    }

    return mat_controller;
}

void lcf::AssimpLoader::processAnimations(const aiScene *scene, const BoneMap &bone_map)
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
        m_model->addAnimation(AnimationPlayer::AnimationPtr(animation));
    }
}

void lcf::AssimpLoader::processNode(aiNode *node, const aiScene *scene, const MaterialControllerList &materials, const BoneMap &bone_map)
{
    Bone *bone = bone_map.find(node->mName.C_Str())->second;
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        unsigned int mesh_id = node->mMeshes[i];
        aiMesh *ai_mesh = scene->mMeshes[mesh_id];
        Mesh *mesh = this->processMesh(ai_mesh, scene, materials, bone_map);
        m_model->addMesh(Model::MeshPtr(mesh));
        if (scene->mNumAnimations > 0) { continue; }
        mesh->setLocalMatrix(bone->getWorldMatrix());
    }
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        this->processNode(node->mChildren[i], scene, materials, bone_map);
    }
}

lcf::Mesh *lcf::AssimpLoader::processMesh(aiMesh *ai_mesh, const aiScene *scene, const MaterialControllerList &materials, const BoneMap &bone_map)
{
    int num_vertices = ai_mesh->mNumVertices;
    std::vector<float> positions(num_vertices * 3);
    std::vector<float> normals(num_vertices * 3);
    std::vector<float> uvs(num_vertices * 2);
    std::vector<float> colors;
    std::vector<float> tangents;
    std::vector<unsigned int> indices;
    memcpy(positions.data(), ai_mesh->mVertices, sizeof(float) * positions.size());
    memcpy(normals.data(), ai_mesh->mNormals, sizeof(float) * normals.size());
    if (ai_mesh->mTextureCoords[0]) {
        for (int i = 0; i < num_vertices; ++i) {
            memcpy(uvs.data() + i * 2, ai_mesh->mTextureCoords[0] + i, sizeof(float) * 2);
        }
    }
    if (ai_mesh->HasVertexColors(0)) {
        colors.resize(num_vertices * 4);
        memcpy(colors.data(), ai_mesh->mColors[0], sizeof(float) * colors.size());
    }
    if (ai_mesh->HasTangentsAndBitangents()) {
        tangents.resize(num_vertices * 3);
        memcpy(tangents.data(), ai_mesh->mTangents, sizeof(float) * tangents.size());
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
    const auto &mat_controller = materials[ai_mesh->mMaterialIndex];
    Geometry *geometry = new Geometry;
    geometry->addAttribute(positions.data(), positions.size(), 0, 3);
    geometry->addAttribute(normals.data(), normals.size(), 1, 3);
    geometry->addAttribute(uvs.data(), uvs.size(), 2, 2);
    geometry->addAttribute(colors.data(), colors.size(), 3, 4);
    geometry->addAttribute(tangents.data(), tangents.size(), 4, 3);
    geometry->setIndices(indices.data(), indices.size());
    Mesh *mesh = new Mesh(Mesh::GeometryPtr(geometry));
    mesh->m_material_controller = mat_controller;
    Skeleton::MatricesPtr matrices_ptr = std::make_shared<Skeleton::Matrices>(std::move(offset_matrices));
    mesh->setSkeleton(std::make_unique<Skeleton>(std::move(bones), matrices_ptr));
    if (matrices_ptr->empty()) { return mesh ; }    
    geometry->addAttribute(bone_ids.data(), bone_ids.size(), 5, 4);
    geometry->addAttribute(bone_weights.data(), bone_weights.size(), 6, 4);
    return mesh;
}

lcf::Bone *lcf::AssimpLoader::processSkeleton(aiNode *node, const aiScene *scene, Bone *parent, BoneMap &bone_map)
{
    Bone *bone = new Bone;
    // bone->setParent(parent);
    bone->attachTo(parent);
    bone->setName(node->mName.C_Str());
    // bone->setLocalMatrix(toMatrix4x4(node->mTransformation));
    bone->setLocalMatrix(toMatrix4x4(node->mTransformation));
    // bone_map.insert(std::make_pair(bone->name(), bone));
    bone_map.insert(std::make_pair(bone->getName(), bone));
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        this->processSkeleton(node->mChildren[i], scene, bone, bone_map);
    }
    return bone;
}