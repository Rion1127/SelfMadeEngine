#pragma once
#include "AssimpLoader.h"
#include "WorldTransform.h"
#include "Texture.h"
#include "LightGroup.h"
#include <memory>
#include "Material.h"
#include "myMath.h"
class AssimpModel
{
public:
	AssimpModel();
	static void SetLightGroup(LightGroup* lightGroup) { slightGroup_ = lightGroup; }
	void Create(const wchar_t* modelFile);

	void Draw(const WorldTransform& WT);
private:
	std::vector<Mesh> meshes_;
	std::vector<Material> materials_;
	std::vector <Texture> texture_;
	std::unique_ptr<ImportSettings> importSetting_;

	WorldTransform WorldTransform_;

	static LightGroup* slightGroup_;
public:
	static const uint32_t MAX_BONES = 32;

	struct ConstBufferDataSkin {
		Matrix4 bones[MAX_BONES];
	};
};
