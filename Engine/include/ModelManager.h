#pragma once

#include "Model.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "Animation.h"


namespace lcf {
    class ModelManager : public QObject
    {
        Q_OBJECT
        using Materials = std::vector<Material*>;
        using BoneMap = std::unordered_map<std::string, Bone*>;
    public:
        static ModelManager *instance();
        static void initialize();
        Model *loadSingleThread(const QString &model_path);
        void load(const QString &model_path);
    signals:
        void startLoading(const QString &model_path);
        void modelLoaded(Model *model);
        void startInitialLoading(const QString &model_path);
        void initialModelLoaded(Model *model);
    private:
        ModelManager();
        void initialLoad(const QString &model_path);
        Material *processMaterial(aiMaterial *ai_material, const aiScene *scene, const QString &path);
        void processAnimations(Model *model, const aiScene *scene, const BoneMap &bone_map);
        void processNode(Model *model, aiNode *node, const aiScene *scene, const Materials &materials, const BoneMap &bone_map);
        Mesh *processMesh(aiMesh *ai_mesh, const aiScene *scene, const Materials &materials, const BoneMap &bone_map);
        Bone *processSkeleton(aiNode *node, const aiScene *scene, Bone *parent, BoneMap &bone_map);
    };
}