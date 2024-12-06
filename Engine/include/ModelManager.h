#pragma once

#include "Model.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "Animation.h"
#include <unordered_map>


namespace lcf {
    class ModelManager : public QObject
    {
        Q_OBJECT
        using MaterialPtr = std::shared_ptr<Material>;
        using Materials = std::vector<MaterialPtr>;
        using BoneMap = std::unordered_map<std::string, Bone*>;
    public:
        static ModelManager *instance();
        Model *loadSingleThread(const QString &model_path);
        void loadSingleThreadWithModel(Model *model, const QString &model_path);
        void load(const QString &model_path);
        void load(Model *&model, const QString &model_path);
    signals:
        void startLoading(const QString &model_path);
        void startLoadingWithModel(Model *model, const QString &model_path);
        void modelLoaded(Model *model);
        void loadingFinished(const QString &model_path, Model *model);
    private:
        ModelManager();
        MaterialPtr processMaterial(aiMaterial *ai_material, const aiScene *scene, const QString &path);
        void processAnimations(Model *model, const aiScene *scene, const BoneMap &bone_map);
        void processNode(Model *model, aiNode *node, const aiScene *scene, const Materials &materials, const BoneMap &bone_map);
        Mesh *processMesh(aiMesh *ai_mesh, const aiScene *scene, const Materials &materials, const BoneMap &bone_map);
        Bone *processSkeleton(aiNode *node, const aiScene *scene, Bone *parent, BoneMap &bone_map);
    private:
        std::unordered_map<QString, Model::UniquePtr> m_models;
    };
}