#include "Model.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "Animation.h"
#include <QThread>


namespace lcf {
    class AssimpLoader : public QThread
    {
        Q_OBJECT
        using MaterialPtr = std::shared_ptr<Material>;
        using Materials = std::vector<MaterialPtr>;
        using BoneMap = std::unordered_map<std::string, Bone*>;
    public:
        AssimpLoader(Model *model, const QString &model_path, QObject *parent = nullptr);
        void run() override;
    signals:
        void loaded(Model *model);
    private:
        MaterialPtr processMaterial(aiMaterial *ai_material, const aiScene *scene);
        void processAnimations(const aiScene *scene, const BoneMap &bone_map);
        void processNode(aiNode *node, const aiScene *scene, const Materials &materials, const BoneMap &bone_map);
        Mesh *processMesh(aiMesh *ai_mesh, const aiScene *scene, const Materials &materials, const BoneMap &bone_map);
        Bone *processSkeleton(aiNode *node, const aiScene *scene, Bone *parent, BoneMap &bone_map);
    private:
        Model *m_model;
        QString m_path;
    };
}