#include "AssimpModel.h"
#include "Texture.h"
#include "DirectX.h"

LightGroup* AssimpModel::slightGroup_ = nullptr;

AssimpModel::AssimpModel()
{
}

void AssimpModel::Create(const wchar_t* modelFile)
{
	ImportSettings importSetting = {
		modelFile,
		meshes_,
		false,
		true
	};

	importSetting_ = std::move(std::make_unique<ImportSettings>(importSetting));
	//assimp�t�@�C���ǂݍ���
	AssimpLoader::GetInstance()->Load(importSetting_.get());

	texture_.resize(meshes_.size());
	materials_.resize(meshes_.size());
	for (uint32_t i = 0; i < importSetting_->meshes.size(); i++)
	{
		importSetting_->meshes[i].Vertices.CreateBuffer();

		std::string texturename = WStringToString(meshes_[i].diffuseMap);
		TextureManager::GetInstance()->LoadGraph(texturename, texturename);
		texture_[i] = *TextureManager::GetInstance()->GetTexture(texturename);

		//�}�e���A������
		//�}�e���A���̒l��������i���݂͓K���Ȓl�����Ă���j
		materials_[i].SetAmbient({ 0.8f,0.8f,0.8f });
		materials_[i].SetDiffuse({ 0.3f,0.3f,0.3f });
		materials_[i].SetSpecular({ 0.3f,0.3f,0.3f });
	}
}

void AssimpModel::Draw(const WorldTransform& WT)
{
	slightGroup_->Draw(3);
	for (uint32_t i = 0; i < importSetting_->meshes.size(); i++)
	{
		materials_[i].Draw(texture_[i].textureHandle);

		importSetting_->meshes[i].Vertices.Draw(WT, 0);
	}
}
